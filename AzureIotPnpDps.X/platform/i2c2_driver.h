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

#ifndef __I2C2_DRIVER_H
#define __I2C2_DRIVER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define INLINE  inline 
 
typedef void (*interruptHandler)(void);

/* arbitration interface */
extern INLINE void i2c2_driver_close(void);

/* Interrupt interfaces */
extern INLINE void i2c2_enableIRQ(void);
extern INLINE bool i2c2_IRQisEnabled(void);
extern INLINE void i2c2_disableIRQ(void);
extern INLINE void i2c2_clearIRQ(void);
extern INLINE void i2c2_setIRQ(void);
extern INLINE void i2c2_waitForEvent(uint16_t*);

/* I2C interfaces */
bool  i2c2_driver_driver_open(void);
extern INLINE char i2c2_driver_getRXData(void);
extern INLINE void i2c2_driver_TXData(uint8_t);
extern INLINE void i2c2_driver_resetBus(void);
extern INLINE void i2c2_driver_start(void);
extern INLINE void i2c2_driver_restart(void);
extern INLINE void i2c2_driver_stop(void);
extern INLINE bool i2c2_driver_isNACK(void);
extern INLINE void i2c2_driver_startRX(void);
extern INLINE void i2c2_driver_sendACK(void);
extern INLINE void i2c2_driver_sendNACK(void);
extern INLINE void i2c2_driver_clearBusCollision(void);

bool  i2c2_driver_initSlaveHardware(void);
extern INLINE void i2c2_driver_releaseClock(void);
extern INLINE bool i2c2_driver_isBuferFull(void);
extern INLINE bool i2c2_driver_isStart(void);
extern INLINE bool i2c2_driver_isStop(void);
extern INLINE bool i2c2_driver_isAddress(void);
extern INLINE bool i2c2_driver_isData(void);
extern INLINE bool i2c2_driver_isRead(void);
extern INLINE void i2c2_driver_enableStartIRQ(void);
extern INLINE void i2c2_driver_disableStartIRQ(void);
extern INLINE void i2c2_driver_enableStopIRQ(void);
extern INLINE void i2c2_driver_disableStopIRQ(void);

extern INLINE void i2c2_driver_setBusCollisionISR(interruptHandler handler);
extern INLINE void i2c2_driver_setMasterI2cISR(interruptHandler handler);
extern INLINE void i2c2_driver_setSlaveI2cISR(interruptHandler handler);

#endif // __I2C2_DRIVER_H
