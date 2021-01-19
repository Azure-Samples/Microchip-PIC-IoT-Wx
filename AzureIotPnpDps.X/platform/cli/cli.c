/*
    \file   cli.c

    \brief  Command Line Interpreter source file.

    (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "../drivers/timeout.h"
#include "../delay.h"
#include "../drivers/uart.h"
#include "cli.h"
#include "../cloud/crypto_client/crypto_client.h"
#include "../credentials_storage/credentials_storage.h"
#include "../mqtt/mqtt_core/mqtt_core.h"
#include "../debug_print.h"
#include "../cloud/wifi_service.h"
#include "../cloud/cloud_service.h"
#include "../../mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.h"

#define WIFI_PARAMS_OPEN    1
#define WIFI_PARAMS_PSK     2
#define WIFI_PARAMS_WEP     3
#define MAX_COMMAND_SIZE    100
#define MAX_PUB_KEY_LEN     200
#define NEWLINE "\r\n"

//#define LED_CLI
#ifdef LED_CLI
#include "../led.h"
#endif

#ifndef LED_CLI
#define UNKNOWN_CMD_MSG "--------------------------------------------" NEWLINE\
                        "Unknown command. List of available commands:" NEWLINE\
                        "reset"NEWLINE\
                        "device"NEWLINE\
                        "key"NEWLINE\
                        "reconnect" NEWLINE\
                        "version" NEWLINE\
                        "cli_version" NEWLINE\
                        "wifi <ssid>[,<pass>,[authType]]" NEWLINE\
                        "debug <Debug Level : 0 ~ 4>" NEWLINE\
                        "idscope [DPS ID Scope]" NEWLINE\
                        "--------------------------------------------"NEWLINE"\4"
#else
#define UNKNOWN_CMD_MSG "--------------------------------------------" NEWLINE\
                        "Unknown command. List of available commands:" NEWLINE\
                        "reset"NEWLINE\
                        "device"NEWLINE\
                        "key"NEWLINE\
                        "reconnect" NEWLINE\
                        "version" NEWLINE\
                        "cli_version" NEWLINE\
                        "wifi <ssid>[,<pass>,[authType]]" NEWLINE\
                        "debug <Debug Level : 0 ~ 4>" NEWLINE\
                        "led <led : 1 ~ 4>,<state : 1 ~ 4> [-? for help]" NEWLINE\
                        "idscope [DPS ID Scope]" NEWLINE\
                        "--------------------------------------------"NEWLINE"\4"
#endif

static char command[MAX_COMMAND_SIZE];
static bool isCommandReceived = false;
static uint8_t index = 0;
static bool commandTooLongFlag = false;

const char * const cli_version_number             = "2.1";
const char * const firmware_version_number        = "1.1.1";

static void command_received(char *command_text);
static void reset_cmd(char *pArg);
static void reconnect_cmd(char *pArg);
static void set_wifi_auth(char *ssid_pwd_auth);
static void get_public_key(char *pArg);
static void get_device_id(char *pArg);
static void get_cli_version(char *pArg);
static void get_firmware_version(char *pArg);
static void get_set_debug_level(char *pArg);
#ifdef LED_CLI
static void set_led(char *pArg);
#endif
static void get_set_dps_idscope(char *pArg);

static bool endOfLineTest(char c);
static void enableUsartRxInterrupts(void);

#define CLI_TASK_INTERVAL timeout_mSecToTicks(50)

uint32_t CLI_task(void*);
timerStruct_t CLI_task_timer = {CLI_task};

struct cmd
{
    const char * const command;
    void (* const handler)(char *argument);
};

const struct cmd commands[] =
{
    { "reset",       reset_cmd},
    { "reconnect",   reconnect_cmd },
    { "wifi",        set_wifi_auth },
    { "key",         get_public_key },
    { "device",      get_device_id },
    { "cli_version", get_cli_version },
    { "version",     get_firmware_version },
    { "debug",       get_set_debug_level },
#ifdef LED_CLI
    { "led",         set_led },
#endif
    { "idscope",     get_set_dps_idscope }
};

const char* debug_level[] =
{
    "SEVERITY_NONE",
    "SEVERITY_ERROR",
    "SEVERITY_WARN",
    "SEVERITY_DEBUG",
    "SEVERITY_INFO"
};

#ifdef LED_CLI
const char* led_string[] =
{
    "Blue",
    "Green",
    "Yellow",
    "Red"
};
#endif

void CLI_init(void)
{
    enableUsartRxInterrupts();
    timeout_create(&CLI_task_timer, CLI_TASK_INTERVAL);
}

static bool endOfLineTest(char c)
{
   static char test = 0;
   bool retvalue = true;

   if(c == '\n')
   {
      if(test == '\r')
      {
         retvalue = false;
      }
   }
   test = c;
   return retvalue;
}

uint32_t CLI_task(void* param)
{
    bool cmd_rcvd = false;
    char c = 0;
    // read all the EUSART bytes in the queue
    while(uart[CLI].DataReady() && !isCommandReceived)
    {
        c = uart[CLI].Read();
        // read until we get a newline
        if(c == '\r' || c == '\n')
        {
            command[index] = 0;

            if(!commandTooLongFlag)
            {
                if(endOfLineTest(c) && !cmd_rcvd)
                {
                    command_received((char*)command);
                    cmd_rcvd = true;
                }
            }
            if(commandTooLongFlag)
            {
                printf(NEWLINE"Command too long"NEWLINE);
            }
            index = 0;
            commandTooLongFlag = false;
        }
        else // otherwise store the character
        {
            if(index < MAX_COMMAND_SIZE)
            {
                command[index++] = c;
            }
            else
            {
                commandTooLongFlag = true;
            }
        }
    }

    return CLI_TASK_INTERVAL;
}

static void set_wifi_auth(char *ssid_pwd_auth)
{
    char *credentials[3];
    char *pch;
    uint8_t params = 0;
    uint8_t i;

    for(i=0;i<=2;i++)credentials[i]='\0';

    pch = strtok (ssid_pwd_auth, ",");
    credentials[0]=pch;
        
    while (pch != NULL && params <= 2)
    {
        credentials[params] = pch;
        params++;
        pch = strtok (NULL, ",");
    }

    if(credentials[0]!=NULL)
    {
        if(credentials[1]==NULL && credentials[2]==NULL) params=1;
        else if(credentials[1]!= NULL && credentials[2]== NULL)
        {
            params=atoi(credentials[1]);//Resuse the same variable to store the auth type
            if (params==2 || params==3)params=5;
            else if(params==1);
            else params=2;
        }
        else params = atoi(credentials[2]);		
    }

    switch (params)
    {
        case WIFI_PARAMS_OPEN:
            strncpy(ssid, credentials[0],MAX_WIFI_CREDENTIALS_LENGTH-1);
            strcpy(pass, "\0");
            strncpy(authType, "1", 2);
            break;

        case WIFI_PARAMS_PSK:
            case WIFI_PARAMS_WEP:
            strncpy(ssid, credentials[0],MAX_WIFI_CREDENTIALS_LENGTH-1);
            strncpy(pass, credentials[1],MAX_WIFI_CREDENTIALS_LENGTH-1);
            sprintf(authType, "%d", params);                
            break;
            
        default:
            params = 0;
            break;
    }
    if (params)
    {
        printf("OK\r\n\4");
        if(CLOUD_isConnected())
        {
            MQTT_Close(MQTT_GetClientConnectionInfo());
        }        
        wifi_disconnectFromAp();
    }
    else
    {
        printf("Error. Wi-Fi command format is wifi <ssid>[,<pass>,[authType]]\r\n\4");
    }
}

static void reconnect_cmd(char *pArg)
{
    (void)pArg;

    MQTT_Disconnect(MQTT_GetClientConnectionInfo());
    printf("OK\r\n");
}

static void reset_cmd(char *pArg)
{
    (void)pArg;

    DELAY_milliseconds(30);
    asm("RESET");
}

static void get_set_debug_level(char *pArg)
{
   debug_severity_t level = SEVERITY_NONE;

    if (pArg == NULL)
    {
        level = debug_getSeverity();
        printf("Current Debug Level = %s (%d)"NEWLINE, debug_level[level], level);
    }
    else if(*pArg >= '0' && *pArg <= '4')
    {
        level = *pArg - '0';
        debug_setSeverity(level);
        level = debug_getSeverity();
        printf("Debug level set to %s (%d)"NEWLINE, debug_level[level], level);
    }
    else
    {
        printf("debug parameter must be between 0 (Least) and 4 (Most)"NEWLINE);
    }
}

static void get_public_key(char *pArg)
{
    char key_pem_format[MAX_PUB_KEY_LEN];
    (void)pArg;

    if (CRYPTO_CLIENT_printPublicKey(key_pem_format) == NO_ERROR)
    {
        printf(key_pem_format);
    }
    else
    {
        printf("Error getting key.\r\n\4");
    }
}

static char *ateccsn = NULL;
void CLI_setdeviceId(char *id)
{
    ateccsn = id;
}

static void get_device_id(char *pArg)
{
   (void) pArg;
    if (ateccsn)
    {
        printf("%s\r\n\4", ateccsn);
    }
    else
    {
        printf("Unknown.\r\n\4");
    }
}

static void get_cli_version(char *pArg)
{
    (void)pArg;
    printf("v%s\r\n\4", cli_version_number);
}

static void get_firmware_version(char *pArg)
{
    (void)pArg;
    printf("v%s\r\n\4", firmware_version_number);
}

#ifdef LED_CLI
static void print_led_help(char* msg)
{
    if (msg)
    {
        printf(msg);
    }
    printf("============================================"NEWLINE \
           "led <LED Number>,<LED State>"NEWLINE \
           "---------------------"NEWLINE \
           "LED Number"NEWLINE \
           "1 = Blue"NEWLINE \
           "2 = Green"NEWLINE \
           "3 = Yellow"NEWLINE \
           "4 = Red"NEWLINE \
           "---------------------"NEWLINE \
           "LED State"NEWLINE \
           "1 = On"NEWLINE \
           "2 = Off"NEWLINE \
           "3 = Fast Blink"NEWLINE \
           "4 = Slow Blink"NEWLINE \
           NEWLINE );
    return;
}

static void set_led(char *pArg)
{
    //	LED number
    //	1 = Blue
    //	2 = Green
    //	3 = Yellow
    //	4 = Red
    //
    //	State
    //	1 = On
    //	2 = Off
    //	3 = Blink
    char *led_param[3];
    char *pch;
    uint8_t i;
    uint8_t params = 0;
    uint8_t led_num = 0;
    uint8_t led_state = 0;
    led_set_state_t new_led_state = LED_STATE_OFF;

    for(i = 0 ; i <= 2 ; i++)
    {
        led_param[i]='\0';
    }

    pch = strtok (pArg, ",");
    led_param[0]=pch;

    while (pch != NULL && params <= 2)
    {
        led_param[params] = pch;
        params++;
        pch = strtok(NULL, ",");
    }

    if (params == 1)
    {
        if (led_param[0][0] == '-' && (led_param[0][1] == 'h' || led_param[0][1] == 'H' || led_param[0][1] == '?'))
        {
            return print_led_help(NULL);
        }
    }

    if (led_param[0] != NULL)
    {
        if (led_param[0][0] == '-' && (led_param[0][1] == 'h' || led_param[0][1] == 'H' || led_param[0][1] == '?'))
        {
            return print_led_help(NULL);
        }
        else if (!isDigit((int)led_param[0][0]))
        {
            return print_led_help("Please provide LED Number" NEWLINE);
        }
        led_num = atoi(&led_param[0][0]);
    }
    else
    {
        return print_led_help("Please provide LED Number" NEWLINE);
    }

    if (led_param[1] != NULL)
    {
        if (!isDigit((int)led_param[1][0])) {
            return print_led_help("Please provide LED State Number" NEWLINE);
        }
        led_state = atoi(&led_param[1][0]);
    }
    else
    {
        return print_led_help("Please provide LED State Number" NEWLINE);
    }

    switch (led_state)
    {
        case 1:
            new_led_state = LED_STATE_HOLD;
            break;
        case 2:
            new_led_state = LED_STATE_OFF;
            break;
        case 3:
            new_led_state = LED_STATE_BLINK_FAST;
            break;
        case 4:
            new_led_state = LED_STATE_BLINK_SLOW;
            break;
    }

    switch (led_num)
    {
        case 1:
            LED_SetBlue(new_led_state);
            break;
        case 2:
            LED_SetGreen(new_led_state);
            break;
        case 3:
            LED_SetYellow(new_led_state);
            break;
        case 4:
            LED_SetRed(new_led_state);
            break;
    }
}

#endif

static void get_set_dps_idscope(char *pArg)
{
    char *dps_param;
    char atca_id_scope[12]; //idscope 0ne001825F3

    dps_param = pArg;

    if (dps_param == NULL)
    {
        atcab_read_bytes_zone(ATCA_ZONE_DATA, ATCA_SLOT_DPS_IDSCOPE, 0, (uint8_t*)atca_id_scope, sizeof(atca_id_scope));
        printf("Current ID Scope : %s" NEWLINE, atca_id_scope);
    }
    else
    {
        if (strlen(dps_param) != 11)
        {
            printf("ID Scope entered : %s. Wrong ID Scope length.  Should be 0neXXXXXXXX format" NEWLINE, dps_param);
            return;
        }

        if (dps_param[0] != '0' || dps_param[1] != 'n' || dps_param[2] != 'e')
        {
            printf("ID Scope entered : %s. ID Scope should start with 0ne" NEWLINE, dps_param);
            return;
        }

        uint8_t i;

        for (i = 3 ; i < 10 ; i++)
        {
            if (!isalnum(dps_param[i]))
            {
                    printf("ID Scope entered : %s. Non-alpha numberic letter detected.  Should be 0neXXXXXXXX format" NEWLINE, dps_param);
                    return;
            }
        }

        printf("Writing ID Scope %s to ATCA" NEWLINE, dps_param);
        atcab_write_bytes_zone(ATCA_ZONE_DATA, ATCA_SLOT_DPS_IDSCOPE, 0, (uint8_t*)dps_param, sizeof(atca_id_scope));
        DELAY_milliseconds(500);
        atcab_read_bytes_zone(ATCA_ZONE_DATA, ATCA_SLOT_DPS_IDSCOPE, 0, (uint8_t*)atca_id_scope, sizeof(atca_id_scope));
        printf("New ID Scope : %s" NEWLINE, atca_id_scope);
    }

    return;
}

static void command_received(char *command_text)
{
    char *argument = strstr(command_text, " ");
    uint8_t cmp;
    uint8_t ct_len;
    uint8_t cc_len;
    uint8_t i = 0;

    if (argument != NULL)
    {
        /* Replace the delimiter with string terminator */
        *argument = '\0';
        /* Point after the string terminator, to the actual string */
        argument++;
    }

    for (i = 0; i < sizeof(commands)/sizeof(*commands); i++)
    {
        cmp = strcmp(command_text, commands[i].command);
        ct_len = strlen(command_text);        
        cc_len = strlen(commands[i].command);

        if (cmp == 0 && ct_len == cc_len)
        {
            if (commands[i].handler != NULL)
            {
                commands[i].handler(argument);
                return;
            }
        }
    }

    printf(UNKNOWN_CMD_MSG);
}

static void enableUsartRxInterrupts(void)
{
    // Empty RX buffer
    while(uart[CLI].DataReady())
    {
        uart[CLI].Read();
    }    
}
