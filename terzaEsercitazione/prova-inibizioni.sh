#!/bin/sh
#File prova-inibizioni.sh
a=ciao 			#assegno alla variabile a la stringa ciao
echo Valore della variabile a = $a, valore della dir corrente `pwd` e valore di ASTERISCO * 	# stampo il contenuto di a, l'output del comando pwd e l'espansione di *
echo SE INIBISCO TUTTE PER ESPANSIONI: '$a `pwd` *' 	# inibisco tutte le espansioni 
echo SE INIBISCO SOLO L\'ULTIMA ESPANSIONE "$a `pwd` *" 	# inibisco solo l'ultima espansione 
#NOTARE CHE SE NELLE STRINGHE STAMPATE CON echo VOGLIAMO USARE l'APOSTROFO DOBBIAMO USARE IL CARATTERE DI ESCAPE!
