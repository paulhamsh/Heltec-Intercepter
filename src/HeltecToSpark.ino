#include "BluetoothSerial.h"
#include "heltec.h"

HardwareSerial HWSerial(2);
BluetoothSerial SerialBT;

#define HW_BAUD 1000000
#define PGM_NAME "Heltec - Spark"
#define SCR_HEIGHT 10

#define SPARK_NAME "Spark 40 Audio"
#define MY_NAME    "HeltecToSpark"

uint8_t ser_byte, bt_byte;
bool connected;
uint8_t buf[5000];
int pos;
int scr_line;
uint8_t pre;

unsigned long count;

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

byte get_serial[]{0x01, 0xfe, 0x00, 0x00, 0x53, 0xfe, 0x17, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0xf0, 0x01, 0x01, 0x01, 0x02, 0x23, 0xf7};

byte set_preset[]{0x01, 0xfe, 0x00, 0x00, 0x53, 0xfe, 0x1a, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0xf0, 0x01, 0x01, 0x01, 0x01, 0x38, 
                  0x00, 0x00, 0x01, 0xf7}; 
                  
byte upd_preset[]{0x01, 0xfe, 0x00, 0x00, 0x41, 0xff, 0x1a, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0xf0, 0x01, 0x01, 0x01, 0x03, 0x38, 
                  0x00, 0x00, 0x01, 0xf7}; 

void start_bt() {
  if (!SerialBT.begin (MY_NAME, true)) {
    printit("Init fail");
    while (true);
  }    
  connected = false;
}

void connect_to_spark() {
  int rec;

  while (!connected) {
    printit("Connecting");
    connected = SerialBT.connect(SPARK_NAME);
    if (connected && SerialBT.hasClient()) {
      printit("Connected");
    }
    else {
       connected = false;
       delay(3000);
    }
  }
  // flush anything read from Spark - just in case

  while (SerialBT.available())
    rec = SerialBT.read(); 
}

void flush_in() {
  bt_byte = SerialBT.read();
  while (bt_byte != 0xf7)
    bt_byte = SerialBT.read();
}

void send_preset(uint8_t pres) {
  set_preset[24] = pres;
  SerialBT.write(set_preset, 26);
  flush_in(); 
  
  upd_preset[24] = pres;
  // this is actually the 8 bit xor checksum, 
  // but for this short message it is just the same as the preset number.  
//  upd_preset[19] = pres;  
// this was the fix 
//  upd_preset[19] = 0x01; 

//  upd_preset[18] = seq++;
  if (pres == 0) upd_preset[19] = 0x01; // this works
  if (pres == 1) upd_preset[19] = 0x02; // this works
  if (pres == 2) upd_preset[19] = 0x01; // this works
  if (pres == 3) upd_preset[19] = 0x01; // 0x02 works, 0x01, 0x04 works - just not 0x03
  HWSerial.write(upd_preset, 26);
}

void setup() {

//  HWSerial.begin(HW_BAUD, SERIAL_8N1, 33, 32);

  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);
  HWSerial.begin(HW_BAUD, SERIAL_8N1, 12, 14);

  Heltec.display->clear();
  Heltec.display->drawString(0, 0, PGM_NAME);
  Heltec.display->display();
  scr_line = 1;
  
  start_bt();
  connect_to_spark();

  pos = 0;
  count = millis();
  pre = 0;
}

char str[40];

void loop() {
  if (false) {  // put something here to detect a button to send a preset
    printit("Send preset ");
    send_preset(pre);
    pre++;
    if (pre > 3) pre = 0;
    delay(400);
  }
  
  while (HWSerial.available()) {
    ser_byte = HWSerial.read();
    buf[pos++] = ser_byte;
    
    if (ser_byte == 0xf7) {
      // end of the packet, so write it to Spark
      SerialBT.write(buf, pos);
      pos = 0;
      count = millis();
      snprintf(str, sizeof(str), "> %2.2x %2.2x %2.2x", buf[6], buf[20], buf[21]);
      printit(str);
    }
  }

  while (SerialBT.available()) {
    count = millis();
    bt_byte = SerialBT.read();
    HWSerial.write(bt_byte);
  }

  if (millis() - count > 10000) {
    // request serial number and read returned bytes and discard - keep-alive link to Spark
    count = millis();
    SerialBT.write(get_serial, 23);
    flush_in();
    printit("Keep alive sent");
  }
}
