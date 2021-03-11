#bash script

echo $1 $2 $3 $4 $5 $6
virusfile=$1.txt
countriesfile=$2.txt
fnamesfile=$3.txt
lastnamesfile=$4.txt
filelength=$5
duplicates=$6

# create viruses, female names, male names, last names, countries arrays
declare -a viruses_array

while IFS= read -r line; do
		viruses_array+=($line)
done < $virusfile

# for index in "${viruses_array[@]}"; do echo "$index"; done

# create first names array
declare -a fnames_array

while IFS= read -r line; do
		fnames_array+=($line)
done < $fnamesfile

# for index in "${mnames_array[@]}"; do echo "$index"; done

# create last names array
declare -a lnames_array

while IFS= read -r line; do
		lnames_array+=($line)
done < $lastnamesfile

# for index in "${lnames_array[@]}"; do echo "$index"; done

# create female names array
declare -a countries_array

while IFS= read -r line; do
		countries_array+=($line)
done < $countriesfile

# for index in "${countries_array[@]}"; do echo "$index"; done

# create new data file

for (( c=1; c<=filelength; c++ ))
do

	citizenId=$((0 + $RANDOM % 9999))
	# if(($duplicates == 0))
	# then
	# 	while((citizenId | awk '!($0 in seen){seen[$0];next} 1'))
	# 	do
	# 			citizenId=$((0 + $RANDOM % 9999))
	# 	done
	# fi

	len=${#viruses_array[@]}
	randnum=$((0 + $RANDOM % len-1))
	virus=${viruses_array[randnum]}

	len=${#fnames_array[@]}
	randnum=$((0 + $RANDOM % len-1))
	firstname=${fnames_array[randnum]}

	len=${#lnames_array[@]}
	randnum=$((0 + $RANDOM % len-1))
	lastname=${lnames_array[randnum]}

	len=${#countries_array[@]}
	randnum=$((0 + $RANDOM % len-1))
	country=${countries_array[randnum]}

	age=$((1 + $RANDOM % 120))

	printf "$citizenId $virus $firstname $lastname $country $age\n">> Data.txt
done
