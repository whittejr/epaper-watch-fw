# Documentação do Fluxo de Firmware - E-Paper Watch

Este documento descreve detalhadamente o funcionamento do firmware do smartwatch, desde a energização (inicialização) até o seu estado de estabilização (loop principal) e como ele reage às interações do usuário.

---

## 1. Ponto de Entrada: O Início de Tudo (`main.c`)

Quando o relógio é ligado, o microcontrolador executa o arquivo `core/src/main.c`. A função `main()` é intencionalmente mantida simples para delegar a lógica ao orquestrador do sistema:

```c
int main(void) {
    HAL_Init();          // 1. Inicializa a camada de abstração de hardware (HAL da ST)
    app_system_init();   // 2. Inicializa o sistema, módulos e tarefas
    app_system_loop();   // 3. Inicia o escalonador do RTOS (loop infinito)
    return 0;
}
```

## 2. Inicialização do Sistema (`app_system_init`)

Localizado em `app/system/app_system.c`, a função `app_system_init()` é responsável por preparar todo o ambiente antes que o controle seja entregue ao sistema operacional em tempo real (FreeRTOS). A ordem das inicializações é crucial:

1. **Hardware (BSP):** `bsp_init()` configura os pinos básicos, clocks e barramentos de comunicação (I2C, SPI, UART).
2. **Display:** `app_display_init()` liga o display e-paper e prepara seu buffer de memória.
3. **Animação de Boot:** Imediatamente após ligar a tela, um loop envia frames de uma animação inicial (`anim_frames`) para o display.
4. **Sensores e Funcionalidades:**
   - `app_accel_init()`: Inicializa o acelerômetro (para detecção de movimento/tilt).
   - `app_alarm_init()`: Configura a lógica de alarmes.
   - `app_oximeter_init()`: Inicializa o sensor de batimentos e oxigenação.
5. **Gerenciador de Interface (UI):** `UI_Manager_Init(&Screen_Watchface)` define que a primeira tela a ser exibida quando o sistema estabilizar será o mostrador de horas (Watchface).
6. **Fila de Eventos (Queue):** É criada a `xSystemEventQueue`. Essa fila é a espinha dorsal da arquitetura, pois é por onde botões e temporizadores enviam comandos para a interface de forma segura.
7. **Criação das Tarefas (FreeRTOS Tasks):** O sistema se divide em diferentes "processos" (tarefas) que rodam concorrentemente:
   - `UI_Task`: Gerencia a interface gráfica e telas.
   - `Alarm_Task`: Cuida da passagem do tempo e ativação de alarmes.
   - `Button_Task`: Lê os cliques do botão físico.
   - `Oximeter_Task`: Gerencia a leitura de batimentos.
   - `Accel_Task`: Lê dados de movimento.

## 3. A Estabilização: O RTOS Assume o Controle (`app_system_loop`)

A última instrução do `main()` é chamar `app_system_loop()`, que basicamente executa `vTaskStartScheduler()`. A partir deste momento, o loop tradicional do C é abandonado. O FreeRTOS assume o controle do processador e começa a chavear a execução entre as tarefas criadas, baseando-se em suas prioridades.

---

## 4. O Sistema em Execução (Como as coisas funcionam)

Uma vez estabilizado, o relógio passa a operar orientado a eventos. A arquitetura é baseada no padrão Produtor-Consumidor. As tarefas atuam produzindo eventos que são consumidos pela Tarefa de Interface (`UI_Task`).

### A Tarefa de Tempo e Alarmes (`Alarm_Task`)
Esta tarefa roda em um loop contínuo e exato (usando `vTaskDelayUntil`). A cada intervalo de tempo, ela **produz eventos de relógio** e os injeta na `xSystemEventQueue`:
- Produz `EVENT_TICK_50MS` a cada 50ms.
- Produz `EVENT_TICK_100MS` a cada 100ms.
- Produz `EVENT_TICK_1SEC` a cada 1 segundo.
Além disso, ela lê o relógio interno (RTC) e verifica se algum alarme precisa tocar. Se sim, ela força a mudança de tela para `Screen_AlarmRinging`.

### A Tarefa do Botão (`Button_Task`)
Fica analisando o pino do botão físico e lidando com o *"debounce"* (ruído mecânico). Dependendo da forma como o usuário aperta, ela injeta eventos de controle na Fila:
- Clique rápido: `EVENT_BTN_NEXT` (Avançar/Mudar item).
- Duplo clique: `EVENT_BTN_DOUBLE`.
- Clique longo: `EVENT_BTN_SELECT` (Confirmar/Entrar).

### A Tarefa de Interface (`UI_Task` e `UI_Manager`)
A `vUITask` fica essencialmente dormindo até que chegue um evento na fila `xSystemEventQueue`. Quando chega um evento (seja um tick de tempo ou um clique de botão), ela o repassa para a função `UI_Manager_ProcessEvent()`.

O Gerenciador de UI (`ui_manager.c`) age em duas camadas:

1. **Camada Global:**
   - **Timeout de Inatividade:** Se receber eventos de 1 segundo (`EVENT_TICK_1SEC`) e perceber que não houve interação do usuário (clique em botões) por 10 segundos, ele força o sistema a retornar para a tela inicial (`Screen_Watchface`). Apenas algumas telas, como jogos e oxímetro, previnem este timeout.
   - **Wake On Wrist (Giro do Pulso):** Se receber um `EVENT_WAKE` (gerado pelo acelerômetro) enquanto está na tela de horas e a configuração estiver ativa, ele abre o Menu (`Screen_Menu`) automaticamente.
   
2. **Camada Específica de Tela:**
   Se o evento não foi tratado pelas regras globais, o gerenciador passa o evento para a tela atualmente ativa chamando `current_screen->on_event(event)`. 
   
   Por exemplo, se a tela atual for o Menu e recebermos um `EVENT_BTN_NEXT` do botão, o Menu vai descer um item na lista. Se receber um `EVENT_TICK_1SEC`, a tela de horas (`Watchface`) atualizará os dígitos do relógio na tela.

## 5. Estrutura das Telas (Screens)

Para adicionar uma nova interface, o código utiliza o padrão definido por `AppScreen_t`. Cada tela (ex: `screen_menu.c`, `screen_oximeter.c`) é um conjunto de três funções:
- `on_enter()`: Chamada uma única vez quando a tela é aberta (para desenhar a base da tela, zerar variáveis, etc).
- `on_event(event)`: Chamada cada vez que há um clique de botão, evento de tick (para animações) ou comandos globais. É aqui que a tela processa a interação e se redesenha.
- `on_exit()`: Chamada antes de sair da tela (para liberar recursos, encerrar medições, salvar placares, etc).

## 6. Estado e Configurações (`app_state.h`)

O estado global do relógio — como nível de brilho, idioma, ativação de vibração e tema — fica unificado em uma estrutura global `g_watch_settings` inicializada no `app_system.c`. Qualquer tela ou componente pode acessar o arquivo `app_state.h` para ler essas preferências de forma centralizada.

---

### Resumo Visual do Fluxo:

```text
[ ENERGIA LIGADA ] -> HAL_Init() -> bsp_init() -> Hardware Pronto
                         |
                         v
                app_system_init() -> Tela Inicializa -> Animação Toca -> RTOS Configurado
                         |
                         v
[ ESTADO ESTÁVEL ] app_system_loop() -> (FreeRTOS Scheduler Ativo)
                         |
      +------------------+------------------+------------------+
      |                  |                  |                  |
[Button_Task]      [Alarm_Task]       [Accel_Task]       [UI_Task] (Dormindo até ter evento)
Lê Botão           Lê Relógio         Lê Sensores             |
  |                  |                  |                     |
  +-> Eventos Botão  +-> Eventos Tempo  +-> Eventos Tilt      |
  |                  |                  |                     |
  +-----------+------+------------------+                     |
              |                                               |
              v                                               |
        [ Fila: xSystemEventQueue ] <-------------------------+ Acorda a UI_Task!
              |
              v
      UI_Manager_ProcessEvent()
              |
              +--> Verifica regras globais (Timeout 10s? Voltar ao inicio!)
              |
              +--> current_screen->on_event(evento) ---> A TELA ATUAL É ATUALIZADA
```
