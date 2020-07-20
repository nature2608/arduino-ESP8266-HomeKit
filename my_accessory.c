#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
	printf("accessory identify\n");
}

// Switch (HAP section 8.38)
// required: ON
// optional: NAME

// format: bool; HAP section 9.70; write the .setter function to get the switch-event sent from iOS Home APP.
homekit_characteristic_t cha_switch_on1 = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch_on2 = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch_on3 = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch_on4 = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t temperature = HOMEKIT_CHARACTERISTIC_(CURRENT_TEMPERATURE, 0);
homekit_characteristic_t humidity    = HOMEKIT_CHARACTERISTIC_(CURRENT_RELATIVE_HUMIDITY, 0);
// format: string; HAP section 9.62; max length 64
homekit_characteristic_t cha_name1 = HOMEKIT_CHARACTERISTIC_(NAME, "Switch");
homekit_characteristic_t cha_name2 = HOMEKIT_CHARACTERISTIC_(NAME, "Switch2");
homekit_characteristic_t cha_name3 = HOMEKIT_CHARACTERISTIC_(NAME, "Switch3");
homekit_characteristic_t cha_name4 = HOMEKIT_CHARACTERISTIC_(NAME, "Switch4");
homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Controler"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Nature"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "20200718"),
            HOMEKIT_CHARACTERISTIC(MODEL, "Light_UP"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
		HOMEKIT_SERVICE(SWITCH, .characteristics=(homekit_characteristic_t*[]){
			&cha_switch_on1,
			&cha_name1,
			NULL
		}),
   HOMEKIT_SERVICE(SWITCH, .characteristics=(homekit_characteristic_t*[]){
     &cha_switch_on2,
      &cha_name2,
      NULL
    }),
    HOMEKIT_SERVICE(SWITCH, .characteristics=(homekit_characteristic_t*[]){
     &cha_switch_on3,
      &cha_name3,
      NULL
    }),
    HOMEKIT_SERVICE(SWITCH, .characteristics=(homekit_characteristic_t*[]){
     &cha_switch_on4,
      &cha_name4,
      NULL
    }),
    HOMEKIT_SERVICE(TEMPERATURE_SENSOR, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Temperature Sensor"),
            &temperature,
            NULL
        }),
        HOMEKIT_SERVICE(HUMIDITY_SENSOR, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Humidity Sensor"),
            &humidity,
            NULL
        }),
        NULL
    }),
    NULL
};

homekit_server_config_t config = {
		.accessories = accessories,
		.password = "111-11-111"
};
