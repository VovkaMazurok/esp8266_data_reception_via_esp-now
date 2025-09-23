#include <ESP8266WiFi.h>
extern "C" {
  #include <espnow.h>
}

// MAC-адреса ESP8266 (має співпадати з тією, що у передавача)
uint8_t myMac[] = {0x94, 0xb9, 0x7e, 0x15, 0x57, 0x0b};

void onDataRecv(uint8_t *mac, uint8_t *data, uint8_t len) {

  // Для передачі тексту
  // Serial.print("Отримано дані: ");
  // for (uint8_t i = 0; i < len; i++) {
  //   Serial.write(data[i]);
  // }
  // Serial.println();
  if (len == 1) {
    Serial.print("Стан кнопки: ");
    Serial.println(data[0]);
  } else {
    Serial.println("Отримано невідомі дані");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  wifi_set_macaddr(STATION_IF, myMac);

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed!");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataRecv);
  Serial.println("Готовий приймати дані через ESP-NOW");
}

void loop() {
  // нічого не потрібно тут
}