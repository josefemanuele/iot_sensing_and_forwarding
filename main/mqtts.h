
#define BROKER_URI  CONFIG_MQTT_BROKER_URI
#define MQTT_TOPIC CONFIG_MQTT_TOPIC

/**
 * Start MQTT connection. Set connection parameters through menucongfig.
*/
void mqtt_start(char *data);