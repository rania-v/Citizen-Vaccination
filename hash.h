#ifndef HASH_H_
#define HASH_H_

#include <stdio.h>
#include <stdlib.h>
// #include "bloom_filter.h"
// // #include "dataprocessing.h"



#define HASHSIZE 10

typedef struct Node
{
	char* data;
	struct Node* next;
  struct Node* prev;
	struct Bucket* bucket;
	// // ==========================
	struct Bloom_Filter* bloom_filter;
	// // ==========================
	struct Skip_List* vaccinated_persons;
	struct Skip_List* not_vaccinated_persons;
	// =============================
	int total_count;
	int vaccinated_count;
	void* data4questions;
	// int not_vaccinated_count;
}Node;

typedef struct Bucket
{
	int node_count;
	Node* first;
	Node* last;
}Bucket;

typedef struct Hash
{
	Bucket* harray[HASHSIZE];
	int total_count;
}Hash;

int Hash_Function(char* entry);

// INITIALIZATION FUNCTIONS
Node* Node_Initialize(char* entry);
// ------------------------
Bucket* Bucket_Initialize();
// ------------------------
Hash* Hash_Initialize();


// INSERTTION FUNCTIONS
Bucket* Bucket_Insert_Node(Bucket* B, Node* node);
// ------------------------
Node* Hash_Insert_Node(Hash* H, Node* node);


// SEARCH FUNCTIONS
Node* Hash_Search(Hash* H, char* entry);


// DISCARDING FUNCTIONS
Hash* Hash_Discard_Data(Hash* H, Node* node);


// PRINT FUNCTIONS
void Node_Print(Node* node);
// ------------------------
void Bucket_Print(Bucket* B);
void Bucket_Print_Data_Count(Bucket* B);
// ------------------------
void Hash_Print(Hash* H);
void Hash_Print_Status(Hash* H);


// DELETION FUNCTIONS
void Node_Delete(Node* node);
// ------------------------
void Bucket_Delete(Bucket* B);
// ------------------------
void Hash_Delete(Hash* H);


#endif
