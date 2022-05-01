#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define PERM 0644	/* in ottale per diritti UNIX */

int appendfile (char *f1)
{  
   int outfile, nread;		/* usati per open/creat e valore ritorno read */		
   char buffer[BUFSIZ];	/* buffer per la lettura/scrittura */

	if ( (outfile = open (f1, O_WRONLY | O_APPEND | O_CREAT, PERM)) < 0 )
	/* apertura in append o creazione */
   			return 1;	

	while (( nread = read (0, buffer, BUFSIZ)) > 0 )
	/* si legge dallo standard input */
	{   
		if ( write (outfile, buffer, nread ) < nread ) 
		{ 
			close (outfile); 
			return 2; /* errore */ 
		}
	}	/* fine del file di input */

	close (outfile); 
	return 0;
}	

int main (int argc, char ** argv)
{   int status; 	/*variabile per memorizzare il valore di ritorno della funrione invocata */

	if (argc != 2) /* controllo sul numero di parametri: deve essere esattamente 1! */
    	{       printf("Errore: numero di argomenti sbagliato dato che argc = %d\n", argc);
        	printf("Ci vuole 1 argomento: nome-file\n");
            	exit (1);
    	}
    	status = appendfile (argv[1]);   /* invochiamo la funzione appenfile passando la stringa passata come parametro */
    	if (status != 0)
        	printf("Ci sono stati degli errori durante l'append dato che status = %d\n", status);

    	exit(status);
}
