#ifndef __DEBUG_H
    #define __DEBUG_H
    
#include <stdint.h>
#include <stdbool.h>
#include "scanoss.h"

extern bool debug_on; //= false; //set debug mode from main.
extern bool quiet;


void scanlog(const char *fmt, ...);
void map_dump(scan_data *scan);
long microseconds_now(void);
void scan_benchmark(int m, int n);
void slow_query_log(scan_data *scan);


#endif
