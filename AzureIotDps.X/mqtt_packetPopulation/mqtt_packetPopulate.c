// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "mqtt_packetPopulate.h"


char* hub_device_key = HUB_DEVICE_KEY;
char* hub_hostname = CFG_MQTT_HUB_HOST;
char mqtt_password_buf[512];
char mqtt_username_buf[200];

char digit_to_hex(char number)
{
	return (char)(number + (number < 10 ? '0' : 'A' - 10));
}

char* url_encode_rfc3986(char* s, char* dest, size_t dest_len) {

	for (; *s && dest_len > 1; s++) {

		if (isalnum(*s) || *s == '~' || *s == '-' || *s == '.' || *s == '_')
		{
			*dest++ = *s;
		}
		else if (dest_len < 4)
		{
			break;
		}
		else
		{
			*dest++ = '%';
			*dest++ = digit_to_hex(*s / 16);
			*dest++ = digit_to_hex(*s % 16);
		}
	}

	*dest++ = '\0';
	return dest;
}
