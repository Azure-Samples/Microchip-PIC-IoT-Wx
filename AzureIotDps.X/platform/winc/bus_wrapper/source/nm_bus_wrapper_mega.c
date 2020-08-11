/**
 *
 * \file
 *
 * \brief This module contains NMC1000 bus wrapper APIs implementation.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include <stdio.h>
#include "../../bsp/include/nm_bsp.h"
#include "../../common/include/nm_common.h"
#include "../include/nm_bus_wrapper.h"
#include "../../../config/conf_winc.h"
#include "../../../pin_manager.h"
#include "../../../drivers/spi_master.h"

#define NM_BUS_MAX_TRX_SZ	256

tstrNmBusCapabilities egstrNmBusCapabilities =
{
    NM_BUS_MAX_TRX_SZ
};

#ifdef CONF_WINC_USE_I2C

#define SLAVE_ADDRESS 0x60

/** Number of times to try to send packet if failed. */
#define I2C_TIMEOUT 100

static sint8 nm_i2c_write(uint8 *b, uint16 sz)
{
    /* Not implemented */
}

static sint8 nm_i2c_read(uint8 *rb, uint16 sz)
{
    /* Not implemented */
}

static sint8 nm_i2c_write_special(uint8 *wb1, uint16 sz1, uint8 *wb2, uint16 sz2)
{
    /* Not implemented */
}
#endif

#ifdef CONF_WINC_USE_SPI
static sint8 spi_rw(uint8* pu8Mosi, uint8* pu8Miso, uint16 u16Sz)
{
    //struct spi_device spi_device_conf;
    //spi_device_conf.id = CONF_WIFI_M2M_SPI_CS_PIN;
    uint8 u8Dummy = 0;
    uint8 u8SkipMosi = 0, u8SkipMiso = 0;
    uint16_t txd_data = 0;
    uint16_t rxd_data = 0;

    if (!pu8Mosi) {
        pu8Mosi = &u8Dummy;
        u8SkipMosi = 1;
    }
    else if(!pu8Miso) {
        pu8Miso = &u8Dummy;
        u8SkipMiso = 1;
    }
    else {
        return M2M_ERR_BUS_FAIL;
    }

    //spi_select_device(CONF_WIFI_M2M_SPI_MODULE, &spi_device_conf);
    LATCbits.LATC1 = 0; /* set nCS output low */;

    while (u16Sz) {
        txd_data = *pu8Mosi;
        /* Write one byte */
        rxd_data = spiMaster[WINC].exchangeByte(txd_data);				

        *pu8Miso = rxd_data;

        u16Sz--;
        if (!u8SkipMiso)
            pu8Miso++;
        if (!u8SkipMosi)
            pu8Mosi++;
    }
    LATCbits.LATC1 = 1; /* set nCS output high */;

    return M2M_SUCCESS;
}
#endif

/*
*	@fn		nm_bus_init
*	@brief	Initialize the bus wrapper
*	@return	M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*/
sint8 volatile nm_bus_init(void *pvinit)
{
    sint8 result = M2M_SUCCESS;

#ifdef CONF_WINC_USE_I2C
    /* Not implemented */
#elif defined CONF_WINC_USE_SPI
    /* Configure pins */
    TRISCbits.TRISC1 = 0; /* set nCS as output */;
    LATCbits.LATC1 = 1; /* set nCS output high */;

    /* Configure IOC - TODO: Workaround for CC16DEV-1563 */
    IOCNAbits.IOCNA12 = 1; /* enable positive interrupt for INT */
    PADCONbits.IOCON = 1; /* enable interrupt on change configuration */

    //struct spi_device spi_device_conf;
    //spi_device_conf.id = CONF_WIFI_M2M_SPI_CS_PIN;

    /* Configure the SPI master. */
    spiMaster[WINC].spiOpen();

    //spi_master_setup_device(CONF_WIFI_M2M_SPI_MODULE, &spi_device_conf, SPI_MODE_0, CONF_WIFI_M2M_SPI_BAUDRATE, 0);

    /* Enable the SPI master. */
    //spi_enable(CONF_WIFI_M2M_SPI_MODULE);

    nm_bsp_reset();
    nm_bsp_sleep(1);
#endif
    return result;
}

/*
*	@fn		nm_bus_ioctl
*	@brief	send/receive from the bus
*	@param[IN]	u8Cmd
*					IOCTL command for the operation
*	@param[IN]	pvParameter
*					Arbitrary parameter depenging on IOCTL
*	@return	M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@note	For SPI only, it's important to be able to send/receive at the same time
*/
sint8 nm_bus_ioctl(uint8 u8Cmd, void* pvParameter)
{
    sint8 s8Ret = 0;
    switch(u8Cmd)
    {
#ifdef CONF_WINC_USE_I2C
        case NM_BUS_IOCTL_R: {
            tstrNmI2cDefault *pstrParam = (tstrNmI2cDefault *)pvParameter;
            s8Ret = nm_i2c_read(pstrParam->pu8Buf, pstrParam->u16Sz);
        }
        break;
        case NM_BUS_IOCTL_W: {
            tstrNmI2cDefault *pstrParam = (tstrNmI2cDefault *)pvParameter;
            s8Ret = nm_i2c_write(pstrParam->pu8Buf, pstrParam->u16Sz);
        }
        break;
        case NM_BUS_IOCTL_W_SPECIAL: {
            tstrNmI2cSpecial *pstrParam = (tstrNmI2cSpecial *)pvParameter;
            s8Ret = nm_i2c_write_special(pstrParam->pu8Buf1, pstrParam->u16Sz1, pstrParam->pu8Buf2, pstrParam->u16Sz2);
        }
        break;
#elif defined CONF_WINC_USE_SPI
        case NM_BUS_IOCTL_RW: {
            tstrNmSpiRw *pstrParam = (tstrNmSpiRw *)pvParameter;
            s8Ret = spi_rw(pstrParam->pu8InBuf, pstrParam->pu8OutBuf, pstrParam->u16Sz);
        }
        break;
#endif
        default:
            s8Ret = -1;
            M2M_ERR("invalide ioclt cmd\n");
            break;
    }

    return s8Ret;
}

/*
*	@fn		nm_bus_deinit
*	@brief	De-initialize the bus wrapper
*/
sint8 nm_bus_deinit(void)
{
    return M2M_SUCCESS;
}

/*
*	@fn			nm_bus_reinit
*	@brief		re-initialize the bus wrapper
*	@param [in]	void *config
*					re-init configuration data
*	@return		M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author		Dina El Sissy
*	@date		19 Sept 2012
*	@version	1.0
*/
sint8 nm_bus_reinit(void* config)
{
    return M2M_SUCCESS;
}

