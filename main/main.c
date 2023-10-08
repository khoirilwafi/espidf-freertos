#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

esp_err_t status;
spi_device_handle_t spi;
uint8_t number = 0;

void meter_display(uint8_t channel, uint8_t value)
{
    const uint8_t meter_index[9] = {
        0xff,
        0x7f,
        0x3f,
        0x1f,
        0x0f,
        0x07,
        0x03,
        0x01,
        0x00,
    };

    uint8_t data[2] = {channel, meter_index[value]};

    spi_transaction_t trx = {
        .tx_buffer = data,
        .length = 2 * 8,
    };

    status = spi_device_polling_transmit(spi, &trx);

    if (status != ESP_OK)
    {
        printf("gagal mengirim data\r\n");
    }
}

void app_main(void)
{
    spi_bus_config_t spi_config = {
        .miso_io_num = 19,
        .mosi_io_num = 23,
        .sclk_io_num = 18,
        .max_transfer_sz = 3,
    };

    spi_device_interface_config_t shift_register = {
        .clock_speed_hz = 100000,
        .mode = 0,
        .spics_io_num = 5,
        .queue_size = 1,
    };

    status = spi_bus_initialize(SPI3_HOST, &spi_config, SPI_DMA_DISABLED);

    if (status != ESP_OK)
    {
        printf("gagal melakukan konfigurasi spi\r\n");
        while (1)
        {
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }

    status = spi_bus_add_device(SPI3_HOST, &shift_register, &spi);

    if (status != ESP_OK)
    {
        printf("gagal menambahkan device spi\r\n");
        while (1)
        {
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }

    printf("konfigurasi selesai\r\n");

    while (1)
    {
        for (uint8_t index = 0; index <= 6; index++)
        {
            meter_display(1, index);
            vTaskDelay(pdMS_TO_TICKS(80));
        }
    }
}