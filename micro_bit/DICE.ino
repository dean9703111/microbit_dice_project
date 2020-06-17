//這些是BLNYK手機APP，以及相關藍牙模組
#define BLYNK_USE_DIRECT_CONNECT
#define BLYNK_PRINT Serial

#include <BlynkSimpleSerialBLE.h>
#include <BLEPeripheral.h>
#include "BLESerial.h"
#define BLYNK_USE_DIRECT_CONNECT

#define BLYNK_PRINT Serial

#include <BlynkSimpleSerialBLE.h>
#include <BLEPeripheral.h>
#include "BLESerial.h"

//加速度偵測套件
#include "Wire.h"
#include "MMA8653.h"

MMA8653 accel;

char auth[] = "C5t19LDXjSbXpGvH1DzVHxkyJvVNqOYL";//放自己Blynk App的token

// Create ble serial instance, parameters are ignored for MicroBit
BLESerial SerialBLE(0, 0, 0);


void setup() {
  Serial.begin(9600);

  Serial.println("microbit accel test");
  //加速度設定
  accel.begin(false, 2); // 8-bit mode, 2g range

  //藍芽設定
  SerialBLE.setLocalName("Blynk"); //這是你會在藍牙搜尋裡面看到的名稱
  SerialBLE.setDeviceName("Blynk");
  SerialBLE.setAppearance(0x0080);
  SerialBLE.begin();

  Blynk.begin(SerialBLE, auth);
}

float format(int value) { //用來正規化角度數值
  return (float)value * 0.0156;
}

void loop() {
  accel.update(); //每次都更新
  //抓出xyz的數值
  float xaxis =  format(accel.getX());
  float yaxis =  format(accel.getY());
  float zaxis =  format(accel.getZ());
  int dice_number = 0;

  if (xaxis < 0.5 && xaxis > -0.5) {
    if (yaxis < 0.5 && yaxis > -0.5) {
      if (zaxis > 0) {
        Serial.println(1);
        dice_number = 1;
      } else {
        Serial.println(6);
        dice_number = 6;
      }
    }
    else {
      if (yaxis > 0) {
        Serial.println(4);
        dice_number = 4;
      } else {
        Serial.println(3);
        dice_number = 3;
      }
    }
  } else {
    if (xaxis > 0) {
      Serial.println(2);
      dice_number = 2;
    } else {
      Serial.println(5);
      dice_number = 5;
    }
  }
  //  Serial.print(xaxis); Serial.print(" , ");
  //  Serial.print(yaxis); Serial.print(", ");
  //  Serial.println(zaxis);
  //  Serial.println();


  if (SerialBLE) {    // 如果藍牙連線成功
    Blynk.run();
    Blynk.virtualWrite(V0, dice_number);//寫入V0角位骰子的數值
  }
  delay(1000);//因為BLNYK手機APP也是每秒偵測，所以這裡每秒發送就好
}
