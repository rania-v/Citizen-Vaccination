#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entries.h"


// ------------------INITIALIZATION FUNCTIONS------------------
Entry* Entry_Initialize(void)
{
	Entry* entry = malloc(sizeof(Entry));
	entry->Id = NULL;
	entry->Age = NULL;
	entry->Name = NULL;
	entry->Surname = NULL;
	entry->Country = NULL;
	entry->Virus = NULL;
	// entry->Condition = NULL;
	entry->Date = NULL;
	entry->virus_count = 0;
	return entry;
}

void Entry_Print(Entry* entry)
{
	printf("%s ", entry->Id->data);
	printf("%s ", entry->Name->data);
	printf("%s ", entry->Surname->data);
	printf("%s ", entry->Country->data);
	printf("%s ", entry->Age->data);
	for (int i = 0; i < entry->virus_count; i++)
	{
		if(entry->Virus[i] != NULL)
		printf("%s ", entry->Virus[i]->data);
		if(entry->Date != NULL)
			if(entry->Date[i] != NULL)
			printf("%s\n", entry->Date[i]->data);
		printf("\t\t\t\t");
	}
	printf("\n");
}

Entry* Entry_Add_Id(Entry* entry, Node* id)
{
	entry->Id = id;
	// pointer to the entry, only for Id hash nodes
	id->data4questions = entry;
	return entry;
}

Entry* Entry_Add_Age(Entry* entry, Node* age)
{
	entry->Age = age;
	return entry;
}

Entry* Entry_Add_Name(Entry* entry, Node* name)
{
	entry->Name = name;
	return entry;
}
Entry* Entry_Add_Surname(Entry* entry, Node* surname)
{
	entry->Surname = surname;
	return entry;
}
Entry* Entry_Add_Country(Entry* entry,Node* country)
{
	entry->Country = country;
	return entry;
}
Entry* Entry_Add_Virus(Entry* entry,Node* virus)
{
	int i;
	// search for virus
	for (i = 0; i < entry->virus_count; i++)
	{
		if(strcmp(virus->data, entry->Virus[i]->data) == 0)
		break;
	}
	// if virus already in virus array, dont insert
	// if not, realloc space and save update array
	if(i==entry->virus_count)
	{
		entry->virus_count++;
		entry->Virus = realloc(entry->Virus, entry->virus_count * sizeof(Node*));
		entry->Virus[entry->virus_count -1] = virus;
	}
	return entry;
}
// Entry* Entry_Add_Condition(Node* condition);
Entry* Entry_Add_Date(Entry* entry,Node* date)
{
	entry->Date = realloc(entry->Date, sizeof(Node*));
	entry->Date[entry->virus_count -1] = date;
	return entry;
}

// =========================================================

char* Entry_Get_Id(Entry* entry)
{
	if(entry->Id->data == NULL)
	{
		printf("ENTRY IS NULL\n" );
		return NULL;
	}
	return entry->Id->data;
}

int Entry_Get_Virus_position(Entry* entry, char* virus)
{
	int i;
	for (i = 0; i < entry->virus_count; i++)
	{
		if(strcmp(virus, entry->Virus[i]->data) == 0)
		break;
	}
	if(i == entry->virus_count)
		return -1;
	return i;
}

char* Entry_Get_Country(Entry* entry)
{
	if(entry->Country->data == NULL)
	{
		printf("ENTRY IS NULL\n" );
		return NULL;
	}
	return entry->Country->data;
}

char* Entry_Get_Age(Entry* entry)
{
	if(entry->Age->data == NULL)
	{
		printf("ENTRY IS NULL\n" );
		return NULL;
	}
	return entry->Age->data;
}

char* Entry_Get_Date(Entry* entry, int pos)
{
	if(entry->Date[pos]->data == NULL)
	{
		printf("ENTRY IS NULL\n" );
		return NULL;
	}
	return entry->Date[pos]->data;
}


Entry* Entry_Delete(Entry* entry)
{
	if(entry->Virus != NULL)
		free(entry->Virus);
	if(entry->Virus != NULL)
		free(entry->Date);
	free(entry);
	return NULL;
}
