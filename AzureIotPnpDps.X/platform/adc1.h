/**
  ADC1 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    adc1.h

  @Summary
    This is the generated header file for the ADC1 driver using Foundation Services Library

  @Description
    This header file provides APIs for driver for ADC1.
    Generation Information :
        Product Revision  :  Foundation Services Library - pic24-dspic-pic32mm : v1.26
        Device            :  PIC24FJ128GA705
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.30
        MPLAB 	          :  MPLAB X 3.45
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#ifndef _ADC1_H
#define _ADC1_H

/**
  Section: Included Files
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
  Section: Data Types
*/

/** ADC Channel Definition
 
 @Summary 
   Defines the channels available for conversion
 
 @Description
   This routine defines the channels that are available for the module to use.
 
 Remarks:
   None
 */
typedef enum 
{
    LIGHT_AN =  0x8,
    ADC1_CHANNEL_CTMU_TEMPERATURE_SENSOR_INPUT =  0xE,
    ADC1_CHANNEL_CTMU =  0xF,
    ADC1_CHANNEL_VBG =  0x1C,
    ADC1_CHANNEL_AVSS =  0x1D,
    ADC1_CHANNEL_AVDD =  0x1E,
    ADC1_MAX_CHANNEL_COUNT = 6
} ADC1_CHANNEL;

/**
  Section: Interface Routines
*/


/**
  @Summary
    This function initializes ADC instance : 1

  @Description
    This routine initializes the ADC driver instance for : 1
    index, making it ready for clients to open and use it. It also initializes any
    internal data structures.
    This routine must be called before any other ADC routine is called. 

  @Preconditions
    None.

  @Param
    None.

  @Returns
    None.

  @Comment
    
 
  @Example
    <code>
        int conversion;
        ADC1_Initialize();
        ADC1_ChannelSelect(AN1_Channel);
        ADC1_Start();
        //Provide Delay
        for(int i=0;i <1000;i++)
        {
        }
        ADC1_Stop();
        while(!ADC1_IsConversionComplete())
        {
            ADC1_Tasks();   
        }
        conversion = ADC1_ConversionResultGet();
    </code>

*/

void ADC1_Initialize (void);

/**
  @Summary
    Starts sampling manually.

  @Description
    This routine is used to start the sampling manually.
 
  @Preconditions
    ADC1_Initializer function should have been called 
    before calling this function.

  @Param
    None.

  @Returns
    None.

  @Example
    Refer to ADC1_Initializer() for an example

*/

void ADC1_Start(void);
/**
  @Summary
    Stops sampling manually.

  @Description
    This routine is used to stop the sampling manually before conversion
    is triggered.
 
  @Preconditions
    ADC1_Initializer() function should have been 
    called before calling this function.

  @Param
    None.

  @Returns
    None.

  @Example
    Refer to ADC1_Initializer() for an example
*/

void ADC1_Stop(void);
/**
  @Summary
    Gets the buffer loaded with conversion results.

  @Description
    This routine is used to get the analog to digital converted values in a
    buffer. This routine gets converted values from multiple channels.
 
  @Preconditions
    This routine returns the buffer containing the conversion values only after 
    the conversion is complete. Completion status conversion can be checked using 
    ADC1_IsConversionComplete() routine.
 
  @Param
    None.

  @Returns
    Returns the count of the buffer containing the conversion values.

  @Example
    <code>
        int count;
        //Initialize for channel scanning
        ADC1_Initializer();
        ADC1_Start();
        //Provide Delay
        for(int i=0;i <1000;i++)
        {
        }
        ADC1_Stop();
        while(!ADC1_IsConversionComplete())
        {
            count = ADC1_ConversionResultBufferGet();
        }

*/

uint16_t ADC1_ConversionResultBufferGet(uint16_t *buffer);
/**
  @Summary
    Returns the ADC1 conversion value.

  @Description
    This routine is used to get the analog to digital converted value. This
    routine gets converted values from the channel specified.
 
  @Preconditions
    The channel required must be selected before calling this routine using
    ADC1_ChannelSelect(channel). This routine returns the 
    conversion value only after the conversion is complete. Completion status 
    conversion can be checked using ADC1_IsConversionComplete()
    routine.
   
  @Returns
    Returns the buffer containing the conversion value.

  @Param
    Buffer address
  
  @Example
    Refer to ADC1_Initializer}(); for an example
 */

uint16_t ADC1_ConversionResultGet(void);
/**
  @Summary
    Returns true when the conversion is completed

  @Description
    This routine is used to determine if conversion is completed. This routine
    returns the value of the DONE bit. When conversion is complete the routine
    returns 1. It returns 0 otherwise.
 
  @Preconditions
    ADC1_Initializer() function should have been 
    called before calling this function.
 
  @Returns
    Returns true if conversion is completed

  @Param
    None
  
  @Example
    Refer to ADC1_Initializer(); for an example
 */

bool ADC1_IsConversionComplete( void );
/**
  @Summary
    Allows selection of a channel for conversion

  @Description
    This routine is used to select desired channel for conversion.
  
  @Preconditions
    ADC1_Initializer() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required channel from the ADC1_CHANNEL list
  
  @Example
    Refer to ADC1_Initializer(); for an example
 
*/

void ADC1_ChannelSelect( ADC1_CHANNEL channel );
/**
  @Summary
    Allows single conversion of a channel

  @Description
    This routine is used to get a single blocking conversion of a desired 
    channel.
  
  @Preconditions
    ADC1_Initializer() function should have been 
    called before calling this function.
 
  @Returns
    Returns the conversion value.

  @Param
    Pass in required channel from the ADC1_CHANNEL list
   
*/

uint16_t ADC1_GetConversion(ADC1_CHANNEL channel);
/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    ADC1_Initializer() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to ADC1_Initializer(); for an example
    
*/
void ADC1_Tasks(void);


        
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif //_ADC1_H
    
/**
 End of File
*/
