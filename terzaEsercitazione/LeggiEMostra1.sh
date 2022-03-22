#!/bin/sh
#file comandi LeggiEMmostra1.sh
#CONTROLLO SU PRESENZA DI ALMENO UN PARAMETRO (uso di case)
case $# in
0) echo $# non numero giusto di parametri! Uso: $0 file1 [file2 file3 etc.]
   exit 1;;
*) ;;
esac

for i 	#eseguiamo per tutti i parametri di invocazione
do
	#controlliamo che il parametro corrente sia un file e sia leggibile, altrimenti errore!
	if test ! -f $1 -o ! -r $1
	then 	echo Errore: $1 non file oppure non leggibile
		exit 2
	fi
	#chiediamo all'utente se vuole visualizzare il contenuto del file: N.B. forziamo l'ouput sul terminale corrente per non avere problemi se si usasse la ridirezione dello standard output
	echo -n "Vuoi visualizzare $i ? " > /dev/tty
	#leggiamo la risposta dell'utente
	read risposta
	#usiamo in questo caso un case per vincolare meno l'utente nelle risposte!
	case $risposta in
	S* | s* | Y* | y*) ls -la $i; cat $i;;
	*) echo niente stampa di $i;;
	esac
done
