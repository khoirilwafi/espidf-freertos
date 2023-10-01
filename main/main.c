#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

int shared_resource = 0;
static SemaphoreHandle_t mutex;

void task_pertama(void *parameter)
{
    while (1)
    {
        if (xSemaphoreTake(mutex, 0) == pdTRUE)
        {
            shared_resource++;
            printf("task pertama - %d\r\n", shared_resource);

            // misalnya pengolahan data memerlukan waktu 2 detik
            vTaskDelay(pdMS_TO_TICKS(2000));

            xSemaphoreGive(mutex);
        }
        else
        {
            printf("task pertama - shared resource sedang digunakan oleh task lain\r\n");
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void task_kedua(void *parameter)
{
    while (1)
    {
        if (xSemaphoreTake(mutex, 0) == pdTRUE)
        {
            shared_resource++;
            printf("task kedua   - %d\r\n", shared_resource);

            xSemaphoreGive(mutex);
        }
        else
        {
            printf("task kedua   - shared resource sedang digunakan oleh task lain\r\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    mutex = xSemaphoreCreateMutex();

    xTaskCreate(task_pertama, "pertama", 2048, NULL, 1, NULL);
    xTaskCreate(task_kedua, "kedua", 2048, NULL, 1, NULL);
}
