# SHM-5 Watch Firmware

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Build](https://img.shields.io/badge/Build-CMake-green.svg)](#-building)

[English](#english) | [Português](#português)

---

<a name="english"></a>
## English

Smartwatch firmware for the STM32WB55 platform, designed for high-efficiency multitasking and a clean monochrome interface.

### Features
*   **OS:** Powered by FreeRTOS for reliable real-time operation.
*   **Display:** Native support for Sharp Memory LCD (128x128) with ultra-low power consumption.
*   **Applications:** Includes system utilities and specialized apps (Doom Lite, Temple Watch, etc.).
*   **Sensors:** Integrated support for motion tracking and biometric data processing.

### Technical Specifications
| Category | Specification |
| :--- | :--- |
| **Microcontroller** | STM32WB55VG (ARM Cortex-M4/M0+) |
| **Kernel** | FreeRTOS v10.x |
| **Graphics** | Custom monochrome GFX toolkit |
| **Build System** | CMake + GCC Arm Toolchain |

### Building
```bash
cmake --preset chip-debug
cmake --build --preset chip-debug
```

---

<a name="português"></a>
## Português

Firmware para smartwatch baseado no STM32WB55, focado em multitarefa eficiente e uma interface monocromática limpa.

### Recursos
- **Sistema:** Baseado em FreeRTOS para operação robusta em tempo real.
- **Display:** Suporte nativo para Sharp Memory LCD (128x128) de baixíssimo consumo.
- **Aplicações:** Inclui ferramentas de sistema e apps especializados (Doom Lite, Temple Watch, etc).
- **Sensores:** Suporte integrado para rastreamento de movimento e biometria.

### Especificações Técnicas
| Categoria | Especificação |
| :--- | :--- |
| **Microcontrolador** | STM32WB55VG (ARM Cortex-M4/M0+) |
| **Kernel** | FreeRTOS v10.x |
| **Gráficos** | Biblioteca GFX customizada |
| **Build System** | CMake + GCC Arm Toolchain |

### Compilação
```bash
cmake --preset chip-debug
cmake --build --preset chip-debug
```
