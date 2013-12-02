#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
* True, if system is little endian
*/
int is_little_endian();

/*
*True if system is big endian
*/
int is_big_endian();

/*
* Converts to big endian
* @param: char *data - Pointer to memory which will be converted
* @param: int size - The total size in bytes of *data
* @return: Modified *data
*/
void to_le(char *data, int size);


/*
* Converts to big endian
* @param: char *data - Pointer to memory which will be converted
* @param: int size - The total size in bytes of *data
* @return: Modified *data
*/
void to_be(char *data, int size);

/*
* Swaps the endianness
* @param: char *data - Pointer to memory which will be converted
* @param: int size - The total size in bytes of *data
* @return: Modified *data
*/
void swap_endianness(char *data,int size);


/*
* Some convenient-functions
*/
uint16_t to_be16(uint16_t value);
uint16_t to_le16(uint16_t value);
uint32_t to_be32(uint32_t value);
uint32_t to_le32(uint32_t value);