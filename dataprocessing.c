#include <stdio.h>
#include <string.h>
#include "virus.h"
#include "dataprocessing.h"


Hash_Data* Hash_Data_Iniitalise(int bloom_size)
{
	Hash_Data* hd = malloc(sizeof(Hash_Data));
	hd->Id = Hash_Initialize();
	hd->Age = Hash_Initialize();
	hd->Name = Hash_Initialize();
	hd->Surname = Hash_Initialize();
	hd->Country = Hash_Initialize();
	hd->Virus = Hash_Initialize();
	// CONDITION?
	hd->Date = Hash_Initialize();
	hd->bloom_size = bloom_size;
	return hd;
}

int DataProcessing(const char* filename, Hash_Data* hd)
{
	// open file
	FILE *fptr;
	fptr  = fopen(filename, "r");

	size_t length = 200* sizeof(char);
	char* line = malloc(length);
	char* word;
	int wordcount;


	while(getline(&line, &length, fptr) != -1)
	{
		// create entry with inserted data
		Entry* entry = Entry_Initialize();
		// Entry* entry;

		word = strtok(line, " ");
		wordcount = 0;
		// read each word and insert to hash and entry
		while (word != NULL)
		{
			// check if id already exists
			if(wordcount == 0)
			{
				Node* dupcheck = Hash_Search(hd->Id, word);
				if(dupcheck != NULL)
				{
					printf("Duplicate Id detected and dismissed : %s\n", word);
					Entry_Delete(entry);
					// dont insert entry, as id alreday exists
					word = NULL;
					break;
				}
			}
			hd = InsertWord(word, wordcount, hd, entry);
			word = strtok(NULL, " \n");
			wordcount++;
		}

		// Entry_Print(entry);
		// entry is inserted to skip lists from InsertWord CASE-6 if needed
	}

	free(line);
	return 1;
}

// insert word to the right hash hash and to entry
Hash_Data* InsertWord(char* word, int wordcount, Hash_Data* hd, Entry* entry)
{
	// printf("%s, %d\n", word, wordcount);
	Node* node;
	switch(wordcount)
	{
		case 0:
			node = InsertId(word, hd);
			Entry_Add_Id(entry, node);
			break;
		case 1:
			node = InsertName(word, hd);
			Entry_Add_Name(entry, node);
			break;
		case 2:
			node = InsertSurname(word, hd);
			Entry_Add_Surname(entry, node);
			break;
		case 3:
			node = InsertCountry(word, hd);
			Entry_Add_Country(entry, node);
			break;
		case 4:
			node = InsertAge(word, hd);
			Entry_Add_Age(entry, node);
			break;
		case 5:
			node = InsertVirus(word, hd);
			entry = Entry_Add_Virus(entry, node);
			// printf("	virus %s\n", entry->Virus[0]->data);
			break;
		case 6:
			// Entry_Add_Condition
			if((strcmp(word, "YES") == 0))
			{
				// printf("%s\n", word);
				// create skip list node
				SLNode* sl_node = SLNode_Initialize(entry, MAXLEVEL);
				Skip_List_Insert_SLNode(entry->Virus[0]->vaccinated_persons, sl_node);
				// add to bloomfilter
				if(entry->Virus[0]->bloom_filter == NULL)
					entry->Virus[0]->bloom_filter = Bloom_Filter_Initialize(hd->bloom_size);

				entry->Virus[0]->bloom_filter = Bloom_Filter_Insert_Id(entry->Virus[0]->bloom_filter, Entry_Get_Id(entry));
			}
			else if((strcmp(word, "NO") == 0))
			{
				SLNode* sl_node = SLNode_Initialize(entry, MAXLEVEL);
				Skip_List_Insert_SLNode(entry->Virus[0]->not_vaccinated_persons, sl_node);
				// Skip_List_Print(entry->Virus[0]->not_vaccinated_persons);
			}
			break;
		case 7:
			node = InsertDate(word, hd);
			entry = Entry_Add_Date(entry, node);
			// printf("	date %s\n", entry->Date[0]->data);

			break;
	}
	return hd;
}

Node* InsertId(char* id, Hash_Data* hd)
{
	// check if id already exists
	// printf("ID is %s\n", id);
	Node* node = Node_Initialize(id);
	node = Hash_Insert_Node(hd->Id, node);
	return node;
}

Node* InsertName(char* name, Hash_Data* hd)
{
	// printf("NAME is %s\n", name);
	Node* node = Node_Initialize(name);
	node = Hash_Insert_Node(hd->Name, node);
	return node;
}

Node* InsertSurname(char* surname, Hash_Data* hd)
{
	// printf("SURNAME is %s\n", surname);
	Node* node = Node_Initialize(surname);
	node = Hash_Insert_Node(hd->Surname, node);
	return node;
}

Node* InsertCountry(char* country, Hash_Data* hd)
{
	Node* node = Node_Initialize(country);
	node = Hash_Insert_Node(hd->Country, node);
	return node;
}

Node* InsertAge(char* age, Hash_Data* hd)
{
	// printf("AGE is %s\n", age);
	Node* node = Node_Initialize(age);
	node = Hash_Insert_Node(hd->Age, node);
	return node;
}

Node* InsertVirus(char* virus, Hash_Data* hd)
{
	// check if virus already in hash
	Node* node = Hash_Search(hd->Virus, virus);
	//if not, make node to insert
	if(node == NULL)
	{
		// printf("VIRUS NOT IN HASH\n" );
		node = Node_Initialize(virus);

		// initialization of skip lists and bloom filter for node...
		Skip_List* sl1 = Skip_List_Initialize(MAXLEVEL);
		Skip_List* sl2 = Skip_List_Initialize(MAXLEVEL);

		node->vaccinated_persons = sl1;
		node->not_vaccinated_persons = sl2;

		node = Hash_Insert_Node(hd->Virus, node);
	}
	return node;
}

Node* InsertCondition(char* condition, Hash_Data* hd)
{
	// printf("ID is %s\n", condition);
	// if((strcmp(condition, "YES")) == 0);

}

Node* InsertDate(char* date, Hash_Data* hd)
{
	// printf("DATE is %s\n", date);
	Node* node = Node_Initialize(date);
	node = Hash_Insert_Node(hd->Date, node);
	return node;
}

// ------------------PRINT FUNCTIONS ------------------
void Hash_Data_Print(Hash_Data* hd)
{
	Hash_Print(hd->Id);
	Hash_Print(hd->Age);
	Hash_Print(hd->Name);
	Hash_Print(hd->Surname);
	Hash_Print(hd->Country);
	Hash_Print(hd->Virus);
	Hash_Print(hd->Date);

}

// ------------------DELETION FUNCTIONS------------------
void Hash_Data_Delete(Hash_Data* hd)
{
	Hash_Delete(hd->Virus);
	Hash_Delete(hd->Date);
	Hash_Delete(hd->Id);
	Hash_Delete(hd->Age);
	Hash_Delete(hd->Name);
	Hash_Delete(hd->Surname);
	Hash_Delete(hd->Country);

	free(hd);
}
