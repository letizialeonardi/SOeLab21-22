#!/bin/sh
#file comandi for4Ter.sh
#supponiamo che in un file (il cui nome relativo semplice verra' passato come parametro) siano stati memorizzati i nomi di alcuni file insieme con la loro lunghezza in linee

#controllo sul numero di parametri
case $# in
	1) #controlliamo sia un nome relativo semplice
	   case $1 in
		   */*) echo ERRORE non nome relativo semplice
			exit 2;;
		   *)   echo il nome del file passato = $1;;
	   esac;;
	*) echo ERRORE il numero dei parametri deve essere 1 e non $#
	   exit 1;;
esac
c=0	#variabile che ci serve all'interno del for per capire se siamo su un elemento dispari (nome file) o su un elemento pari (numero di linee)
for i in `cat $1`	 #la lista usata nel for corrisponde al contenuto di un file!
do
 	c=`expr $c + 1` 	#ad ogni iterazione incrementiamo il contatore
        if test `expr $c % 2` -eq 1    	#se il contatore e' dispari
	then 
		echo elemento dispari, quindi nome di file $i
		if test -f $i		#controlliamo sia il nome di un file
		then cat $i		#e se si' lo visualizziamo
		fi
        else 				#se il contatore e' pari
		echo elemento pari, quindi lunghezza in linee del file appena visualizzato = $i 
        fi
done
