# Arquitetura do Sistema - E-Paper Watch FW

Este documento descreve a organização das tarefas, fluxos de dados e interações entre os componentes do sistema.

## Diagrama de Tarefas e Comunicação (Mermaid)

```mermaid
graph TD
    subgraph Hardware
        MAX30102[MAX30102 Oximeter]
        BTN[Physical Button]
        RTC[Internal RTC]
        EPD[E-Paper Display]
    end

    subgraph "Camada HAL / Drivers"
        OX_HAL[oximeter_hal.c]
        BTN_DRV[button.c]
        RTC_DRV[rtc.c]
        DISP_DRV[display_hal.c]
    end

    subgraph "FreeRTOS Tasks"
        TaskOX[vOximeterTask]
        TaskBTN[vButtonTask]
        TaskALM[vAlarmTask]
        TaskUI[vUITask]
    end

    subgraph "Filas e Estado Global"
        QueueEVT((xSystemEventQueue))
        StateOX[app_oximeter State]
    end

    %% Fluxos do Oximetro
    MAX30102 -->|I2C| OX_HAL
    OX_HAL -->|FIFO Read| TaskOX
    TaskOX -->|Update| StateOX

    %% Fluxos de Eventos
    BTN --> BTN_DRV
    BTN_DRV --> TaskBTN
    TaskBTN -->|EVENT_BTN_NEXT/SELECT| QueueEVT

    RTC --> RTC_DRV
    RTC_DRV --> TaskALM
    TaskALM -->|EVENT_TICK_1SEC| QueueEVT

    %% Fluxo da UI
    QueueEVT -->|Receive| TaskUI
    TaskUI -->|Manager_ProcessEvent| UIMgr[UI Manager]
    UIMgr -->|Read Results| StateOX
    TaskUI -->|Update Screen| DISP_DRV
    DISP_DRV -->|SPI| EPD

    %% Estilização
    style QueueEVT fill:#f9f,stroke:#333,stroke-width:2px
    style StateOX fill:#bbf,stroke:#333,stroke-width:2px
    style TaskUI fill:#dfd,stroke:#333,stroke-width:2px
```

## Descrição das Camadas

1.  **Oximeter Flow**: A `vOximeterTask` faz o polling do sensor a 100Hz (via HAL), aplica os filtros CMSIS-DSP e atualiza o estado global de Heart Rate e SpO2.
2.  **Event Flow**: Botões e Alarme (via RTC) enviam eventos para a `xSystemEventQueue`.
3.  **UI Task**: É o consumidor central. Ela acorda quando recebe um evento, delega para o `UI_Manager` (que decide qual tela mostrar) e atualiza o display E-Paper.
4.  **Data Isolation**: As telas da UI não processam dados brutos; elas apenas consultam o estado processado pelo aplicativo de oximetria ou acelerômetro.
