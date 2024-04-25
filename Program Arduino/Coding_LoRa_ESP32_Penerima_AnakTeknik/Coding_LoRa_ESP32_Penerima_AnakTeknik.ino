#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "KelasRobot.com";
const char* password = "k3l4sr0b0t";
// Ganti dengan token bot dan chat ID Anda
const char* BOT_TOKEN = "7165753105:AAGwfx16mtOFMSfHGXpFxCQJKMVlTEs0Qt0";
const char* CHAT_ID = "262249300";

boolean flag = false;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver");
  LoRa.setPins(5, 14, 2); //ss, rst, dio0
  while (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    delay(500);
  }
  LoRa.setSyncWord(0xF3);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  Serial.println("Tersambung ke WiFi!");

}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet '");
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData);

      int tinggi = LoRaData.toInt();

      if( (tinggi >= 85) &&  (flag == false) ){
          kirimPesanTelegram(String() + "Peringatan Ketinggian:" + tinggi);
          flag = true;
      }

      if( tinggi < 85){
        flag = false;
      }

    }
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}

void kirimPesanTelegram(String pesan) {
  HTTPClient http;
  http.begin("https://api.telegram.org/bot" + String(BOT_TOKEN) + "/sendMessage?chat_id=" + String(CHAT_ID) + "&text=" + pesan);
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    Serial.print("Pesan Terkirim ke Telegram, kode respons: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Gagal mengirim pesan, kode respons: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}
