#ifndef MAIN_H_
#define MAIN_H_
#include <stdint.h>
#include <stdbool.h>

typedef union
{
	struct {
		unsigned short version_found:1;
		unsigned short isGet:1;
		unsigned short max_temp_updated:1;
		unsigned short telemetry_interval_found:1;
		unsigned short yellow_led_found:1;
		unsigned short reserved:11;
	};
	unsigned short AsUSHORT;
} twin_update_flag_t;

typedef struct
{
	twin_update_flag_t flag;
	int32_t version_num;
	int32_t desired_led_yellow;	// 1 = On, 2 = Off, 3 = Blink
	int8_t reported_led_red;
	int8_t reported_led_blue;
	int8_t reported_led_green;
} twin_properties_t;

void check_led_status(twin_properties_t* twin_properties);
void check_button_status(void);

#endif /* MAIN_H_ */
