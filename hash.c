#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "hash.h"
#include "skip_list.h"
#include "bloom_filter.h"


int Hash_Function(char* Data_num)
{
	int max = strlen(Data_num) - 1;	// - the terminating char
	int temp = 0;
	int sum = 1;
	int extra = 0;
	int count = 0;

	// int max = 13;
	if(max>13)
		max=13;
	while(temp < max){
		count++;
		sum += (Data_num[temp] - '0' + extra) * pow(3, count);
		extra += (Data_num[temp] - '0') % (temp+1);
		temp++;
	}
	if(sum < 0)
		sum = sum * -1;
	// printf("sum is %d\n", sum);
	return sum % HASHSIZE;
}

// ------------------INITIALIZATION FUNCTIONS------------------

Node* Node_Initialize(char* entry)
{
	Node* node = malloc(sizeof(Node));
	node->data = strdup(entry);

	node->next = NULL;
	node->prev = NULL;
	// ====================================
	node->bloom_filter = NULL;
	// ====================================
	node->vaccinated_persons = NULL;
	node->not_vaccinated_persons = NULL;
	// ====================================
	node->total_count = 0;
	// ================== used only from and for question-functions
	node->vaccinated_count = 0;
	node->data4questions = NULL;
	// node->not_vaccinated_count = 0;
	return node;
}

Bucket* Bucket_Initialize()
{
	Bucket* bucket = malloc(sizeof(Bucket));
	bucket->node_count = 0;
	bucket->first = NULL;
	bucket->last = NULL;
	return bucket;
}

Hash* Hash_Initialize()
{
	Hash* hash = malloc(sizeof(Hash));
	for(int i=0; i<HASHSIZE; i++)
	{
		hash->harray[i] = Bucket_Initialize();
	}
	hash->total_count = 0;
	return hash;
}

// ------------------INSERTTION FUNCTIONS------------------

Bucket* Bucket_Insert_Node(Bucket* B, Node* node)
{
	node->bucket = B;
	if(B->first == NULL)	// insert fisrt
		B->first = node;
	else	//insert at the end of coll list
	{
		node->prev = B->last;
		B->last->next = node;
	}
	B->last = node;
	B->node_count++;
	// Bucket_Print(B);
	return B;
}

Node* Hash_Insert_Node(Hash* H, Node* node)
{
	Node* nptr = Hash_Search(H, node->data);
	if(nptr == NULL)
	{
		node->total_count++;
		int h = Hash_Function(node->data);
		H->harray[h] = Bucket_Insert_Node(H->harray[h], node);
		H->total_count++;
		return node;
	}
	else
	{
		nptr->total_count++;
		Node_Delete(node);
		return nptr;
	}

}

//------------------ SEARCH FUNCTIONS------------------

Node* Hash_Search(Hash* H, char* entry)
{
	int h = Hash_Function(entry);
	Bucket* B = H->harray[h];
	Node* pptr = B->first;
	//search for Data to discard
	while (pptr != NULL)
	{
		if(strcmp(entry, pptr->data) == 0)	//if Data num is found
			return pptr;
		pptr = pptr->next;
	}
	// if Data numer doesnt exist in hash
	return NULL;
}

// ------------------DISCARDING FUNCTIONS------------------

Hash* Hash_Discard_Node(Hash* H, Node* node)
{
	if(node->prev == NULL)	// discard first Data of Bucket
	{
			node->bucket->first = node->next;
			if(node->next != NULL)	//if it has next Data, make it null
				node->next->prev = NULL;
	}
	else if(node->bucket->last == node)	//discard lat Data of bucket (with >1 entries)
	{
			node->bucket->last = node->prev;
			node->prev->next = NULL;	//it will have a prev cause its not the first of the bucket
	}
	else	//discard Data from inside the coll list
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}
	node->bucket->node_count--;
	H->total_count--;

	return H;
}

// ------------------PRINT FUNCTIONS ------------------

void Node_Print(Node* node)
{
	printf("Entryr:\t%s\n", node->data);
	printf("-----------------------\n");
}

void Bucket_Print(Bucket* B)
{
	Node* pptr = B->first;
	while (pptr != NULL)
	{
		Node_Print(pptr);
		pptr = pptr->next;
	}
}

void Bucket_Print_Data_Count(Bucket* B)
{
	printf("Data Count:\t%d\n", B->node_count);
}

void Hash_Print(Hash* H)
{
	for(int i=0; i<HASHSIZE; i++)
	{
		// printf("Bucket %d ----> ", i);
		// Bucket_Print_Data_Count(H->harray[i]);
		Bucket_Print(H->harray[i]);
	}
	printf("\n");
}

void Hash_Print_Status(Hash* H)
{
	printf("Total Datas in Hash:\t%d\n", H->total_count);
}

// ------------------DELETION FUNCTIONS------------------

void Node_Delete(Node* node)
{
	free(node->data);
	// free(node->data);
	// check because this can be used to delete a node created
	// in oreder to be inserted to hash, but the entry already existed
	// so, it wont have skip lists yet
	// but also if not for Virus hash, skip lists are not initialized so theyre null
	if(node->vaccinated_persons != NULL)
		Skip_List_Delete(node->vaccinated_persons);
	if(node->not_vaccinated_persons != NULL)
		Skip_List_Delete(node->not_vaccinated_persons);
	if(node->bloom_filter != NULL)
		Bloom_Filter_Delete(node->bloom_filter);
	// all data that could be contained in here, have been freed in sl_del
	// if you uncomment --> double free
	// if(node->data4questions != NULL)
	// 	free(node->data4questions);
	free(node);
}

void Bucket_Delete(Bucket* B)
{
	Node* pptr = B->first;
	Node* pnext;
	while (pptr != NULL)
	{
		pnext = pptr->next;
		Node_Delete(pptr);
		pptr = pnext;
	}
	free(B);
}

void Hash_Delete(Hash* H)
{
	for(int i=0; i<HASHSIZE; i++)
	{
		Bucket_Delete(H->harray[i]);
	}
	free(H);
}
