#include "BluetoothSerial.h" //Header File for Serial Bluetooth, will be added by default into Arduino
#include "heltec.h"

BluetoothSerial SerialBT; //Object for Bluetooth
HardwareSerial HWSerial(2);

#define HW_BAUD 1000000
#define PGM_NAME "App - Heltec"
#define CONNECT_PIN 27

uint8_t bt_byte, ser_byte;
uint8_t buf[5000];
int pos, len;
int scr_line;

void printit(char *str) {
  if (scr_line >= 8) {
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, PGM_NAME);
    scr_line = 1;
  }

  Heltec.display->drawString(0,scr_line *8, str);
  Heltec.display->display();
  scr_line++;
}

#define SPARK_BT_NAME "Spark 40 Audio"

void setup() {
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);
  HWSerial.begin(HW_BAUD, SERIAL_8N1, 14, 12);

  pinMode(CONNECT_PIN, INPUT);

  Heltec.display->clear();
  Heltec.display->drawString(0, 0, PGM_NAME);
  Heltec.display->display();
  scr_line = 1;

  printit("Waiting for other Heltec");
  delay(1000);
  while (digitalRead(CONNECT_PIN) == LOW);
  printit("Other Heltec is ready");

  printit("Starting..");
  if (!SerialBT.begin(SPARK_BT_NAME)) 
  {
    printit("Failed"); 
    while (true) {};
  }
  printit("Ready ");

  pos = 0;
  len = -1;
}

char str[40];

void loop() {
  
  while (SerialBT.available()) {
    bt_byte = SerialBT.read();  
    HWSerial.write(bt_byte);    
  };
  
  while (HWSerial.available()) {
    ser_byte = HWSerial.read();
    
    buf[pos++] = ser_byte;
    if (pos == 7) {
      len = ser_byte; 
    }
    if (pos == len) {
      SerialBT.write(buf, pos);    
      pos = 0;
      len = -1;  
      
      snprintf(str, sizeof(str), "> %2.2x %2.2x %2.2x", buf[6], buf[20], buf[21]);
      printit(str);
    }
  }
}
