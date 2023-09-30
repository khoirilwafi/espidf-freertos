#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

void read_serial_task(void *parameter)
{
    int data_length = 0;
    uint8_t *read_buffer = (uint8_t *)pvPortMalloc(1024);

    if (read_buffer == NULL)
    {
        printf("gagal membuat alokasi memori uart. menghapus task\r\n");
        vTaskDelete(NULL);
    }

    while (1)
    {
        data_length = uart_read_bytes(UART_NUM_0, read_buffer, 1024, pdMS_TO_TICKS(100));

        if (data_length > 0)
        {
            // menambahkan ending teks
            read_buffer[data_length] = '\0';

            // menampilkan teks
            printf("%d karakter diterima: %s\r\n", data_length, read_buffer);
        }
    }
}

void blink_task(void *parameter)
{
    gpio_set_level(23, 0);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpio_set_level(23, 1);
    vTaskDelay(pdMS_TO_TICKS(500));
}

void app_main(void)
{
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, 2, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0);

    gpio_set_direction(23, GPIO_MODE_OUTPUT);

    xTaskCreate(read_serial_task, "serial task", 2048, NULL, 1, NULL);
    xTaskCreate(blink_task, "blink task", 1024, NULL, 1, NULL);
}
