/* versione con N ulteriori pipe di comunicazione fra padre e figli */
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

int main (int argc, char **argv)
{
   int N; 				/* numero di file */
   int H; 				/* numero positivo */
   int pid;				/* pid per fork */
   pipe_t *pipes;			/* array di pipe usate a pipeline da primo figlio, a secondo figlio .... ultimo figlio e poi a padre: ogni processo legge dalla pipe i-1 e scrive sulla pipe i */
   pipe_t *pipePadre;			/* array di pipe di comunicazione fra padre e figli */
   int i,j; 				/* contatori */
   int fd; 				/* file descriptor */
   int pidFiglio, status, ritorno;	/* per valore di ritorno figli */
   char linea[256]; 			/* buffer linea: si puo' supporre che una linea sia lunga al massimo 255 caratteri (256 con il terminatore di linea)  */
   int stampate; 			/* numero linee stampate */
   s_occ cur; 				/* struttura corrente usata da ogni figlio */
   s_occ pip; 				/* struttura usata dal figlio e per la pipe */
   int letto;				/* indice mandato dal padre */
   int nr;

	if (argc < 3)
	{
		printf("Errore numero di parametri, dato che argc=%d\n", argc);
		exit(1);
	}

	H = atoi(argv[argc-1]);
	printf("Numero di linee %d\n", H);
	if (H < 0 || H > 255)
	{
		printf("Errore numero linee\n");
		exit(2); 
	}

	N = argc-2;
	printf("DEBUG-Numero di processi da creare %d\n", N);
	/* allocazione pipe usate in pipeline */
	if ((pipes=(pipe_t *)malloc(N*sizeof(pipe_t))) == NULL)
	{
		printf("Errore allocazione pipe\n");
		exit(3); 
	}

	/* allocazione delle pipe padre-figli */
	if ((pipePadre=(pipe_t *)malloc(N*sizeof(pipe_t))) == NULL)
	{
		printf("Errore allocazione pipe padre\n");
		exit(4);
	}

	/* creazione pipe */
	for (i=0;i<N;i++)
		if(pipe(pipes[i])<0)
		{
			printf("Errore creazione pipe\n");
			exit(5);
		}

	/* creazione di altre N pipe di sincronizzazione/comunicazione con i figli */
	for (i=0;i<N;i++)
		if(pipe(pipePadre[i])<0)
		{
			printf("Errore creazione pipe\n");
			exit(6);
		}

	for (i=0;i<N;i++){
		if ((pid=fork())<0)
		{
			printf("Errore creazione figli\n");
			exit(7);
		}
		if (pid==0)
		{ 	/* codice figlio */
			printf("DEBUG-Sono il figlio %d associato al file %s\n", getpid(), argv[i+1]);
			/* chiusura pipes inutilizzate */
			for (j=0;j<N;j++)
			{
				if (j!=i)
					close (pipes[j][1]);
				if ((i == 0) || (j != i-1))
					close (pipes[j][0]);
			}
			for (j=0;j<N;j++)
			{
				close (pipePadre[j][1]);
				if (j != i) 
					close (pipePadre[j][0]);
			}
			/* inizializzazione struttura */
			cur.id = i;
			cur.num = 0;
			stampate = 0;
			/* apertura file */
			if ((fd=open(argv[i+1],O_RDONLY))<0)
			{
				printf("Impossibile aprire il file %s\n", argv[i+1]);
				/* torniamo -1 che verra' interpretato come 255 e quindi errore */
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
						nr=read(pipes[i-1][0],&pip,sizeof(s_occ));
						if (nr!=sizeof(pip)) 
						{
							printf("Errore nella read per processo %d-esimo\n",i);
							exit(-1); 
						}
						/* se la lunghezza calcolata e' minore di quella ricevuta, si deve mandare avanti quella ricevuta */
						if (cur.num < pip.num)
						{	
							cur.id = pip.id;
							cur.num = pip.num;
						}
					}
					/* comunicazione lunghezza linea */
					write(pipes[i][1],&cur,sizeof(s_occ));
					/* aspetto indice dal padre */
					read(pipePadre[i][0], &letto, sizeof(letto));
					if (letto == i)
					{	 /* se il padre ha detto che devo scrivere allora si scrive la linea su standard output usando le write su 1 */
		 				write(1,linea,cur.num);
						stampate++;
					}
					/* dopo l'arrivo dell'indice azzero il conteggio linea precedente */
					cur.num=0;
					/* e ripristino l'indice del processo */
					cur.id=i;
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
	/* chiusura pipePadre: tutte in lettura */
	for(i=0;i<N;i++)
	{
		close (pipePadre[i][0]);
	}

	/* il padre deve leggere H strutture, tante quante sono le linee lette dai figli */
	for(j=0;j<H;j++)
	{
		read(pipes[N-1][0],&pip,sizeof(s_occ));
		for(i=0;i<N;i++)
		{
			/* il padre manda a tutti i figli l'indice del processo che deve stampare */
			write(pipePadre[i][1], &(pip.id), sizeof(pip.id));
		}	
	}

	/* Il padre aspetta i figli */
	for (i=0; i < N; i++)
        {
                if ((pidFiglio = wait(&status)) < 0)
                {
                	printf("Errore in wait\n");
                	exit(8);
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
