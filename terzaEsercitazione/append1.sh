#!/bin/sh
#file comandi append1.sh invocazione append dadove sucosa
scrivibile= 		#la variabile scrivibile ci dira' se $2 e' scrivibile e se non esiste se e' scrivibile la directory corrente
#controllo stretto sul numero di parametri
case $# in	#controlliamo il numero di parametri
2) echo Numero di par ok $#;; 	#caso giusto 
*) echo uso: append dadove sucosa
   exit 1;;
esac
#controllo su nomi relativi
#poiche' i parametri sono piu' di 1 e il controllo che dobbiamo fare su entrambi e' lo stesso, possiamo usare un for i che implicitamente vuole dire for i in $#
for i 
do
	case $i in
   	*/*) echo $i non nome relativo semplice; exit 2 ;;
   	*) echo OK nome $i ;;
   	esac
done
#controllo su esistenza file $1 e leggibilita'
if test -f $1 -a -r $1
then
	#controllo esistenza file $2
	if test -f $2
 	then		
		#controllo se scrivibile
		if test -w $2
		then scrivibile=true
		else echo file $2 non scrivibile; exit 3
  		fi
   	else #il file non esiste e quindi dobbiamo verificare di avere il diritto di scrittura sulla dir corrente e nel caso settiamo scrivibile a true
   		if test -w .
   		then scrivibile=true
		else echo directory corrente non scrivibile; exit 4
   		fi
   	fi
   	if test $scrivibile = true
   	then	
		cat < $1 >> $2
   	fi
else
echo $1 non esiste o non leggibile; exit 5
fi
