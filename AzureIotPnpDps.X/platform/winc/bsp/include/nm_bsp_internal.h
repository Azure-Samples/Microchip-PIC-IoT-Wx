/**
 *
 * \file
 *
 * \brief This module contains NMC1500 BSP APIs declarations.
 *
 * Copyright (c) 2016 Atmel Corporation. All rights reserved.
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
/**@defgroup  BSPDefine Defines
 * @ingroup nm_bsp
 * @{
 */
#ifndef _NM_BSP_INTERNAL_H_
#define _NM_BSP_INTERNAL_H_



#ifdef WIN32
#include "nm_bsp_win32.h"
#endif

#ifdef __K20D50M__
#include "nm_bsp_k20d50m.h"
#endif

#ifdef __MSP430FR5739__
#include "bsp_msp430fr5739.h"
#endif

#ifdef _FREESCALE_MCF51CN128_
#include "bsp/include/nm_bsp_mcf51cn128.h"
#endif

#ifdef __MCF964548__
#include "bsp/include/nm_bsp_mc96f4548.h"
#endif

#ifdef __APP_APS3_CORTUS__
#include "nm_bsp_aps3_cortus.h"
#endif

#if (defined __SAMR21G18A__)
#include "bsp/include/nm_bsp_samr21.h"
#endif

#if (defined __SAML21J18A__) || (defined __SAML21J18B__)
#include "bsp/include/nm_bsp_saml21.h"
#endif

#if (defined __SAML22N18A__)
#include "bsp/include/nm_bsp_saml22.h"
#endif

#if (defined __SAMD21J18A__) || (defined __SAMD21G18A__)
#include "bsp/include/nm_bsp_samd21.h"
#endif

#if (defined __SAM4S16C__) || (defined __SAM4SD32C__)
#include "bsp/include/nm_bsp_sam4s.h"
#endif

#ifdef __SAMG53N19__
#include "bsp/include/nm_bsp_samg53.h"
#endif

#ifdef __SAMG55J19__
#include "bsp/include/nm_bsp_samg55.h"
#endif

#if (defined __SAME70Q21__) || (defined __SAMV71Q21__)
#include "bsp/include/nm_bsp_same70.h"
#endif

#ifdef CORTUS_APP
#include "crt_iface.h"
#endif

#ifdef NRF51
#include "nm_bsp_nrf51822.h"
#endif

#ifdef _ARDUINO_UNO_
#include "bsp/include/nm_bsp_arduino_uno.h"
#endif


#endif //_NM_BSP_INTERNAL_H_