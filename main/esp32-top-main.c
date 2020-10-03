/* esp32-htop based on 'Hello World' Example and https://github.com/versamodule/ESP32-HTOP.git

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_task_wdt.h"
#include <math.h>


#ifdef CONFIG_IDF_TARGET_ESP32
#define CHIP_NAME "ESP32"
#endif

#ifdef CONFIG_IDF_TARGET_ESP32S2BETA
#define CHIP_NAME "ESP32-S2 Beta"
#endif

#include "esp32_htop.h"


#define PRO_CPU	0
#define APP_CPU	1

// Report Detailes stats or not
// 0 = Just printf Cpu0 & Cpu1 stats
// 1 = Print all thread stats as well.
#define DETAIL_LEVEL	1

#define PI 3.14159265

void workload_task(void *arg){
    for (int i = 9999999992; i >= 0; i--) {
        //printf("Restarting in %d seconds...\n", i);
		if(i%10000 == 0){
			vTaskDelay(15 / portTICK_PERIOD_MS);
		}

		double x, ret, val;
		x = 45.0;
		val = PI / 180;
		ret = sin(x*val);
		// esp_task_wdt_reset();
   
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}

/*
 * Macro to check the outputs of TWDT functions and trigger an abort if an
 * incorrect code is returned.
 */
#define CHECK_ERROR_CODE(returned, expected) ({                        \
            if(returned != expected){                                  \
                printf("TWDT ERROR\n");                                \
                abort();                                               \
            }                                                          \
})

void app_main(void)
{
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU cores, WiFi%s%s, ",
            CHIP_NAME,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    // deactivate WDT for IDLE tasks
    for(int i = 0; i < portNUM_PROCESSORS; i++){
		//unsubscribe idle task
		// CHECK_ERROR_CODE(esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(i)), ESP_OK);     //Unsubscribe Idle Task from TWDT
		// CHECK_ERROR_CODE(esp_task_wdt_status(xTaskGetIdleTaskHandleForCPU(i)), ESP_ERR_NOT_FOUND);      //Confirm Idle task has unsubscribed
	}
	
	//Create and start stats task
	xTaskCreatePinnedToCore(stats_task, "stats", 4096, (void *) DETAIL_LEVEL, STATS_TASK_PRIO, NULL, tskNO_AFFINITY);
	
	TaskHandle_t taskHandle;
	//Create and start worker task
	xTaskCreatePinnedToCore(workload_task, "workload_task1", 4096, (void *) DETAIL_LEVEL, 5, &taskHandle, tskNO_AFFINITY);
	xTaskCreatePinnedToCore(workload_task, "workload_task2", 4096, (void *) DETAIL_LEVEL, 5, &taskHandle, tskNO_AFFINITY);
	xTaskCreatePinnedToCore(workload_task, "workload_task3", 4096, (void *) DETAIL_LEVEL, 5, &taskHandle, tskNO_AFFINITY);
	xTaskCreatePinnedToCore(workload_task, "workload_task4", 4096, (void *) DETAIL_LEVEL, 5, &taskHandle, tskNO_AFFINITY);
	xTaskCreatePinnedToCore(workload_task, "workload_task5", 4096, (void *) DETAIL_LEVEL, 5, &taskHandle, tskNO_AFFINITY);
	
	// xTaskCreatePinnedToCore(workload_task, "workload_task11", 4096, (void *) DETAIL_LEVEL, 5, &taskHandle, tskNO_AFFINITY);
	// xTaskCreatePinnedToCore(workload_task, "workload_task12", 4096, (void *) DETAIL_LEVEL, 5, &taskHandle, tskNO_AFFINITY);
	// xTaskCreatePinnedToCore(workload_task, "workload_task13", 4096, (void *) DETAIL_LEVEL, 5, &taskHandle, tskNO_AFFINITY);
	// xTaskCreatePinnedToCore(workload_task, "workload_task14", 4096, (void *) DETAIL_LEVEL, 5, &taskHandle, tskNO_AFFINITY);
	// xTaskCreatePinnedToCore(workload_task, "workload_task15", 4096, (void *) DETAIL_LEVEL, 5, &taskHandle, tskNO_AFFINITY);

}
