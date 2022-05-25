#!/bin/sh
#esplora dirass D H

cd $1
#definiamo una variabile per memorizzare i nomi dei file 
file=

if test -d $2
then
	cd $2
	for i in *
	do
	if test -f $i -a -r $i
	then
	  nl=`wc -l < $i` 
	  if test $nl -eq $3 
	  then 
	  file="$file $i" 
	  fi
	fi
	done
	cd ..
fi

if test $file
then
  echo TROVATO DIRETTORIO `pwd`/$2
  echo CHIAMO PARTE C 
  partec $file $3  
fi

for i in *
do
if test -d $i -a -x $i
then
  echo RICORSIONE in `pwd`/$i 
  esplora.sh `pwd`/$i $2 $3
fi
done
