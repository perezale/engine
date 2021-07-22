#ifndef __LDB_CLIENT_H
    #define __LDB_CLIENT_H

#include <stdint.h>
#include "limits.h"
#include "ldb.h"

typedef enum 
{
    SHORTEST_PATH = 0,
    GET_ALL_FILE_IDS,
    ATTRIBUTION,
    NOTICES,
    GET_FIRST_COPYRIHGHT,
    LDB_GET_RECORDS,
    GET_COMPONENT_AGE,
    SCAN_FILE,
    COLLECT_ALL_FILES
} funtion_t;

typedef enum
{
    TABLE_COMPONENTS = 0,
    TABLE_FILES,
    TABLE_URLS,
} table_t;

#pragma pack(1)
typedef struct engine_funtion_t
{
    uint8_t type;
    uint8_t key[16];
    uint8_t table;
    uint8_t records_qty;
} engine_funtion_t;

#pragma pack(1)
typedef struct ldb_response_t
{
    uint8_t type;
    uint16_t response_size;
    uint8_t * response;
} ldb_response_t;

#pragma pack(1)
typedef struct ldb_recorset_t
{
    uint8_t md5[MD5_LEN];
    uint16_t len;
    uint8_t data[MAX_PATH];
} ldb_recorset_t;

#define LDB_RECORDSET_BUFFER_SIZE 30

#pragma pack(1)
typedef struct ldb_recorset_buffet_t
{
    uint16_t * index;
    ldb_recorset_t * records;
} ldb_recorset_buffet_t;


 ldb_response_t ldb_socket_request(char * ip, int port, engine_funtion_t * req);

#endif
