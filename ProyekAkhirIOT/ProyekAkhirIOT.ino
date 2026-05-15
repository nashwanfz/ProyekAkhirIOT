#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <PubSubClient.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <OneWire.h>
#include <DallasTemperature.h>

// ================= WIFI =================
const char* ssid = "A35";
const char* password = "monyetlu";

// ================= TELEGRAM =================
#define BOT_TOKEN "8603321188:AAEWG3oPfX3YRMI471weiybLoe-BAOq0-hw"
#define CHAT_ID "1262953121"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// ================= MQTT =================
const char* mqtt_server = "broker.emqx.io";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ================= SOIL SENSOR =================
#define SOIL_SENSOR 33

// ================= DS18B20 =================
#define ONE_WIRE_BUS 25

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ================= LED =================
#define LED_MERAH 19
#define LED_KUNING 18
#define LED_HIJAU 4

// ================= LCD =================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= VARIABLE =================
int soilValue = 0;
String soilStatus = "NORMAL";

float temperature = 0;
String tempStatus = "NORMAL";

// ================= UPDATE LCD =================
void updateLCD(String line1, String line2) {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(line1);

  lcd.setCursor(0, 1);
  lcd.print(line2);
}

// ================= UPDATE LED =================
void updateLED() {

  digitalWrite(LED_MERAH, LOW);
  digitalWrite(LED_KUNING, LOW);
  digitalWrite(LED_HIJAU, LOW);

  // ================= HIJAU =================
  if (soilStatus == "SANGAT BASAH" ||
      soilStatus == "BASAH") {

    digitalWrite(LED_HIJAU, HIGH);
  }

  // ================= KUNING =================
  else if (soilStatus == "LEMBAB") {

    digitalWrite(LED_KUNING, HIGH);
  }

  // ================= MERAH =================
  else if (soilStatus == "KERING") {

    digitalWrite(LED_MERAH, HIGH);
  }
}

// ================= BACA SOIL =================
void bacaSoil() {

  soilValue = analogRead(SOIL_SENSOR);

  // ================= STATUS =================
  if (soilValue < 1200) {

    soilStatus = "SANGAT BASAH";

  }

  else if (soilValue < 2200) {

    soilStatus = "BASAH";

  }

  else if (soilValue < 3200) {

    soilStatus = "LEMBAB";

  }

  else {

    soilStatus = "KERING";
  }

  updateLED();
}

// ================= BACA TEMPERATURE =================
void bacaTemperature() {

  sensors.requestTemperatures();

  temperature = sensors.getTempCByIndex(0);

  // ================= STATUS SUHU =================
  if (temperature >= 24 && temperature <= 28) {

    tempStatus = "OPTIMAL";

  }

  else if (temperature < 15) {

    tempStatus = "TERLALU DINGIN";

  }

  else if (temperature > 33) {

    tempStatus = "TERLALU PANAS";

  }

  else {

    tempStatus = "MASIH DAPAT DITOLERANSI";
  }
}

// ================= MQTT CALLBACK =================
void callback(char* topic, byte* payload, unsigned int length) {

  String message = "";

  for (int i = 0; i < length; i++) {

    message += (char)payload[i];
  }

  Serial.println("MQTT Topic : " + String(topic));
  Serial.println("MQTT Message : " + message);

  // ================= REQUEST =================
  if (String(topic) == "wan/iot/request") {

    // ================= SUHU =================
    if (message == "Suhu") {

      bacaTemperature();

      String data =
        "Suhu : " + String(temperature) +
        " C | " + tempStatus;

      mqttClient.publish(
        "wan/iot/Suhu",
        data.c_str()
      );

      updateLCD(
        "Temp:" + String(temperature) + "C",
        tempStatus
      );
    }

    // ================= KELEMBAPAN =================
    else if (message == "Kelembapan") {

      bacaSoil();

      String data =
        "Nilai : " + String(soilValue) +
        " | " + soilStatus;

      mqttClient.publish(
        "wan/iot/Kelembapan",
        data.c_str()
      );

      updateLCD(
        "Soil:" + soilStatus,
        String(soilValue)
      );
    }
  }
}

// ================= MQTT RECONNECT =================
unsigned long lastReconnect = 0;

void reconnectMQTT() {

  if (millis() - lastReconnect > 3000) {

    lastReconnect = millis();

    Serial.println("Menghubungkan MQTT...");

    String clientId = "wan-" + String(random(1000, 9999));

    if (mqttClient.connect(clientId.c_str())) {

      Serial.println("MQTT Connected");

      mqttClient.subscribe("wan/iot/#");

      mqttClient.publish(
        "wan/iot/koneksi",
        "ESP32 Connected"
      );

      updateLCD("MQTT Connected", "");

    } else {

      Serial.print("Gagal MQTT : ");
      Serial.println(mqttClient.state());
    }
  }
}

// ================= SETUP =================
void setup() {

  Serial.begin(115200);
  randomSeed(micros());

  pinMode(SOIL_SENSOR, INPUT);

  // ================= DS18B20 =================
  sensors.begin();

  // ================= LED =================
  pinMode(LED_MERAH, OUTPUT);
  pinMode(LED_KUNING, OUTPUT);
  pinMode(LED_HIJAU, OUTPUT);

  // ================= LCD =================
  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();

  updateLCD("Menghubungkan", "WiFi...");

  // ================= WIFI =================
  WiFi.begin(ssid, password);

  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");

  // ================= TELEGRAM =================
  client.setInsecure();

  bool test = bot.sendMessage(
    CHAT_ID,
    "ESP32 berhasil terkoneksi ke Bot Telegram!",
    ""
  );

  if (test) {

    Serial.println("Pesan Telegram berhasil dikirim");

    updateLCD("Bot Connected", "Siap Digunakan");

  } else {

    Serial.println("Gagal mengirim pesan");

    updateLCD("Bot Gagal", "Terkoneksi");
  }

  // ================= MQTT =================
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback);

  delay(2000);

  lcd.clear();
}

// ================= HANDLE MESSAGE =================
void handleNewMessages(int numNewMessages) {

  for (int i = 0; i < numNewMessages; i++) {

    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    text.trim();

    Serial.println("==========");
    Serial.println("Pesan : " + text);

    // ================= MENU =================
    if (text == "/start" || text == "/menu") {

      String menu = "MENU BOT SENSOR\n\n";
      menu += "/soil -> Cek kondisi tanah\n";
      menu += "/temperature -> Cek suhu";

      bot.sendMessage(chat_id, menu, "");

      updateLCD("Command:", "/menu");
    }

    // ================= SOIL =================
    else if (text == "/soil") {

      bacaSoil();

      String msg = "STATUS SOIL SENSOR\n\n";

      msg += "Nilai Sensor : ";
      msg += String(soilValue);
      msg += "\n";

      msg += "Kondisi Tanah : ";
      msg += soilStatus;

      bot.sendMessage(chat_id, msg, "");

      updateLCD(
        "Soil:" + soilStatus,
        "Value:" + String(soilValue)
      );

      Serial.println(msg);
    }

    // ================= TEMPERATURE =================
    else if (text == "/temperature") {

      bacaTemperature();

      String msg = "STATUS TEMPERATURE\n\n";

      msg += "Suhu : ";
      msg += String(temperature);
      msg += " C\n";

      msg += "Status : ";
      msg += tempStatus;

      bot.sendMessage(chat_id, msg, "");

      updateLCD(
        "Temp:" + String(temperature) + "C",
        tempStatus
      );

      Serial.println(msg);
    }

    // ================= UNKNOWN =================
    else {

      bot.sendMessage(chat_id,
                      "Command tidak dikenali!\nKetik /menu",
                      "");

      updateLCD("Command", "Tidak Valid");
    }
  }
}

// ================= LOOP =================
void loop() {

  // ================= MQTT =================
  if (!mqttClient.connected()) {

    reconnectMQTT();
  }

  mqttClient.loop();

  // ================= TELEGRAM =================
  int numNewMessages =
    bot.getUpdates(bot.last_message_received + 1);

  while (numNewMessages) {

    Serial.println("Ada pesan Telegram masuk");

    handleNewMessages(numNewMessages);

    numNewMessages =
      bot.getUpdates(bot.last_message_received + 1);
  }

  delay(1000);
}
