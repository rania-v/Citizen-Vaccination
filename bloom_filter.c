#ifndef BLOOM_FILTER_H_
#define BLOOM_FILTER_H_

#include <stdio.h>
#include <stdlib.h>
#include "bloom_filter.h"
// #include "entries.h"

Bloom_Filter* Bloom_Filter_Initialize(int size)
{
	Bloom_Filter* bf = malloc(sizeof(Bloom_Filter));
	bf->array_size = size / sizeof(int);
	bf->size_in_bits = size * 8;
	bf->bit_array = malloc(sizeof(int)*(bf->array_size));
	for(int i=0; i< bf->array_size; i++)
		bf->bit_array[i] = 0;
	return bf;
}


Bloom_Filter* Bloom_Filter_Set_Bit(Bloom_Filter* bf, unsigned long bitnum)
{
	unsigned long temp = 1;
	unsigned long array_cell = bitnum / 32;
	unsigned long bit_position = bitnum % 32;

	temp = temp << bit_position;
	bf->bit_array[array_cell] = bf->bit_array[array_cell] | temp;
	return bf;
}

Bloom_Filter* Bloom_Filter_Insert_Id(Bloom_Filter* bf, char* id)
{
	unsigned long a, b, c;
	a = hash_i((unsigned char*)id, 1) % (unsigned long)bf->array_size;
	b = hash_i((unsigned char*)id, 2) % (unsigned long)bf->array_size;
	c = hash_i((unsigned char*)id, 3) % (unsigned long)bf->array_size;
// ===================================
	bf = Bloom_Filter_Set_Bit(bf, a);
	bf = Bloom_Filter_Set_Bit(bf, b);
	bf = Bloom_Filter_Set_Bit(bf, c);

	return bf;
}

int Bloom_Filter_Get_Bit(Bloom_Filter* bf, unsigned long bitnum)
{
	unsigned long temp = 1;
	unsigned long  array_cell = bitnum / 32;
	unsigned long  bit_position = bitnum % 32;

	temp = temp << bit_position;
	if((bf->bit_array[array_cell] & temp) == 0)
		return 0;
	else
		return 1;
}

int Bloom_Filter_Search(Bloom_Filter* bf, char* id)
{
	if(bf == NULL)
	{return 0;}
	unsigned long a, b, c;
	int f1, f2, f3;
	a = hash_i((unsigned char*)id, 1) % (unsigned long)bf->array_size;
	b = hash_i((unsigned char*)id, 2) % (unsigned long)bf->array_size;
	c = hash_i((unsigned char*)id, 3) % (unsigned long)bf->array_size;
// ===================================
	f1 = Bloom_Filter_Get_Bit(bf, a);
	f2 = Bloom_Filter_Get_Bit(bf, b);
	f3 = Bloom_Filter_Get_Bit(bf, c);
// ===================================
	if(f1 == 0 && f2 == 0 && f3 == 0)
		return 0;
	else
		return 1;
}

Bloom_Filter* Bloom_Filter_Delete(Bloom_Filter* bf)
{
	free(bf->bit_array);
	free(bf);
	return NULL;
}


/*-------------------- HASH FUNCTIONS --------------------*/

unsigned long djb2(unsigned char *str)
{
	int c;
	unsigned long hash = 5381;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; // hash * 33 + c

	return hash;
}

unsigned long sdbm(unsigned char *str)
{
	int c;
	unsigned long hash = 0;

	while ((c = *str++))
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash;
}

unsigned long hash_i(unsigned char *str, unsigned int i)
{
	return djb2(str) + i * sdbm(str) + i * i;
}
#endif
