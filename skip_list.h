#ifndef SKIP_LIST_H_
#define SKIP_LIST_H_

#include "entries.h"
// // #include "dataprocessing.h"

#define MAXLEVEL 3

// struct Entry;

// skip list's node
typedef struct SLNode
{
	int levels;
	Entry* entry;
	// struct SLNode* next;
	struct SLNode** level_array;
}SLNode;

typedef struct Skip_List
{
	int levels;
	int count;	//how many nodes it countains
	// SLNode* first;
	SLNode** level_array;
}Skip_List;

SLNode** Level_Array_Initialize(int levels);


SLNode* SLNode_Initialize(Entry* entry, int maxlevels);

Skip_List* Skip_List_Initialize(int maxlevels);

SLNode* SLNode_Search_List(SLNode* first_level_node, int level, char* id);
SLNode* Skip_List_Search_Entry(Skip_List* sl, char* id);

Skip_List* SLNode_Insert_to_List(Skip_List* sl, int level, SLNode* new_node);

Skip_List* Skip_List_Insert_SLNode(Skip_List* sl, SLNode* new_node);
void Skip_List_Print(Skip_List* sl);
void Skip_List_Print_Entries(Skip_List* sl);


SLNode* Skip_List_Remove_Node(SLNode* slnode, Skip_List* sl);

void SLNode_Delete(SLNode* slnode);
void Skip_List_Delete(Skip_List* sl);

#endif
