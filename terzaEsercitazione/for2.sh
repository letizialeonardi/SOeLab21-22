#!/bin/sh
#file comandi for2.sh
for i in p*	#la lista del for sara' costituita da tutti i nomi di file/directory il cji inizia per p; se tale lista fosse nulla il for non eseguira' nulla!
do
	echo ELEMENTO CORRENTE: $i	#stampiamo il nome corrente
	if test -f $i			#verifichiamo che sia un file 
	then 
		echo VISUALIZZAZIONE DI $i
		echo ===	#inseriamo delle separazioni per rendere piu' chiara la stampa
		cat $i
		echo ===	#inseriamo delle separazioni per rendere piu' chiara la stampa
	else 
		#si puo' dare per scontato che se non e' un file e' una directory
		echo $i e\' una directory
	fi
done
