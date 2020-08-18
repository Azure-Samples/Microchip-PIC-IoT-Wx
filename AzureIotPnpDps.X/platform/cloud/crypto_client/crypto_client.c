/*
    \file   crypto_client.c

    \brief  Crypto Client source file.

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
#include <string.h>
#include "../../config/cryptoauthlib_config.h"
#include "crypto_client.h"
#include "../cloud_service.h"

#include "../../winc/driver/include/m2m_ssl.h"
#include "../../debug_print.h"

#ifndef ATCA_NO_HEAP
#error : This project uses CryptoAuthLibrary V2. Please add "ATCA_NO_HEAP" to toolchain symbols.
#endif

#ifndef ATCA_NO_POLL
#error : This project uses ATCA_NO_POLL option. Please add "ATCA_NO_POLL" to toolchain symbols.
#endif

#ifndef ATCA_HAL_I2C
#error : This project uses I2C interface. Please add "ATCA_HAL_I2C" to toolchain symbols.
#endif

#ifndef ATCA_PRINTF
#error : This project uses ATCA_debug_print. Please add "ATCA_PRINTF" to toolchain symbols.
#endif

#define DEVICE_KEY_SLOT            (0)
static uint32_t g_ecdh_key_slot_index = 0;
static uint16_t g_ecdh_key_slot[] = {2};

const uint8_t public_key_x509_header[] = { 0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2A, 0x86,
    0x48, 0xCE, 0x3D, 0x02, 0x01, 0x06, 0x08, 0x2A,
    0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07, 0x03,
0x42, 0x00, 0x04 };

uint8_t g_serial_number[ATCA_SERIAL_NUM_SIZE];

/** \brief custom configuration for an ECCx08A device */
ATCAIfaceCfg cfg_ateccx08a_i2c_custom = {
    .iface_type             = ATCA_I2C_IFACE,
    .devtype                = ATECC608A,
    .atcai2c.slave_address  = 0xB0,
    .atcai2c.bus            = 2,
    .atcai2c.baud           = 100000,
    .wake_delay             = 1500,
    .rx_retries             = 20
};
	
uint8_t cryptoDeviceInitialized = false;

uint8_t CRYPTO_CLIENT_printPublicKey(char *s)
{
    char buf[128];
    uint8_t *tmp;
    size_t bufferLen = sizeof(buf);
    ATCA_STATUS retVal;
    
    if (ATCA_SUCCESS != atcab_init(&cfg_ateccx08a_i2c_custom))
    {
        return ERROR;
    }
    
    /* Calculate where the raw data will fit into the buffer */
    tmp = (uint8_t*)buf + sizeof(buf) - ATCA_PUB_KEY_SIZE - sizeof(public_key_x509_header);
    
    /* Copy the header */
    memcpy(tmp, public_key_x509_header, sizeof(public_key_x509_header));
    
    /* Get public key without private key generation */
    retVal = atcab_get_pubkey(0, tmp + sizeof(public_key_x509_header));
    
    if (ATCA_SUCCESS != retVal )
    {
        return ERROR;
    }
    
    /* Convert to base 64 */
    retVal = atcab_base64encode(tmp, ATCA_PUB_KEY_SIZE + sizeof(public_key_x509_header), buf, &bufferLen);
    
    if(ATCA_SUCCESS != retVal)
    {
        return ERROR;
    }
    
    /* Add a null terminator */
    buf[bufferLen] = 0;
    
    /* Print out the key */
    sprintf(s, "-----BEGIN PUBLIC KEY-----\r\n%s\r\n-----END PUBLIC KEY-----\r\n\4", buf);
    
    return NO_ERROR;
}

uint8_t CRYPTO_CLIENT_printSerialNumber(char *s)
{
    ATCA_STATUS status = ATCA_SUCCESS;
	uint8_t i = 0;

    int retVal = atcab_init(&cfg_ateccx08a_i2c_custom);

    if (ATCA_SUCCESS != retVal)
    {
        return retVal;
    }

    status = atcab_read_serial_number(g_serial_number);

    if (status == ATCA_SUCCESS)
    {
        for (i = 0; i < ATCA_SERIAL_NUM_SIZE; i++)
        {
            sprintf(s, "%02X", g_serial_number[i]);
            s += 2;
        }
    }
    else{
        return ERROR;
    }

    return NO_ERROR;
}

int8_t ecdh_derive_client_shared_secret(tstrECPoint *server_public_key, uint8_t *ecdh_shared_secret, tstrECPoint *client_public_key)
{
    int8_t status = M2M_ERR_FAIL;
    uint8_t ecdh_mode;
    uint16_t key_id;
    
    if ((g_ecdh_key_slot_index < 0) ||
    (g_ecdh_key_slot_index >= (sizeof(g_ecdh_key_slot) / sizeof(g_ecdh_key_slot[0]))))
    {
        g_ecdh_key_slot_index = 0;
    }
    
    if(_gDevice->mIface->mIfaceCFG->devtype == ATECC608A)
    {
        //do special ecdh functions for the 608, keep ephemeral keys in SRAM
        ecdh_mode = ECDH_MODE_SOURCE_TEMPKEY | ECDH_MODE_COPY_OUTPUT_BUFFER;
        key_id = GENKEY_PRIVATE_TO_TEMPKEY;
    }
    else
    {
        //specializations for the 508, use an EEPROM key slot
        ecdh_mode = ECDH_PREFIX_MODE;
        key_id = g_ecdh_key_slot[g_ecdh_key_slot_index];
        g_ecdh_key_slot_index++;
    }
    
    //generate an ephemeral key
    if(atcab_genkey(key_id, client_public_key->X) == ATCA_SUCCESS)
    {
        client_public_key->u16Size = 32;
        //do the ecdh from the private key in tempkey, results put in ecdh_shared_secret
        if(atcab_ecdh_base(ecdh_mode, key_id, server_public_key->X, ecdh_shared_secret, NULL) == ATCA_SUCCESS)
        {
            status = M2M_SUCCESS;
        }
    }

    return status;
}

int8_t ecdh_derive_key_pair(tstrECPoint *server_public_key)
{
    int8_t status = M2M_ERR_FAIL;
    
    if ((g_ecdh_key_slot_index < 0) ||
    (g_ecdh_key_slot_index >= (sizeof(g_ecdh_key_slot) / sizeof(g_ecdh_key_slot[0]))))
    {
        g_ecdh_key_slot_index = 0;
    }

    if( (status = atcab_genkey(g_ecdh_key_slot[g_ecdh_key_slot_index], server_public_key->X) ) == ATCA_SUCCESS)
    {
        server_public_key->u16Size      = 32;
        server_public_key->u16PrivKeyID = g_ecdh_key_slot[g_ecdh_key_slot_index];

        g_ecdh_key_slot_index++;

        status = M2M_SUCCESS;
    }

    return status;
}

int8_t ecdsa_process_sign_verify_request(uint32_t number_of_signatures)
{
    int8_t status = M2M_ERR_FAIL;
    tstrECPoint    Key;
    uint32_t index = 0;
    uint8_t signature[80];
    uint8_t hash[80] = {0};
    uint16_t curve_type = 0;
    
    for(index = 0; index < number_of_signatures; index++)
    {
        status = m2m_ssl_retrieve_cert(&curve_type, hash, signature, &Key);

        if (status != M2M_SUCCESS)
        {
            debug_printInfo("m2m_ssl_retrieve_cert() failed with ret=%d", status);
            return status;
        }

        if(curve_type == EC_SECP256R1)
        {
            bool is_verified = false;
            
            status = atcab_verify_extern(hash, signature, Key.X, &is_verified);
            if(status == ATCA_SUCCESS)
            {
                status = (is_verified == true) ? M2M_SUCCESS : M2M_ERR_FAIL;
                if(is_verified == false)
                {
                    debug_printInfo("ECDSA SigVerif FAILED");
                }
            }
            else
            {
                status = M2M_ERR_FAIL;
            }
            
            if(status != M2M_SUCCESS)
            {
                m2m_ssl_stop_processing_certs();
                break;
            }
        }
    }

    return status;
}

int8_t ecdsa_process_sign_gen_request(tstrEcdsaSignReqInfo *sign_request, uint8_t *signature, uint16_t *signature_size)
{
    int8_t status = M2M_ERR_FAIL;
    uint8_t hash[32];
    
    status = m2m_ssl_retrieve_hash(hash, sign_request->u16HashSz);
    if (status != M2M_SUCCESS)
    {
        debug_printInfo("m2m_ssl_retrieve_hash() failed with ret=%d", status);
        return status;
    }

    if(sign_request->u16CurveType == EC_SECP256R1)
    {
        *signature_size = 64;
        status = atcab_sign(DEVICE_KEY_SLOT, hash, signature);
    }

    return status;
}

int8_t ecdh_derive_server_shared_secret(uint16_t private_key_id, tstrECPoint *client_public_key, uint8_t *ecdh_shared_secret)
{
    uint16_t key_slot    = private_key_id;
    int8_t status = M2M_ERR_FAIL;
    uint8_t atca_status = ATCA_STATUS_UNKNOWN;

    atca_status = atcab_ecdh(key_slot, client_public_key->X, ecdh_shared_secret);
    if(atca_status == ATCA_SUCCESS)
    {
        status = M2M_SUCCESS;
    }
    else
    {
        debug_printInfo("__SLOT = %u, Err = %X", key_slot, atca_status);
    }
    
    return status;
}

void CRYPTO_CLIENT_processEccRequest(tstrEccReqInfo *ecc_request)
{
    tstrEccReqInfo ecc_response;
    uint8_t signature[80];
    uint16_t response_data_size = 0;
    uint8_t *response_data_buffer = NULL;
    
    ecc_response.u16Status = 1;
    
    switch (ecc_request->u16REQ)
    {
        case ECC_REQ_CLIENT_ECDH:
        ecc_response.u16Status = ecdh_derive_client_shared_secret(&(ecc_request->strEcdhREQ.strPubKey),
        ecc_response.strEcdhREQ.au8Key,
        &ecc_response.strEcdhREQ.strPubKey);
        break;

        case ECC_REQ_GEN_KEY:
        ecc_response.u16Status = ecdh_derive_key_pair(&ecc_response.strEcdhREQ.strPubKey);
        break;

        case ECC_REQ_SERVER_ECDH:
        ecc_response.u16Status = ecdh_derive_server_shared_secret(ecc_request->strEcdhREQ.strPubKey.u16PrivKeyID,
        &(ecc_request->strEcdhREQ.strPubKey),
        ecc_response.strEcdhREQ.au8Key);
        break;
        
        case ECC_REQ_SIGN_VERIFY:
        ecc_response.u16Status = ecdsa_process_sign_verify_request(ecc_request->strEcdsaVerifyREQ.u32nSig);
        break;
        
        case ECC_REQ_SIGN_GEN:
        ecc_response.u16Status = ecdsa_process_sign_gen_request(&(ecc_request->strEcdsaSignREQ), signature,
        &response_data_size);
        response_data_buffer = signature;
        break;
        
        default:
        // Do nothing
        break;
    }
    
    ecc_response.u16REQ      = ecc_request->u16REQ;
    ecc_response.u32UserData = ecc_request->u32UserData;
    ecc_response.u32SeqNo    = ecc_request->u32SeqNo;

    m2m_ssl_ecc_process_done();
    m2m_ssl_handshake_rsp(&ecc_response, response_data_buffer, response_data_size);
}