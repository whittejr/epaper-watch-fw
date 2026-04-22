# Notas de Desenvolvimento e Manutenção - E-Paper Watch FW

Este documento detalha as implementações técnicas, correções de hardware e decisões de design tomadas durante o desenvolvimento do firmware.

## 1. Mapa de Pinagem (Pinout)

O sistema utiliza o microcontrolador STM32WB55. Abaixo está a configuração final de hardware:

| Periférico | Função | Pino | Observação |
| :--- | :--- | :--- | :--- |
| **SPI1** | SCK (Clock) | **PA5** | Compartilhado entre Display e Acelerômetro |
| **SPI1** | MISO (Data In) | **PA6** | Usado apenas pelo ADXL362 |
| **SPI1** | MOSI (Data Out) | **PA7** | Compartilhado |
| **I2C1** | SCL (Clock) | **PA9** | Usado pelo Oxímetro MAX30102 |
| **I2C1** | SDA (Data) | **PA10** | Usado pelo Oxímetro MAX30102 |
| **Display** | CS (Chip Select) | **PB0** | **Ativo em ALTO (1)** |
| **Display** | DISP_ON | **PB1** | Liga/Desliga o painel |
| **Display** | EXTCOM | **PB2** | Sinal PWM (LPTIM1) para evitar burn-in |
| **Accel** | CS (Chip Select) | **PA1** | **Ativo em BAIXO (0)** |
| **Botão** | Botão Principal | **PB9** | Interrupção na linha EXTI9_5 |

---

## 2. Gerenciamento de Barramento (Concurrency)

### O Problema do SPI Compartilhado
O Display (E-Paper) e o Acelerômetro dividem o mesmo barramento SPI1. O display é ativo em nível alto e o acelerômetro em nível baixo. Sem proteção, uma tarefa interrompia a outra, deixando o pino CS errado ativado e causando conflito de dados (leitura retornando `0x00`).

### A Solução (SPI Mutex)
Implementamos um **Mutex do FreeRTOS** centralizado no driver `lib/bsp/src/spi.c`.
*   As funções `spi_lock()` e `spi_unlock()` garantem que a transação SPI seja atômica.
*   **Importante:** O CS de cada periférico só deve ser manipulado **dentro** de um bloco `lock/unlock`.
*   **Inicialização:** O Mutex é criado no final de `app_system_init`, logo antes do RTOS iniciar, para evitar travamentos no boot.

---

## 3. Subsistema de Saúde (Oxímetro MAX30102)

### Algoritmo de Processamento (CMSIS-DSP)
O oxímetro opera a **100Hz** e utiliza um filtro **Biquad IIR Passa-Banda (0.5Hz - 4Hz)** para isolar o pulso cardíaco.
*   **Janela Deslizante:** O sistema mantém 5 segundos de dados (500 amostras) na RAM.
*   **Cálculo em Tempo Real:** Após os primeiros 2 segundos, o BPM começa a ser calculado e é atualizado a cada 1 segundo (janela deslizante).
*   **Detecção Adaptativa:** O threshold de detecção de picos é dinâmico (50% do RMS do sinal), adaptando-se a diferentes pressões de dedo.

### Configuração de Hardware
Para garantir leituras estáveis em um relógio de pulso, o sensor está configurado para **potência máxima**:
*   Corrente do LED: `0xFF` (máxima).
*   Ganho do ADC: `16384` (máximo).

---

## 4. Interface e Experiência do Usuário (UI/UX)

### Gestão de Inatividade (Power Saving)
Implementado no `ui_manager.c` um timeout global de **10 segundos**.
*   Se o usuário não interagir por 10s, o relógio volta automaticamente para a Watchface.
*   **Exceções:** O timeout é desativado na **Watchface**, no **Oxímetro** (para permitir medição) e no **Jogo do Seu Décio** (controle por movimento).

### Tilt-to-Menu
O acelerômetro detecta a inclinação do pulso (Eixo Y > 0.7G). Quando disparado na Watchface, o evento `EVENT_WAKE` abre o menu principal sem necessidade de clique no botão.

---

## 5. Seu Décio Jump (Gaming)

### Motor Gráfico e Performance
*   **Taxa de Quadros:** Aumentada para **20 FPS** (`EVENT_TICK_50MS`).
*   **Controle por Inclinação:** O Eixo X do acelerômetro controla a velocidade horizontal, com aplicação de **Deadzone** e **Filtro Passa-Baixa** para suavizar o movimento e eliminar tremores.
*   **Sprites:** Implementado suporte a desenho de bitmaps 1bpp com espelhamento horizontal automático.

---

## 6. Notas de Manutenção (Debugging)

### Floating Point no GCC
Para que o `snprintf` consiga imprimir números decimais (float) na tela, a flag de linker `-Wl,-u,_printf_float` deve estar ativa no `CMakeLists.txt`. Sem ela, o BPM/G aparecerá vazio ou travará.

### Travamentos no HAL_Delay
Se o relógio travar no boot, verifique a ordem de inicialização no `app_system.c`. Funções do FreeRTOS (como Mutex) não devem ser chamadas antes de `vTaskStartScheduler` se o `HAL_Delay` (SysTick) estiver sendo usado no boot.

### Diagnóstico de Sensores
Na tela do Oxímetro e do Acelerômetro, foram incluídas rotinas que leem o **PART_ID** dos chips diretamente via barramento. Se a tela mostrar `I2C FALHOU` ou `ID: 00`, o problema é **físico** (cabo solto ou alimentação).
