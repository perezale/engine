#ifndef __LDB_CLIENT_H
    #define __LDB_CLIENT_H

#include <stdint.h>
#include "limits.h"

typedef enum 
{
    SHORTEST_PATH,
    GET_ALL_FILE_IDS,
    ATTRIBUTION,
    NOTICES,
    GET_FIRST_COPYRIHGHT,
    GET_FIRST_NON_BLACKLISTED,
    GET_COMPONENT_AGE,
    SCAN_FILE,
    GET_URL_RECORDS,
} funtion_t;

typedef struct engine_funtion_t
{
    funtion_t type;
    uint8_t key[16];
} engine_funtion_t;

typedef struct ldb_response_t
{
    funtion_t type;
    uint16_t response_size;
    uint8_t * response;
} ldb_response_t;

typedef struct ldb_recorset_t
{
    uint8_t md5[MD5_LEN];
    uint16_t len;
    uint8_t data[MAX_PATH];
} ldb_recorset_t;

#define LDB_RECORDSET_BUFFER_SIZE 30

typedef struct ldb_recorset_buffet_t
{
    uint8_t index;
    ldb_recorset_t records[LDB_RECORDSET_BUFFER_SIZE];
} ldb_recorset_buffet_t;


 ldb_response_t ldb_socket_request(char * ip, int port, engine_funtion_t * req);

#endif
