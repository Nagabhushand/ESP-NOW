#include <string.h>
#include "esp_wifi.h"
#include "esp_now.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_mac.h"

static const char *TAG = "ESP_NOW_RECEIVER";

static void on_data_recv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
    char incomingData[50];
    memcpy(&incomingData, data, sizeof(incomingData));
    ESP_LOGI(TAG, "Data received from %02x:%02x:%02x:%02x:%02x:%02x",MAC2STR(recv_info->src_addr));
    ESP_LOGI(TAG, "%s", incomingData);
}

void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_ERROR_CHECK(esp_now_init());
    ESP_LOGI(TAG, "ESP-NOW initialized.");

    ESP_ERROR_CHECK(esp_now_register_recv_cb(on_data_recv));

    ESP_LOGI(TAG, "Receiver ready to receive data...");
}
