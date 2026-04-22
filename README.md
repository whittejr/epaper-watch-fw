# E-Paper Watch Firmware (Seiko Homage)

Firmware de alta performance para smartwatch baseado no **STM32WB55**, com foco em baixíssimo consumo, multitarefa em tempo real e uma interface elegante inspirada nos relógios clássicos da Seiko.

## 🚀 Destaques do Projeto

-   **Interface Seiko Style**: Design de alto contraste (Preto no Branco) com janela de data clássica, logotipo "SHM-5" e menus profissionais.
-   **Sistema de Menus com Scroll**: Navegação fluida em listas longas com rolagem automática e indicadores visuais.
-   **Configurações Reais**: Ajuste de formato de hora (12/24h), brilho, vibração, temas e ativação por pulso diretamente no relógio.
-   **Ecossistema de Jogos (1 Botão)**:
    -   **Doom Lite**: Motor 3D com raycasting e inimigos animados.
    -   **Temple Watch**: Runner 3D de evasão.
    -   **Doodle Watch**: Jogo de plataforma vertical infinito.
    -   **Snake & Flappy**: Clássicos adaptados.
-   **Arquitetura Robusta**: Baseada em **FreeRTOS** com separação clara entre drivers de hardware (HAL), ferramentas de UI (Toolkit) e lógica de telas.

## 🛠️ Arquitetura e Tecnologia

-   **Microcontrolador**: STM32WB55 (Dual Core, ARM Cortex-M4).
-   **Display**: E-Paper / MIP (Memory-In-Pixel) 128x128.
-   **RTOS**: FreeRTOS para gerenciamento de tarefas de Sensores, UI e Eventos.
-   **Gráficos**: Biblioteca GFX otimizada para monocromático com fontes customizadas.
-   **Build System**: CMake com suporte a Presets.

## 📁 Documentação Detalhada

Para uma visão técnica aprofundada das camadas, fluxos de dados e estrutura do código, consulte:
-   [Arquitetura do Sistema](docs/ARCHITECTURE.md)
-   [Documentação Técnica Completa](docs/PROJECT_DOC.md)

## ⚡ Como Compilar

Certifique-se de ter o `arm-none-eabi-gcc` e o `CMake` instalados.

```bash
# Configurar e Gerar o Build
cmake --preset chip-debug

# Compilar o Firmware
cmake --build --preset chip-debug
```

O binário gerado estará em `build/debug/firmware.elf`.

---
*Este projeto foi desenvolvido com foco em eficiência energética e elegância visual.*
