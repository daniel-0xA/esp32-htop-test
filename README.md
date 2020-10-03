# esp32-htop-test

esp32-htop-test based on 'esp-idf\examples\system\freertos\real_time_stats' showing the amount of CPU usage for each task and each CPU core.

This was compiled using ESP32-IDF v4.1

in sdkconfig.defaults
CONFIG_FREERTOS_USE_TRACE_FACILITY=y
CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS=y
