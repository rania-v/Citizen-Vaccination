#include <stdio.h>
#include <stdlib.h>
#include "entries.h"

typedef struct Bloom_Filter
{
	// int size_in_bytes;
	// int size_in_bits;
	int array_size;
	int* bit_array;
}Bloom_Filter;


Bloom_Filter* Bloom_Filter_Initialize(int size);

Bloom_Filter* Bloom_Filter_Set_Bit(Bloom_Filter* bf, unsigned long bitnum);
int Bloom_Filter_Get_Bit(Bloom_Filter* bf, unsigned long bitnum);
Bloom_Filter* Bloom_Filter_Insert_Id(Bloom_Filter* bf, char* id);
int Bloom_Filter_Search(Bloom_Filter* bf, char* id);
Bloom_Filter* Bloom_Filter_Delete(Bloom_Filter* bf);

unsigned long djb2(unsigned char *str);
unsigned long sdbm(unsigned char *str);
unsigned long hash_i(unsigned char *str, unsigned int i);
