#!/bin/sh
#princ dirass D H

case $# in
0|1|2) 	echo Errore: Usage is $0 dirass D H
	exit 1;;
3) ;;
*) echo TROPPI PARAMETRI
   exit 1;;
esac

#Controllo primo parametro
case $1 in
/*) if test ! -d $1 -o ! -x $1
    then
    echo $1 non direttorio
    exit 2
    fi;;
*)  echo $1 non nome assoluto
    exit 3;;
esac

#Controllo secondo parametro
case $2 in
*/*) echo $2 NON relativo
     exit 4;;
esac

#Controllo terzo parametro
expr $3 + 0  > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -a $N1 -ne 3 
then echo numerico $3 #siamo sicuri che numerico
     if test $3 -le 0 
     then echo $3 non positivo
       	  exit 4
     fi
else
  echo $3 non numerico
  exit 5
fi

PATH=`pwd`:$PATH
export PATH
esplora.sh $*


