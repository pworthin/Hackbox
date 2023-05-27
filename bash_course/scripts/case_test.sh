#!/bin/bash

select city in Tokyo Kyoto Paris Toulousse Rome Venice Berlin Frankfurt;
do
case "$city" in 
	Tokyo|Kyoto) country="Japan";;
	Paris|Toulousse) country="France";;
	Rome|Venice) country="Italy";;
	Berlin|Frankfurt) country="Germany";;
	*) echo "Invalid city"; break;;
	
esac
echo "$city is in $country"
break
done
