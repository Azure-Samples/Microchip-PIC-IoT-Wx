#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/AzureIotDps.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/AzureIotDps.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=azure-c-sdk/sdk/src/azure/iot/az_iot_common.c azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.c azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.c azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.c azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.c azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.c azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.c azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.c azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.c azure-c-sdk/sdk/src/azure/core/az_json_parser.c azure-c-sdk/sdk/src/azure/core/az_json_pointer.c azure-c-sdk/sdk/src/azure/core/az_json_string.c azure-c-sdk/sdk/src/azure/core/az_log.c azure-c-sdk/sdk/src/azure/core/az_precondition.c azure-c-sdk/sdk/src/azure/core/az_span.c azure-c-sdk/sdk/src/azure/platform/az_noplatform.c azure-c-sdk/sdk/src/azure/core/az_json_token.c platform/cli/cli.c platform/cloud/bsd_adapter/bsdWINC.c platform/cloud/crypto_client/crypto_client.c platform/cloud/crypto_client/cryptoauthlib_main.c platform/cloud/crypto_client/hmac-sha256.c platform/cloud/cloud_service.c platform/cloud/wifi_service.c platform/credentials_storage/credentials_storage.c platform/cryptoauthlib/lib/atcacert/atcacert_client.c platform/cryptoauthlib/lib/atcacert/atcacert_date.c platform/cryptoauthlib/lib/atcacert/atcacert_def.c platform/cryptoauthlib/lib/atcacert/atcacert_der.c platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.c platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.c platform/cryptoauthlib/lib/atcacert/atcacert_pem.c platform/cryptoauthlib/lib/basic/atca_basic.c platform/cryptoauthlib/lib/basic/atca_basic_aes.c platform/cryptoauthlib/lib/basic/atca_basic_checkmac.c platform/cryptoauthlib/lib/basic/atca_basic_counter.c platform/cryptoauthlib/lib/basic/atca_basic_derivekey.c platform/cryptoauthlib/lib/basic/atca_basic_ecdh.c platform/cryptoauthlib/lib/basic/atca_basic_gendig.c platform/cryptoauthlib/lib/basic/atca_basic_genkey.c platform/cryptoauthlib/lib/basic/atca_basic_hmac.c platform/cryptoauthlib/lib/basic/atca_basic_info.c platform/cryptoauthlib/lib/basic/atca_basic_kdf.c platform/cryptoauthlib/lib/basic/atca_basic_lock.c platform/cryptoauthlib/lib/basic/atca_basic_mac.c platform/cryptoauthlib/lib/basic/atca_basic_nonce.c platform/cryptoauthlib/lib/basic/atca_basic_privwrite.c platform/cryptoauthlib/lib/basic/atca_basic_random.c platform/cryptoauthlib/lib/basic/atca_basic_read.c platform/cryptoauthlib/lib/basic/atca_basic_secureboot.c platform/cryptoauthlib/lib/basic/atca_basic_selftest.c platform/cryptoauthlib/lib/basic/atca_basic_sha.c platform/cryptoauthlib/lib/basic/atca_basic_sign.c platform/cryptoauthlib/lib/basic/atca_basic_updateextra.c platform/cryptoauthlib/lib/basic/atca_basic_verify.c platform/cryptoauthlib/lib/basic/atca_basic_write.c platform/cryptoauthlib/lib/basic/atca_helpers.c platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.c platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.c platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.c platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.c platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.c platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.c platform/cryptoauthlib/lib/hal/atca_hal.c platform/cryptoauthlib/lib/hal/hal_i2c.c platform/cryptoauthlib/lib/hal/hal_timer.c platform/cryptoauthlib/lib/host/atca_host.c platform/cryptoauthlib/lib/tls/atcatls.c platform/cryptoauthlib/lib/tls/atcatls_cfg.c platform/cryptoauthlib/lib/atca_cfgs.c platform/cryptoauthlib/lib/atca_command.c platform/cryptoauthlib/lib/atca_device.c platform/cryptoauthlib/lib/atca_execution.c platform/cryptoauthlib/lib/atca_iface.c platform/drivers/i2c_master.c platform/drivers/i2c_simple_master.c platform/drivers/i2c_types.c platform/drivers/spi_master.c platform/drivers/timeout.c platform/drivers/uart.c platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.c platform/mqtt/mqtt_core/mqtt_core.c platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.c platform/mqtt/mqtt_packetTransfer_interface.c platform/winc/bsp/source/nm_bsp_mega.c platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.c platform/winc/common/source/nm_common.c platform/winc/driver/source/m2m_ate_mode.c platform/winc/driver/source/m2m_crypto.c platform/winc/driver/source/m2m_hif.c platform/winc/driver/source/m2m_ota.c platform/winc/driver/source/m2m_periph.c platform/winc/driver/source/m2m_ssl.c platform/winc/driver/source/m2m_wifi.c platform/winc/driver/source/nmasic.c platform/winc/driver/source/nmbus.c platform/winc/driver/source/nmdrv.c platform/winc/driver/source/nmi2c.c platform/winc/driver/source/nmspi.c platform/winc/driver/source/nmuart.c platform/winc/socket/source/socket.c platform/winc/spi_flash/source/spi_flash.c platform/adc1.c platform/application_manager.c platform/clock.c platform/debug_print.c platform/delay.c platform/i2c2_driver.c platform/interrupt_manager.c platform/led.c platform/mcc.c platform/pin_manager.c platform/sensors_handling.c platform/spi1_driver.c platform/system.c platform/tmr1.c platform/tmr2.c platform/traps.c platform/uart1.c main.c mqtt_packetPopulation/mqtt_iothub_packetPopulate.c mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.c mqtt_packetPopulation/mqtt_packetPopulate.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_common.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_parser.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_pointer.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_string.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_log.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_precondition.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_span.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform/az_noplatform.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_token.o ${OBJECTDIR}/platform/cli/cli.o ${OBJECTDIR}/platform/cloud/bsd_adapter/bsdWINC.o ${OBJECTDIR}/platform/cloud/crypto_client/crypto_client.o ${OBJECTDIR}/platform/cloud/crypto_client/cryptoauthlib_main.o ${OBJECTDIR}/platform/cloud/crypto_client/hmac-sha256.o ${OBJECTDIR}/platform/cloud/cloud_service.o ${OBJECTDIR}/platform/cloud/wifi_service.o ${OBJECTDIR}/platform/credentials_storage/credentials_storage.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_client.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_date.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_def.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_der.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_pem.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_aes.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_checkmac.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_counter.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_derivekey.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_ecdh.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_gendig.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_genkey.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_hmac.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_info.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_kdf.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_lock.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_mac.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_nonce.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_privwrite.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_random.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_read.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_secureboot.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_selftest.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sha.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sign.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_updateextra.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_verify.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_write.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_helpers.o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/atca_hal.o ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_i2c.o ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_timer.o ${OBJECTDIR}/platform/cryptoauthlib/lib/host/atca_host.o ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls.o ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls_cfg.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_cfgs.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_command.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_device.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_execution.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_iface.o ${OBJECTDIR}/platform/drivers/i2c_master.o ${OBJECTDIR}/platform/drivers/i2c_simple_master.o ${OBJECTDIR}/platform/drivers/i2c_types.o ${OBJECTDIR}/platform/drivers/spi_master.o ${OBJECTDIR}/platform/drivers/timeout.o ${OBJECTDIR}/platform/drivers/uart.o ${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.o ${OBJECTDIR}/platform/mqtt/mqtt_core/mqtt_core.o ${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.o ${OBJECTDIR}/platform/mqtt/mqtt_packetTransfer_interface.o ${OBJECTDIR}/platform/winc/bsp/source/nm_bsp_mega.o ${OBJECTDIR}/platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.o ${OBJECTDIR}/platform/winc/common/source/nm_common.o ${OBJECTDIR}/platform/winc/driver/source/m2m_ate_mode.o ${OBJECTDIR}/platform/winc/driver/source/m2m_crypto.o ${OBJECTDIR}/platform/winc/driver/source/m2m_hif.o ${OBJECTDIR}/platform/winc/driver/source/m2m_ota.o ${OBJECTDIR}/platform/winc/driver/source/m2m_periph.o ${OBJECTDIR}/platform/winc/driver/source/m2m_ssl.o ${OBJECTDIR}/platform/winc/driver/source/m2m_wifi.o ${OBJECTDIR}/platform/winc/driver/source/nmasic.o ${OBJECTDIR}/platform/winc/driver/source/nmbus.o ${OBJECTDIR}/platform/winc/driver/source/nmdrv.o ${OBJECTDIR}/platform/winc/driver/source/nmi2c.o ${OBJECTDIR}/platform/winc/driver/source/nmspi.o ${OBJECTDIR}/platform/winc/driver/source/nmuart.o ${OBJECTDIR}/platform/winc/socket/source/socket.o ${OBJECTDIR}/platform/winc/spi_flash/source/spi_flash.o ${OBJECTDIR}/platform/adc1.o ${OBJECTDIR}/platform/application_manager.o ${OBJECTDIR}/platform/clock.o ${OBJECTDIR}/platform/debug_print.o ${OBJECTDIR}/platform/delay.o ${OBJECTDIR}/platform/i2c2_driver.o ${OBJECTDIR}/platform/interrupt_manager.o ${OBJECTDIR}/platform/led.o ${OBJECTDIR}/platform/mcc.o ${OBJECTDIR}/platform/pin_manager.o ${OBJECTDIR}/platform/sensors_handling.o ${OBJECTDIR}/platform/spi1_driver.o ${OBJECTDIR}/platform/system.o ${OBJECTDIR}/platform/tmr1.o ${OBJECTDIR}/platform/tmr2.o ${OBJECTDIR}/platform/traps.o ${OBJECTDIR}/platform/uart1.o ${OBJECTDIR}/main.o ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iothub_packetPopulate.o ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.o ${OBJECTDIR}/mqtt_packetPopulation/mqtt_packetPopulate.o
POSSIBLE_DEPFILES=${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_common.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_parser.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_pointer.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_string.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_log.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_precondition.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_span.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform/az_noplatform.o.d ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_token.o.d ${OBJECTDIR}/platform/cli/cli.o.d ${OBJECTDIR}/platform/cloud/bsd_adapter/bsdWINC.o.d ${OBJECTDIR}/platform/cloud/crypto_client/crypto_client.o.d ${OBJECTDIR}/platform/cloud/crypto_client/cryptoauthlib_main.o.d ${OBJECTDIR}/platform/cloud/crypto_client/hmac-sha256.o.d ${OBJECTDIR}/platform/cloud/cloud_service.o.d ${OBJECTDIR}/platform/cloud/wifi_service.o.d ${OBJECTDIR}/platform/credentials_storage/credentials_storage.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_client.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_date.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_def.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_der.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_pem.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_aes.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_checkmac.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_counter.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_derivekey.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_ecdh.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_gendig.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_genkey.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_hmac.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_info.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_kdf.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_lock.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_mac.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_nonce.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_privwrite.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_random.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_read.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_secureboot.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_selftest.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sha.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sign.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_updateextra.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_verify.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_write.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_helpers.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/atca_hal.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_i2c.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_timer.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/host/atca_host.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls_cfg.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_cfgs.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_command.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_device.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_execution.o.d ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_iface.o.d ${OBJECTDIR}/platform/drivers/i2c_master.o.d ${OBJECTDIR}/platform/drivers/i2c_simple_master.o.d ${OBJECTDIR}/platform/drivers/i2c_types.o.d ${OBJECTDIR}/platform/drivers/spi_master.o.d ${OBJECTDIR}/platform/drivers/timeout.o.d ${OBJECTDIR}/platform/drivers/uart.o.d ${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.o.d ${OBJECTDIR}/platform/mqtt/mqtt_core/mqtt_core.o.d ${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.o.d ${OBJECTDIR}/platform/mqtt/mqtt_packetTransfer_interface.o.d ${OBJECTDIR}/platform/winc/bsp/source/nm_bsp_mega.o.d ${OBJECTDIR}/platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.o.d ${OBJECTDIR}/platform/winc/common/source/nm_common.o.d ${OBJECTDIR}/platform/winc/driver/source/m2m_ate_mode.o.d ${OBJECTDIR}/platform/winc/driver/source/m2m_crypto.o.d ${OBJECTDIR}/platform/winc/driver/source/m2m_hif.o.d ${OBJECTDIR}/platform/winc/driver/source/m2m_ota.o.d ${OBJECTDIR}/platform/winc/driver/source/m2m_periph.o.d ${OBJECTDIR}/platform/winc/driver/source/m2m_ssl.o.d ${OBJECTDIR}/platform/winc/driver/source/m2m_wifi.o.d ${OBJECTDIR}/platform/winc/driver/source/nmasic.o.d ${OBJECTDIR}/platform/winc/driver/source/nmbus.o.d ${OBJECTDIR}/platform/winc/driver/source/nmdrv.o.d ${OBJECTDIR}/platform/winc/driver/source/nmi2c.o.d ${OBJECTDIR}/platform/winc/driver/source/nmspi.o.d ${OBJECTDIR}/platform/winc/driver/source/nmuart.o.d ${OBJECTDIR}/platform/winc/socket/source/socket.o.d ${OBJECTDIR}/platform/winc/spi_flash/source/spi_flash.o.d ${OBJECTDIR}/platform/adc1.o.d ${OBJECTDIR}/platform/application_manager.o.d ${OBJECTDIR}/platform/clock.o.d ${OBJECTDIR}/platform/debug_print.o.d ${OBJECTDIR}/platform/delay.o.d ${OBJECTDIR}/platform/i2c2_driver.o.d ${OBJECTDIR}/platform/interrupt_manager.o.d ${OBJECTDIR}/platform/led.o.d ${OBJECTDIR}/platform/mcc.o.d ${OBJECTDIR}/platform/pin_manager.o.d ${OBJECTDIR}/platform/sensors_handling.o.d ${OBJECTDIR}/platform/spi1_driver.o.d ${OBJECTDIR}/platform/system.o.d ${OBJECTDIR}/platform/tmr1.o.d ${OBJECTDIR}/platform/tmr2.o.d ${OBJECTDIR}/platform/traps.o.d ${OBJECTDIR}/platform/uart1.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iothub_packetPopulate.o.d ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.o.d ${OBJECTDIR}/mqtt_packetPopulation/mqtt_packetPopulate.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_common.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_parser.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_pointer.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_string.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_log.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_precondition.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_span.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform/az_noplatform.o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_token.o ${OBJECTDIR}/platform/cli/cli.o ${OBJECTDIR}/platform/cloud/bsd_adapter/bsdWINC.o ${OBJECTDIR}/platform/cloud/crypto_client/crypto_client.o ${OBJECTDIR}/platform/cloud/crypto_client/cryptoauthlib_main.o ${OBJECTDIR}/platform/cloud/crypto_client/hmac-sha256.o ${OBJECTDIR}/platform/cloud/cloud_service.o ${OBJECTDIR}/platform/cloud/wifi_service.o ${OBJECTDIR}/platform/credentials_storage/credentials_storage.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_client.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_date.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_def.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_der.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_pem.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_aes.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_checkmac.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_counter.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_derivekey.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_ecdh.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_gendig.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_genkey.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_hmac.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_info.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_kdf.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_lock.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_mac.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_nonce.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_privwrite.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_random.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_read.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_secureboot.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_selftest.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sha.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sign.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_updateextra.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_verify.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_write.o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_helpers.o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/atca_hal.o ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_i2c.o ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_timer.o ${OBJECTDIR}/platform/cryptoauthlib/lib/host/atca_host.o ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls.o ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls_cfg.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_cfgs.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_command.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_device.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_execution.o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_iface.o ${OBJECTDIR}/platform/drivers/i2c_master.o ${OBJECTDIR}/platform/drivers/i2c_simple_master.o ${OBJECTDIR}/platform/drivers/i2c_types.o ${OBJECTDIR}/platform/drivers/spi_master.o ${OBJECTDIR}/platform/drivers/timeout.o ${OBJECTDIR}/platform/drivers/uart.o ${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.o ${OBJECTDIR}/platform/mqtt/mqtt_core/mqtt_core.o ${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.o ${OBJECTDIR}/platform/mqtt/mqtt_packetTransfer_interface.o ${OBJECTDIR}/platform/winc/bsp/source/nm_bsp_mega.o ${OBJECTDIR}/platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.o ${OBJECTDIR}/platform/winc/common/source/nm_common.o ${OBJECTDIR}/platform/winc/driver/source/m2m_ate_mode.o ${OBJECTDIR}/platform/winc/driver/source/m2m_crypto.o ${OBJECTDIR}/platform/winc/driver/source/m2m_hif.o ${OBJECTDIR}/platform/winc/driver/source/m2m_ota.o ${OBJECTDIR}/platform/winc/driver/source/m2m_periph.o ${OBJECTDIR}/platform/winc/driver/source/m2m_ssl.o ${OBJECTDIR}/platform/winc/driver/source/m2m_wifi.o ${OBJECTDIR}/platform/winc/driver/source/nmasic.o ${OBJECTDIR}/platform/winc/driver/source/nmbus.o ${OBJECTDIR}/platform/winc/driver/source/nmdrv.o ${OBJECTDIR}/platform/winc/driver/source/nmi2c.o ${OBJECTDIR}/platform/winc/driver/source/nmspi.o ${OBJECTDIR}/platform/winc/driver/source/nmuart.o ${OBJECTDIR}/platform/winc/socket/source/socket.o ${OBJECTDIR}/platform/winc/spi_flash/source/spi_flash.o ${OBJECTDIR}/platform/adc1.o ${OBJECTDIR}/platform/application_manager.o ${OBJECTDIR}/platform/clock.o ${OBJECTDIR}/platform/debug_print.o ${OBJECTDIR}/platform/delay.o ${OBJECTDIR}/platform/i2c2_driver.o ${OBJECTDIR}/platform/interrupt_manager.o ${OBJECTDIR}/platform/led.o ${OBJECTDIR}/platform/mcc.o ${OBJECTDIR}/platform/pin_manager.o ${OBJECTDIR}/platform/sensors_handling.o ${OBJECTDIR}/platform/spi1_driver.o ${OBJECTDIR}/platform/system.o ${OBJECTDIR}/platform/tmr1.o ${OBJECTDIR}/platform/tmr2.o ${OBJECTDIR}/platform/traps.o ${OBJECTDIR}/platform/uart1.o ${OBJECTDIR}/main.o ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iothub_packetPopulate.o ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.o ${OBJECTDIR}/mqtt_packetPopulation/mqtt_packetPopulate.o

# Source Files
SOURCEFILES=azure-c-sdk/sdk/src/azure/iot/az_iot_common.c azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.c azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.c azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.c azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.c azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.c azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.c azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.c azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.c azure-c-sdk/sdk/src/azure/core/az_json_parser.c azure-c-sdk/sdk/src/azure/core/az_json_pointer.c azure-c-sdk/sdk/src/azure/core/az_json_string.c azure-c-sdk/sdk/src/azure/core/az_log.c azure-c-sdk/sdk/src/azure/core/az_precondition.c azure-c-sdk/sdk/src/azure/core/az_span.c azure-c-sdk/sdk/src/azure/platform/az_noplatform.c azure-c-sdk/sdk/src/azure/core/az_json_token.c platform/cli/cli.c platform/cloud/bsd_adapter/bsdWINC.c platform/cloud/crypto_client/crypto_client.c platform/cloud/crypto_client/cryptoauthlib_main.c platform/cloud/crypto_client/hmac-sha256.c platform/cloud/cloud_service.c platform/cloud/wifi_service.c platform/credentials_storage/credentials_storage.c platform/cryptoauthlib/lib/atcacert/atcacert_client.c platform/cryptoauthlib/lib/atcacert/atcacert_date.c platform/cryptoauthlib/lib/atcacert/atcacert_def.c platform/cryptoauthlib/lib/atcacert/atcacert_der.c platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.c platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.c platform/cryptoauthlib/lib/atcacert/atcacert_pem.c platform/cryptoauthlib/lib/basic/atca_basic.c platform/cryptoauthlib/lib/basic/atca_basic_aes.c platform/cryptoauthlib/lib/basic/atca_basic_checkmac.c platform/cryptoauthlib/lib/basic/atca_basic_counter.c platform/cryptoauthlib/lib/basic/atca_basic_derivekey.c platform/cryptoauthlib/lib/basic/atca_basic_ecdh.c platform/cryptoauthlib/lib/basic/atca_basic_gendig.c platform/cryptoauthlib/lib/basic/atca_basic_genkey.c platform/cryptoauthlib/lib/basic/atca_basic_hmac.c platform/cryptoauthlib/lib/basic/atca_basic_info.c platform/cryptoauthlib/lib/basic/atca_basic_kdf.c platform/cryptoauthlib/lib/basic/atca_basic_lock.c platform/cryptoauthlib/lib/basic/atca_basic_mac.c platform/cryptoauthlib/lib/basic/atca_basic_nonce.c platform/cryptoauthlib/lib/basic/atca_basic_privwrite.c platform/cryptoauthlib/lib/basic/atca_basic_random.c platform/cryptoauthlib/lib/basic/atca_basic_read.c platform/cryptoauthlib/lib/basic/atca_basic_secureboot.c platform/cryptoauthlib/lib/basic/atca_basic_selftest.c platform/cryptoauthlib/lib/basic/atca_basic_sha.c platform/cryptoauthlib/lib/basic/atca_basic_sign.c platform/cryptoauthlib/lib/basic/atca_basic_updateextra.c platform/cryptoauthlib/lib/basic/atca_basic_verify.c platform/cryptoauthlib/lib/basic/atca_basic_write.c platform/cryptoauthlib/lib/basic/atca_helpers.c platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.c platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.c platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.c platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.c platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.c platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.c platform/cryptoauthlib/lib/hal/atca_hal.c platform/cryptoauthlib/lib/hal/hal_i2c.c platform/cryptoauthlib/lib/hal/hal_timer.c platform/cryptoauthlib/lib/host/atca_host.c platform/cryptoauthlib/lib/tls/atcatls.c platform/cryptoauthlib/lib/tls/atcatls_cfg.c platform/cryptoauthlib/lib/atca_cfgs.c platform/cryptoauthlib/lib/atca_command.c platform/cryptoauthlib/lib/atca_device.c platform/cryptoauthlib/lib/atca_execution.c platform/cryptoauthlib/lib/atca_iface.c platform/drivers/i2c_master.c platform/drivers/i2c_simple_master.c platform/drivers/i2c_types.c platform/drivers/spi_master.c platform/drivers/timeout.c platform/drivers/uart.c platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.c platform/mqtt/mqtt_core/mqtt_core.c platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.c platform/mqtt/mqtt_packetTransfer_interface.c platform/winc/bsp/source/nm_bsp_mega.c platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.c platform/winc/common/source/nm_common.c platform/winc/driver/source/m2m_ate_mode.c platform/winc/driver/source/m2m_crypto.c platform/winc/driver/source/m2m_hif.c platform/winc/driver/source/m2m_ota.c platform/winc/driver/source/m2m_periph.c platform/winc/driver/source/m2m_ssl.c platform/winc/driver/source/m2m_wifi.c platform/winc/driver/source/nmasic.c platform/winc/driver/source/nmbus.c platform/winc/driver/source/nmdrv.c platform/winc/driver/source/nmi2c.c platform/winc/driver/source/nmspi.c platform/winc/driver/source/nmuart.c platform/winc/socket/source/socket.c platform/winc/spi_flash/source/spi_flash.c platform/adc1.c platform/application_manager.c platform/clock.c platform/debug_print.c platform/delay.c platform/i2c2_driver.c platform/interrupt_manager.c platform/led.c platform/mcc.c platform/pin_manager.c platform/sensors_handling.c platform/spi1_driver.c platform/system.c platform/tmr1.c platform/tmr2.c platform/traps.c platform/uart1.c main.c mqtt_packetPopulation/mqtt_iothub_packetPopulate.c mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.c mqtt_packetPopulation/mqtt_packetPopulate.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/AzureIotDps.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ128GA705
MP_LINKER_FILE_OPTION=,--script=p24FJ128GA705.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_common.o: azure-c-sdk/sdk/src/azure/iot/az_iot_common.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_common.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_common.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_common.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_common.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_common.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_common.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.o: azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.o: azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.o: azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.o: azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.o: azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.o: azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.o: azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.o: azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_parser.o: azure-c-sdk/sdk/src/azure/core/az_json_parser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_parser.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_parser.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/core/az_json_parser.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_parser.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_parser.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_parser.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_pointer.o: azure-c-sdk/sdk/src/azure/core/az_json_pointer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_pointer.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_pointer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/core/az_json_pointer.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_pointer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_pointer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_pointer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_string.o: azure-c-sdk/sdk/src/azure/core/az_json_string.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_string.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_string.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/core/az_json_string.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_string.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_string.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_string.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_log.o: azure-c-sdk/sdk/src/azure/core/az_log.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_log.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_log.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/core/az_log.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_log.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_log.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_log.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_precondition.o: azure-c-sdk/sdk/src/azure/core/az_precondition.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_precondition.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_precondition.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/core/az_precondition.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_precondition.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_precondition.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_precondition.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_span.o: azure-c-sdk/sdk/src/azure/core/az_span.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_span.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_span.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/core/az_span.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_span.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_span.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_span.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform/az_noplatform.o: azure-c-sdk/sdk/src/azure/platform/az_noplatform.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform/az_noplatform.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform/az_noplatform.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/platform/az_noplatform.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform/az_noplatform.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform/az_noplatform.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform/az_noplatform.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_token.o: azure-c-sdk/sdk/src/azure/core/az_json_token.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_token.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_token.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/core/az_json_token.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_token.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_token.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_token.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cli/cli.o: platform/cli/cli.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cli" 
	@${RM} ${OBJECTDIR}/platform/cli/cli.o.d 
	@${RM} ${OBJECTDIR}/platform/cli/cli.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cli/cli.c  -o ${OBJECTDIR}/platform/cli/cli.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cli/cli.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cli/cli.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cloud/bsd_adapter/bsdWINC.o: platform/cloud/bsd_adapter/bsdWINC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cloud/bsd_adapter" 
	@${RM} ${OBJECTDIR}/platform/cloud/bsd_adapter/bsdWINC.o.d 
	@${RM} ${OBJECTDIR}/platform/cloud/bsd_adapter/bsdWINC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cloud/bsd_adapter/bsdWINC.c  -o ${OBJECTDIR}/platform/cloud/bsd_adapter/bsdWINC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cloud/bsd_adapter/bsdWINC.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cloud/bsd_adapter/bsdWINC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cloud/crypto_client/crypto_client.o: platform/cloud/crypto_client/crypto_client.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cloud/crypto_client" 
	@${RM} ${OBJECTDIR}/platform/cloud/crypto_client/crypto_client.o.d 
	@${RM} ${OBJECTDIR}/platform/cloud/crypto_client/crypto_client.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cloud/crypto_client/crypto_client.c  -o ${OBJECTDIR}/platform/cloud/crypto_client/crypto_client.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cloud/crypto_client/crypto_client.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cloud/crypto_client/crypto_client.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cloud/crypto_client/cryptoauthlib_main.o: platform/cloud/crypto_client/cryptoauthlib_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cloud/crypto_client" 
	@${RM} ${OBJECTDIR}/platform/cloud/crypto_client/cryptoauthlib_main.o.d 
	@${RM} ${OBJECTDIR}/platform/cloud/crypto_client/cryptoauthlib_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cloud/crypto_client/cryptoauthlib_main.c  -o ${OBJECTDIR}/platform/cloud/crypto_client/cryptoauthlib_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cloud/crypto_client/cryptoauthlib_main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cloud/crypto_client/cryptoauthlib_main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cloud/crypto_client/hmac-sha256.o: platform/cloud/crypto_client/hmac-sha256.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cloud/crypto_client" 
	@${RM} ${OBJECTDIR}/platform/cloud/crypto_client/hmac-sha256.o.d 
	@${RM} ${OBJECTDIR}/platform/cloud/crypto_client/hmac-sha256.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cloud/crypto_client/hmac-sha256.c  -o ${OBJECTDIR}/platform/cloud/crypto_client/hmac-sha256.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cloud/crypto_client/hmac-sha256.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cloud/crypto_client/hmac-sha256.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cloud/cloud_service.o: platform/cloud/cloud_service.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cloud" 
	@${RM} ${OBJECTDIR}/platform/cloud/cloud_service.o.d 
	@${RM} ${OBJECTDIR}/platform/cloud/cloud_service.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cloud/cloud_service.c  -o ${OBJECTDIR}/platform/cloud/cloud_service.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cloud/cloud_service.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cloud/cloud_service.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cloud/wifi_service.o: platform/cloud/wifi_service.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cloud" 
	@${RM} ${OBJECTDIR}/platform/cloud/wifi_service.o.d 
	@${RM} ${OBJECTDIR}/platform/cloud/wifi_service.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cloud/wifi_service.c  -o ${OBJECTDIR}/platform/cloud/wifi_service.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cloud/wifi_service.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cloud/wifi_service.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/credentials_storage/credentials_storage.o: platform/credentials_storage/credentials_storage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/credentials_storage" 
	@${RM} ${OBJECTDIR}/platform/credentials_storage/credentials_storage.o.d 
	@${RM} ${OBJECTDIR}/platform/credentials_storage/credentials_storage.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/credentials_storage/credentials_storage.c  -o ${OBJECTDIR}/platform/credentials_storage/credentials_storage.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/credentials_storage/credentials_storage.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/credentials_storage/credentials_storage.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_client.o: platform/cryptoauthlib/lib/atcacert/atcacert_client.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_client.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_client.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atcacert/atcacert_client.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_client.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_client.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_client.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_date.o: platform/cryptoauthlib/lib/atcacert/atcacert_date.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_date.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_date.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atcacert/atcacert_date.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_date.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_date.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_date.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_def.o: platform/cryptoauthlib/lib/atcacert/atcacert_def.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_def.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_def.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atcacert/atcacert_def.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_def.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_def.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_def.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_der.o: platform/cryptoauthlib/lib/atcacert/atcacert_der.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_der.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_der.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atcacert/atcacert_der.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_der.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_der.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_der.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.o: platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.o: platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_pem.o: platform/cryptoauthlib/lib/atcacert/atcacert_pem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_pem.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_pem.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atcacert/atcacert_pem.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_pem.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_pem.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_pem.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic.o: platform/cryptoauthlib/lib/basic/atca_basic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_aes.o: platform/cryptoauthlib/lib/basic/atca_basic_aes.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_aes.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_aes.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_aes.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_aes.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_aes.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_aes.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_checkmac.o: platform/cryptoauthlib/lib/basic/atca_basic_checkmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_checkmac.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_checkmac.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_checkmac.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_checkmac.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_checkmac.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_checkmac.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_counter.o: platform/cryptoauthlib/lib/basic/atca_basic_counter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_counter.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_counter.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_counter.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_counter.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_counter.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_counter.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_derivekey.o: platform/cryptoauthlib/lib/basic/atca_basic_derivekey.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_derivekey.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_derivekey.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_derivekey.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_derivekey.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_derivekey.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_derivekey.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_ecdh.o: platform/cryptoauthlib/lib/basic/atca_basic_ecdh.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_ecdh.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_ecdh.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_ecdh.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_ecdh.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_ecdh.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_ecdh.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_gendig.o: platform/cryptoauthlib/lib/basic/atca_basic_gendig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_gendig.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_gendig.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_gendig.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_gendig.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_gendig.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_gendig.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_genkey.o: platform/cryptoauthlib/lib/basic/atca_basic_genkey.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_genkey.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_genkey.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_genkey.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_genkey.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_genkey.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_genkey.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_hmac.o: platform/cryptoauthlib/lib/basic/atca_basic_hmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_hmac.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_hmac.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_hmac.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_hmac.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_hmac.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_hmac.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_info.o: platform/cryptoauthlib/lib/basic/atca_basic_info.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_info.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_info.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_info.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_info.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_info.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_info.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_kdf.o: platform/cryptoauthlib/lib/basic/atca_basic_kdf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_kdf.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_kdf.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_kdf.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_kdf.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_kdf.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_kdf.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_lock.o: platform/cryptoauthlib/lib/basic/atca_basic_lock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_lock.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_lock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_lock.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_lock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_lock.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_lock.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_mac.o: platform/cryptoauthlib/lib/basic/atca_basic_mac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_mac.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_mac.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_mac.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_mac.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_mac.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_mac.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_nonce.o: platform/cryptoauthlib/lib/basic/atca_basic_nonce.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_nonce.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_nonce.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_nonce.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_nonce.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_nonce.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_nonce.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_privwrite.o: platform/cryptoauthlib/lib/basic/atca_basic_privwrite.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_privwrite.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_privwrite.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_privwrite.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_privwrite.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_privwrite.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_privwrite.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_random.o: platform/cryptoauthlib/lib/basic/atca_basic_random.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_random.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_random.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_random.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_random.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_random.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_random.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_read.o: platform/cryptoauthlib/lib/basic/atca_basic_read.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_read.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_read.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_read.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_read.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_read.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_read.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_secureboot.o: platform/cryptoauthlib/lib/basic/atca_basic_secureboot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_secureboot.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_secureboot.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_secureboot.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_secureboot.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_secureboot.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_secureboot.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_selftest.o: platform/cryptoauthlib/lib/basic/atca_basic_selftest.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_selftest.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_selftest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_selftest.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_selftest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_selftest.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_selftest.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sha.o: platform/cryptoauthlib/lib/basic/atca_basic_sha.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sha.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sha.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_sha.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sha.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sha.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sha.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sign.o: platform/cryptoauthlib/lib/basic/atca_basic_sign.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sign.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sign.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_sign.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sign.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sign.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sign.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_updateextra.o: platform/cryptoauthlib/lib/basic/atca_basic_updateextra.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_updateextra.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_updateextra.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_updateextra.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_updateextra.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_updateextra.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_updateextra.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_verify.o: platform/cryptoauthlib/lib/basic/atca_basic_verify.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_verify.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_verify.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_verify.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_verify.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_verify.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_verify.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_write.o: platform/cryptoauthlib/lib/basic/atca_basic_write.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_write.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_write.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_write.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_write.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_write.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_write.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_helpers.o: platform/cryptoauthlib/lib/basic/atca_helpers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_helpers.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_helpers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_helpers.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_helpers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_helpers.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_helpers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.o: platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.o: platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o: platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o: platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o: platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o: platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/hal/atca_hal.o: platform/cryptoauthlib/lib/hal/atca_hal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/hal" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/atca_hal.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/atca_hal.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/hal/atca_hal.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/atca_hal.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/hal/atca_hal.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/hal/atca_hal.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_i2c.o: platform/cryptoauthlib/lib/hal/hal_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/hal" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_i2c.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/hal/hal_i2c.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_i2c.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_timer.o: platform/cryptoauthlib/lib/hal/hal_timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/hal" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_timer.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/hal/hal_timer.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_timer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/host/atca_host.o: platform/cryptoauthlib/lib/host/atca_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/host" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/host/atca_host.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/host/atca_host.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/host/atca_host.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/host/atca_host.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/host/atca_host.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/host/atca_host.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls.o: platform/cryptoauthlib/lib/tls/atcatls.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/tls" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/tls/atcatls.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls_cfg.o: platform/cryptoauthlib/lib/tls/atcatls_cfg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/tls" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls_cfg.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls_cfg.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/tls/atcatls_cfg.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls_cfg.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls_cfg.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls_cfg.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atca_cfgs.o: platform/cryptoauthlib/lib/atca_cfgs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_cfgs.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_cfgs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atca_cfgs.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_cfgs.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_cfgs.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_cfgs.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atca_command.o: platform/cryptoauthlib/lib/atca_command.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_command.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_command.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atca_command.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_command.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_command.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_command.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atca_device.o: platform/cryptoauthlib/lib/atca_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_device.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atca_device.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_device.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_device.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atca_execution.o: platform/cryptoauthlib/lib/atca_execution.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_execution.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_execution.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atca_execution.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_execution.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_execution.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_execution.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atca_iface.o: platform/cryptoauthlib/lib/atca_iface.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_iface.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_iface.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atca_iface.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_iface.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_iface.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_iface.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/drivers/i2c_master.o: platform/drivers/i2c_master.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/drivers" 
	@${RM} ${OBJECTDIR}/platform/drivers/i2c_master.o.d 
	@${RM} ${OBJECTDIR}/platform/drivers/i2c_master.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/drivers/i2c_master.c  -o ${OBJECTDIR}/platform/drivers/i2c_master.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/drivers/i2c_master.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/drivers/i2c_master.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/drivers/i2c_simple_master.o: platform/drivers/i2c_simple_master.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/drivers" 
	@${RM} ${OBJECTDIR}/platform/drivers/i2c_simple_master.o.d 
	@${RM} ${OBJECTDIR}/platform/drivers/i2c_simple_master.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/drivers/i2c_simple_master.c  -o ${OBJECTDIR}/platform/drivers/i2c_simple_master.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/drivers/i2c_simple_master.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/drivers/i2c_simple_master.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/drivers/i2c_types.o: platform/drivers/i2c_types.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/drivers" 
	@${RM} ${OBJECTDIR}/platform/drivers/i2c_types.o.d 
	@${RM} ${OBJECTDIR}/platform/drivers/i2c_types.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/drivers/i2c_types.c  -o ${OBJECTDIR}/platform/drivers/i2c_types.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/drivers/i2c_types.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/drivers/i2c_types.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/drivers/spi_master.o: platform/drivers/spi_master.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/drivers" 
	@${RM} ${OBJECTDIR}/platform/drivers/spi_master.o.d 
	@${RM} ${OBJECTDIR}/platform/drivers/spi_master.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/drivers/spi_master.c  -o ${OBJECTDIR}/platform/drivers/spi_master.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/drivers/spi_master.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/drivers/spi_master.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/drivers/timeout.o: platform/drivers/timeout.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/drivers" 
	@${RM} ${OBJECTDIR}/platform/drivers/timeout.o.d 
	@${RM} ${OBJECTDIR}/platform/drivers/timeout.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/drivers/timeout.c  -o ${OBJECTDIR}/platform/drivers/timeout.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/drivers/timeout.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/drivers/timeout.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/drivers/uart.o: platform/drivers/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/drivers" 
	@${RM} ${OBJECTDIR}/platform/drivers/uart.o.d 
	@${RM} ${OBJECTDIR}/platform/drivers/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/drivers/uart.c  -o ${OBJECTDIR}/platform/drivers/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/drivers/uart.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/drivers/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.o: platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd" 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.o.d 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.c  -o ${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/mqtt/mqtt_core/mqtt_core.o: platform/mqtt/mqtt_core/mqtt_core.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/mqtt/mqtt_core" 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_core/mqtt_core.o.d 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_core/mqtt_core.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/mqtt/mqtt_core/mqtt_core.c  -o ${OBJECTDIR}/platform/mqtt/mqtt_core/mqtt_core.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/mqtt/mqtt_core/mqtt_core.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/mqtt/mqtt_core/mqtt_core.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.o: platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer" 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.o.d 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.c  -o ${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/mqtt/mqtt_packetTransfer_interface.o: platform/mqtt/mqtt_packetTransfer_interface.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/mqtt" 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_packetTransfer_interface.o.d 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_packetTransfer_interface.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/mqtt/mqtt_packetTransfer_interface.c  -o ${OBJECTDIR}/platform/mqtt/mqtt_packetTransfer_interface.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/mqtt/mqtt_packetTransfer_interface.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/mqtt/mqtt_packetTransfer_interface.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/bsp/source/nm_bsp_mega.o: platform/winc/bsp/source/nm_bsp_mega.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/bsp/source" 
	@${RM} ${OBJECTDIR}/platform/winc/bsp/source/nm_bsp_mega.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/bsp/source/nm_bsp_mega.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/bsp/source/nm_bsp_mega.c  -o ${OBJECTDIR}/platform/winc/bsp/source/nm_bsp_mega.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/bsp/source/nm_bsp_mega.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/bsp/source/nm_bsp_mega.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.o: platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/bus_wrapper/source" 
	@${RM} ${OBJECTDIR}/platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.c  -o ${OBJECTDIR}/platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/common/source/nm_common.o: platform/winc/common/source/nm_common.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/common/source" 
	@${RM} ${OBJECTDIR}/platform/winc/common/source/nm_common.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/common/source/nm_common.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/common/source/nm_common.c  -o ${OBJECTDIR}/platform/winc/common/source/nm_common.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/common/source/nm_common.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/common/source/nm_common.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/m2m_ate_mode.o: platform/winc/driver/source/m2m_ate_mode.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_ate_mode.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_ate_mode.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/m2m_ate_mode.c  -o ${OBJECTDIR}/platform/winc/driver/source/m2m_ate_mode.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/m2m_ate_mode.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/m2m_ate_mode.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/m2m_crypto.o: platform/winc/driver/source/m2m_crypto.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_crypto.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_crypto.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/m2m_crypto.c  -o ${OBJECTDIR}/platform/winc/driver/source/m2m_crypto.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/m2m_crypto.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/m2m_crypto.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/m2m_hif.o: platform/winc/driver/source/m2m_hif.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_hif.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_hif.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/m2m_hif.c  -o ${OBJECTDIR}/platform/winc/driver/source/m2m_hif.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/m2m_hif.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/m2m_hif.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/m2m_ota.o: platform/winc/driver/source/m2m_ota.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_ota.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_ota.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/m2m_ota.c  -o ${OBJECTDIR}/platform/winc/driver/source/m2m_ota.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/m2m_ota.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/m2m_ota.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/m2m_periph.o: platform/winc/driver/source/m2m_periph.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_periph.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_periph.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/m2m_periph.c  -o ${OBJECTDIR}/platform/winc/driver/source/m2m_periph.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/m2m_periph.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/m2m_periph.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/m2m_ssl.o: platform/winc/driver/source/m2m_ssl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_ssl.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_ssl.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/m2m_ssl.c  -o ${OBJECTDIR}/platform/winc/driver/source/m2m_ssl.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/m2m_ssl.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/m2m_ssl.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/m2m_wifi.o: platform/winc/driver/source/m2m_wifi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_wifi.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_wifi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/m2m_wifi.c  -o ${OBJECTDIR}/platform/winc/driver/source/m2m_wifi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/m2m_wifi.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/m2m_wifi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/nmasic.o: platform/winc/driver/source/nmasic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmasic.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmasic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/nmasic.c  -o ${OBJECTDIR}/platform/winc/driver/source/nmasic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/nmasic.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/nmasic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/nmbus.o: platform/winc/driver/source/nmbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmbus.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmbus.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/nmbus.c  -o ${OBJECTDIR}/platform/winc/driver/source/nmbus.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/nmbus.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/nmbus.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/nmdrv.o: platform/winc/driver/source/nmdrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmdrv.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmdrv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/nmdrv.c  -o ${OBJECTDIR}/platform/winc/driver/source/nmdrv.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/nmdrv.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/nmdrv.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/nmi2c.o: platform/winc/driver/source/nmi2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmi2c.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmi2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/nmi2c.c  -o ${OBJECTDIR}/platform/winc/driver/source/nmi2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/nmi2c.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/nmi2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/nmspi.o: platform/winc/driver/source/nmspi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmspi.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmspi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/nmspi.c  -o ${OBJECTDIR}/platform/winc/driver/source/nmspi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/nmspi.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/nmspi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/nmuart.o: platform/winc/driver/source/nmuart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmuart.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmuart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/nmuart.c  -o ${OBJECTDIR}/platform/winc/driver/source/nmuart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/nmuart.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/nmuart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/socket/source/socket.o: platform/winc/socket/source/socket.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/socket/source" 
	@${RM} ${OBJECTDIR}/platform/winc/socket/source/socket.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/socket/source/socket.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/socket/source/socket.c  -o ${OBJECTDIR}/platform/winc/socket/source/socket.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/socket/source/socket.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/socket/source/socket.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/spi_flash/source/spi_flash.o: platform/winc/spi_flash/source/spi_flash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/spi_flash/source" 
	@${RM} ${OBJECTDIR}/platform/winc/spi_flash/source/spi_flash.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/spi_flash/source/spi_flash.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/spi_flash/source/spi_flash.c  -o ${OBJECTDIR}/platform/winc/spi_flash/source/spi_flash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/spi_flash/source/spi_flash.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/spi_flash/source/spi_flash.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/adc1.o: platform/adc1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/adc1.o.d 
	@${RM} ${OBJECTDIR}/platform/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/adc1.c  -o ${OBJECTDIR}/platform/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/adc1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/adc1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/application_manager.o: platform/application_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/application_manager.o.d 
	@${RM} ${OBJECTDIR}/platform/application_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/application_manager.c  -o ${OBJECTDIR}/platform/application_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/application_manager.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/application_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/clock.o: platform/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/clock.o.d 
	@${RM} ${OBJECTDIR}/platform/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/clock.c  -o ${OBJECTDIR}/platform/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/clock.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/clock.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/debug_print.o: platform/debug_print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/debug_print.o.d 
	@${RM} ${OBJECTDIR}/platform/debug_print.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/debug_print.c  -o ${OBJECTDIR}/platform/debug_print.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/debug_print.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/debug_print.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/delay.o: platform/delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/delay.o.d 
	@${RM} ${OBJECTDIR}/platform/delay.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/delay.c  -o ${OBJECTDIR}/platform/delay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/delay.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/delay.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/i2c2_driver.o: platform/i2c2_driver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/i2c2_driver.o.d 
	@${RM} ${OBJECTDIR}/platform/i2c2_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/i2c2_driver.c  -o ${OBJECTDIR}/platform/i2c2_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/i2c2_driver.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/i2c2_driver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/interrupt_manager.o: platform/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/platform/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/interrupt_manager.c  -o ${OBJECTDIR}/platform/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/interrupt_manager.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/interrupt_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/led.o: platform/led.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/led.o.d 
	@${RM} ${OBJECTDIR}/platform/led.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/led.c  -o ${OBJECTDIR}/platform/led.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/led.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/led.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/mcc.o: platform/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/mcc.o.d 
	@${RM} ${OBJECTDIR}/platform/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/mcc.c  -o ${OBJECTDIR}/platform/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/mcc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/mcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/pin_manager.o: platform/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/platform/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/pin_manager.c  -o ${OBJECTDIR}/platform/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/pin_manager.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/pin_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/sensors_handling.o: platform/sensors_handling.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/sensors_handling.o.d 
	@${RM} ${OBJECTDIR}/platform/sensors_handling.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/sensors_handling.c  -o ${OBJECTDIR}/platform/sensors_handling.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/sensors_handling.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/sensors_handling.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/spi1_driver.o: platform/spi1_driver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/spi1_driver.o.d 
	@${RM} ${OBJECTDIR}/platform/spi1_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/spi1_driver.c  -o ${OBJECTDIR}/platform/spi1_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/spi1_driver.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/spi1_driver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/system.o: platform/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/system.o.d 
	@${RM} ${OBJECTDIR}/platform/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/system.c  -o ${OBJECTDIR}/platform/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/system.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/tmr1.o: platform/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/tmr1.o.d 
	@${RM} ${OBJECTDIR}/platform/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/tmr1.c  -o ${OBJECTDIR}/platform/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/tmr1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/tmr1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/tmr2.o: platform/tmr2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/tmr2.o.d 
	@${RM} ${OBJECTDIR}/platform/tmr2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/tmr2.c  -o ${OBJECTDIR}/platform/tmr2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/tmr2.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/tmr2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/traps.o: platform/traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/traps.o.d 
	@${RM} ${OBJECTDIR}/platform/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/traps.c  -o ${OBJECTDIR}/platform/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/traps.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/traps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/uart1.o: platform/uart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/uart1.o.d 
	@${RM} ${OBJECTDIR}/platform/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/uart1.c  -o ${OBJECTDIR}/platform/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/uart1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/uart1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mqtt_packetPopulation/mqtt_iothub_packetPopulate.o: mqtt_packetPopulation/mqtt_iothub_packetPopulate.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mqtt_packetPopulation" 
	@${RM} ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iothub_packetPopulate.o.d 
	@${RM} ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iothub_packetPopulate.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mqtt_packetPopulation/mqtt_iothub_packetPopulate.c  -o ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iothub_packetPopulate.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mqtt_packetPopulation/mqtt_iothub_packetPopulate.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mqtt_packetPopulation/mqtt_iothub_packetPopulate.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.o: mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mqtt_packetPopulation" 
	@${RM} ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.o.d 
	@${RM} ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.c  -o ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mqtt_packetPopulation/mqtt_packetPopulate.o: mqtt_packetPopulation/mqtt_packetPopulate.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mqtt_packetPopulation" 
	@${RM} ${OBJECTDIR}/mqtt_packetPopulation/mqtt_packetPopulate.o.d 
	@${RM} ${OBJECTDIR}/mqtt_packetPopulation/mqtt_packetPopulate.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mqtt_packetPopulation/mqtt_packetPopulate.c  -o ${OBJECTDIR}/mqtt_packetPopulation/mqtt_packetPopulate.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mqtt_packetPopulation/mqtt_packetPopulate.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mqtt_packetPopulation/mqtt_packetPopulate.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_common.o: azure-c-sdk/sdk/src/azure/iot/az_iot_common.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_common.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_common.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_common.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_common.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_common.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_common.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.o: azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.o: azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_c2d.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.o: azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_methods.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.o: azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_sas.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.o: azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_telemetry.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.o: azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_hub_client_twin.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.o: azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.o: azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/iot/az_iot_provisioning_client_sas.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_parser.o: azure-c-sdk/sdk/src/azure/core/az_json_parser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_parser.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_parser.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/core/az_json_parser.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_parser.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_parser.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_parser.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_pointer.o: azure-c-sdk/sdk/src/azure/core/az_json_pointer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_pointer.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_pointer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/core/az_json_pointer.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_pointer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_pointer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_pointer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_string.o: azure-c-sdk/sdk/src/azure/core/az_json_string.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_string.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_string.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/core/az_json_string.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_string.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_string.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_string.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_log.o: azure-c-sdk/sdk/src/azure/core/az_log.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_log.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_log.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/core/az_log.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_log.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_log.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_log.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_precondition.o: azure-c-sdk/sdk/src/azure/core/az_precondition.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_precondition.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_precondition.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/core/az_precondition.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_precondition.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_precondition.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_precondition.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_span.o: azure-c-sdk/sdk/src/azure/core/az_span.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_span.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_span.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/core/az_span.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_span.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_span.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_span.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform/az_noplatform.o: azure-c-sdk/sdk/src/azure/platform/az_noplatform.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform/az_noplatform.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform/az_noplatform.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/platform/az_noplatform.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform/az_noplatform.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform/az_noplatform.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/platform/az_noplatform.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_token.o: azure-c-sdk/sdk/src/azure/core/az_json_token.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core" 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_token.o.d 
	@${RM} ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_token.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  azure-c-sdk/sdk/src/azure/core/az_json_token.c  -o ${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_token.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_token.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/azure-c-sdk/sdk/src/azure/core/az_json_token.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cli/cli.o: platform/cli/cli.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cli" 
	@${RM} ${OBJECTDIR}/platform/cli/cli.o.d 
	@${RM} ${OBJECTDIR}/platform/cli/cli.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cli/cli.c  -o ${OBJECTDIR}/platform/cli/cli.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cli/cli.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cli/cli.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cloud/bsd_adapter/bsdWINC.o: platform/cloud/bsd_adapter/bsdWINC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cloud/bsd_adapter" 
	@${RM} ${OBJECTDIR}/platform/cloud/bsd_adapter/bsdWINC.o.d 
	@${RM} ${OBJECTDIR}/platform/cloud/bsd_adapter/bsdWINC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cloud/bsd_adapter/bsdWINC.c  -o ${OBJECTDIR}/platform/cloud/bsd_adapter/bsdWINC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cloud/bsd_adapter/bsdWINC.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cloud/bsd_adapter/bsdWINC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cloud/crypto_client/crypto_client.o: platform/cloud/crypto_client/crypto_client.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cloud/crypto_client" 
	@${RM} ${OBJECTDIR}/platform/cloud/crypto_client/crypto_client.o.d 
	@${RM} ${OBJECTDIR}/platform/cloud/crypto_client/crypto_client.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cloud/crypto_client/crypto_client.c  -o ${OBJECTDIR}/platform/cloud/crypto_client/crypto_client.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cloud/crypto_client/crypto_client.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cloud/crypto_client/crypto_client.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cloud/crypto_client/cryptoauthlib_main.o: platform/cloud/crypto_client/cryptoauthlib_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cloud/crypto_client" 
	@${RM} ${OBJECTDIR}/platform/cloud/crypto_client/cryptoauthlib_main.o.d 
	@${RM} ${OBJECTDIR}/platform/cloud/crypto_client/cryptoauthlib_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cloud/crypto_client/cryptoauthlib_main.c  -o ${OBJECTDIR}/platform/cloud/crypto_client/cryptoauthlib_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cloud/crypto_client/cryptoauthlib_main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cloud/crypto_client/cryptoauthlib_main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cloud/crypto_client/hmac-sha256.o: platform/cloud/crypto_client/hmac-sha256.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cloud/crypto_client" 
	@${RM} ${OBJECTDIR}/platform/cloud/crypto_client/hmac-sha256.o.d 
	@${RM} ${OBJECTDIR}/platform/cloud/crypto_client/hmac-sha256.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cloud/crypto_client/hmac-sha256.c  -o ${OBJECTDIR}/platform/cloud/crypto_client/hmac-sha256.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cloud/crypto_client/hmac-sha256.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cloud/crypto_client/hmac-sha256.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cloud/cloud_service.o: platform/cloud/cloud_service.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cloud" 
	@${RM} ${OBJECTDIR}/platform/cloud/cloud_service.o.d 
	@${RM} ${OBJECTDIR}/platform/cloud/cloud_service.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cloud/cloud_service.c  -o ${OBJECTDIR}/platform/cloud/cloud_service.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cloud/cloud_service.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cloud/cloud_service.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cloud/wifi_service.o: platform/cloud/wifi_service.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cloud" 
	@${RM} ${OBJECTDIR}/platform/cloud/wifi_service.o.d 
	@${RM} ${OBJECTDIR}/platform/cloud/wifi_service.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cloud/wifi_service.c  -o ${OBJECTDIR}/platform/cloud/wifi_service.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cloud/wifi_service.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cloud/wifi_service.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/credentials_storage/credentials_storage.o: platform/credentials_storage/credentials_storage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/credentials_storage" 
	@${RM} ${OBJECTDIR}/platform/credentials_storage/credentials_storage.o.d 
	@${RM} ${OBJECTDIR}/platform/credentials_storage/credentials_storage.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/credentials_storage/credentials_storage.c  -o ${OBJECTDIR}/platform/credentials_storage/credentials_storage.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/credentials_storage/credentials_storage.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/credentials_storage/credentials_storage.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_client.o: platform/cryptoauthlib/lib/atcacert/atcacert_client.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_client.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_client.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atcacert/atcacert_client.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_client.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_client.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_client.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_date.o: platform/cryptoauthlib/lib/atcacert/atcacert_date.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_date.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_date.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atcacert/atcacert_date.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_date.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_date.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_date.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_def.o: platform/cryptoauthlib/lib/atcacert/atcacert_def.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_def.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_def.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atcacert/atcacert_def.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_def.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_def.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_def.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_der.o: platform/cryptoauthlib/lib/atcacert/atcacert_der.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_der.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_der.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atcacert/atcacert_der.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_der.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_der.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_der.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.o: platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_hw.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.o: platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_host_sw.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_pem.o: platform/cryptoauthlib/lib/atcacert/atcacert_pem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_pem.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_pem.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atcacert/atcacert_pem.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_pem.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_pem.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atcacert/atcacert_pem.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic.o: platform/cryptoauthlib/lib/basic/atca_basic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_aes.o: platform/cryptoauthlib/lib/basic/atca_basic_aes.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_aes.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_aes.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_aes.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_aes.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_aes.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_aes.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_checkmac.o: platform/cryptoauthlib/lib/basic/atca_basic_checkmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_checkmac.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_checkmac.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_checkmac.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_checkmac.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_checkmac.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_checkmac.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_counter.o: platform/cryptoauthlib/lib/basic/atca_basic_counter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_counter.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_counter.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_counter.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_counter.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_counter.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_counter.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_derivekey.o: platform/cryptoauthlib/lib/basic/atca_basic_derivekey.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_derivekey.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_derivekey.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_derivekey.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_derivekey.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_derivekey.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_derivekey.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_ecdh.o: platform/cryptoauthlib/lib/basic/atca_basic_ecdh.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_ecdh.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_ecdh.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_ecdh.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_ecdh.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_ecdh.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_ecdh.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_gendig.o: platform/cryptoauthlib/lib/basic/atca_basic_gendig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_gendig.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_gendig.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_gendig.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_gendig.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_gendig.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_gendig.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_genkey.o: platform/cryptoauthlib/lib/basic/atca_basic_genkey.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_genkey.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_genkey.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_genkey.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_genkey.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_genkey.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_genkey.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_hmac.o: platform/cryptoauthlib/lib/basic/atca_basic_hmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_hmac.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_hmac.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_hmac.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_hmac.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_hmac.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_hmac.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_info.o: platform/cryptoauthlib/lib/basic/atca_basic_info.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_info.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_info.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_info.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_info.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_info.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_info.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_kdf.o: platform/cryptoauthlib/lib/basic/atca_basic_kdf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_kdf.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_kdf.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_kdf.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_kdf.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_kdf.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_kdf.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_lock.o: platform/cryptoauthlib/lib/basic/atca_basic_lock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_lock.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_lock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_lock.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_lock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_lock.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_lock.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_mac.o: platform/cryptoauthlib/lib/basic/atca_basic_mac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_mac.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_mac.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_mac.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_mac.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_mac.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_mac.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_nonce.o: platform/cryptoauthlib/lib/basic/atca_basic_nonce.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_nonce.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_nonce.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_nonce.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_nonce.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_nonce.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_nonce.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_privwrite.o: platform/cryptoauthlib/lib/basic/atca_basic_privwrite.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_privwrite.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_privwrite.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_privwrite.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_privwrite.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_privwrite.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_privwrite.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_random.o: platform/cryptoauthlib/lib/basic/atca_basic_random.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_random.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_random.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_random.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_random.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_random.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_random.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_read.o: platform/cryptoauthlib/lib/basic/atca_basic_read.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_read.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_read.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_read.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_read.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_read.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_read.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_secureboot.o: platform/cryptoauthlib/lib/basic/atca_basic_secureboot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_secureboot.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_secureboot.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_secureboot.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_secureboot.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_secureboot.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_secureboot.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_selftest.o: platform/cryptoauthlib/lib/basic/atca_basic_selftest.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_selftest.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_selftest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_selftest.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_selftest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_selftest.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_selftest.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sha.o: platform/cryptoauthlib/lib/basic/atca_basic_sha.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sha.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sha.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_sha.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sha.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sha.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sha.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sign.o: platform/cryptoauthlib/lib/basic/atca_basic_sign.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sign.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sign.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_sign.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sign.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sign.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_sign.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_updateextra.o: platform/cryptoauthlib/lib/basic/atca_basic_updateextra.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_updateextra.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_updateextra.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_updateextra.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_updateextra.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_updateextra.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_updateextra.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_verify.o: platform/cryptoauthlib/lib/basic/atca_basic_verify.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_verify.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_verify.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_verify.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_verify.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_verify.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_verify.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_write.o: platform/cryptoauthlib/lib/basic/atca_basic_write.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_write.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_write.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_basic_write.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_write.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_write.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_basic_write.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_helpers.o: platform/cryptoauthlib/lib/basic/atca_helpers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_helpers.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_helpers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/basic/atca_helpers.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_helpers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_helpers.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/basic/atca_helpers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.o: platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha1_routines.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.o: platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/hashes/sha2_routines.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o: platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_ecdsa.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o: platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_rand.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o: platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o: platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/crypto/atca_crypto_sw_sha2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/hal/atca_hal.o: platform/cryptoauthlib/lib/hal/atca_hal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/hal" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/atca_hal.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/atca_hal.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/hal/atca_hal.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/atca_hal.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/hal/atca_hal.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/hal/atca_hal.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_i2c.o: platform/cryptoauthlib/lib/hal/hal_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/hal" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_i2c.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/hal/hal_i2c.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_i2c.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_timer.o: platform/cryptoauthlib/lib/hal/hal_timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/hal" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_timer.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/hal/hal_timer.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_timer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/hal/hal_timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/host/atca_host.o: platform/cryptoauthlib/lib/host/atca_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/host" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/host/atca_host.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/host/atca_host.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/host/atca_host.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/host/atca_host.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/host/atca_host.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/host/atca_host.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls.o: platform/cryptoauthlib/lib/tls/atcatls.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/tls" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/tls/atcatls.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls_cfg.o: platform/cryptoauthlib/lib/tls/atcatls_cfg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib/tls" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls_cfg.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls_cfg.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/tls/atcatls_cfg.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls_cfg.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls_cfg.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/tls/atcatls_cfg.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atca_cfgs.o: platform/cryptoauthlib/lib/atca_cfgs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_cfgs.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_cfgs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atca_cfgs.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_cfgs.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_cfgs.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_cfgs.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atca_command.o: platform/cryptoauthlib/lib/atca_command.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_command.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_command.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atca_command.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_command.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_command.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_command.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atca_device.o: platform/cryptoauthlib/lib/atca_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_device.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atca_device.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_device.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_device.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atca_execution.o: platform/cryptoauthlib/lib/atca_execution.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_execution.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_execution.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atca_execution.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_execution.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_execution.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_execution.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/cryptoauthlib/lib/atca_iface.o: platform/cryptoauthlib/lib/atca_iface.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/cryptoauthlib/lib" 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_iface.o.d 
	@${RM} ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_iface.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/cryptoauthlib/lib/atca_iface.c  -o ${OBJECTDIR}/platform/cryptoauthlib/lib/atca_iface.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_iface.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/cryptoauthlib/lib/atca_iface.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/drivers/i2c_master.o: platform/drivers/i2c_master.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/drivers" 
	@${RM} ${OBJECTDIR}/platform/drivers/i2c_master.o.d 
	@${RM} ${OBJECTDIR}/platform/drivers/i2c_master.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/drivers/i2c_master.c  -o ${OBJECTDIR}/platform/drivers/i2c_master.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/drivers/i2c_master.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/drivers/i2c_master.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/drivers/i2c_simple_master.o: platform/drivers/i2c_simple_master.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/drivers" 
	@${RM} ${OBJECTDIR}/platform/drivers/i2c_simple_master.o.d 
	@${RM} ${OBJECTDIR}/platform/drivers/i2c_simple_master.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/drivers/i2c_simple_master.c  -o ${OBJECTDIR}/platform/drivers/i2c_simple_master.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/drivers/i2c_simple_master.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/drivers/i2c_simple_master.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/drivers/i2c_types.o: platform/drivers/i2c_types.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/drivers" 
	@${RM} ${OBJECTDIR}/platform/drivers/i2c_types.o.d 
	@${RM} ${OBJECTDIR}/platform/drivers/i2c_types.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/drivers/i2c_types.c  -o ${OBJECTDIR}/platform/drivers/i2c_types.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/drivers/i2c_types.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/drivers/i2c_types.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/drivers/spi_master.o: platform/drivers/spi_master.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/drivers" 
	@${RM} ${OBJECTDIR}/platform/drivers/spi_master.o.d 
	@${RM} ${OBJECTDIR}/platform/drivers/spi_master.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/drivers/spi_master.c  -o ${OBJECTDIR}/platform/drivers/spi_master.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/drivers/spi_master.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/drivers/spi_master.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/drivers/timeout.o: platform/drivers/timeout.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/drivers" 
	@${RM} ${OBJECTDIR}/platform/drivers/timeout.o.d 
	@${RM} ${OBJECTDIR}/platform/drivers/timeout.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/drivers/timeout.c  -o ${OBJECTDIR}/platform/drivers/timeout.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/drivers/timeout.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/drivers/timeout.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/drivers/uart.o: platform/drivers/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/drivers" 
	@${RM} ${OBJECTDIR}/platform/drivers/uart.o.d 
	@${RM} ${OBJECTDIR}/platform/drivers/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/drivers/uart.c  -o ${OBJECTDIR}/platform/drivers/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/drivers/uart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/drivers/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.o: platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd" 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.o.d 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.c  -o ${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/mqtt/mqtt_comm_bsd/mqtt_comm_layer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/mqtt/mqtt_core/mqtt_core.o: platform/mqtt/mqtt_core/mqtt_core.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/mqtt/mqtt_core" 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_core/mqtt_core.o.d 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_core/mqtt_core.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/mqtt/mqtt_core/mqtt_core.c  -o ${OBJECTDIR}/platform/mqtt/mqtt_core/mqtt_core.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/mqtt/mqtt_core/mqtt_core.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/mqtt/mqtt_core/mqtt_core.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.o: platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer" 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.o.d 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.c  -o ${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/mqtt/mqtt_exchange_buffer/mqtt_exchange_buffer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/mqtt/mqtt_packetTransfer_interface.o: platform/mqtt/mqtt_packetTransfer_interface.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/mqtt" 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_packetTransfer_interface.o.d 
	@${RM} ${OBJECTDIR}/platform/mqtt/mqtt_packetTransfer_interface.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/mqtt/mqtt_packetTransfer_interface.c  -o ${OBJECTDIR}/platform/mqtt/mqtt_packetTransfer_interface.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/mqtt/mqtt_packetTransfer_interface.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/mqtt/mqtt_packetTransfer_interface.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/bsp/source/nm_bsp_mega.o: platform/winc/bsp/source/nm_bsp_mega.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/bsp/source" 
	@${RM} ${OBJECTDIR}/platform/winc/bsp/source/nm_bsp_mega.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/bsp/source/nm_bsp_mega.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/bsp/source/nm_bsp_mega.c  -o ${OBJECTDIR}/platform/winc/bsp/source/nm_bsp_mega.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/bsp/source/nm_bsp_mega.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/bsp/source/nm_bsp_mega.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.o: platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/bus_wrapper/source" 
	@${RM} ${OBJECTDIR}/platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.c  -o ${OBJECTDIR}/platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/bus_wrapper/source/nm_bus_wrapper_mega.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/common/source/nm_common.o: platform/winc/common/source/nm_common.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/common/source" 
	@${RM} ${OBJECTDIR}/platform/winc/common/source/nm_common.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/common/source/nm_common.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/common/source/nm_common.c  -o ${OBJECTDIR}/platform/winc/common/source/nm_common.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/common/source/nm_common.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/common/source/nm_common.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/m2m_ate_mode.o: platform/winc/driver/source/m2m_ate_mode.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_ate_mode.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_ate_mode.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/m2m_ate_mode.c  -o ${OBJECTDIR}/platform/winc/driver/source/m2m_ate_mode.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/m2m_ate_mode.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/m2m_ate_mode.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/m2m_crypto.o: platform/winc/driver/source/m2m_crypto.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_crypto.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_crypto.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/m2m_crypto.c  -o ${OBJECTDIR}/platform/winc/driver/source/m2m_crypto.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/m2m_crypto.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/m2m_crypto.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/m2m_hif.o: platform/winc/driver/source/m2m_hif.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_hif.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_hif.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/m2m_hif.c  -o ${OBJECTDIR}/platform/winc/driver/source/m2m_hif.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/m2m_hif.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/m2m_hif.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/m2m_ota.o: platform/winc/driver/source/m2m_ota.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_ota.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_ota.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/m2m_ota.c  -o ${OBJECTDIR}/platform/winc/driver/source/m2m_ota.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/m2m_ota.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/m2m_ota.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/m2m_periph.o: platform/winc/driver/source/m2m_periph.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_periph.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_periph.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/m2m_periph.c  -o ${OBJECTDIR}/platform/winc/driver/source/m2m_periph.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/m2m_periph.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/m2m_periph.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/m2m_ssl.o: platform/winc/driver/source/m2m_ssl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_ssl.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_ssl.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/m2m_ssl.c  -o ${OBJECTDIR}/platform/winc/driver/source/m2m_ssl.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/m2m_ssl.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/m2m_ssl.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/m2m_wifi.o: platform/winc/driver/source/m2m_wifi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_wifi.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/m2m_wifi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/m2m_wifi.c  -o ${OBJECTDIR}/platform/winc/driver/source/m2m_wifi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/m2m_wifi.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/m2m_wifi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/nmasic.o: platform/winc/driver/source/nmasic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmasic.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmasic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/nmasic.c  -o ${OBJECTDIR}/platform/winc/driver/source/nmasic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/nmasic.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/nmasic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/nmbus.o: platform/winc/driver/source/nmbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmbus.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmbus.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/nmbus.c  -o ${OBJECTDIR}/platform/winc/driver/source/nmbus.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/nmbus.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/nmbus.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/nmdrv.o: platform/winc/driver/source/nmdrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmdrv.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmdrv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/nmdrv.c  -o ${OBJECTDIR}/platform/winc/driver/source/nmdrv.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/nmdrv.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/nmdrv.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/nmi2c.o: platform/winc/driver/source/nmi2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmi2c.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmi2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/nmi2c.c  -o ${OBJECTDIR}/platform/winc/driver/source/nmi2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/nmi2c.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/nmi2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/nmspi.o: platform/winc/driver/source/nmspi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmspi.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmspi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/nmspi.c  -o ${OBJECTDIR}/platform/winc/driver/source/nmspi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/nmspi.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/nmspi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/driver/source/nmuart.o: platform/winc/driver/source/nmuart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/driver/source" 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmuart.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/driver/source/nmuart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/driver/source/nmuart.c  -o ${OBJECTDIR}/platform/winc/driver/source/nmuart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/driver/source/nmuart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/driver/source/nmuart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/socket/source/socket.o: platform/winc/socket/source/socket.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/socket/source" 
	@${RM} ${OBJECTDIR}/platform/winc/socket/source/socket.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/socket/source/socket.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/socket/source/socket.c  -o ${OBJECTDIR}/platform/winc/socket/source/socket.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/socket/source/socket.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/socket/source/socket.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/winc/spi_flash/source/spi_flash.o: platform/winc/spi_flash/source/spi_flash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform/winc/spi_flash/source" 
	@${RM} ${OBJECTDIR}/platform/winc/spi_flash/source/spi_flash.o.d 
	@${RM} ${OBJECTDIR}/platform/winc/spi_flash/source/spi_flash.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/winc/spi_flash/source/spi_flash.c  -o ${OBJECTDIR}/platform/winc/spi_flash/source/spi_flash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/winc/spi_flash/source/spi_flash.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/winc/spi_flash/source/spi_flash.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/adc1.o: platform/adc1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/adc1.o.d 
	@${RM} ${OBJECTDIR}/platform/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/adc1.c  -o ${OBJECTDIR}/platform/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/adc1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/adc1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/application_manager.o: platform/application_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/application_manager.o.d 
	@${RM} ${OBJECTDIR}/platform/application_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/application_manager.c  -o ${OBJECTDIR}/platform/application_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/application_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/application_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/clock.o: platform/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/clock.o.d 
	@${RM} ${OBJECTDIR}/platform/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/clock.c  -o ${OBJECTDIR}/platform/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/clock.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/clock.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/debug_print.o: platform/debug_print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/debug_print.o.d 
	@${RM} ${OBJECTDIR}/platform/debug_print.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/debug_print.c  -o ${OBJECTDIR}/platform/debug_print.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/debug_print.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/debug_print.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/delay.o: platform/delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/delay.o.d 
	@${RM} ${OBJECTDIR}/platform/delay.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/delay.c  -o ${OBJECTDIR}/platform/delay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/delay.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/delay.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/i2c2_driver.o: platform/i2c2_driver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/i2c2_driver.o.d 
	@${RM} ${OBJECTDIR}/platform/i2c2_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/i2c2_driver.c  -o ${OBJECTDIR}/platform/i2c2_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/i2c2_driver.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/i2c2_driver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/interrupt_manager.o: platform/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/platform/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/interrupt_manager.c  -o ${OBJECTDIR}/platform/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/interrupt_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/interrupt_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/led.o: platform/led.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/led.o.d 
	@${RM} ${OBJECTDIR}/platform/led.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/led.c  -o ${OBJECTDIR}/platform/led.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/led.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/led.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/mcc.o: platform/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/mcc.o.d 
	@${RM} ${OBJECTDIR}/platform/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/mcc.c  -o ${OBJECTDIR}/platform/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/mcc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/mcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/pin_manager.o: platform/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/platform/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/pin_manager.c  -o ${OBJECTDIR}/platform/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/pin_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/pin_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/sensors_handling.o: platform/sensors_handling.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/sensors_handling.o.d 
	@${RM} ${OBJECTDIR}/platform/sensors_handling.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/sensors_handling.c  -o ${OBJECTDIR}/platform/sensors_handling.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/sensors_handling.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/sensors_handling.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/spi1_driver.o: platform/spi1_driver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/spi1_driver.o.d 
	@${RM} ${OBJECTDIR}/platform/spi1_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/spi1_driver.c  -o ${OBJECTDIR}/platform/spi1_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/spi1_driver.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/spi1_driver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/system.o: platform/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/system.o.d 
	@${RM} ${OBJECTDIR}/platform/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/system.c  -o ${OBJECTDIR}/platform/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/system.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/tmr1.o: platform/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/tmr1.o.d 
	@${RM} ${OBJECTDIR}/platform/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/tmr1.c  -o ${OBJECTDIR}/platform/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/tmr1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/tmr1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/tmr2.o: platform/tmr2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/tmr2.o.d 
	@${RM} ${OBJECTDIR}/platform/tmr2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/tmr2.c  -o ${OBJECTDIR}/platform/tmr2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/tmr2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/tmr2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/traps.o: platform/traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/traps.o.d 
	@${RM} ${OBJECTDIR}/platform/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/traps.c  -o ${OBJECTDIR}/platform/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/traps.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/traps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/platform/uart1.o: platform/uart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/platform" 
	@${RM} ${OBJECTDIR}/platform/uart1.o.d 
	@${RM} ${OBJECTDIR}/platform/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  platform/uart1.c  -o ${OBJECTDIR}/platform/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/platform/uart1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/platform/uart1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mqtt_packetPopulation/mqtt_iothub_packetPopulate.o: mqtt_packetPopulation/mqtt_iothub_packetPopulate.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mqtt_packetPopulation" 
	@${RM} ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iothub_packetPopulate.o.d 
	@${RM} ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iothub_packetPopulate.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mqtt_packetPopulation/mqtt_iothub_packetPopulate.c  -o ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iothub_packetPopulate.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mqtt_packetPopulation/mqtt_iothub_packetPopulate.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mqtt_packetPopulation/mqtt_iothub_packetPopulate.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.o: mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mqtt_packetPopulation" 
	@${RM} ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.o.d 
	@${RM} ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.c  -o ${OBJECTDIR}/mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mqtt_packetPopulation/mqtt_iotprovisioning_packetPopulate.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mqtt_packetPopulation/mqtt_packetPopulate.o: mqtt_packetPopulation/mqtt_packetPopulate.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mqtt_packetPopulation" 
	@${RM} ${OBJECTDIR}/mqtt_packetPopulation/mqtt_packetPopulate.o.d 
	@${RM} ${OBJECTDIR}/mqtt_packetPopulation/mqtt_packetPopulate.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mqtt_packetPopulation/mqtt_packetPopulate.c  -o ${OBJECTDIR}/mqtt_packetPopulation/mqtt_packetPopulate.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mqtt_packetPopulation/mqtt_packetPopulate.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -ffunction-sections -mlarge-code -O1 -I"azure-c-sdk/sdk/inc" -Duintptr_t=void* -msmart-io=1 -Wall -msfr-warn=off   -std=gnu99  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/mqtt_packetPopulation/mqtt_packetPopulate.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/AzureIotDps.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/AzureIotDps.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)      -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp=${DFP_DIR}/xc16 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/AzureIotDps.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/AzureIotDps.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp=${DFP_DIR}/xc16 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/AzureIotDps.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp=${DFP_DIR}/xc16 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
