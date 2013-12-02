#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "endianness.h"
/*
* True, if system is little endian
*/
int is_little_endian()
{
    int one = 1;
    char *ptr = (char *) &one;
    return *ptr == 1;
}

/*
*True if system is big endian
*/
int is_big_endian()
{
    return ! is_little_endian();
}

/*
* Converts to big endian
* @param: char *data - Pointer to memory which will be converted
* @param: int size - The total size in bytes of *data
* @return: Modified *data
*/
void to_le(char *data, int size) {
    if(is_little_endian())
        return;

    swap_endianness(data,size);

    return;
}

/*
* Converts to big endian
* @param: char *data - Pointer to memory which will be converted
* @param: int size - The total size in bytes of *data
* @return: Modified *data
*/
void to_be(char *data, int size) {
    if(is_big_endian())
        return;

    swap_endianness(data,size);

    return;
}

/*
* Swaps the endianness
* @param: char *data - Pointer to memory which will be converted
* @param: int size - The total size in bytes of *data
* @return: Modified *data
*/
void swap_endianness(char *data,int size) 
{
    if(data==NULL)
        return;

    char *tmp = malloc(sizeof(char)*size);

    if(tmp == NULL)
        return;

    int i;
    for(i=0; i < size; i++){
        *(tmp + (size -i -1)) = *(data + i);
    }

    memcpy(data,tmp,size);
    free(tmp);
}


uint32_t to_be32(uint32_t value) {
    uint32_t tmp = value;
    to_be((char*) &tmp, sizeof(uint32_t));
    return tmp;
}

uint16_t to_be16(uint16_t value) {
    uint16_t tmp = value;
    to_be((char*) &tmp, sizeof(uint16_t));
    return tmp;
}

uint32_t to_le32(uint32_t value) {
    uint32_t tmp = value;
    to_le((char*) &tmp, sizeof(uint32_t));
    return tmp;
}

uint16_t to_le16(uint16_t value) {
    uint16_t tmp = value;
    to_le((char*) &tmp, sizeof(uint16_t));
    return tmp;
}