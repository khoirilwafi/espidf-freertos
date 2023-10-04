#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

void i2c_init(void)
{
    // i2c configuration as master
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 21,
        .scl_io_num = 22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };

    // send a configuration
    i2c_param_config(I2C_NUM_0, &i2c_config);
    // install i2c driver
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);

    printf("i2c config done\r\n");
}

void app_main(void)
{
    i2c_init();

    while (1)
    {
        for (uint8_t i = 0; i < 128; i++)
        {
            // create command
            i2c_cmd_handle_t cmd = i2c_cmd_link_create();

            // add start condition
            i2c_master_start(cmd);

            // add the address of slave device
            i2c_master_write_byte(cmd, (i << 1), true);

            // add stop condition
            i2c_master_stop(cmd);

            // send the command
            esp_err_t status = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(500));
            i2c_cmd_link_delete(cmd);

            if (status == ESP_OK)
            {
                printf("device found at 0x%.2x\r\n", i);
                vTaskDelay(pdMS_TO_TICKS(3000));
            }
            else
            {
                printf(".\r\n");
                vTaskDelay(pdMS_TO_TICKS(100));
            }
        }

        printf("scanning done, waiting to next scanning ... \r\n");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}