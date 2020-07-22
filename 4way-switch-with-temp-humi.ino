
#include <SimpleDHT.h>
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

int pinDHT11 = 14;            //define pin of DHT11
SimpleDHT11 dht11(pinDHT11); // define device

void setup() {
	Serial.begin(115200);
	wifi_connect(); // in wifi_info.h
	//homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
	my_homekit_setup();
}

void loop() {
	my_homekit_loop();
	delay(10);
}

//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_switch_on1; //The firt relay
extern "C" homekit_characteristic_t cha_switch_on2; //The secondry relay
extern "C" homekit_characteristic_t cha_switch_on3; //The third relay
extern "C" homekit_characteristic_t cha_switch_on4; //The fouth relay
extern "C" homekit_characteristic_t temperature;    //temperature
extern "C" homekit_characteristic_t humidity;       //humidity
static uint32_t next_heap_millis = 0;

#define PIN_SWITCH1 16  //The first relay
#define PIN_SWITCH2 5   //The secondry relay
#define PIN_SWITCH3 4   //The third relay
#define PIN_SWITCH4 0   //The fouth relay
  
//Called when the switch value is changed by iOS Home APP
void cha_switch_on_setter1(const homekit_value_t value) {
	bool on = value.bool_value;
	cha_switch_on1.value.bool_value = on;	//sync the value
	LOG_D("Switch1: %s", on ? "ON" : "OFF");
	digitalWrite(PIN_SWITCH1, on ? HIGH : LOW);
}

void cha_switch_on_setter2(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_switch_on2.value.bool_value = on;  //sync the value
  LOG_D("Switch2: %s", on ? "ON" : "OFF");
  digitalWrite(PIN_SWITCH2, on ? HIGH : LOW);
}
void cha_switch_on_setter3(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_switch_on3.value.bool_value = on; //sync the value
  LOG_D("Switch3: %s", on ? "ON" : "OFF");
  digitalWrite(PIN_SWITCH3, on ? HIGH : LOW);
}

void cha_switch_on_setter4(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_switch_on4.value.bool_value = on;  //sync the value
  LOG_D("Switch4: %s", on ? "ON" : "OFF");
  digitalWrite(PIN_SWITCH4, on ? HIGH : LOW);
}

void my_homekit_setup() {

	pinMode(PIN_SWITCH1, OUTPUT);
	digitalWrite(PIN_SWITCH1, LOW);
  pinMode(PIN_SWITCH2, OUTPUT);
  digitalWrite(PIN_SWITCH2, LOW);
  pinMode(PIN_SWITCH3, OUTPUT);
  digitalWrite(PIN_SWITCH3, LOW);
  pinMode(PIN_SWITCH4, OUTPUT);
  digitalWrite(PIN_SWITCH4, LOW);

	//Add the .setter function to get the switch-event sent from iOS Home APP.
	//The .setter should be added before arduino_homekit_setup.
	//HomeKit sever uses the .setter_ex internally, see homekit_accessories_init function.
	//Maybe this is a legacy design issue in the original esp-homekit library,
	//and I have no reason to modify this "feature".
	cha_switch_on1.setter = cha_switch_on_setter1;
  cha_switch_on2.setter = cha_switch_on_setter2;
  cha_switch_on3.setter = cha_switch_on_setter3;
  cha_switch_on4.setter = cha_switch_on_setter4;  
	arduino_homekit_setup(&config);

	//report the switch value to HomeKit if it is changed (e.g. by a physical button)
	//bool switch_is_on = true/false;
	//cha_switch_on.value.bool_value = switch_is_on;
	//homekit_characteristic_notify(&cha_switch_on, cha_switch_on.value);
}
//get temperature and humidity first.then give parm to HomeKit
void get_sensor()
{ 
  byte temperature_value = 0;
  byte humidity_value = 0;
  int err = SimpleDHTErrSuccess;
   delay(1000);
  if ((err = dht11.read(&temperature_value, &humidity_value, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);
   delay(1000);
  }
  temperature.value.float_value = temperature_value;
  humidity.value.float_value = humidity_value;
  homekit_characteristic_notify(&temperature, temperature.value);
  homekit_characteristic_notify(&humidity, humidity.value);
//  Serial.print(temperature_value); Serial.print(" *C, "); 
//  Serial.print(humidity_value); Serial.println(" H");
  }  

void my_homekit_loop() {  
  arduino_homekit_loop();
  const uint32_t t = millis();
  if(GPIO_INPUT_GET(GPIO_ID_PIN(12)) == 0x01)
 {
    delay(3000);
    if(GPIO_INPUT_GET(GPIO_ID_PIN(12)) == 0x01)
    {homekit_server_reset();
       delay(500);    
      WiFi.disconnect();
      delay(500);    
      wifi_connect(); // in wifi_info.h
    }    
  }
  if (t > next_heap_millis) {
    // show heap info every 5 seconds
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
        ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
        get_sensor();
}
}
