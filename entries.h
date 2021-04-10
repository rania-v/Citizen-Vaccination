#ifndef ENTRIES_H_
#define ENTRIES_H_

#include "hash.h"

// typedef struct Node;
// struct Hash;

typedef struct Entry
{
	Node* Id;
	Node* Age;
	Node* Name;
	Node* Surname;
	Node* Country;
	Node** Virus;
	// Node* Condition;
	Node** Date;
	int virus_count;
}Entry;

Entry* Entry_Initialize(void);

Entry* Entry_Add_Id(Entry* entry,Node* id);
Entry* Entry_Add_Age(Entry* entry,Node* age);
Entry* Entry_Add_Name(Entry* entry,Node* name);
Entry* Entry_Add_Surname(Entry* entry,Node* surname);
Entry* Entry_Add_Country(Entry* entry,Node* country);
Entry* Entry_Add_Virus(Entry* entry,Node* virus);
// Entry* Entry_Add_Condition(Node* CONDITION);
Entry* Entry_Add_Date(Entry* entry,Node* date);

void Entry_Print(Entry* entry);

char* Entry_Get_Id(Entry* entry);
// char* Entry_Get_Virus(Entry* entry);
char* Entry_Get_Country(Entry* entry);
char* Entry_Get_Age(Entry* entry);
char* Entry_Get_Date(Entry* entry, int pos);

int Entry_Get_Virus_position(Entry* entry, char* virus);


Entry* Entry_Delete(Entry* entry);

#endif
