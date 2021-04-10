#include "dataprocessing.h"
#include "skip_list.h"

typedef struct countByAge
{
	int age1, age2, age3, age4;
}countByAge;

Skip_List* Virus_Vaccinated_persons_List(char* virus, Hash_Data* Hd);

Skip_List* Virus_Not_Vaccinated_persons_List(char* virus, Hash_Data* Hd);

int VaccineStatusBloom(char* citizenId, char* virusName, Hash_Data* Hd);

int VaccineStatus(char* citizenId, char* virusName, Hash_Data* Hd);

void populationStatus(char* country, char* virusName , char* date1, char* date2, Hash_Data* Hd);

void popStatusByAge(char* country, char* virusName, char* date1, char* date2, Hash_Data* Hd);

void insertCitizenRecord(char* citizenId, char* firstName, char* lastName, char* country, char* age, char* virusName, char* status, char* date, Hash_Data* Hd);

int Compare_Date(char* date1, char* date2);
