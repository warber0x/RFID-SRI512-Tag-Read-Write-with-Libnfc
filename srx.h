#include <nfc/nfc-types.h>
#include "pn53x-internal.h"

uint8_t srx_write(struct nfc_device *pnd, const nfc_modulation nm, uint8_t address, int data);
uint8_t **srx_read(struct nfc_device *pnd, const nfc_modulation nm);
uint8_t *srx_read_uid(struct nfc_device *pnd, const nfc_modulation nm);
