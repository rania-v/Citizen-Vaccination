#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dataprocessing.h"
#include "questions.h"
// #include "bloom_filter.h"
#include "entries.h"
#include "hash.h"
#include "skip_list.h"

int main(int argc, char const *argv[]) {

	struct tm *tm;
	time_t t;
	char str_date[100];
	t = time(NULL);
	tm = localtime(&t);
	strftime(str_date, sizeof(str_date), "%d %m %Y", tm);

	srand((unsigned) time(0));

// error checking in arguments
	if(strcmp(argv[1], "-c"))
	{
		printf("Error in arguments\n");
		return 0;
	}
	if(strcmp(argv[3], "-b"))
	{
		printf("Error in arguments\n");
		return 0;
	}
	const char* filename = argv[2];
	int bloomSize = 0;
	sscanf(argv[4], "%d", &bloomSize);

	Hash_Data *Hd = Hash_Data_Iniitalise(bloomSize);

	DataProcessing(filename, Hd);

	// Hash_Data_Print(Hd);
	// Virus_Vaccinated_persons_List("Ebola", Hd);
	VaccineStatusBloom("2048", "Measles", Hd);
	VaccineStatus("2048", "Measles", Hd);
	// populationStatus("Micronesia", "Human_papillomavirus", "2-2-1800", "1-1-2022", Hd);
	// popStatusByAge("Cuba", "Junin", "2-2-1800", "1-1-2022", Hd);

	insertCitizenRecord("2953", "Zara", "Arsenault", "Myanmar", "18", "Junin", "YES", "9-9-2009", Hd);

	// Virus_Not_Vaccinated_persons_List("Junin", Hd);

	char buff[200], question[50], *word, citizenId[5], virusName[30], date1[10], date2[10], country[20], firstName[20], lastName[20], age[4], status[4];
	while (1)
	{
		fgets(buff, sizeof(buff), stdin);
		sscanf(buff, "%s ", question);

		if(strcmp(question, "vaccineStatusBloom") == 0)
		{
			sscanf(buff, "%s %s %s", question, citizenId, virusName);
			VaccineStatusBloom(citizenId, virusName, Hd);

		}
		else if(strcmp(question, "vaccineStatus") == 0)
		{
			sscanf(buff, "%s %s %s", question, citizenId, virusName);
			VaccineStatus(citizenId, virusName, Hd);

		}
		else if(strcmp(question, "populationStatus") == 0)
		{
			// populationStatus Micronesia Human_papillomavirus 2-2-1800 1-1-2022
			char *cptr = strdup(buff);
			word = strtok(cptr, " ");
			int wordcount = 0;
			// read each word and insert to hash and entry
			while (word != NULL)
			{
				wordcount++;
				word = strtok(NULL, " \n");
			}
			free(cptr);

			if(wordcount == 5)
			{
				sscanf(buff, "%s %s %s %s %s", question, country, virusName, date1, date2);
				popStatusByAge(country, virusName, date1, date2, Hd);
			}
			else if(wordcount == 4)
			{
				sscanf(buff, "%s %s %s %s", question, virusName, date1, date2);
				popStatusByAge(NULL, virusName, date1, date2, Hd);
			}
			else
				printf("Invalid question\n");
		}
		else if(strcmp(question, "popStatusByAge") == 0)
		{
			// popStatusByAge Cuba Junin 2-2-1800 1-1-2022
			char *cptr = strdup(buff);
			word = strtok(cptr, " ");
			int wordcount = 0;
			// read each word and insert to hash and entry
			while (word != NULL)
			{
				wordcount++;
				word = strtok(NULL, " \n");
			}
			free(cptr);

			if(wordcount == 5)
			{
				sscanf(buff, "%s %s %s %s %s", question, country, virusName, date1, date2);
				popStatusByAge(country, virusName, date1, date2, Hd);
			}
			else if(wordcount == 4)
			{
				sscanf(buff, "%s %s %s %s", question, virusName, date1, date2);
				popStatusByAge(NULL, virusName, date1, date2, Hd);
			}
			else
				printf("Invalid question\n");
		}
		else if(strcmp(question, "insertCitizenRecord") == 0)
		{
			// insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date]
			sscanf(buff, "%s %s %s %s %s %s %s %s %s", question, citizenId, firstName, lastName, country, age, virusName, status, date1);
			if(strcmp(status, "NO"))
				insertCitizenRecord(citizenId, firstName, lastName, country, age, virusName, status, NULL, Hd);
			else
				insertCitizenRecord(citizenId, firstName, lastName, country, age, virusName, status, date1, Hd);

		}
		else if(strcmp(question, "vaccinateNow") == 0)
		{
			sscanf(buff, "%s %s %s %s %s %s %s", question, citizenId, firstName, lastName, country, age, virusName);
			insertCitizenRecord(citizenId, firstName, lastName, country, age, virusName, "YES", str_date, Hd);

			// insertCitizenRecord(citizenId, firstName, lastName, country, age, virusName, "YES", date1, Hd);
		}
		else if(strcmp(question, "list-nonVaccinated-Persons") == 0)
		{
			sscanf(buff, "%s %s", question, virusName);

			Virus_Not_Vaccinated_persons_List(virusName, Hd);
		}
		else if(strcmp(question, "exit") == 0)
		{
			Hash_Data_Delete(Hd);
			return 0;
		}
	}


	return 0;
}
