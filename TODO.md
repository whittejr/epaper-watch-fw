# Lista de Tarefas, Melhorias e Ideias (TODO)

Este documento registra possíveis mudanças, ajustes arquiteturais e ideias para o firmware do E-Paper Watch.

---

## 1. Melhorias no Core e RTOS

### 1.1. Refatorar `bsp_delay_ms` (Transição Inteligente de Delays)
**Descrição:** Atualizar a função genérica de atraso da placa (BSP) para ser ciente do estado do FreeRTOS.
**Motivo:** Atualmente, a função `bsp_delay_ms()` (em `lib/bsp/src/gpio.c`) repassa a chamada diretamente para o `HAL_Delay(ms)`. O `HAL_Delay` faz "busy-wait" ocupando 100% da CPU, o que é um comportamento que destrói as vantagens do RTOS se chamado dentro de uma Task.
**Solução Sugerida:**
Adicionar uma camada de inteligência que verifica se o kernel do FreeRTOS já iniciou o escalonador.
- Se **NÃO** iniciou (ex: durante o boot da placa/sensores): Use `HAL_Delay(ms)`.
- Se **JÁ** iniciou (ex: durante a execução de uma das Tasks): Use `vTaskDelay(pdMS_TO_TICKS(ms))`.

**Trecho de Código Sugerido:**
```c
void bsp_delay_ms(uint32_t ms) {
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        // Escalonador rodando: dorme sem travar a CPU
        vTaskDelay(pdMS_TO_TICKS(ms));
    } else {
        // Escalonador parado (boot): usa delay bloqueante da HAL
        HAL_Delay(ms);
    }
}
```

---
*(Adicione novas ideias abaixo desta linha)*
