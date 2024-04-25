#include <SPI.h>
#include <LoRa.h>
#include <NewPing.h>

NewPing sonar1(3, 3, 400); //trig, echo, max dist
float jarak;
int iteration = 10;
float tinggiMaksimal = 100;
float tinggi;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSyncWord(0xF3);

}

void loop() {

  jarak = (sonar1.ping_median(iteration) / 2.0) * 0.0343;
  Serial.print("Jarak: ");
  Serial.print(jarak);
  if (jarak <= tinggiMaksimal) {
    tinggi = tinggiMaksimal - jarak;
    Serial.print(" | Tinggi: ");
    Serial.println(tinggi);
  } else {
    Serial.println(" | Objek diluar jangkauan");
  }

  Serial.print("Sending packet: ");
  Serial.println(tinggi);

  // send packet
  LoRa.beginPacket();
  LoRa.print(tinggi);
  LoRa.endPacket();

  delay(1000);
}
