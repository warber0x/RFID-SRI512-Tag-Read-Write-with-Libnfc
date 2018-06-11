/**
 @Author: SAMIR Radouane
 @Date  : 31/03/2018
 @Desc  :
 
 * This code can read/write data blocks from STx card *
 ******************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <nfc/nfc.h>

#define RED   \033[0;31m
#define GREEN \033[1;32m
#define RESET \033[0m

#define INIT		0 
#define UID		1
#define READ		2
#define WRITE		3

#define MAX_TARGET_COUNT 16

#define ST25TB04K_SZ     	0x80
#define ST25TB02K_SZ		0x40
#define ST25TB512_SZ		0x10

#define TAG_SIZE	ST25TB512_SZ

int write_data(uint8_t address, int data)
{
	nfc_device  *pnd;
	nfc_context *context;

	/** initiate libnfc and set nfc_context **/
	nfc_init(&context);
	if (context == NULL) {
		printf("\033[0;31m[-]\033[0m Error loading libnfc, must quit ... \n");
		exit(EXIT_FAILURE);
	}

	/** Get libnfc Version **/
	const char *libnfc_version = nfc_version();
	printf("\033[1;32m[*]\033[0m Libnfc version: %s\n", libnfc_version);

	/** Open NFC Device **/
	pnd = nfc_open(context, NULL);
	if (pnd == NULL) {
		printf("\033[0;31m[-]\033[0m Couldn't open the NFC device, must quit ...\n");
		exit(EXIT_FAILURE);
	}

	//Initiate the device
	if (nfc_initiator_init(pnd) < 0) {
		printf("\033[0;31m[-]\033[0m Error to initiate the NFC device, must quit...\n");
		exit(EXIT_FAILURE);
	}

	//Everything is all right, so now is opened ...
	printf("\033[1;32m[*]\033[0m The device is opened: %s\n", nfc_device_get_name(pnd));

	/**=======================================================================**/
	/* This is a known bug from libnfc. To read 14443BSR you should initiate
	 * ISO14443B to prepare register for PN532 and after you can call
	 * SRx function.
	 */

	nfc_target anttemp[1];
	nfc_modulation nmtemp;
	nmtemp.nmt = NMT_ISO14443B;
	nmtemp.nbr = NBR_106;
	nfc_initiator_list_passive_targets(pnd, nmtemp, anttemp, MAX_TARGET_COUNT);
	/**=========================================================================**/

	/** Choose the type you wanna read **/
	const nfc_modulation SRx = {
		.nmt = NMT_ISO14443B2SR,
		.nbr = NBR_106,
	};
	
	nfc_write_data(pnd, SRx, address, data);
	
	nfc_close(pnd);
	nfc_exit(context);
	return 0;
}

int read_data(void)
{
	nfc_device  *pnd;
	nfc_context *context;

	/** initiate libnfc and set nfc_context **/
	nfc_init(&context);
	if (context == NULL) {
		printf("\033[0;31m[-]\033[0m Error loading libnfc, must quit ... \n");
		exit(EXIT_FAILURE);
	}

	/** Get libnfc Version **/
	const char *libnfc_version = nfc_version();
	printf("\033[1;32m[*]\033[0m Libnfc version: %s\n", libnfc_version);

	/** Open NFC Device **/
	pnd = nfc_open(context, NULL);
	if (pnd == NULL) {
		printf("\033[0;31m[-]\033[0m Couldn't open the NFC device, must quit ...\n");
		exit(EXIT_FAILURE);
	}

	//Initiate the device
	if (nfc_initiator_init(pnd) < 0) {
		printf("\033[0;31m[-]\033[0m Error to initiate the NFC device, must quit...\n");
		exit(EXIT_FAILURE);
	}

	//Everything is all right, so now is opened ...
	printf("\033[1;32m[*]\033[0m The device is opened: %s\n", nfc_device_get_name(pnd));

	/**=======================================================================**/
	/* This is a known bug from libnfc. To read 14443BSR you should initiate
	 * ISO14443B to prepare register for PN532 and after you can call
	 * SRx function.
	 */

	nfc_target anttemp[1];
	nfc_modulation nmtemp;
	nmtemp.nmt = NMT_ISO14443B;
	nmtemp.nbr = NBR_106;
	nfc_initiator_list_passive_targets(pnd, nmtemp, anttemp, MAX_TARGET_COUNT);  
	/**=========================================================================**/

	/** Choose the type you wanna read **/
	const nfc_modulation SRx = {
		.nmt = NMT_ISO14443B2SR,
		.nbr = NBR_106,
	};
	

	uint8_t **buffer = NULL;
	buffer = nfc_read_data(pnd, SRx);
	
	printf ("== === Tag Data === ==\n");
	if (buffer != NULL) 
	{
		for (int c = 0; c < TAG_SIZE; c++) 
		{
			printf("\033[0;31m[B]\033[0m %02x:   ", c);
			for (int i = 0; i < 4; i++)
				printf("%02x ", buffer[c][i]);
			printf("\n");
		}
		free(buffer);
	}
	printf("== === End Data === ==\n");
	
	nfc_close(pnd);
	nfc_exit(context);
	return 0;
}

int read_uid(void)
{
	nfc_device  *pnd;
	nfc_context *context;

	/** initiate libnfc and set nfc_context **/
	nfc_init(&context);
	if (context == NULL) {
		printf("\033[0;31m[-]\033[0m Error loading libnfc, must quit ... \n");
		exit(EXIT_FAILURE);
	}

	/** Get libnfc Version **/
	const char *libnfc_version = nfc_version();
	printf("\033[1;32m[*]\033[0m Libnfc version: %s\n", libnfc_version);

	/** Open NFC Device **/
	pnd = nfc_open(context, NULL);
	if (pnd == NULL) {
		printf("\033[0;31m[-]\033[0m Couldn't open the NFC device, must quit ...\n");
		exit(EXIT_FAILURE);
	}

	//Initiate the device
	if (nfc_initiator_init(pnd) < 0) {
		printf("\033[0;31m[-]\033[0m Error to initiate the NFC device, must quit...\n");
		exit(EXIT_FAILURE);
	}

	//Everything is all right, so now is opened ...
	printf("\033[1;32m[*]\033[0m The device is opened: %s\n", nfc_device_get_name(pnd));

	/**=======================================================================**/
	/* This is a known bug from libnfc. To read 14443BSR you should initiate
	 * ISO14443B to prepare register for PN532 and after you can call
	 * SRx function.
	 */

	nfc_target anttemp[1];
	nfc_modulation nmtemp;
	nmtemp.nmt = NMT_ISO14443B;
	nmtemp.nbr = NBR_106;
	nfc_initiator_list_passive_targets(pnd, nmtemp, anttemp, MAX_TARGET_COUNT);     
	/**=========================================================================**/

	/** Choose the type you wanna read **/
	const nfc_modulation SRx = {
		.nmt = NMT_ISO14443B2SR,
		.nbr = NBR_106,
	};
	

	uint8_t *buffer = nfc_read_uid_data(pnd, SRx);
	int k = 0;
	printf ("== === UID Data === ==\n");
	for ( k = 0; k < 10; k++)
	printf ("%02x", buffer[k]);
	
	printf("\n");
	printf("== === End Data === ==\n");
	free(buffer);
	
	nfc_close(pnd);
	nfc_exit(context);
	return 0;
}

int main(int argc, char **argv)
{
	uint8_t address = 0;
	uint32_t  data  = 0x00000000;
	
	if (argc != 3)
	{
		printf ("Usage: ./writeData [address in hex] [Data in hex]\n");
		printf("Ex: ./writeData 0x0F 0x02020101\n");
		printf("By Red1 - Morocco\n");
		return -1;
	}
	
	address = strtol(argv[1], NULL, 16);
	data      = strtol(argv[2], NULL, 16);
	
	if (address >= 0 && address <= 0x0F) 
	printf("Address: %x\n", address);
	else
	{
		printf ("Address not valid for SRI512\n");
		exit(0);
	}
	
	printf ("\033[1;32m[*]\033[0m Preparing the reader ...\n");
	printf ("\033[1;32m[*]\033[0m Write data: 0x%x at address: 0x%02x\n", data, address);
	write_data(address, data);
	printf("\033[1;32m[*]\033[0m Done\n");
	
	return 0;
}

