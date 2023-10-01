#include "stdio.h"
#include "math.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

static QueueHandle_t msg_queue;

void sender_task(void *parameter)
{
    int number = 0;

    while (1)
    {
        number = (int)random();

        if (xQueueSend(msg_queue, (void *)&number, 0) == pdFALSE)
        {
            printf("antrian penuh\r\n");
        }
        else
        {
            printf("%d berhasil terkirim\r\n", number);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void receiver_task(void *parameter)
{
    int recv_number = 0;

    while (1)
    {
        if (xQueueReceive(msg_queue, &recv_number, 0) == pdTRUE)
        {
            printf("mendapatkan data dari antrian : %d\r\n", recv_number);
        }
        else
        {
            printf("antrian kosong\r\n");
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(void)
{
    msg_queue = xQueueCreate(10, sizeof(int));

    xTaskCreate(sender_task, "sender", 2048, NULL, 1, NULL);
    xTaskCreate(receiver_task, "receiver", 2048, NULL, 1, NULL);
}
