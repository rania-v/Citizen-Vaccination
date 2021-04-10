#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "questions.h"
#include "skip_list.h"


Skip_List* Virus_Vaccinated_persons_List(char* virus, Hash_Data* Hd)
{
	Node* virus_node = Hash_Search(Hd->Virus, virus);
	if(virus_node == NULL)
	{	printf("VIRUS DOES NOT EXIST IN DATABASE\n"); return NULL;}

	Skip_List_Print(virus_node->vaccinated_persons);
	return virus_node->vaccinated_persons;
}

Skip_List* Virus_Not_Vaccinated_persons_List(char* virus, Hash_Data* Hd)
{
	Node* virus_node = Hash_Search(Hd->Virus, virus);
	if(virus_node == NULL)
	{	printf("VIRUS DOES NOT EXIST IN DATABASE\n"); return NULL;}

	Skip_List_Print_Entries(virus_node->not_vaccinated_persons);
	return virus_node->not_vaccinated_persons;
}

int VaccineStatusBloom(char* citizenId, char* virusName, Hash_Data* Hd)
{
	Node *virus_in_hash ;
	//search for virusName in virus_in_hash
	virus_in_hash = Hash_Search(Hd->Virus, virusName);
	if(virus_in_hash == NULL)
	{
		return -1;
			// printf("VIRUS HASH IS NULL\n");
	}

	if(Bloom_Filter_Search(virus_in_hash->bloom_filter, citizenId))
		printf("MAYBE\n");
	else
		printf("NOT VACCINATED\n");
}

int VaccineStatus(char* citizenId, char* virusName, Hash_Data* Hd)
{
	SLNode* slnodeptr;
	Node* pptr, *virus_in_hash ;
	if(virusName != NULL)
	{
		//search for virusName in virus_in_hash
		virus_in_hash = Hash_Search(Hd->Virus, virusName);
		if(virus_in_hash == NULL)
		{
			return -1;
				// printf("VIRUS HASH IS NULL\n");
		}

		slnodeptr = Skip_List_Search_Entry(virus_in_hash->vaccinated_persons, citizenId);

		if(slnodeptr != NULL)
		{
			int pos = Entry_Get_Virus_position(slnodeptr->entry, virusName);
			printf("Citizen %s vaccinated on %s \n", citizenId, Entry_Get_Date(slnodeptr->entry, pos));
			return 1;
		}
		else
		{	printf("NOT VACCINATED\n"); return 0;}
	}
	else
	{
		// for each bucket in hash
		for(int i=0; i<HASHSIZE; i++)
		{
			virus_in_hash = Hd->Virus->harray[i]->first;

			// for each node in coll_list of bucket
			while (virus_in_hash != NULL)
			{
				slnodeptr = Skip_List_Search_Entry(virus_in_hash->vaccinated_persons, citizenId);

				// checking in vaccinated skip list
				if(slnodeptr != NULL)
				{
					int pos = Entry_Get_Virus_position(slnodeptr->entry, virusName);
					printf("Citizen %s vaccinated for %s on %s\n", citizenId, virusName, Entry_Get_Date(slnodeptr->entry, pos));
				}
				// checking in not vaccinated skip list
				slnodeptr = Skip_List_Search_Entry(virus_in_hash->not_vaccinated_persons, citizenId);

				if(slnodeptr != NULL)
				{
					// int pos = Entry_Get_Virus_position(slnodeptr->entry, virusName);
					printf("Citizen %s not vaccinated for %s\n", citizenId, virusName);
				}
				virus_in_hash = virus_in_hash->next;
			}
		}
	}
}

int Compare_Date(char* date1, char* date2)
{
	// returns 2 if(date1 > date2)
	//				1	if(date1 == date2)
	// 				0 if(date1 < date 2)
	int d1, d2, m1, m2, y1, y2;
	sscanf(date1, "%d-%d-%d", &d1, &m1, &y1);
	// =============
	sscanf(date2, "%d-%d-%d", &d2, &m2, &y2);
	if(y1 > y2)
		return 2;
	else if(y1 < y2)
		return 0;
	else if(m1 > m2)	//same year
		return 2;
	else if(m1 < m2)
		return 0;
	else	if(d1 > d2)	//same month
		return 2;
	else if(d1 < d2)
		return 0;

	return 1;
}


void populationStatus(char* country, char* virusName , char* date1, char* date2, Hash_Data* Hd)
{
	float perc;
	int searchid, entry_id, vaccinated_count = 0, total_country_population, pos;
	SLNode* slnodeptr, *slnext;
	Node* virus_in_hash, *country_in_hash;
	// search for virusName in virus_in_hash
	// printf("%s\n", virusName);
	virus_in_hash = Hash_Search(Hd->Virus, virusName);
	if(virus_in_hash == NULL)
	{
		return;
		// printf("VIRUS HASH IS NULL\n");
	}


	if(country == NULL)
	{
		//traverse level 0 (the whole list)
		slnodeptr = virus_in_hash->vaccinated_persons->level_array[0];
		while(slnodeptr != NULL)
		{
			pos = Entry_Get_Virus_position(slnodeptr->entry, virusName);
			// check if fits date criteria
			if(Compare_Date(Entry_Get_Date(slnodeptr->entry, pos), date1))
				if(Compare_Date(date2, Entry_Get_Date(slnodeptr->entry, pos)))
				{
					//search country in hash, and update count data
					country_in_hash = Hash_Search(Hd->Country, Entry_Get_Country(slnodeptr->entry));
					country_in_hash-> vaccinated_count++;
				}
			slnodeptr = slnodeptr->level_array[0];
		}
		// for every country print vaccinated population data
		for(int i=0; i<HASHSIZE; i++)
		{
			country_in_hash = Hd->Country->harray[i]->first;
			// for each node in coll_list of bucket
			while (country_in_hash != NULL)
			{
				total_country_population = country_in_hash->total_count;
				perc = (float)country_in_hash->vaccinated_count / total_country_population * 100.0;
				// printf("%d\n", total_country_population);
				printf("%s		%d	%.2f%%\n", country_in_hash->data, country_in_hash->vaccinated_count, perc);
				// return counter in initial state
				country_in_hash->vaccinated_count = 0;
				country_in_hash = country_in_hash->next;
			}
		}
	}
	else
	{
		// search level 0 (the whole list), for nodes with wanted country
		slnodeptr = virus_in_hash->vaccinated_persons->level_array[0];
		while(slnodeptr != NULL)
		{
			pos = Entry_Get_Virus_position(slnodeptr->entry, virusName);

			// check if same country
			if(strcmp(Entry_Get_Country(slnodeptr->entry), country) == 0)
			{
				// check if fits date criteria
				if(Compare_Date(Entry_Get_Date(slnodeptr->entry, pos), date1))
					if(Compare_Date(date2, Entry_Get_Date(slnodeptr->entry, pos)))
						//add to counter
						vaccinated_count++;
			}
			slnodeptr = slnodeptr->level_array[0];
		}
		country_in_hash = Hash_Search(Hd->Country, country);
		if(country_in_hash == NULL)
		{
			return ;
			// printf("NO COUNTRY IN HD\n");
		}
		// print results
		int total_country_population = country_in_hash->total_count;
		float perc = (float)vaccinated_count / total_country_population * 100.0;
		printf("%d\n", total_country_population);
		printf("%s\t%d\t%.2f%%\n", country, vaccinated_count, perc);

	}
}

void popStatusByAge(char* country, char* virusName, char* date1, char* date2, Hash_Data* Hd)
{
	float perc;
	//age1 -> vaccinated count for 0-20
	//age2 ->       ''				 for 20-40
	//age3 -> 			''				 for 40-60
	//age4 -> 			''				 for 60+
	int searchid, entry_id, total_country_population, age1=0, age2=0, age3=0, age4=0, age, pos;
	SLNode* slnodeptr, *slnext;
	Node* virus_in_hash, *country_in_hash;
	countByAge* country_CountByAge;

	// search virus in data hash -> virus
	virus_in_hash = Hash_Search(Hd->Virus, virusName);
	if(virus_in_hash == NULL)
	{
		return;
		// printf("VIRUS HASH IS NULL\n");
	}


	if(country == NULL)
	{
		//traverse level 0 (the whole list)
		slnodeptr = virus_in_hash->vaccinated_persons->level_array[0];
		while(slnodeptr != NULL)
		{
			pos = Entry_Get_Virus_position(slnodeptr->entry, virusName);

			// check if fits date criteria
			if(Compare_Date(Entry_Get_Date(slnodeptr->entry, pos), date1))
				if(Compare_Date(date2, Entry_Get_Date(slnodeptr->entry, pos)))
				{
					//search country in hash, and update count data
					country_in_hash = Hash_Search(Hd->Country, Entry_Get_Country(slnodeptr->entry));
					country_CountByAge = ((countByAge*)country_in_hash->data4questions);
					if(country_CountByAge == NULL)
					{
						// initialize
						country_CountByAge = malloc(sizeof(countByAge));
						country_CountByAge->age1 = 0;
						country_CountByAge->age2 = 0;
						country_CountByAge->age3 = 0;
						country_CountByAge->age4 = 0;
					}
					// update age groups counts
					sscanf(Entry_Get_Age(slnodeptr->entry), "%d", &age);
					if(age < 20)
						country_CountByAge->age1++;
					else if (age < 40)
						country_CountByAge->age2++;
					else if(age < 60)
						country_CountByAge->age3++;
					else
						country_CountByAge->age4++;

					country_in_hash->data4questions = country_CountByAge;
				}
			// go to next node in Virus->vaccinated_persons level 0
			slnodeptr = slnodeptr->level_array[0];
		}
		// for every country print vaccinated population data
		for(int i=0; i<HASHSIZE; i++)
		{
			country_in_hash = Hd->Country->harray[i]->first;
			// for each node in coll_list of bucket
			while (country_in_hash != NULL)
			{
				country_CountByAge = ((countByAge*)country_in_hash->data4questions);
				total_country_population = country_in_hash->total_count;
				// print vaccinated count and percentage per age group for country
				if(country_CountByAge != NULL)
				{
					printf("%s\n", country_in_hash->data);
					perc = (float)country_CountByAge->age1 / total_country_population * 100.0;
					printf("0-20	%d	%.2f%%\n",country_CountByAge->age1, perc);

					perc = (float)country_CountByAge->age2 / total_country_population * 100.0;
					printf("20-40	%d	%.2f%%\n", country_CountByAge->age2, perc);

					perc = (float)country_CountByAge->age3 / total_country_population * 100.0;
					printf("40-60	%d	%.2f%%\n", country_CountByAge->age3, perc);

					perc = (float)country_CountByAge->age4 / total_country_population * 100.0;
					printf("60+	%d	%.2f%%\n\n", country_CountByAge->age4, perc);

					// return counter in initial state and free
					free(country_CountByAge);
					country_in_hash->data4questions = NULL;
				}
				// next country
				country_in_hash = country_in_hash->next;
			}
		}
	}
	else	//for given country
	{
		// search level 0 (the whole list), for nodes with wanted country
		slnodeptr = virus_in_hash->vaccinated_persons->level_array[0];
		while(slnodeptr != NULL)
		{
			pos = Entry_Get_Virus_position(slnodeptr->entry, virusName);

			// check if same country
			if(strcmp(Entry_Get_Country(slnodeptr->entry), country) == 0)
			{
				// keep pointer to Country object in hash
				country_in_hash = slnodeptr->entry->Country;
				// check if fits date criteria
				if(Compare_Date(Entry_Get_Date(slnodeptr->entry, pos), date1))
					if(Compare_Date(date2, Entry_Get_Date(slnodeptr->entry, pos)))
					{
						// add to the right agr group counter
						sscanf(Entry_Get_Age(slnodeptr->entry), "%d", &age);
						if(age < 20)
							age1++;
						else if (age < 40)
							age2++;
						else if(age < 60)
							age3++;
						else
							age4++;
					}
			}
			slnodeptr = slnodeptr->level_array[0];
		}
		total_country_population = country_in_hash->total_count;
		printf("%s\n", country);
		perc = (float)age1 / total_country_population * 100.0;
		printf("0-20	%d	%.2f%%\n",age1, perc);

		perc = (float)age2 / total_country_population * 100.0;
		printf("20-40	%d	%.2f%%\n", age2, perc);

		perc = (float)age3 / total_country_population * 100.0;
		printf("40-60	%d	%.2f%%\n", age3, perc);

		perc = (float)age4 / total_country_population * 100.0;
		printf("60+	%d	%.2f%%\n\n", age4, perc);
	}
}

void insertCitizenRecord(char* citizenId, char* firstName, char* lastName, char* country, char* age, char* virusName, char* status, char* date, Hash_Data* Hd)
{
	SLNode* slnodeptr;
	Node* vnode, *virus_in_hash, *id_in_hash ;

	//search for virusName in virus_in_hash
	virus_in_hash = Hash_Search(Hd->Virus, virusName);
	if(virus_in_hash == NULL)
	{
		return;
		// printf("VIRUS HASH IS NULL\n");
	}


	slnodeptr = Skip_List_Search_Entry(virus_in_hash->vaccinated_persons, citizenId);

	if(slnodeptr != NULL)
	{
		int pos = Entry_Get_Virus_position(slnodeptr->entry, virusName);
		printf("ERROR: CITIZEN %s ALREADY VACCINATED ON %s \n", citizenId, Entry_Get_Date(slnodeptr->entry, pos));
		return;
	}
	else	//not vaccinated for virus yet
	{
		Entry* entry;
		// search in virus not_vaccinated_persons skip list to delete entry from there if needed
		slnodeptr = Skip_List_Search_Entry(virus_in_hash->not_vaccinated_persons, citizenId);
		// Skip_List_Print(virus_in_hash->not_vaccinated_persons);
		if(slnodeptr == NULL)	//if id not in any skip list, add to entry
		{
			// search for id in id hash, check if it exists and get the entry(id->data4questions) to add virus
			id_in_hash = Hash_Search(Hd->Id, citizenId);
			if(id_in_hash == NULL)
			{	printf("ID HASH IS NULL\n"); return;}
			// Entry_Print(id_in_hash->data4questions);
			entry = ((Entry*)id_in_hash->data4questions);
			// add virus
			Node* vnode = InsertVirus(virusName, Hd);
			id_in_hash->data4questions = Entry_Add_Virus(id_in_hash->data4questions, vnode);
			// add date
			Node* dnode = InsertDate(date, Hd);
			id_in_hash->data4questions = Entry_Add_Date(id_in_hash->data4questions, dnode);
			Entry_Print(id_in_hash->data4questions);

		}
		else	//entry in not_vacc skip list, must delete and add to vacc skip list
		{
			entry = slnodeptr->entry;
			int pos = Entry_Get_Virus_position(slnodeptr->entry, virusName);
			Skip_List_Remove_Node(slnodeptr, slnodeptr->entry->Virus[pos]->not_vaccinated_persons);
		}
		// add to skip list
		SLNode* sl_node = SLNode_Initialize(entry, MAXLEVEL);
		// virus will be last to be inserted in virus array of entry
		int pos = entry->virus_count -1;
		Skip_List_Insert_SLNode(entry->Virus[pos]->vaccinated_persons, sl_node);
		// add to bloom filter
		if(entry->Virus[pos]->bloom_filter == NULL)
			entry->Virus[pos]->bloom_filter = Bloom_Filter_Initialize(Hd->bloom_size);
		entry->Virus[pos]->bloom_filter = Bloom_Filter_Insert_Id(entry->Virus[pos]->bloom_filter, Entry_Get_Id(entry));


	}
}
