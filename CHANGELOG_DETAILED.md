# Documentação Detalhada de Alterações Arquiteturais (Changelog)

Esta documentação detalha as amplas mudanças estruturais, de hardware e lógicas implementadas na transição do repositório legado (`epaper-watch-fw-29e920...`) para a versão atual.

---

## 1. Migração do Sistema: Bare-metal para FreeRTOS

A principal mudança no núcleo do projeto foi o abandono do "Superloop" em prol de um RTOS (Real-Time Operating System).

### 1.1. Arquitetura de `app_system.c`
- **Antes:** O código funcionava de maneira sequencial em `app_system_loop()`. Era baseado em *polling* (`HAL_GetTick()`) para contar o tempo de 1 segundo e verificar interrupções de botões (`bsp_btn_exti_flag`).
- **Agora:** `app_system_loop()` agora chama `vTaskStartScheduler()`, inicializando o **FreeRTOS**.
- **Novas Tasks Criadas:**
  - `vUITask`: Gerencia a UI, desbloqueando apenas quando recebe um evento na fila `xSystemEventQueue`.
  - `vAlarmTask`: Roda com prioridade e usa `vTaskDelayUntil` a cada 100ms. Checa o tempo de 1s e envia eventos na fila (`EVENT_TICK_100MS` e `EVENT_TICK_1SEC`). Notifica também o alarme.
  - `vButtonTask`: Fica em modo de espera para interagir com debouncing dos botões.
  - `vOximeterTask` & `vAccelTask`: Processam os periféricos de forma paralela e isolada.

### 1.2. Kernel e IRQs (`core/src/it.c` & `main.c`)
- A função `SysTick_Handler` foi alterada para notificar o FreeRTOS com `xPortSysTickHandler()` apenas quando o escalonador estiver ativo.
- O tratador de interrupções de botão passou de `EXTI0_IRQHandler` para `EXTI15_10_IRQHandler` (mapeado para o pino do novo botão no FreeRTOS).
- Arquivos de configuração do FreeRTOS (`FreeRTOSConfig.h`) e suporte do STM32Cube (`stm32wbxx_hal_conf.h`) foram explicitamente incorporados ao `core/inc/`.

---

## 2. Processamento Digital de Sinais (CMSIS-DSP)

### 2.1. Novo Módulo Oximetria (`app_oximeter.c`)
A oximetria simples por HAL foi substituída por um modelo robusto para calcular HR (Batimentos) e SpO2 com alta precisão através do CMSIS-DSP:
- **Taxa de Amostragem (fs):** Fixada em 100 Hz.
- **Filtro Aplicado:** Filtro IIR Biquad (Butterworth Bandpass) atuando na banda de 0.5Hz a 4Hz para rejeitar tanto ruído de linha base contínua quanto ruídos de alta frequência.
- O código agora gerencia instâncias do DSP via `arm_biquad_casd_df1_inst_f32`, instanciando arrays flutuantes `float32_t` para o sinal vermelho (RED) e infravermelho (IR).
- Possui um buffer histórico (`WINDOW_SIZE = 500` / 5 segundos) usado para derivar os cálculos finais de SpO2.
- A função no `CMakeLists.txt` foi fortemente otimizada filtrando funções do CMSIS-DSP que não são de *FilteringFunctions* ou *BasicMathFunctions*, visando economizar a preciosa memória Flash.

---

## 3. Substituição Total do Display: E-Paper para Memory LCD

Houve uma migração radical devido à limitação de taxa de atualização (refresh rate) do E-Paper (SSD1681), limitante inaceitável para animações e jogos. O display atual é o **LS013B7DH03 (Memory LCD da Sharp)**.

### 3.1. Drivers BSP e GPIOs (`gpio.c` / `board_config.h`)
- Apagados todos os arquivos e instâncias para `ssd1681` na `/lib`.
- O tamanho do frame buffer lógico foi reduzido de `200x200` para `128x128`.
- Pinos como `EPD_RST` e `EPD_DC` foram suprimidos. Agora operamos com `DISP_CS`, `DISP_ON` e `DISP_EXTCOM`.
- **Implementação PWM (EXTCOM):** Para evitar a queima/degradação do cristal líquido, as telas Sharp memory exigem inversão periódica do sinal comum VCOM. Em `gpio.c`, foi instanciado o `LPTIM1` (Timer de Baixo Consumo) gerando um PWM nativo de 50% de duty cycle (Pulso: 8333, Período: 16666) no pino EXTCOM (`GPIOB_PIN_2`).

### 3.2. Atualização das Comunicações (`spi.c` e `clock.c`)
- O clock base sofreu a inclusão do `LSI` (Low-Speed Internal oscillator) explícito em `clock.c` para gerenciar o Real-Time Clock (RTC).
- A configuração da SPI1 foi modernizada e enxugada para `SPI_BAUDRATEPRESCALER_32` gerando clock aproximado de 1MHz e `SPI_CRCCALCULATION_DISABLE` para otimizar os ciclos de CPU ao conversar com o novo LCD Sharp, o qual usa tramas de byte bem específicas e endereçadas.

---

## 4. O Nova Engine Gráfica e Menus

### 4.1. GFX e Otimização RAM
- Criado o diretório `lib/components/gfx/`, encapsulando funções gráficas nativas.
- Fonte `shmfonts` substituiu fontes customizadas do antigo SSD1681, e gerencia desenho rápido de texto (`app_display_draw_text`) lidando internamente com as coordenadas `X/Y`.

### 4.2. Integração de Jogos e Múltiplas Telas (`screen_games.c`)
Foram criadas e acopladas funções lúdicas inteiramente em C no diretório `/app/ui/screens/`:
1. **Snake** (`screen_snake.c`)
2. **Doom Lite** (`screen_doom.c`)
3. **Flappy Watch** (`screen_flappy.c`)
4. **Parkour Watch** (`screen_parkour.c`)
- A adição do arquivo `anim_frames.c` em `app/ui/assets` trouxe uma impressionante animação de "boot" (logotipo animado exibido a ~20 fps, atrasado por `HAL_Delay(50)` em `app_system_init`).

---

## 5. Alarmes, Tempo e Relógio Interno (RTC)

### 5.1. Integração de Hardware (`rtc.c`)
Foi implementada uma camada em `lib/bsp/src/rtc.c` que mapeia os registradores de hardware do RTC nativo do STM32WB55.
A tarefa `vAlarmTask` consome a função `rtc_get_time` e faz a ponte (Polling RTC cada 1 segundo) com `app_alarm_tick(Horas, Minutos)`.
Se os valores baterem, a task aciona: `UI_Manager_SwitchScreen(&Screen_AlarmRinging);` ativando imediatamente uma tela dedicada de notificação ao usuário.

---

## 6. Alterações da Toolchain (CMake)

Para suportar essas complexidades, o `CMakeLists.txt` sofreu severas adições:
- Foram introduzidas rotinas para mapear as dezenas de pastas recursivas na árvore (ex: `app/alarm/*.c`, `app/oximeter/*.c`, `app/ui/screens/*.c`).
- Inclusão direta no binário das dezenas de arquivos do kernel FreeRTOS (`tasks.c`, `queue.c`, `port.c` e especialmente o `heap_4.c` para alocação flexível de blocos de RAM seguros do RTOS).
- Filtragem cirúrgica dos módulos do DSP nativos da CMSIS, retirando dependências enormes de MatrixFunctions ou FastMathFunctions, e inserindo apenas BasicMathFunctions e FilteringFunctions na montagem via `list(FILTER CMSIS_DSP_SOURCES EXCLUDE REGEX...)`.

---
*Fim da Documentação.*

---

## 7. Integração Profunda e Arquitetura do FreeRTOS

A adoção do FreeRTOS exigiu que múltiplos subsistemas do firmware fossem reescritos para acomodar as restrições e benefícios de um ambiente multitarefa de tempo real. Abaixo estão todos os pontos de contato da migração e integração do RTOS.

### 7.1. Gerenciamento do Kernel e Build System (CMake)
- **CMakeLists.txt:** O script de build foi atualizado para compilar o código fonte nativo do FreeRTOS diretamente. Os arquivos adicionados na compilação foram: `tasks.c`, `queue.c`, `list.c`, `timers.c`, `event_groups.c`, `stream_buffer.c`.
- **Port de Arquitetura:** Foi mapeado e incluído o diretório do port do processador (`portable/GCC/ARM_CM4F/port.c`), garantindo o suporte do kernel para salvar/restaurar o contexto do processador ARM Cortex-M4 (STM32WB55).
- **Gerenciamento de Memória (Heap):** Foi configurado o uso do `heap_4.c` (presente em `portable/MemMang/heap_4.c`), que fornece um algoritmo robusto capaz de juntar blocos de RAM adjacentes que foram liberados para combater a fragmentação de memória.
- **Configurações Globais:** A adição do arquivo `FreeRTOSConfig.h` no diretório `core/inc/`. Ele gerencia características como `configUSE_PREEMPTION`, as macros que mapeiam as interrupções de prioridade mais alta da ARM para o FreeRTOS (`vPortSVCHandler`, `xPortPendSVHandler`, `xPortSysTickHandler`) e define a frequência de *tick* do sistema.

### 7.2. O Orquestrador Multitarefa (`app_system.c`)
Toda a lógica `main` do relógio foi redesenhada. Antes o loop atuava num `while(1)` infinito rodando em força bruta e baseando a precisão do tempo através da `HAL_GetTick()`. Agora:
- **`vTaskStartScheduler()`:** O encerramento de `app_system_init()` dá início ao Escalonador.
- **Filas IPC (`xSystemEventQueue`):** Uma Queue (Tamanho 10, de tipo `UI_Event_t`) foi instanciada com `xQueueCreate` para coordenar mensagens. A task da tela (`vUITask`) fica adormecida usando a constante `portMAX_DELAY` esperando receber os comandos desta fila.

**Mapeamento de Tasks:**
Para garantir tempos de respostas seguros, as tasks foram divididas da seguinte forma na criação (`xTaskCreate`):
1. **`vButtonTask`** (Prioridade 4): Alta prioridade para capturar eventos duplos ou cliques simples com precisão.
2. **`vUITask`** (Prioridade 3): Responsável pela atualização e repintura do display assíncronamente.
3. **`vAlarmTask`** (Prioridade 2): Roda isolada cuidando de verificar RTC.
4. **`vOximeterTask`** (Prioridade 2): Executada via escalonamento por atraso para coletar os dados biológicos.
5. **`vAccelTask`** (Prioridade 2): Também escalonada para ler o posicionamento 3D.

### 7.3. Interrupções (ISRs) e Sincronismo (`core/src/it.c`)
- O evento base do hardware, `SysTick_Handler()`, agora inclui o roteamento para `xPortSysTickHandler()` – mas com a ressalva segura de `if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)` de modo que a HAL não acione acidentalmente o FreeRTOS antes do ambiente estar 100% inicializado.
- A prioridade base de chamadas `SysTick_IRQn` via NVIC foi intencionalmente forçada a 15 (`HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);`) em `app_system.c` para não invadir ou atropelar o kernel (que deve ter as permissões de mais baixa prioridade no contexto do Córtex-M em suas ISRs).

### 7.4. Gerenciamento de Atrasos e Controle de Tempo nos Módulos
As funções do sistema mudaram de atrasos de hardware (`HAL_Delay`) para atrasos do kernel (`vTaskDelay`), permitindo que as tarefas abram mão da CPU enquanto esperam.
- **Botões (`vButtonTask`):** A detecção de *double-click* usa uma janela baseada no *tick count* (`now = xTaskGetTickCount()`) cruzando o limiar de `pdMS_TO_TICKS(350)` ms. O polling de *debounce* utiliza `vTaskDelay(pdMS_TO_TICKS(10))` para inspecionar os pinos sem ocupar a CPU.
- **Relógio e Alarmes (`vAlarmTask`):** Utiliza um modelo bloqueante preciso com `vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100))`. Assim, em um loop determinístico a cada 10 ciclos (10 x 100ms = 1 segundo), a `vAlarmTask` gera evento na Fila sem erro de atraso flutuante em software.
- **Acelerômetro (`vAccelTask` em `app_accel.c`):** Utiliza `vTaskDelay(pdMS_TO_TICKS(80))` como laço base de polling, atualizando os limites dos eixos a cada 80ms para os mini-games ou rastreio de passos, acionando também `xTaskGetTickCount` para medir o debouncing dos "passos".
- **Oxímetro (`vOximeterTask` em `app_oximeter.c`):** O módulo agora suspende a execução por 100ms em cada iteração `vTaskDelay(pdMS_TO_TICKS(100))` garantindo taxa de amostragem constante para o filtro CMSIS DSP.