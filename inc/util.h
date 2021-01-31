#ifndef __UTIL_H
    #define __UTIL_H
    
#include <stdint.h>
#include <stdbool.h>

/* Reverse an uint32 number  */
void uint32_reverse(uint8_t *data);

/* Converts hex to binary */
void hex_to_bin(char *hex, uint32_t len, uint8_t *out);

/* Compares two MD5 checksums */
bool md5cmp(uint8_t *md51, uint8_t *md52);

/* Trim str */
void trim(char *str);

/* Trim string at first non-printable char */
void printable_only(char *text);

/* Returns the pair md5 of "component/vendor" */
void component_vendor_md5(char *component, char *vendor, uint8_t *out);

/* Returns the current date stamp */
char *datestamp(void);

/* Prints a "created" JSON element with the current datestamp */
void print_datestamp(void);

//void file_md5(char *filepath, uint8_t *md5_result);

/* Returns a string with a hex representation of md5 */
char *md5_hex(uint8_t *md5);

/* Removes chr from str */
void remove_char(char *str, char chr);

/* Case insensitive string start comparison,
	returns true if a starts with b or viceversa */
bool stristart(char *a, char *b);

#endif
