/**
 * @file    main.c
 * @brief   Leitura de BPM com MAX30102 (Buffer Acumulativo)
 * @version 1.0.0
 * @author  Alessandro Davi
 * @date    2025-12-31
 */

#include "gpio.h"
#include "max30102.h"
#include "max30102_interface.h"
#include "ssd1681.h"
#include "ssd1681_interface.h"
#include "stm32wbxx_hal.h"
#include <stdio.h>

/* --- CONFIGURAÇÕES --- */
#define TAMANHO_BUFFER_ANALISE 100 // 100 amostras a 50Hz = 2 segundos de histórico

/* --- VARIÁVEIS GLOBAIS --- */
static ssd1681_handle_t gs_handle;

/* MAX30102 Handles e Buffers */
max30102_handle_t max_handle;

// DEBUG
volatile int32_t debug_sinal_plot = 0;
volatile int32_t debug_limiar_plot = 0; // Para ver a linha de corte
volatile int32_t debug_bpm_visual = 0;

// Buffers Curtos (Leitura Imediata do Hardware - 32 amostras máx)
uint32_t gs_red_buffer[32];
uint32_t gs_ir_buffer[32];

// Buffer Longo (Acumulador para o Algoritmo - 100 amostras)
uint32_t ir_buffer_acumulado[TAMANHO_BUFFER_ANALISE];
uint16_t indice_acumulado = 0;

volatile uint8_t g_sensor_data_ready = 0;

/* --- PROTÓTIPOS --- */
int calcular_bpm(uint32_t *buffer, int tamanho);

/* --- CALLBACK DA INTERRUPÇÃO --- */
void max30102_interface_receive_callback(uint8_t type) {
    switch (type) {
    case MAX30102_INTERRUPT_STATUS_FIFO_FULL:
    case MAX30102_INTERRUPT_STATUS_PPG_RDY: {
        g_sensor_data_ready = 1;
        break;
    }
    default:
        break;
    }
}

/* --- MAIN --- */
int main(void) {
    // 1. Inicialização do Hardware
    HAL_Init();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_I2C1_CLK_ENABLE();

    // 2. Inicialização do Sensor
    // Vincula as funções de hardware ao driver
    // max30102_interface_bind(&max_handle);

    // Configuração padrão
    max30102_interface_default_config(&max_handle);

    // AJUSTE CRÍTICO: Forçar 50Hz para a matemática (100 amostras = 2s) funcionar
    // max30102_set_spo2_sample_rate(&max_handle, MAX30102_SPO2_SAMPLE_RATE_50_HZ);

    // Inicializa pino de interrupção
    max_irq_gpio_init();

    printf("Sistema Iniciado. Coloque o dedo no sensor.\n");

    while (1) {
        // Verifica se o hardware avisou que tem dados novos
        if (g_sensor_data_ready == 1) {
            g_sensor_data_ready = 0; // Baixa a bandeira

            uint8_t len = 32; // Tenta ler até 32 amostras

            // Lê do sensor (enche os buffers pequenos gs_red/gs_ir)
            if (max30102_read(&max_handle, gs_red_buffer, gs_ir_buffer, &len) == 0) {

                // SUCESSO NA LEITURA
                // Agora transferimos do "copinho" (buffer pequeno) para o "balde" (acumulado)
                for (int i = 0; i < len; i++) {
                    if (indice_acumulado < TAMANHO_BUFFER_ANALISE) {
                        ir_buffer_acumulado[indice_acumulado] = gs_ir_buffer[i];
                        indice_acumulado++;
                    }
                    debug_sinal_plot = gs_ir_buffer[i];
                    debug_limiar_plot = 40;
                }

                // --- PROCESSAMENTO ---
                // O balde encheu? (Chegamos em 100 amostras?)
                if (indice_acumulado >= TAMANHO_BUFFER_ANALISE) {

                    // Executa o algoritmo matemático
                    int bpm = calcular_bpm(ir_buffer_acumulado, TAMANHO_BUFFER_ANALISE);
                    debug_bpm_visual = bpm;

                    if (bpm > 0) {
                        printf(">>> BPM: %d <<<\n", bpm);
                        // ssd1681_show_bpm(bpm); // Exibir no e-paper
                    } else {
                        printf("Leitura instavel ou sem dedo.\n");
                    }

                    // Esvazia o balde para começar os próximos 2 segundos
                    indice_acumulado = 0;
                }

            } else {
                // Falha na comunicação I2C
                printf("Erro de Leitura I2C.\n");
            }
        }
    }
}

/* --- FUNÇÃO DO ALGORITMO (Limiar Automático) --- */
int calcular_bpm(uint32_t *buffer, int tamanho) {
    long soma = 0;
    long media_dc = 0;

    // Variáveis para encontrar o Máximo e Mínimo do sinal AC
    int32_t max_ac = 0;
    int32_t min_ac = 0; // Opcional, mas bom para debug

    // Configuração de Tempo (Mantemos o bloqueio de eco)
    int distancia_minima = 20; // 400ms de cegueira pós-pico

    int indices_picos[20];
    int total_picos_encontrados = 0;
    int ultimo_pico_index = -100;

    // 1. Calcula Média (DC)
    for (int i = 0; i < tamanho; i++)
        soma += buffer[i];
    media_dc = soma / tamanho;

    if (media_dc < 50000)
        return 0; // Sem dedo

    // 2. PASSO EXTRA: Descobrir a altura máxima da onda (Auto-Calibração)
    for (int i = 0; i < tamanho; i++) {
        int32_t valor_ac = buffer[i] - media_dc;
        if (valor_ac > max_ac)
            max_ac = valor_ac;
    }

    // O Pulo do Gato: O limiar agora é 60% do tamanho da SUA onda.
    // Se sua onda for gigante (1000), o limiar será 600.
    // Se sua onda for pequena (100), o limiar será 60.
    int32_t limiar_dinamico = (max_ac * 60) / 100;

    // Segurança: Se o sinal for muito lixo (muito pequeno), ignora
    if (max_ac < 50)
        return 0; // Ruído puro

    // 3. Detecta Picos usando o Limiar Dinâmico
    int32_t anterior, atual;

    for (int i = 1; i < tamanho; i++) {
        anterior = buffer[i - 1] - media_dc;
        atual = buffer[i] - media_dc;

        // Visualização no Plotter (opcional)
        printf("%ld,%ld,%ld\r\n", atual, (long)0, limiar_dinamico);

        // Subida
        if (anterior < 0 && atual >= 0) {
            // Usa o limiar calculado na hora
            if (atual > limiar_dinamico) {
                if ((i - ultimo_pico_index) > distancia_minima) {
                    if (total_picos_encontrados < 20) {
                        indices_picos[total_picos_encontrados] = i;
                        total_picos_encontrados++;
                    }
                    ultimo_pico_index = i;
                }
            }
        }
    }

    // 4. Cálculo de BPM
    if (total_picos_encontrados < 2)
        return 0;

    int primeiro_pico = indices_picos[0];
    int ultimo_pico = indices_picos[total_picos_encontrados - 1];

    float distancia_media = (float)(ultimo_pico - primeiro_pico) / (total_picos_encontrados - 1);
    int bpm_preciso = (int)(3000.0 / distancia_media);

    return bpm_preciso;
}