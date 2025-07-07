 
#include "BluetoothSerial.h"
 
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif
 

#define LED_GPIO 25
#define MOTER_GPIO 26
 

BluetoothSerial SerialBT;

hw_timer_t *Timer0_Cfg = NULL;
 

const char *pin = "1234"; 
String device_name = "ESP32-BT-sohail";
 

String RxBuffer = "";
char RxByte;
 
void IRAM_ATTR Timer0_ISR()
{
    // Read The Potentiometer ADC Channel & Send Raw Data Over Bluetooth
    SerialBT.println("hello");
}
 
void setup() {
  pinMode(LED_GPIO, OUTPUT);
  pinMode(MOTER_GPIO, OUTPUT);
  Serial.begin(115200);
  SerialBT.begin("sohil ESP"); 
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
  
  Timer0_Cfg = timerBegin(0, 80, true);
  timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR, true);
  timerAlarmWrite(Timer0_Cfg, 100000, true);
  timerAlarmEnable(Timer0_Cfg);
}
 
void loop() {
  
  if (SerialBT.available()){
    RxByte = SerialBT.read();
    if (RxByte != '\n'){
      RxBuffer += String(RxByte);
    }
    else{
      RxBuffer = "";
    }
    Serial.write(RxByte);  
  }
  
  if (RxBuffer =="led_on"){
    digitalWrite(LED_GPIO, HIGH);
  }
  else if (RxBuffer =="led_off"){
    digitalWrite(LED_GPIO, LOW);
  }
  if (RxBuffer == "moter_on"){
    digitalWrite(MOTER_GPIO, HIGH);
  }
  else if(RxBuffer == "moter_off"){
    digitalWrite(MOTER_GPIO, LOW);
  }
  delay(25);
}
