# Documentação Completa do Projeto - E-Paper Watch

Este documento fornece uma visão técnica detalhada da arquitetura, camadas de software e lógica de funcionamento do firmware do relógio.

## 1. Visão Geral
O projeto é um firmware para um smartwatch baseado no microcontrolador **STM32WB55**, utilizando um display E-Paper/MIP (Memory-In-Pixel) para baixíssimo consumo de energia. O sistema é orquestrado pelo **FreeRTOS** para multitarefa eficiente.

---

## 2. Estrutura de Diretórios
- `app/`: Lógica de aplicação de alto nível.
    - `system/`: Orquestrador central (Tarefas FreeRTOS, Filas).
    - `ui/`: Sistema de interface (Gerenciador de telas, Ativos, Telas individuais).
    - `display/`: Camada de abstração de interface (UI Toolkit).
    - `oximeter/`, `accelerometer/`, `alarm/`: Módulos de sensores e funcionalidades.
- `core/`: Configuração de baixo nível do hardware STM32, handlers de interrupção e startup.
- `lib/`: Bibliotecas e drivers.
    - `bsp/`: Board Support Package (Configuração de GPIO, I2C, SPI, RTC específica da placa).
    - `components/`: Drivers para componentes externos (Display LS013B, Sensor MAX30102, etc.).
    - `gfx/`: Biblioteca gráfica básica (desenho de pixels, formas e fontes).
- `tools/`: Ferramentas auxiliares e análise de dados.

---

## 3. Arquitetura de Software (Camadas)

### A. Camada de Hardware (BSP & Components)
Esta camada lida com a comunicação direta com os periféricos.
- **Drivers de Sensores**: Oximetro (MAX30102) e Acelerômetro (ADXL362).
- **Display HAL**: Abstrai o controlador do display, gerenciando o Framebuffer em RAM e transferindo dados via SPI.

### B. Camada de Abstração Gráfica (SHM-5 UI Toolkit)
Para manter o código limpo e visualmente consistente:
- **`display_hal`**: Oferece primitivas básicas (`display_draw_rect`, `display_draw_bitmap`).
- **`app_display`**: Um conjunto de ferramentas de UI que implementa o estilo "SHM-5" (Barra de status, alinhamento de texto, widgets de bateria).

### C. Sistema de Interface (UI Manager)
Baseado em uma máquina de estados:
- **`AppScreen_t`**: Estrutura que define uma tela (`on_enter`, `on_event`, `on_exit`).
- **`UI_Manager`**: Controla qual tela está ativa e despacha eventos para ela.
- **Telas (`screen_*.c`)**: Cada arquivo representa uma funcionalidade (Relógio, Menu, Jogos, Configurações).

---

## 4. Fluxo de Eventos e Multitarefa
O sistema utiliza uma fila central de eventos (`xSystemEventQueue`) para comunicação entre tarefas.

| Tarefa | Função | Frequência/Evento |
| :--- | :--- | :--- |
| `vUITask` | Consome eventos, processa lógica de tela e atualiza display. | Baseado em Eventos |
| `vButtonTask` | Monitora o botão físico e detecta cliques (Curto, Duplo, Longo). | 100Hz (Polling/EXTI) |
| `vAlarmTask` | Gera "ticks" de tempo (1s, 100ms) e verifica alarmes no RTC. | 10Hz |
| `vOximeterTask`| Processa sinais de PPG e calcula Batimentos/SpO2. | 100Hz |

### Ciclo de vida de um clique:
1. O usuário pressiona o botão.
2. `vButtonTask` detecta o clique e envia `EVENT_BTN_NEXT` para a fila.
3. `vUITask` acorda, chama `UI_Manager_ProcessEvent`.
4. A tela ativa (ex: Menu) incrementa o cursor e chama `app_display_update`.

---

## 5. Design Visual (Estética SHM-5)
O projeto segue uma estética inspirada na linha **SHM-5** e relógios analógicos clássicos:
- **Cores**: Alto contraste (Preto no Branco) otimizado para E-Paper.
- **Layout**: Uso de linhas finas (hairlines) e tipografia limpa.
- **Branding**: Logotipo "SHM-5" e janela de data clássica na posição de 3 horas.

---

## 6. Jogos Integrados
- **Doom Lite**: Engine de raycasting 3D com inimigos escalonáveis e animação de arma.
- **Temple Watch**: Runner de perspectiva 3D com troca de pistas.
- **Doodle Watch**: Vertical platformer com física de pulo.
- **Snake / Flappy**: Versões clássicas adaptadas para 1 botão.

---

## 7. Configurações e Persistência
A tela de configurações permite o ajuste de:
- Formato de hora (12/24h).
- Intensidade de brilho e vibração.
- Temas Visuais (Claro/Escuro).
- Ativação por pulso (Wrist Wake).

---

## 8. Build e Desenvolvimento
O projeto utiliza **CMake** com **Presets**:
- **Preset**: `chip-debug`
- **Ferramenta de Build**: Ninja
- **Compilador**: arm-none-eabi-gcc
- **Script de Linker**: Customizado para STM32WB55 (512KB Flash, 192KB RAM).

Comando para build:
```bash
cmake --build --preset chip-debug
```
