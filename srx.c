#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif // HAVE_CONFIG_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#include "nfc/nfc.h"
#include "nfc-internal.h"
#include "srx.h"
#include "pn53x-internal.h"

#include "mirror-subr.h"

#define LOG_CATEGORY "libnfc.chip.pn53x"
#define LOG_GROUP NFC_LOG_GROUP_CHIP

#define INIT		0 
#define UID			1
#define READ		2
#define WRITE	3

#define ST25TB04K_SZ     	0x80
#define ST25TB02K_SZ		0x40
#define ST25TB512_SZ		0x10

#define TAG_SIZE	ST25TB512_SZ

static uint8_t *result = NULL; 
static uint8_t **read_buffer = NULL;

uint8_t **srx_read(struct nfc_device *pnd, const nfc_modulation nm)
{
	/***** Commands *****/
	
	uint8_t initiate[]      = 	{0x06, 0x00};
	uint8_t selectChip[] 	= 	{0x0e, 0x00};
	uint8_t readBlock[]  	= 	{0x08, 0x00};
	
	/***** Responses *****/
	uint8_t ChipBuffer[1]   = {0};
	uint8_t readBuffer [5]  = {0};
	
	int res = 0;
	bool found = false;
	if (nm.nmt == NMT_ISO14443B2SR)
	{
			if (CHIP_DATA(pnd)->type == RCS360) 
			{
				// TODO add support for RC-S360, at the moment it refuses to send raw frames without a first select
				pnd->last_error = NFC_ENOTIMPL;
				return pnd->last_error;
			}
			// No native support in InListPassiveTarget so we do discovery by hand
			if ((res = nfc_device_set_property_bool(pnd, NP_FORCE_ISO14443_B, true)) < 0) {
			  return res;
			}
			if ((res = nfc_device_set_property_bool(pnd, NP_FORCE_SPEED_106, true)) < 0) {
			  return res;
			}
			if ((res = nfc_device_set_property_bool(pnd, NP_HANDLE_CRC, true)) < 0) {
			  return res;
			}
			if ((res = nfc_device_set_property_bool(pnd, NP_EASY_FRAMING, false)) < 0) {
			  return res;
			}
			
			do 
			{
				if (nm.nmt == NMT_ISO14443B2SR) 
				{
					if ((res = pn53x_initiator_transceive_bytes(pnd, initiate, 2, ChipBuffer, sizeof(ChipBuffer), 0)) < 0) 
					{
						  if ((res == NFC_ERFTRANS) && (CHIP_DATA(pnd)->last_status_byte == 0x01)) // Chip timeout
						  continue;
						  else
						  return res;
					}
					
					selectChip[1] = ChipBuffer[0];
					if ((res = pn53x_initiator_transceive_bytes(pnd, selectChip, sizeof(selectChip), ChipBuffer, sizeof(ChipBuffer), 0)) < 0) 
					return res;
					
					read_buffer = malloc(sizeof(uint8_t)*TAG_SIZE+1);
					for (int i = 0x00; i < TAG_SIZE; i++) // For ST25TB512-AC
					{
						readBlock[1] = i;
						if ((res = pn53x_initiator_transceive_bytes(pnd, readBlock, sizeof(readBlock), readBuffer, 4, 0) < 0))
						return res;

						read_buffer[i] = malloc (sizeof(uint8_t)*4);
						for (int c = 0; c < 4; c++) 
						read_buffer[i][c] = readBuffer[c];
					}
				}
				found = true;
				break;
			} while (pnd->bInfiniteSelect);
	}
	else printf("[-] Not the right type of tag ... \n");
		
	return read_buffer;
}

uint8_t *srx_read_uid(struct nfc_device *pnd, const nfc_modulation nm)
{
	/***** Commands *****/
	
	uint8_t initiate[]      	= 	{0x06, 0x00};
	uint8_t selectChip[] 	= 	{0x0e, 0x00};
	uint8_t getUID[]        	=	{0x0b};
	uint8_t ChipBuffer[1]  = {0};
	/***** Responses *****/
	
	uint8_t UIDBuffer[15] 	= 	{0};
	
	int res = 0;
	bool found = false;
	if (nm.nmt == NMT_ISO14443B2SR)
	{
			if (CHIP_DATA(pnd)->type == RCS360) 
			{
				// TODO add support for RC-S360, at the moment it refuses to send raw frames without a first select
				pnd->last_error = NFC_ENOTIMPL;
				return pnd->last_error;
			}
			// No native support in InListPassiveTarget so we do discovery by hand
			if ((res = nfc_device_set_property_bool(pnd, NP_FORCE_ISO14443_B, true)) < 0) {
			  return res;
			}
			if ((res = nfc_device_set_property_bool(pnd, NP_FORCE_SPEED_106, true)) < 0) {
			  return res;
			}
			if ((res = nfc_device_set_property_bool(pnd, NP_HANDLE_CRC, true)) < 0) {
			  return res;
			}
			if ((res = nfc_device_set_property_bool(pnd, NP_EASY_FRAMING, false)) < 0) {
			  return res;
			}
			
			do 
			{
				if (nm.nmt == NMT_ISO14443B2SR) 
				{
					if ((res = pn53x_initiator_transceive_bytes(pnd, initiate, 2, ChipBuffer, sizeof(ChipBuffer), 0)) < 0) 
					{
						  if ((res == NFC_ERFTRANS) && (CHIP_DATA(pnd)->last_status_byte == 0x01)) // Chip timeout
						  continue;
						  else
						  return res;
					}
					
					selectChip[1] = ChipBuffer[0];
					if ((res = pn53x_initiator_transceive_bytes(pnd, selectChip, sizeof(selectChip), ChipBuffer, sizeof(ChipBuffer), 0)) < 0) 
					return res;
				
					if ((res = pn53x_initiator_transceive_bytes(pnd, getUID, sizeof(getUID), UIDBuffer, 10, 0)) < 0)
					return res;

					result = (uint8_t*)malloc(sizeof(uint8_t)*10);
					if (result != 0) 
					memcpy(result, UIDBuffer, sizeof(UIDBuffer));
				}
				found = true;
				break;
			} while (pnd->bInfiniteSelect);
	}
	else printf("[-] Not the right type of tag ... \n");
		
	return result;
}

uint8_t srx_write(struct nfc_device *pnd, const nfc_modulation nm, uint8_t address, int data)
{
	int res = 0;
	uint8_t initiate[]      	= 	{0x06, 0x00};
	uint8_t selectChip[] 		= 	{0x0e, 0x00};
	uint8_t writeBlock[] 		= 	{0x09, 0x00, 0x00, 0x00, 0x00, 0x00}; // See the official documentation
	uint8_t ChipBuffer[1]    	= 	{0};
	
	bool found = false;
	if (nm.nmt == NMT_ISO14443B2SR)
	{
			if (CHIP_DATA(pnd)->type == RCS360) 
			{
				// TODO add support for RC-S360, at the moment it refuses to send raw frames without a first select
				pnd->last_error = NFC_ENOTIMPL;
				return pnd->last_error;
			}
			// No native support in InListPassiveTarget so we do discovery by hand
			if ((res = nfc_device_set_property_bool(pnd, NP_FORCE_ISO14443_B, true)) < 0) {
			  return res;
			}
			if ((res = nfc_device_set_property_bool(pnd, NP_FORCE_SPEED_106, true)) < 0) {
			  return res;
			}
			if ((res = nfc_device_set_property_bool(pnd, NP_HANDLE_CRC, true)) < 0) {
			  return res;
			}
			if ((res = nfc_device_set_property_bool(pnd, NP_EASY_FRAMING, false)) < 0) {
			  return res;
			}
			
			do 
			{
				if (nm.nmt == NMT_ISO14443B2SR) 
				{
					if ((res = pn53x_initiator_transceive_bytes(pnd, initiate, 2, ChipBuffer, sizeof(ChipBuffer), 0)) < 0) 
					{
						  if ((res == NFC_ERFTRANS) && (CHIP_DATA(pnd)->last_status_byte == 0x01)) // Chip timeout
						  continue;
						  else
						  return res;
					}
					
					selectChip[1] = ChipBuffer[0];
					if ((res = pn53x_initiator_transceive_bytes(pnd, selectChip, sizeof(selectChip), ChipBuffer, sizeof(ChipBuffer), 0)) < 0) 
					return res;

					writeBlock[1] = address;
					writeBlock[2] = ((data>>24)&0xFF);
					writeBlock[3] = ((data>>16)&0xFF);
					writeBlock[4] = ((data>>8)&0xFF); //Second byte
					writeBlock[5] = (data&0xFF); //First byte
					pn53x_initiator_transceive_bytes(pnd, writeBlock, sizeof(writeBlock), NULL, 0, 0) ;
				}
			
				found = true;
				break;
			}while (pnd->bInfiniteSelect);
	} 
	return 0
}
