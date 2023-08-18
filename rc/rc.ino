#include "ESP8266WiFi.h"
#include <WiFiUdp.h>
#include <Wire.h>
#include <MPU6050_light.h>

const char* ssid = "gautom5987";
const char* password = "12345678";
const int port = 9999;  // our drone app uses this port

int throttle = 0, pitch = 0, roll = 0;
int angleX = 0, angleY = 0, angleZ = 0;

WiFiUDP Udp;
char dataBuffer[UDP_TX_PACKET_MAX_SIZE];
MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(9600);

  delay(2000);

  pin_setup();

  delay(2000);

  wifi_setup();

  // mpu6050_setup();
}

void mpu6050_setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcOffsets(true);
}

void wifi_setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wifi Connected");

  Serial.print("Use this URL to connect : ");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  Udp.begin(port);   
}

void pin_setup() {
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
}

/* from app we are receving 4 bytes of data and 1 byte for null. so size of packet will be 5 bytes
  we are using wifi udp protocol to receive data from app

  analog reading = (throttle reading/1200)*255
*/

void loop() {
  readDataFromRc();

  int pwm = getRpmInAnalog(throttle);
  Serial.println(pwm);

  analogWrite(D5, pwm); 
  analogWrite(D6, pwm); 
  analogWrite(D7, 0); 
  analogWrite(D8, 0); 

  // readDataFromMpu();
}

void readDataFromRc() {
  int packetSize = Udp.parsePacket();

  if(packetSize) {
    int n = Udp.read(dataBuffer, UDP_TX_PACKET_MAX_SIZE);
    dataBuffer[n] = 0;

    throttle = int(dataBuffer[2])*24;
    roll = int(dataBuffer[0]);
    pitch = int(dataBuffer[1]);
  }
}

void printRcData() {
  Serial.print("Throttle : ");
  Serial.print(throttle);
  Serial.print(", Roll : "); 
  Serial.print(roll);
  Serial.print(", Pitch : ");
  Serial.print(pitch);
  Serial.println("");
}

void readDataFromMpu() {
  mpu6050.update();

  angleX = mpu6050.getAngleX();
  angleY = mpu6050.getAngleY();
  angleZ = mpu6050.getAngleZ();

  // printMpuData();
}

void printMpuData() {
  Serial.print("angleX : ");
  Serial.print(angleX);
  Serial.print(", angleY : ");
  Serial.print(angleY);
  Serial.print(", angleZ : ");
  Serial.println(angleZ);
}

int getRpmInAnalog(int throttle) {
  int analog = (throttle/1200)*255;
  return analog;
}

