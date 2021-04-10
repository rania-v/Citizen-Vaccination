#ifndef DATAPROCESSING_H_
#define DATAPROCESSING_H_

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "skip_list.h"
#include "bloom_filter.h"
#include "entries.h"


typedef struct Hash_Data
{
	Hash* Id;
	Hash* Age;
	Hash* Name;
	Hash* Surname;
	Hash* Country;
	Hash* Virus;
	// Hash* Condition;
	Hash* Date;
	int bloom_size;
}Hash_Data;

Hash_Data* Hash_Data_Iniitalise(int bloom_size);


int DataProcessing(const char* filename, Hash_Data* hd);

Hash_Data* InsertWord(char* word, int wordcount, Hash_Data* hd, Entry* entry);

Node* InsertId(char* word, Hash_Data* hd);
Node* InsertName(char* word, Hash_Data* hd);
Node* InsertSurname(char* word, Hash_Data* hd);
Node* InsertCountry(char* word, Hash_Data* hd);
Node* InsertAge(char* word, Hash_Data* hd);
Node* InsertVirus(char* word, Hash_Data* hd);
Node* InsertCondition(char* word, Hash_Data* hd);
Node* InsertDate(char* word, Hash_Data* hd);

void Hash_Data_Print(Hash_Data* hd);

void Hash_Data_Delete(Hash_Data* hd);


// struct Hash;
//
// typedef char* Id;
// typedef char* Age;
// typedef char* Name;
// typedef char* Surname;
// typedef char* Country;
// typedef char* Virus;
// typedef char* Condition;
// typedef char* Date;

#endif
