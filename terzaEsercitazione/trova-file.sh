#!/bin/sh
#file comandi trova-file.sh 
case $# in 	#controlliamo che sia invocato con un solo parametro
1) echo OK un solo parametro $1 ;;
*) echo echo $# non numero giusto di parametri! Uso: $0 file
   exit 1;;
esac
#controllo che il primo parametro sia dato in forma relativa semplice 
case $1 in
*/*) echo Errore: $1 non in forma relativa semplice 	#notare che questa forma di controllo ingloba anche il caso /*
     exit 2;;
*) ;;
esac
if test -f $1 	#controlliamo che sia un file
then
	echo il file $1 si trova in `pwd`
	echo il suo nome assoluto e\' `pwd`/$1
else 	echo non esiste un file di nome $1 in `pwd`
fi
