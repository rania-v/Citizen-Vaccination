#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
// #include "entries.h"
#include "skip_list.h"
// ------------------INITIALIZATION FUNCTIONS------------------

SLNode** Level_Array_Initialize(int levels)
{
	SLNode** level_array = malloc(levels * sizeof(SLNode*));
	for (int i = 0; i < levels; i++)
	{
		level_array[i] = NULL;
	}
	return level_array;
}

SLNode* SLNode_Initialize(Entry* entry, int maxlevels)
{
	SLNode* slnode = malloc(sizeof(SLNode));

	slnode->levels = rand()%maxlevels+1;
	// printf("RANDOM LEVEL %d\n\n\n", slnode->levels);
	slnode->entry = entry;
	// slnode->next = NULL;
	slnode->level_array = Level_Array_Initialize(slnode->levels);
	return slnode;
}

Skip_List* Skip_List_Initialize(int maxlevel)
{
	Skip_List* sl = malloc(sizeof(Skip_List));

	sl->levels = maxlevel;
	sl->count = 0;
	// create & initialize level array
	sl->level_array = Level_Array_Initialize(maxlevel);

	return sl;
}

//------------------ SEARCH FUNCTIONS------------------


SLNode* Skip_List_Search_Entry(Skip_List* sl, char* id)
{
	int searchid, entry_id;
	SLNode* slnodeptr, *slnext;
	// search top-down all levels
	sscanf(id,"%d", &searchid);
	for (int i = sl->levels-1; i>=0; i--)
	{
		slnodeptr = sl->level_array[i];
		if(slnodeptr != NULL)
		{
			sscanf(Entry_Get_Id(slnodeptr->entry), "%d", &entry_id);
			if(entry_id == searchid)
			{	return slnodeptr;}
		}
		while(slnodeptr != NULL)
		{
			slnext = slnodeptr->level_array[i];
			if(slnext == NULL)
				break;

			sscanf(Entry_Get_Id(slnext->entry), "%d", &entry_id);
			if(entry_id == searchid)
				{	return slnext;}
			// if greater id is found, the id we are searching for does not exist here
			if(entry_id < searchid)	//--> if(entry_id < id)
			{
				break;
			}
			slnodeptr = slnodeptr->level_array[i];
		}
	}
	return NULL;
}



// ------------------INSERTTION FUNCTIONS------------------

Skip_List* Skip_List_Insert_SLNode(Skip_List* sl, SLNode* new_node)
{
	int searchid, entry_id;
	SLNode* slnodeptr, *slnext, **update_array;
	update_array = Level_Array_Initialize(new_node->levels);
	// search top-down all levels
	sscanf(Entry_Get_Id(new_node->entry),"%d", &searchid);
	// printf("SEARCHING FOR %d\n", searchid);
	for (int i = sl->levels-1; i>=0; i--)
	{
		slnodeptr = sl->level_array[i];

		while(slnodeptr != NULL)
		{
			slnext = slnodeptr->level_array[i];
			if(slnext == NULL)
			{
				if(i <= new_node->levels-1)
					update_array[i] = slnodeptr;
				// move to down level
				break;
			}
			sscanf(Entry_Get_Id(slnext->entry), "%d", &entry_id);
			// printf("CHECKING %d\n", entry_id);
			if(entry_id == searchid)
				break;
			// if greater id is found, the id we are searching for does not exist here
			if(entry_id < searchid)	//--> if(entry_id < id)
			{
				// add to update array if needed
				if(i <= new_node->levels-1)
					update_array[i] = slnodeptr;
				// move to down level
				break;
			}
			slnodeptr = slnodeptr->level_array[i];
		}
	}
	// insert
	for(int i=0; i<new_node->levels; i++)
	{
		if(update_array[i] != NULL)
		{
			new_node->level_array[i] = update_array[i]->level_array[i];
			update_array[i]->level_array[i] = new_node;
		}
		else
		{
			new_node->level_array[i] = NULL;
			sl->level_array[i] = new_node;
		}
	}
	// Skip_List_Print(sl);
	free(update_array);
	sl->count++;
	return sl;
}

// ------------------PRINT FUNCTIONS------------------

void Skip_List_Print(Skip_List* sl)
{
	SLNode* slnptr;
	printf("-------* SKIP LIST *-------\n");
	for (int i = sl->levels-1; i>=0; i--)
	{
		printf("i is %d\n", i);
		slnptr = sl->level_array[i];
		while(slnptr != NULL)
		{
			// printf("nananna\n" );
			Entry_Print(slnptr->entry);
			// printf("MPLEKELK\n");
			// printf("%s -> ", slnptr->level_array[i]->entry->Id->data);
			slnptr = slnptr->level_array[i];
		}
	}
}

void Skip_List_Print_Entries(Skip_List* sl)
{
	SLNode* slnptr;
	printf("-------* SKIP LIST *-------\n");
	slnptr = sl->level_array[0];
	while(slnptr != NULL)
	{
		Entry_Print(slnptr->entry);
		slnptr = slnptr->level_array[0];
	}
}

// ------------------REMOVAL FUNCTIONS------------------

SLNode* Skip_List_Remove_Node(SLNode* slnode, Skip_List* sl)
{
	int searchid, entry_id;
	SLNode* slnodeptr, *slnext;
	// get id of node's entry
	sscanf(Entry_Get_Id(slnode->entry),"%d", &searchid);
	// search top-down all node levels
	for (int i = slnode->levels-1; i>=0; i--)
	{
		slnodeptr = sl->level_array[i];
		if(slnodeptr != NULL)//searching 1st node f level array
		{
			sscanf(Entry_Get_Id(slnodeptr->entry), "%d", &entry_id);
			if(entry_id == searchid) //if node is first of level array
			{
				sl->level_array[i] = slnode->level_array[i];
			}
		}
		while(slnodeptr != NULL)
		{
			slnext = slnodeptr->level_array[i];
			if(slnext == NULL)
				break;

			sscanf(Entry_Get_Id(slnext->entry), "%d", &entry_id);
			if(entry_id == searchid)
			{
				slnodeptr->level_array[i] = slnode->level_array[i];
			}
			// if greater id is found, the id we are searching for does not exist here
			if(entry_id < searchid)	//--> if(entry_id < id)
			{
				break;
			}
			slnodeptr = slnodeptr->level_array[i];
		}
	}
	SLNode_Delete(slnode);
	return NULL;
}


// ------------------DELETION FUNCTIONS------------------

// could return ptr to next slnode, use if needed
void SLNode_Delete(SLNode* slnode)
{
	if(slnode->level_array != NULL)
		free(slnode->level_array);
	free(slnode);
}

void Skip_List_Delete(Skip_List* sl)
{
	SLNode* slntemp;
	SLNode* slnode = sl->level_array[0];
	while(slnode != NULL)
	{
		slntemp = slnode->level_array[0];
		if(slnode->entry != NULL)
		{
			// check if entry is part of other skip lists.. that is if entry has any more than 1 virus
			// so reduce counter by one
			// the entry will be deleted by the last viruses skip list deletion
			if(slnode->entry->virus_count == 1)
				slnode->entry = Entry_Delete(slnode->entry);
			else
				slnode->entry->virus_count--;
		}
		SLNode_Delete(slnode);
		slnode = slntemp;
	}
	free(sl->level_array);
	free(sl);
}
