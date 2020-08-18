/*
    \file   crypto_client.h

    \brief  Crypto Client header file.

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

#ifndef CRYPTO_CLIENT_H
#define CRYPTO_CLIENT_H

#define ERROR 1
#define NO_ERROR 0

#include <stdint.h>
#include "../../cryptoauthlib/lib/atca_iface.h"
#include "../../cryptoauthlib/lib/cryptoauthlib.h"
#include "../../winc/driver/include/ecc_types.h"

extern uint8_t cryptoDeviceInitialized;
extern ATCAIfaceCfg cfg_ateccx08a_i2c_custom;

uint8_t CRYPTO_CLIENT_printPublicKey(char *s);
uint8_t CRYPTO_CLIENT_printSerialNumber(char *s);
void CRYPTO_CLIENT_processEccRequest(tstrEccReqInfo *ecc_request);
int8_t ecdsa_process_sign_verify_request(uint32_t number_of_signatures);
int8_t ecdh_derive_key_pair(tstrECPoint *server_public_key);
int8_t ecdh_derive_client_shared_secret(tstrECPoint *server_public_key, uint8_t *ecdh_shared_secret, tstrECPoint *client_public_key);
int8_t ecdsa_process_sign_gen_request(tstrEcdsaSignReqInfo *sign_request, uint8_t *signature, uint16_t *signature_size);
int8_t ecdh_derive_server_shared_secret(uint16_t private_key_id, tstrECPoint *client_public_key, uint8_t *ecdh_shared_secret);

#endif /* CRYPTO_CLIENT_H */
