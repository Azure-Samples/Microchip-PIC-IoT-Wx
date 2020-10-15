/** \file atcatls_cfg.c
 * \brief define
 *
 * \copyright (c) 2017 Microchip Technology Inc. and its subsidiaries.
 *            You may use this software and any derivatives exclusively with
 *            Microchip products.
 *
 * \page License
 *
 * (c) 2017 Microchip Technology Inc. and its subsidiaries. You may use this
 * software and any derivatives exclusively with Microchip products.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIPS TOTAL LIABILITY ON ALL CLAIMS IN
 * ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
 * TERMS.
 */


#include "atcatls_cfg.h"
//#include "certs/cert_def_1_signer.h"
//#include "certs/cert_def_2_device.h"
//#include "atcacert/atcacert_def.h"
#include "../basic/atca_basic.h"
#include <stdio.h>

/** \defgroup auth Node authentication stages for node-auth-basic example
 *
   @{ */

// Modified W25 ECC508 configuration
static const uint8_t g_ecc_configdata[] = {
    // Block1 Start
    0x01,  0x23,  0x00,  0x00, // Not Writable: First 16 bytes are not written
    0x00,  0x00,  0x50,  0x00,
    0x04,  0x05,  0x06,  0x07,
    0xEE,  0x00,  0x01,  0x00,
    D_I2C, 0x00,  0x55,  0x00,
    0x8F,  0x20,  0xC4,  0x44, // SlotConfig 0, 1
    0x87,  0x20,  0xC4,  0x44,
    0x8F,  0x0F,  0xC4,  0x26,
    // Block2 Start -
    0x9F,  0x0F,  0x8F,  0x20,
    0x0F,  0x0F,  0x0F,  0x0F, // SlotConfig 8, 9
    0x0F,  0x0F,  0x0F,  0x0F,
    0x0F,  0x0F,  0x0F,  0x0F,
    0x0F,  0x0F,  0x0F,  0x0F,
    0xFF,  0xFF,  0xFF,  0xFF,
    0x00,  0x00,  0x00,  0x00,
    0xFF,  0xFF,  0xFF,  0xFF,
    // Block2 Start -
    0x00,  0x00,  0x00,  0x00,
    0xFF,  0xFF,  0xFF,  0xFF,
    0xFF,  0xFF,  0xFF,  0xFF,
    0xFF,  0xFF,  0xFF,  0xFF,
    0xFF,  0xFF,  0xFF,  0xFF,
    0x00,  0x00,  0x00,  0x00, // Not Writable: UserExtra, Selector, LockValue, LockConfig (word offset = 5)
    0xFF,  0xFF,  0x00,  0x00,
    0x00,  0x00,  0x00,  0x00,
    // Block3 Start -
    0x33,  0x00,  0x1C,  0x00, // KeyConfig 0, 1
    0x13,  0x00,  0x1C,  0x00,
    0x3C,  0x00,  0x1C,  0x00,
    0x3C,  0x00,  0x33,  0x00,
    0x3C,  0x00,  0x3C,  0x00, // KeyConfig 8, 9
    0x3C,  0x00,  0x3C,  0x00,
    0x3C,  0x00,  0x3C,  0x00,
    0x3C,  0x00,  0x3C,  0x00
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// NOTE: IMPORTANT!!!
//       NEVER! NEVER! EXPOSE THE ROOT PRIVATE KEY IN PRODUCTION APPLICATIONS.
//       THIS IS FOR EXAMPLE ONLY
//
//   This private root key is for development interoperability
//   This root key is coordinated with the root key in the node_auth_basic project for development interoperability.
//   Root key to be written to the device
static const uint8_t g_signer_ca_private_key[36] = {
    0x00, 0x00, 0x00, 0x00,
    0x49, 0x0c, 0x1e, 0x09,0xe2,  0x40,  0xaf,  0x00,  0xe9,  0x6b,  0x43, 0x32, 0x92, 0x0e, 0x15, 0x8f,
    0x69, 0x58, 0x8e, 0xd4,0x25,  0xc7,  0xf6,  0x8b,  0x0c,  0x6a,  0x52, 0x5d, 0x0d, 0x21, 0x4f, 0xee
};

// Certificate definitions
uint8_t g_signer_ca_public_key[64];
const uint8_t g_signer_0_ca_public_key[];
const atcacert_def_t g_cert_def_0_signer;
const atcacert_def_t g_cert_def_0_device;

/** \brief example of building and saving a cert in the ATECC508A.  Normally this would be done during
 * production and provisioning at a factory and therefore things like the root CA public key and
 * other attributes would be coming from the server and HSM.   This is a functional piece of code
 * which emulates that process for illustration, example, and demonstration purposes.
 */
static int build_and_save_cert(
    const atcacert_def_t*    cert_def,
    uint8_t*                 cert,
    size_t*                  cert_size,
    const uint8_t            ca_public_key[64],
    const uint8_t            public_key[64],
    const uint8_t            signer_id[2],
    const atcacert_tm_utc_t* issue_date,
    const uint8_t            config32[32],
    uint8_t                  ca_slot)
{
    int ret;
    atcacert_build_state_t build_state;
    uint8_t tbs_digest[32];
    uint8_t signature[64];
    size_t max_cert_size = *cert_size;
    atcacert_tm_utc_t expire_date = {
        .tm_year = issue_date->tm_year + cert_def->expire_years,
        .tm_mon  = issue_date->tm_mon,
        .tm_mday = issue_date->tm_mday,
        .tm_hour = issue_date->tm_hour,
        .tm_min  = 0,
        .tm_sec  = 0
    };
    const atcacert_device_loc_t config32_dev_loc = {
        .zone   = DEVZONE_CONFIG,
        .offset = 0,
        .count  = 32
    };
    atcacert_device_loc_t device_locs[4];
    size_t device_locs_count = 0;
    size_t i;

    if (cert_def->expire_years == 0)
    {
        ret = atcacert_date_get_max_date(cert_def->expire_date_format, &expire_date);
        if (ret != ATCACERT_E_SUCCESS)
        {
            return ret;
        }
    }

    ret = atcacert_cert_build_start(&build_state, cert_def, cert, cert_size, ca_public_key);
    if (ret != ATCACERT_E_SUCCESS)
    {
        return ret;
    }

    ret = atcacert_set_subj_public_key(build_state.cert_def, build_state.cert, *build_state.cert_size, public_key);
    if (ret != ATCACERT_E_SUCCESS)
    {
        return ret;
    }
    ret = atcacert_set_issue_date(build_state.cert_def, build_state.cert, *build_state.cert_size, issue_date);
    if (ret != ATCACERT_E_SUCCESS)
    {
        return ret;
    }
    ret = atcacert_set_expire_date(build_state.cert_def, build_state.cert, *build_state.cert_size, &expire_date);
    if (ret != ATCACERT_E_SUCCESS)
    {
        return ret;
    }
    ret = atcacert_set_signer_id(build_state.cert_def, build_state.cert, *build_state.cert_size, signer_id);
    if (ret != ATCACERT_E_SUCCESS)
    {
        return ret;
    }
    ret = atcacert_cert_build_process(&build_state, &config32_dev_loc, config32);
    if (ret != ATCACERT_E_SUCCESS)
    {
        return ret;
    }

    ret = atcacert_cert_build_finish(&build_state);
    if (ret != ATCACERT_E_SUCCESS)
    {
        return ret;
    }

    ret = atcacert_get_tbs_digest(build_state.cert_def, build_state.cert, *build_state.cert_size, tbs_digest);
    if (ret != ATCACERT_E_SUCCESS)
    {
        return ret;
    }

    ret = atcab_sign(ca_slot, tbs_digest, signature);
    if (ret != ATCA_SUCCESS)
    {
        return ret;
    }

    ret = atcacert_set_signature(cert_def, cert, cert_size, max_cert_size, signature);
    if (ret != ATCACERT_E_SUCCESS)
    {
        return ret;
    }

    ret = atcacert_get_device_locs(cert_def, device_locs, &device_locs_count, sizeof(device_locs) / sizeof(device_locs[0]), 32);
    if (ret != ATCACERT_E_SUCCESS)
    {
        return ret;
    }

    for (i = 0; i < device_locs_count; i++)
    {
        size_t end_block;
        size_t start_block;
        size_t block;
        uint8_t data[96];

        if (device_locs[i].zone == DEVZONE_CONFIG)
        {
            continue;
        }
        if (device_locs[i].zone == DEVZONE_DATA && device_locs[i].is_genkey)
        {
            continue;
        }

        ret = atcacert_get_device_data(cert_def, cert, *cert_size, &device_locs[i], data);
        if (ret != ATCACERT_E_SUCCESS)
        {
            return ret;
        }

        start_block = device_locs[i].offset / 32;
        end_block = (device_locs[i].offset + device_locs[i].count) / 32;
        for (block = start_block; block < end_block; block++)
        {
            ret = atcab_write_zone(device_locs[i].zone, device_locs[i].slot, (uint8_t)block, 0, &data[(block - start_block) * 32], 32);
            if (ret != ATCA_SUCCESS)
            {
                return ret;
            }
        }
    }
    return 0;
}

/** \brief This function will write and lock the configuration zone, write and lock the data zone,
 *			run GenKey on private keys to prepare a device for a standard configuration.
 *			This configuration can be used for self-provisioning or as a generic configuration
 *			for illustration, example, or demo purposes.
 */
ATCA_STATUS device_init_default(void)
{
    return device_init(&g_cert_def_0_signer, &g_cert_def_0_device);
}

/** \brief This function will write and lock the configuration zone, write and lock the data zone,
 *			run GenKey on private keys to prepare a device for a standard configuration.
 *			This configuration can be used for self-provisioning or as a generic configuration
 *			for illustration, example, or demo purposes.
 */
ATCA_STATUS device_init(const atcacert_def_t* cert_def_signer, const atcacert_def_t* cert_def_device)
{
    ATCA_STATUS ret = 0;
    bool lock_state = 0;
    uint8_t signer_ca_private_key_slot = TLS_SLOT_DEV_CA_PRIV;
    uint8_t signer_private_key_slot = TLS_SLOT_DEV_SIGNER_PRIV;
    uint8_t signer_id[] = { 0xC4, 0x8B };
    const atcacert_tm_utc_t signer_issue_date = {
        .tm_year = 2014 - 1900,
        .tm_mon  = 8 - 1,
        .tm_mday = 2,
        .tm_hour = 20,
        .tm_min  = 0,
        .tm_sec  = 0
    };
    uint8_t device_private_key_slot = 0;
    const atcacert_tm_utc_t device_issue_date = {
        .tm_year = 2015 - 1900,
        .tm_mon  = 9 - 1,
        .tm_mday = 3,
        .tm_hour = 21,
        .tm_min  = 0,
        .tm_sec  = 0
    };
    static const uint8_t access_key_slot = TLS_SLOT_ENC_PARENT;
    const uint8_t access_key[] = {
        0x32, 0x12, 0xd0, 0x66, 0xf5, 0xed, 0x52, 0xc7, 0x79, 0x98, 0xff, 0xaa, 0xac, 0x43, 0x22, 0x60,
        0xdd, 0xff, 0x9c, 0x10, 0x99, 0x6f, 0x41, 0x66, 0x3a, 0x60, 0x23, 0xfa, 0xf6, 0xaa, 0x3e, 0xc5
    };
    uint8_t configdata[ATCA_ECC_CONFIG_SIZE] = { 0 };
    bool is_signer_ca_slot_ext_sig = false;
    bool is_signer_ca_slot_priv_write = false;
    uint8_t signer_public_key[ATCA_PUB_KEY_SIZE] = { 0 };
    uint8_t device_public_key[ATCA_PUB_KEY_SIZE] = { 0 };
    uint8_t signer_cert_ref[512] = { 0 };
    size_t signer_cert_ref_size = 0;
    uint8_t device_cert_ref[512] = { 0 };
    size_t device_cert_ref_size = 0;

    atcab_sleep();

    // Check the lock state of the config zone.  If unlocked, then write and lock.
    ret = atcab_is_locked(LOCK_ZONE_CONFIG, &lock_state);
    if (ret != ATCA_SUCCESS)
    {
        return ret;
    }
    if (!lock_state)
    {
        ret = atcab_write_config_zone(g_ecc_configdata);
        if (ret != ATCA_SUCCESS)
        {
            return ret;
        }

        ret = atcab_lock_config_zone();
        if (ret != ATCA_SUCCESS)
        {
            return ret;
        }
    }
    // Refresh the configuration zone data with the bytes that are now on the device
    ret = atcab_read_config_zone((uint8_t*)&configdata);

    is_signer_ca_slot_ext_sig = (configdata[20 + signer_ca_private_key_slot * 2] & 0x01);
    is_signer_ca_slot_priv_write = (configdata[20 + signer_ca_private_key_slot * 2 + 1] & 0x40);

    // Check the Data zone lock
    ret = atcab_is_locked(LOCK_ZONE_DATA, &lock_state);
    if (ret != ATCA_SUCCESS)
    {
        return ret;
    }
    if (!lock_state)
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // NOTE: IMPORTANT!!!
        //       NEVER! NEVER! EXPOSE THE ROOT PRIVATE KEY IN PRODUCTION APPLICATIONS.
        //       THIS IS FOR EXAMPLE ONLY
        ret = atcab_priv_write(signer_ca_private_key_slot, g_signer_ca_private_key, 0, NULL);
        if (ret != ATCA_SUCCESS)
        {
            return ret;
        }

        ret = atcab_write_zone(DEVZONE_DATA, access_key_slot, 0, 0, access_key, 32);
        if (ret != ATCA_SUCCESS)
        {
            return ret;
        }

        ret = atcab_lock_data_zone();
        if (ret != ATCA_SUCCESS)
        {
            return ret;
        }
    }
    else if (!is_signer_ca_slot_ext_sig)
    {
        // The signer CA slot can't perform external signs.
        // Use the signer slot for both. A little weird, but it lets the example run.
        printf("Signer CA slot %d not available. Signer CA and signer will be sharing a key.\r\n", signer_ca_private_key_slot);
        signer_ca_private_key_slot = signer_private_key_slot;
    }
    else if (is_signer_ca_slot_priv_write)
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        // NOTE: IMPORTANT!!!
        //       NEVER! NEVER! EXPOSE THE ROOT PRIVATE KEY IN PRODUCTION APPLICATIONS.
        //       THIS IS FOR EXAMPLE ONLY
        ret = atcab_priv_write(signer_ca_private_key_slot, g_signer_ca_private_key, access_key_slot, access_key);
        if (ret != ATCA_SUCCESS)
        {
            return printf("Warning: PrivWrite to slot %d failed. Example may still work though.\n", signer_ca_private_key_slot);
        }
    }

    // GenKey or get the public key for the root
    if (signer_ca_private_key_slot != signer_private_key_slot)
    {
        ret = atcab_get_pubkey(signer_ca_private_key_slot, g_signer_ca_public_key);
        if (ret == ATCA_EXECUTION_ERROR)
        {
            ret = atcab_genkey(signer_ca_private_key_slot, g_signer_ca_public_key);
        }
        if (ret != ATCA_SUCCESS)
        {
            return ret;
        }
        atcab_printbin_label("Signer CA Public Key:\r\n", g_signer_ca_public_key, sizeof(g_signer_ca_public_key));
    }

    // GenKey  the public key for the signer
    ret = atcab_genkey(signer_private_key_slot, signer_public_key);
    if (ret != ATCA_SUCCESS)
    {
        return ret;
    }
    atcab_printbin_label("Signer Public Key:\r\n", signer_public_key, sizeof(signer_public_key));

    // GenKey the device private key to get the public key for the device
    ret = atcab_genkey(device_private_key_slot, device_public_key);
    if (ret != ATCA_SUCCESS)
    {
        return ret;
    }
    atcab_printbin_label("Device Public Key:\r\n", device_public_key, sizeof(device_public_key));

    // Build signer cert
    signer_cert_ref_size = sizeof(signer_cert_ref);
    ret = build_and_save_cert(
        cert_def_signer,
        signer_cert_ref,
        &signer_cert_ref_size,
        g_signer_0_ca_public_key,
        signer_public_key,
        signer_id,
        &signer_issue_date,
        configdata,
        signer_ca_private_key_slot);
    if (ret != ATCA_SUCCESS)
    {
        return ret;
    }

    atcab_printbin_label("Signer Certificate:\r\n", signer_cert_ref, signer_cert_ref_size);

    // Build the device cert
    device_cert_ref_size = sizeof(device_cert_ref);
    ret = build_and_save_cert(
        cert_def_device,
        device_cert_ref,
        &device_cert_ref_size,
        signer_public_key,
        device_public_key,
        signer_id,
        &device_issue_date,
        configdata,
        signer_private_key_slot);
    if (ret != ATCA_SUCCESS)
    {
        return ret;
    }

    atcab_printbin_label("Device Certificate:\r\n", device_cert_ref, device_cert_ref_size);

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
// Certificate Templates

// Signer Certificate Template
const uint8_t g_signer_0_ca_public_key[64] = {
    0xF9, 0x29, 0x04, 0xF3, 0xBF, 0xD0, 0x6C, 0x5C, 0x42, 0x02, 0x4C, 0xC3, 0x5E, 0x04,  0x8B, 0x3F,
    0xA3, 0xC7, 0xC3, 0xC8, 0x07, 0xC6, 0x6F, 0xA9, 0xE4, 0x18, 0x42, 0x1E, 0x34, 0x0F,  0x94, 0x7C,
    0xBD, 0x4D, 0xDF, 0x52, 0x6F, 0xD9, 0x5A, 0xA0, 0x55, 0x7E, 0x48, 0x08, 0xAC, 0x11,  0xFB, 0x85,
    0x50, 0x09, 0xCF, 0x59, 0xDB, 0x4F, 0x05, 0x0B, 0x4D, 0x3D, 0x67, 0xAA, 0xE9, 0x97,  0xC4, 0x91
};

const uint8_t g_cert_template_0_signer[] = {
    0x30, 0x82, 0x01, 0xF0, 0x30, 0x82, 0x01, 0x97, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02,  0x11, 0x69,
    0x8A, 0x50, 0x0D, 0x71, 0xA3, 0xFC, 0x37, 0xA5, 0xB8, 0x1D, 0x44, 0x05, 0xBA, 0x2F,  0xC4, 0x01,
    0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02, 0x30, 0x43,  0x31, 0x1D,
    0x30, 0x1B, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x0C, 0x14, 0x45, 0x78, 0x61, 0x6D, 0x70,  0x6C, 0x65,
    0x20, 0x4F, 0x72, 0x67, 0x61, 0x6E, 0x69, 0x7A, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x31,  0x22, 0x30,
    0x20, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x19, 0x45, 0x78, 0x61, 0x6D, 0x70, 0x6C,  0x65, 0x20,
    0x41, 0x54, 0x45, 0x43, 0x43, 0x35, 0x30, 0x38, 0x41, 0x20, 0x52, 0x6F, 0x6F, 0x74,  0x20, 0x43,
    0x41, 0x30, 0x20, 0x17, 0x0D, 0x31, 0x35, 0x31, 0x32, 0x31, 0x37, 0x32, 0x33, 0x30,  0x30, 0x30,
    0x30, 0x5A, 0x18, 0x0F, 0x39, 0x39, 0x39, 0x39, 0x31, 0x32, 0x33, 0x31, 0x32, 0x33,  0x35, 0x39,
    0x35, 0x39, 0x5A, 0x30, 0x47, 0x31, 0x1D, 0x30, 0x1B, 0x06, 0x03, 0x55, 0x04, 0x0A,  0x0C, 0x14,
    0x45, 0x78, 0x61, 0x6D, 0x70, 0x6C, 0x65, 0x20, 0x4F, 0x72, 0x67, 0x61, 0x6E, 0x69,  0x7A, 0x61,
    0x74, 0x69, 0x6F, 0x6E, 0x31, 0x26, 0x30, 0x24, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C,  0x1D, 0x45,
    0x78, 0x61, 0x6D, 0x70, 0x6C, 0x65, 0x20, 0x41, 0x54, 0x45, 0x43, 0x43, 0x35, 0x30,  0x38, 0x41,
    0x20, 0x53, 0x69, 0x67, 0x6E, 0x65, 0x72, 0x20, 0x30, 0x30, 0x33, 0x34, 0x30, 0x59,  0x30, 0x13,
    0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01, 0x06, 0x08, 0x2A, 0x86, 0x48,  0xCE, 0x3D,
    0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x44, 0x0A, 0xE6, 0xC5, 0x5E, 0x3D, 0xD5,  0xBE, 0x2A,
    0xCE, 0x49, 0x00, 0x4B, 0x1A, 0x8D, 0xF3, 0x01, 0x1B, 0x2C, 0x57, 0x26, 0x86, 0xE1,  0x95, 0x9F,
    0xF1, 0x5C, 0x71, 0x18, 0x06, 0x8E, 0xFC, 0x6D, 0xB1, 0x1B, 0x05, 0x8C, 0xE2, 0xBD,  0xEF, 0x96,
    0xD9, 0x54, 0x53, 0x09, 0x81, 0x57, 0xB9, 0xF2, 0x8A, 0x90, 0x40, 0x8B, 0x55, 0x70,  0x83, 0x52,
    0xA6, 0xEB, 0x5A, 0x8B, 0x7F, 0xC1, 0x74, 0xA3, 0x66, 0x30, 0x64, 0x30, 0x12, 0x06,  0x03, 0x55,
    0x1D, 0x13, 0x01, 0x01, 0xFF, 0x04, 0x08, 0x30, 0x06, 0x01, 0x01, 0xFF, 0x02, 0x01,  0x00, 0x30,
    0x0E, 0x06, 0x03, 0x55, 0x1D, 0x0F, 0x01, 0x01, 0xFF, 0x04, 0x04, 0x03, 0x02, 0x02,  0x84, 0x30,
    0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16, 0x04, 0x14, 0x04, 0xCD, 0xE7, 0x19,  0x2E, 0x83,
    0x65, 0xA4, 0xC5, 0x3B, 0xAE, 0xA9, 0x8C, 0xAC, 0xD2, 0x1C, 0xAF, 0xFB, 0xCF, 0x2C,  0x30, 0x1F,
    0x06, 0x03, 0x55, 0x1D, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x37, 0x0A, 0xA1,  0x3A, 0xEB,
    0xEB, 0xCF, 0x09, 0x89, 0x70, 0x82, 0x6B, 0x3A, 0xB2, 0x74, 0xFB, 0x96, 0x72, 0x68,  0xD3, 0x30,
    0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02, 0x03, 0x47, 0x00,  0x30, 0x44,
    0x02, 0x20, 0x77, 0x5E, 0x46, 0x8A, 0xA4, 0x72, 0x23, 0xE8, 0x26, 0x15, 0x9F, 0x1F,  0xF2, 0x71,
    0xE5, 0x0B, 0x73, 0x4A, 0xA8, 0x99, 0xF4, 0xC4, 0xFE, 0x56, 0x65, 0xA7, 0xE6, 0xF7,  0x0A, 0x0B,
    0xE2, 0xCB, 0x02, 0x20, 0x1A, 0x98, 0x24, 0xEC, 0xD0, 0x51, 0x0D, 0x98, 0xD2, 0x1F,  0xE9, 0x88,
    0x83, 0x4C, 0x1F, 0x5F, 0x75, 0xCB, 0x97, 0xD3, 0x0C, 0x53, 0x58, 0xFB, 0x58, 0x30,  0x0A, 0xAC,
    0x1B, 0x45, 0xBA, 0x1D
};

const atcacert_def_t g_cert_def_0_signer = {
    .type                = CERTTYPE_X509,
    .template_id         = 1,
    .chain_id            = 0,
    .private_key_slot    = 0,
    .sn_source           = SNSRC_PUB_KEY_HASH,
    .cert_sn_dev_loc     = {
        .zone            = DEVZONE_NONE,
        .slot            = 0,
        .is_genkey       = 0,
        .offset          = 0,
        .count           = 0
    },
    .issue_date_format   = DATEFMT_RFC5280_UTC,
    .expire_date_format  = DATEFMT_RFC5280_GEN,
    .tbs_cert_loc        = {
        .offset          = 4,
        .count           = 411
    },
    .expire_years        = 0,
    .public_key_dev_loc  = {
        .zone            = DEVZONE_DATA,
        .slot            = 11,
        .is_genkey       = 0,
        .offset          = 0,
        .count           = 72
    },
    .comp_cert_dev_loc   = {
        .zone            = DEVZONE_DATA,
        .slot            = 12,
        .is_genkey       = 0,
        .offset          = 0,
        .count           = 72
    },
    .std_cert_elements   = {
        {   // STDCERT_PUBLIC_KEY
            .offset      = 247,
            .count       = 64
        },
        {   // STDCERT_SIGNATURE
            .offset      = 427,
            .count       = 74
        },
        {   // STDCERT_ISSUE_DATE
            .offset      = 117,
            .count       = 13
        },
        {   // STDCERT_EXPIRE_DATE
            .offset      = 132,
            .count       = 15
        },
        {   // STDCERT_SIGNER_ID
            .offset      = 216,
            .count       = 4
        },
        {   // STDCERT_CERT_SN
            .offset      = 15,
            .count       = 16
        },
        {   // STDCERT_AUTH_KEY_ID
            .offset      = 395,
            .count       = 20
        },
        {   // STDCERT_SUBJ_KEY_ID
            .offset      = 362,
            .count       = 20
        }
    },
    .cert_elements       = NULL,
    .cert_elements_count = 0,
    .cert_template       = g_cert_template_0_signer,
    .cert_template_size  = sizeof(g_cert_template_0_signer),
};


// Default Device Certificate Template

const uint8_t g_cert_template_0_device[] = {
    0x30, 0x82, 0x01, 0xAE, 0x30, 0x82, 0x01, 0x53, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02,  0x11, 0x40,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,  0x0F, 0x02,
    0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02, 0x30, 0x47,  0x31, 0x1D,
    0x30, 0x1B, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x0C, 0x14, 0x45, 0x78, 0x61, 0x6D, 0x70,  0x6C, 0x65,
    0x20, 0x4F, 0x72, 0x67, 0x61, 0x6E, 0x69, 0x7A, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x31,  0x26, 0x30,
    0x24, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x1D, 0x45, 0x78, 0x61, 0x6D, 0x70, 0x6C,  0x65, 0x20,
    0x41, 0x54, 0x45, 0x43, 0x43, 0x35, 0x30, 0x38, 0x41, 0x20, 0x53, 0x69, 0x67, 0x6E,  0x65, 0x72,
    0x20, 0x30, 0x30, 0x33, 0x34, 0x30, 0x20, 0x17, 0x0D, 0x31, 0x35, 0x31, 0x32, 0x31,  0x36, 0x30,
    0x32, 0x35, 0x31, 0x33, 0x37, 0x5A, 0x18, 0x0F, 0x39, 0x39, 0x39, 0x39, 0x31, 0x32,  0x33, 0x31,
    0x32, 0x33, 0x35, 0x39, 0x35, 0x39, 0x5A, 0x30, 0x42, 0x31, 0x1D, 0x30, 0x1B, 0x06,  0x03, 0x55,
    0x04, 0x0A, 0x0C, 0x14, 0x45, 0x78, 0x61, 0x6D, 0x70, 0x6C, 0x65, 0x20, 0x4F, 0x72,  0x67, 0x61,
    0x6E, 0x69, 0x7A, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x31, 0x21, 0x30, 0x1F, 0x06, 0x03,  0x55, 0x04,
    0x03, 0x0C, 0x18, 0x45, 0x78, 0x61, 0x6D, 0x70, 0x6C, 0x65, 0x20, 0x41, 0x54, 0x45,  0x43, 0x43,
    0x35, 0x30, 0x38, 0x41, 0x20, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x30, 0x59, 0x30,  0x13, 0x06,
    0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE,  0x3D, 0x03,
    0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x81, 0x9A, 0x9B, 0x76, 0x23, 0xE3, 0x20, 0xE9,  0x85, 0x6D,
    0x74, 0xEE, 0xCC, 0x94, 0x60, 0x06, 0xDB, 0x7E, 0x64, 0xE3, 0x86, 0xB3, 0x3B, 0xF5,  0x08, 0x42,
    0x47, 0x39, 0x8E, 0x5D, 0xD3, 0x22, 0xAF, 0x5B, 0xDC, 0x57, 0xBB, 0xC6, 0xC0, 0xD2,  0x18, 0xF8,
    0x82, 0x01, 0x2F, 0x1E, 0x3B, 0xDA, 0xF7, 0xF2, 0xED, 0x22, 0xAA, 0xCA, 0x88, 0xF5,  0x7C, 0x3A,
    0xEE, 0x5A, 0x3C, 0x3E, 0x1D, 0xF5, 0xA3, 0x23, 0x30, 0x21, 0x30, 0x1F, 0x06, 0x03,  0x55, 0x1D,
    0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x04, 0xCD, 0xE7, 0x19, 0x2E, 0x83, 0x65,  0xA4, 0xC5,
    0x3B, 0xAE, 0xA9, 0x8C, 0xAC, 0xD2, 0x1C, 0xAF, 0xFB, 0xCF, 0x2C, 0x30, 0x0A, 0x06,  0x08, 0x2A,
    0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02, 0x03, 0x49, 0x00, 0x30, 0x46, 0x02, 0x21,  0x00, 0xC3,
    0x10, 0x54, 0xB6, 0x26, 0xE1, 0x02, 0x0B, 0x9E, 0xA8, 0x02, 0xE7, 0xA4, 0x58, 0x0F,  0xC3, 0x94,
    0x80, 0xA4, 0x64, 0xB2, 0xC8, 0x0A, 0x50, 0xE2, 0x69, 0x71, 0x87, 0x32, 0x4B, 0xC3,  0xAA, 0x02,
    0x21, 0x00, 0x84, 0xA2, 0x28, 0xED, 0xAB, 0x1D, 0xA9, 0x9A, 0x16, 0x36, 0x77, 0x2A,  0x1E, 0xE0,
    0xBB, 0x3B, 0x1F, 0x4B, 0xCF, 0xBC, 0x6B, 0xD2, 0x54, 0xBA, 0xB7, 0x5F, 0x79, 0xBB,  0x56, 0x44,
    0x30, 0xE4
};

const atcacert_def_t g_cert_def_0_device = {
    .type                = CERTTYPE_X509,
    .template_id         = 2,
    .chain_id            = 0,
    .private_key_slot    = 0,
    .sn_source           = SNSRC_PUB_KEY_HASH,
    .cert_sn_dev_loc     = {
        .zone            = DEVZONE_NONE,
        .slot            = 0,
        .is_genkey       = 0,
        .offset          = 0,
        .count           = 0
    },
    .issue_date_format   = DATEFMT_RFC5280_UTC,
    .expire_date_format  = DATEFMT_RFC5280_GEN,
    .tbs_cert_loc        = {
        .offset          = 4,
        .count           = 343
    },
    .expire_years        = 0,
    .public_key_dev_loc  = {
        .zone            = DEVZONE_DATA,
        .slot            = 0,
        .is_genkey       = 1,
        .offset          = 0,
        .count           = 64
    },
    .comp_cert_dev_loc   = {
        .zone            = DEVZONE_DATA,
        .slot            = 10,
        .is_genkey       = 0,
        .offset          = 0,
        .count           = 72
    },
    .std_cert_elements   = {
        {   // STDCERT_PUBLIC_KEY
            .offset      = 246,
            .count       = 64
        },
        {   // STDCERT_SIGNATURE
            .offset      = 359,
            .count       = 74
        },
        {   // STDCERT_ISSUE_DATE
            .offset      = 121,
            .count       = 13
        },
        {   // STDCERT_EXPIRE_DATE
            .offset      = 136,
            .count       = 15
        },
        {   // STDCERT_SIGNER_ID
            .offset      = 113,
            .count       = 4
        },
        {   // STDCERT_CERT_SN
            .offset      = 15,
            .count       = 16
        },
        {   // STDCERT_AUTH_KEY_ID
            .offset      = 327,
            .count       = 20
        },
        {   // STDCERT_SUBJ_KEY_ID
            .offset      = 0,
            .count       = 0
        }
    },
    .cert_elements       = NULL,
    .cert_elements_count = 0,
    .cert_template       = g_cert_template_0_device,
    .cert_template_size  = sizeof(g_cert_template_0_device),
};

/** @} */
