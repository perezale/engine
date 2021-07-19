#ifndef __URL_H
    #define __URL_H

#include "scanoss.h"

bool handle_url_record(uint8_t *key, uint8_t *subkey, int subkey_ln, uint8_t *raw_data, uint32_t datalen, int iteration, void *ptr);
uint32_t url_records_process(uint8_t * buffer, match_data *matches);
#endif
