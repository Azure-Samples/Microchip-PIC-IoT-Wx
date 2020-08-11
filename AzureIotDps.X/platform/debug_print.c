/*
    \file   debug_print.c

    \brief  debug_console printer

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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "debug_print.h"


static const char *severity_strings[] = {
   CSI_WHITE   "   NONE" CSI_WHITE,
   CSI_YELLOW  "WARNING" CSI_WHITE,
   CSI_BLUE    " NOTICE" CSI_WHITE,
   CSI_MAGENTA "   INFO" CSI_WHITE,
   CSI_RED     "  DEBUG" CSI_NORMAL CSI_WHITE
};

static const char *level_strings[] = {
      CSI_WHITE           "NORMAL" CSI_WHITE,
      CSI_GREEN           "  GOOD" CSI_WHITE,
      CSI_RED             "   BAD" CSI_WHITE,
      CSI_RED CSI_INVERSE " ERROR" CSI_NORMAL CSI_WHITE
};
 
static debug_severity_t debug_severity_filter = SEVERITY_NONE;
static char debug_message_prefix[20] = "<PREFIX>";

void debug_init(const char *prefix)
{ 
   debug_setPrefix(prefix);
   debug_setSeverity(SEVERITY_NOTICE);
//   debug_printer(SEVERITY_NONE, LEVEL_NORMAL, CSI_CLS CSI_HOME BANNER CSI_RESET "\r\n\r\n" __DATE__ " " __TIME__ "\r\n");
}

void debug_setSeverity(debug_severity_t debug_level)
{
   debug_severity_filter = debug_level;
}

void debug_setPrefix(const char *prefix)
{
   strncpy(debug_message_prefix,prefix,sizeof(debug_message_prefix));
}

void debug_printer(debug_severity_t debug_severity, debug_errorLevel_t error_level, char* format, ...)
{
   if(debug_severity >= SEVERITY_NONE && debug_severity <= SEVERITY_DEBUG)
   {
      if(debug_severity <= debug_severity_filter)
      {
         if(error_level < LEVEL_NORMAL) error_level = LEVEL_NORMAL;
         if(error_level > LEVEL_ERROR) error_level = LEVEL_ERROR;

         printf("%s\4 %s %s ",debug_message_prefix, severity_strings[debug_severity], level_strings[error_level]);

         va_list argptr;
         va_start(argptr, format);
         vprintf(format , argptr);
         va_end(argptr);
         printf(CSI_RESET"\r\n");
      }
  }
}


