#include <ESP8266WiFi.h>
extern "C" {
  #include <espnow.h>
}

// Піни для керування мотором через драйвер L298N
#define MOTOR_IN1  D1
#define MOTOR_IN2  D2

// MAC-адреса ESP8266 (має співпадати з тією, що у передавача)
uint8_t myMac[] = {0x94, 0xb9, 0x7e, 0x15, 0x57, 0x0b};

// Пороги для визначення напрямку руху (мають співпадати з передавачем)
const int HIGH_THR = 3000;
const int LOW_THR  = 1000;


// Структура для передачі даних. Має бути ідентичною на приймачі і передавачі.
typedef struct struct_message {
  int x;
  int y;
  uint8_t button;
} struct_message;

// Ця функція викликається, коли отримано дані
void onDataRecv(uint8_t *mac, uint8_t *data, uint8_t len) {

  // Копіюємо дані в нашу структуру
  struct_message receivedData;
  memcpy(&receivedData, data, sizeof(receivedData));

  // Виводимо отримані значення
  Serial.print("Отримано від ");
  Serial.print(WiFi.macAddress());
  Serial.print(": X=");
  Serial.print(receivedData.x);
  Serial.print(" Y=");
  Serial.print(receivedData.y);
  Serial.print(" Кнопка=");
  Serial.println(receivedData.button);

  // Логіка керування мотором
  if (receivedData.y > HIGH_THR) {
    // Рух вперед
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    Serial.println("Вперед");
  } else if (receivedData.y < LOW_THR) {
    // Рух назад
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, HIGH);
    Serial.println("Назад");
  } else {
    // Зупинка мотора
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
    Serial.println("Стоп");
  }
}


void setup() {
  Serial.begin(115200);

  // Налаштування пінів мотора як виходи
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  
  // Налаштування режиму WiFi (Station)
  WiFi.mode(WIFI_STA);

  // Ініціалізація ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed!");
    return;
  }

  // Встановлення ролі, яка дозволяє і відправляти, і приймати
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  // Реєстрація функції, яка буде обробляти отримані дані
  esp_now_register_recv_cb(onDataRecv);

  // Додаємо передавача як peer. Це може покращити зв'язок.
  esp_now_add_peer(myMac, ESP_NOW_ROLE_SLAVE, 0, NULL, 0);

  Serial.println("Готовий приймати дані через ESP-NOW");
}

void loop() {
  // Цикл loop не містить коду, оскільки обробка даних відбувається в onDataRecv
}