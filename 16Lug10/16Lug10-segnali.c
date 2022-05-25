/* versione con segnali inviati da padre ai figli */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

typedef int pipe_t[2];
typedef struct{
        int id; 	/* indice figlio (campo c1 del testo) */
	int num; 	/* numero caratteri (campo c2 del testo) */
		} s_occ;

/* Variabili globali */
char linea[256]; 	/* buffer linea: si puo' supporre che una linea sia lunga al massimo 255 caratteri (256 con il terminatore di linea)  */
int stampate; 		/* numero linee stampate (come variabile globale e' automaticamente inizializzata a 0 */
s_occ cur; 		/* struttura usata dal figlio corrente */

/* funzioni per trattare i segnali SIGUSR1 e SIUSR2: la prima fara' la scrittura su stdout, mentre la seconda non fara' nulla */
void scrivi(int sig)
{
	/* se il padre ha detto che devo scrivere allora si scrive la linea su standard output usando le write su 1 */
	write(1,linea,cur.num);
	stampate++;
}

void salta(int sig)
{
	/* non si deve fare nulla */ 
}

int main (int argc, char **argv)
{
   int N; 				/* numero di file */
   int H; 				/* numero positivo */
   int *pid;				/* array di pid: N.B. In questo caso serve un array di pid per fare in modo che il padre invii i segnali ai figli! */
   pipe_t *pipes;			/* array di pipe usate a pipeline da primo figlio, a secondo figlio .... ultimo figlio e poi a padre: ogni processo legge dalla pipe i-1 e scrive sulla pipe i */
   int i,j; 				/* contatori */
   int fd; 				/* file descriptor */
   int pidFiglio, status, ritorno;	/* per valore di ritorno figli */
   s_occ pip; 				/* struttura usata dal figlio e per la pipe */

	if (argc < 3) /* il testo non specifica nessun vincolo, ma per avere senso l'esecuzione ci deve essere almeno un file e il numero di linee */
	{
		printf("Errore numero di parametri, dato che argc=%d\n", argc);
		exit(1);
	}

	H = atoi(argv[argc-1]);
	printf("Numero di linee %d\n", H);
	if (H <= 0 || H >= 255)  /* il numero deve essere strettamente positivo e strettamente minore di 255 */
	{
		printf("Errore numero linee\n");
		exit(2); 
	}

	N = argc-2;
	printf("DEBUG-Numero di processi da creare %d\n", N);
	/* allocazione pipe */
	if ((pipes=(pipe_t *)malloc(N*sizeof(pipe_t))) == NULL)
	{
		printf("Errore allocazione pipe\n");
		exit(3); 
	}
	/* allocazione pid */
	if ((pid=(int *)malloc(N*sizeof(int))) == NULL)
	{
		printf("Errore allocazione pid\n");
		exit(4);
	}

	/* creazione pipe */
	for (i=0;i<N;i++)
		if(pipe(pipes[i])<0)
		{
			printf("Errore creazione pipe\n");
			exit(5);
		}

	/* padre aggancia le due funzioni (scrivi e salta) che useranno i figli alla ricezione dei segnali inviati dal padre */
	signal(SIGUSR1,scrivi);
	signal(SIGUSR2,salta);

	for (i=0;i<N;i++)
	{
		if ((pid[i]=fork())<0)
		{
			printf("Errore creazione figli\n");
			exit(6);
		}
		if (pid[i]==0)
		{	 /* codice figlio */
			printf("DEBUG-Sono il figlio %d e sono associato al file %s\n", getpid(), argv[i+1]);
			/* chiusura pipes inutilizzate */
			for (j=0;j<N;j++)
			{
				if (j!=i)
					close (pipes[j][1]);
				if ((i == 0) || (j != i-1))
					close (pipes[j][0]);
			}	

			/* inizializzazione struttura */
			cur.id = i;
			cur.num = 0;
			/* apertura file */
			if ((fd=open(argv[i+1],O_RDONLY))<0)
			{
				printf("Impossibile aprire il file %s\n", argv[i+1]);
				/* torniamo -1 che verra' interpreatato come 255 e quindi errore */
				exit(-1);
			}	
			while(read(fd,&linea[cur.num],1)>0)
			{
				/* aggiorno conteggio lunghezza linea */
				cur.num++;
				if (linea[cur.num-1]=='\n')
				{		
					if (i!=0) /* se non sono il primo figlio */
					{	
						/* lettura da pipe della struttura */
						read(pipes[i-1][0],&pip,sizeof(s_occ));
						if (cur.num < pip.num)
						{	
							/* se la lunghezza calcolata e' minore di quella ricevuta, si deve mandare avanti quella ricevuta */
							cur.id = pip.id;
							cur.num = pip.num;
						}	
					}	
					/* printf("valore i = %d\n", i);
					printf("valore cur.id = %d\n", cur.id);
					printf("valore cur.num = %d\n", cur.num);
					*/
					/* comunicazione lunghezza linea */
					write(pipes[i][1],&cur,sizeof(s_occ));
					/* aspetto segnale dal padre */
					/* printf("prima di pause\n"); */
					pause();
					/* dopo l'arrivo del segnale azzero il conteggio linea precedente */
					cur.num=0;
					/* e ripristino l'indice del processo */
					cur.id = i;
				}	
			}	
			exit(stampate); /* torna il numero di linee scritte su standard output */
		}
	} /* fine for */

	/* codice del padre */
	/* chiusura pipe: tutte meno l'ultima in lettura */
	for(i=0;i<N;i++)
	{
		close (pipes[i][1]);
		if (i != N-1) close (pipes[i][0]);
	}

	/* il padre deve leggere H strutture, tante quante sono le linee lette dai figli */
	for(j=0;j<H;j++)
	{
		read(pipes[N-1][0],&pip,sizeof(s_occ));
		/* printf("valore j = %d\n", j);
		printf("valore pip.id = %d\n", pip.id);
		printf("valore pip.num = %d\n", pip.num); */
		/* il padre deve mandare il segnale che corrisponde a scrivi solo al processo di cui gli e' arrivato l'indice, mentre agli altri deve mandare il segnale che corrisponde a salta */
		for(i=0;i<N;i++)
		{
			sleep(1); /* per sicurezza */
			if (i==pip.id)
				kill(pid[i],SIGUSR1);
			else
				kill(pid[i],SIGUSR2);
		}	
	}

	/* Il padre aspetta i figli */
	for (i=0; i < N; i++)
	{
		if ((pidFiglio = wait(&status)) < 0)
        	{
                	printf("Errore in wait\n");
                	exit(7);
        	}

        	if ((status & 0xFF) != 0)
        		printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        	else
        	{	 
			ritorno=(int)((status >> 8) & 0xFF);
                	printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi!)\n", pidFiglio, ritorno);
        	}
	}

	exit(0);
}
