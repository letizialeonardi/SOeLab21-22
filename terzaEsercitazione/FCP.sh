#!/bin/sh
#file comandi FCP.sh
case $# in
1) echo Numero giusto parametri: $#;;
*) echo $# non corretto: ci vuole SOLO il nome assoluto di una directory
   exit 1;;
esac

case $1 in
/*) if test ! -d $1 -o ! -x $1	#se nome assoluto, controlliamo che sia una dir traversabile
    then
	echo $1 directory non esistente o non traversabile
	exit 2
    fi;;
*) echo $1 non nome assoluto	#non e' nome assoluto
   exit 3;;
esac

#invochiamo il secondo file comandi: se non si ha il percorso '.' nella variabile PATH chiaramente bisogna invocare con ./file-dir.sh $1
file-dir.sh $1 
