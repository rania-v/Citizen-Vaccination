The programm recieves, process, records and answears user questions about cictizen vaccinations.

The citizen data are read and inserted in the structures used for answearing the questions.
Validity checks are performed during the insertion, so that no entries share the same id (citizenId).

After the data insertion, the programm can answear the following questions:

	->whether a citizen is vaccinated for a virus
		->question format: /vaccineStatus citizenId virusName
		->answear format:  NOT VACCINATED / VACCINATED ON 'date'

	->the vaccination status of a citizen for every virus in given data
		->question format: /vaccineStatus citizenId
		->answear format:  
			'virus1' YES 'vaccination date'
			'virus2' NO
			'virus3' YES 'vaccination date'
			...

	->how many people and the population percentage vaccinated for a virus between two dates, in a specific country
		->question format: /populationStatus virusName date1 date2
		->answear format:
			'country1' '#vaccinated citizens' 'vaccinated percentage'
			'country1' '#vaccinated citizens' 'vaccinated percentage'
			...

	->how many people and the population percentage vaccinated for a virus between two dates
		->question format: populationStatus country virusName date1 date2
		->answear format:
			'country' '#vaccinated citizens' 'vaccinated percentage'

	->how many people and the percentage of them vaccinated for a virus between given dates, by age group, for every country
		->question format: /popStatusByAge virusName date1 date2
		->answear format:
			'COUNTRY1' 
			0-20 '#vaccinated citizens' 'vaccinated percentage'
			20-40 '#vaccinated citizens' 'vaccinated percentage'
			40-60 '#vaccinated citizens' 'vaccinated percentage'
			60+ '#vaccinated citizens' 'vaccinated percentage'

			'COUNTRY2'
			...

	->how many people and the percentage of them vaccinated for a virus between given dates, by age group in specific country
		->question format: /popStatusByAge country virusName date1 date2
		->answear format:
			'country' '#vaccinated citizens' 'vaccinated percentage'

We can also update and change the data:

	->insert a new entry
		->format: /insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date]
		->if there is a nother entry with the same id the response is an error message

	->update an entry so that the citizen appears to be vaccinated for a virus
		->format: /vaccinateNow citizenID firstName lastName country age virusName 
		->if the citizen is already vaccinated the response is an error message with the date of vaccination
			if not, a new entry is inserted to the respective skip-list.

	->print every citizen that is not vaccinated for a virus
		->format:  /list-nonVaccinated-Persons virusName

The programm ends when it reads /exit.

For this project, skip-lists (key: citizenId) are used for the data representation and manipulation, for each virus,
one for the vaccinated and one for the non-vaccinated citizens. Every entry that is stored in the skip-lists concists of pointers to the corresponding nodes of hash table buckets. There are hash tables for every field of the entries, in order to minimize data duplication when entries are stored in the skip-lists.

A bash script is included in order to create a proper input file, with the option to create duplicate citizenIds.
