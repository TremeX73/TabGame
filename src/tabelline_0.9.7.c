#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>

const int DURATA_SFIDA = 15;
const int CAP_TABELLINA = 10;

char nickname[4];
int primato = -1000000;
char filepath[] = "..\\sav\\"; // per Linux -> = "../save/"
FILE *myFile;
char sesso;  // 'f' oppure 'm'
char finale_sesso;  // 'a' oppure 'o'

int domanda;            //comincia a contare le domande da -1 perchè, quando si esce con 'c', l'ultima domanda non deve essere contata
int risposta_corretta;
int risposta_corretta_nei_tempi;
int risposta_errata;
int flag_sfida_interrotta;  //1 -> interrotta
int flag_tempo_scaduto;  //1 -> scaduto
char risposta_ritorno_menu;
int modalita_gioco;
int moltiplicando_max;      //corrisponde alla tabellina che viene scelta
int difficolta;

time_t now;
int risposta;
char input[4];
int moltiplicatore;         //generato casualmente tra 0 e 10
int moltiplicando;          //generato casualmente fino al massimo scelto
int t_inizio_domanda;
int t_fine_domanda;
int t_inizio_partita;
int t_totale_partita;
int t_totale_domanda;
int secondi_difficolta;
int pseudo_r_moltiplicatore[19] = {0, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10 };

int   punteggio;
float fattore_difficolta;
char  descrizione_difficolta[10];
float percentuale;
char  risposta_altra_sfida;

int char_upper(int c);
char *string_upper(char *s);
void normalizza_nome(char *s);
void crea_filepath(char *fp, char *n);
int  file_exists(char *f);
void carica_profilo();
void aggiorna_profilo(int p);
void check_primato(int r, int p);
char imposta_sesso();
char imposta_finale_sesso(char s);
void gestisci_profilo();

void inizializza_menu();
void domanda_ritorno_menu();
void inizializza_partita();
int  controlla_risultato(int f1, int f2, int p);
int  genera_casuale(int max);
int  calcola_tempo(int a, int b);
void attendi(int secondi_attesa);
void saluta();
void devil();
void motore_domanda();
void svolgimento_partita();
void avvio_menu();

int main()
{
    setlocale (LC_ALL,"");      //settings per le lettere accentate insieme a #include <locale.h>
    srand(time(0));             //inizializzatore della sequenza casuale

    puts("        ______________________     ________________________________ ");
    puts("       /____  _______________/    / ______________________________/ ");
    puts("           / /          ____     / /                          ___   ");
    puts("          / /    /|    / - /    / /   ____    /|    /| /|    /      ");
    puts("         / /    /_|   / _<     / /___/ __/   /_|   / |/ |   /-      ");
    puts("        /_/    /  |  /___/    /_______/     /  |  /     |  /__     (v1.0)\n");

    gestisci_profilo();

    printf("\n----------------------Gioco delle TABELLINE!----------------------------\n");
    puts(" Verranno chieste le tabelline fino a quella scelta, in modo casuale.");
    puts(" Se sceglierai 6, per esempio, verranno chieste tutte le tabelline fino\n a quella del 6.");
    puts(" A seconda del livello di difficoltà avrai meno tempo per rispondere\n correttamente, ma prenderai più punti.");
    puts(" Per conoscere l'esatto calcolo del punteggio consulta il manuale\n del gioco.");
    puts(" ATTENZIONE! Per interrompere in qualunque momento, premi \"c\"+Invio.");
    puts("--------------------------------Iniziamo!-------------------------------");

	avvio_menu();

    saluta();

    return 0;
}

/////////////////////////////////////////////////////////
/////////////////  PROFILO  /////////////////////////////
/////////////////////////////////////////////////////////

int controlla_nick(char *s)
{
    int len = strlen(s);
    int conta_char_non_validi = 0;

    for(int i=0; i<len; i++)
    {
        if ( !( (s[i] >= 97 && s[i] <= 122) || (s[i] >= 65 && s[i] <= 90) ) )
            conta_char_non_validi++;
    }

    if (conta_char_non_validi > 0)
        return 0;
    else
        return 1;
}

int char_upper(int c)
{
    if ( c >= 97 && c <= 122 )
        return c - 32;
    else //if ( c >= 65 && c <= 90 )
        return c;
}

char *string_upper(char *s)
{
    int len = strlen(s);
    for(int i=0; i<len; i++)
    {
        s[i] = char_upper(s[i]);
    }
    return s;
}

void normalizza_nome(char *s)
{
    //strcpy(s, strupr(s)); //upper in Windows
    strcpy(s, string_upper(s)); //creata func per Linux

   /* for (int i=1; i<3; i++) //aggiunge _ underscore per arrivare a 3 caratteri
    {
        if (s[i] == 0)
        {
            //printf("Manca indice: %i\n", i);
            strcat(s, "_");
        }
    }   */
    //printf("normalizzato: %s\n", s);
}

void crea_filepath(char *fp, char *n)
{
    strcat(fp, n);
    //printf("Filepath: %s\n", fp);
}

int file_exists(char *f)
{
    if ( myFile = fopen(f, "r") )
    {
        //puts("Il file esiste");
        fclose(myFile);
        return 1;
    }
    else
    {
        //puts("Il file non esiste");
        return 0;
    }
}

void carica_profilo()
{
    myFile = fopen(filepath, "r");
    fscanf(myFile, "%i %c" , &primato, &sesso);
    fclose(myFile);
    //printf("Primato letto: %i\n", primato);
    //printf("Sesso letto: %c\n", sesso);
}

void aggiorna_profilo(int p)
{
    myFile = fopen(filepath, "w");
    fprintf(myFile, "%i %c", p, sesso);
    fclose(myFile);
}

void check_primato(int r, int p)
{
    if (p > r)
    {
        puts("\n   N   N            RRRR");
        puts("   NN  N            R   R");
        puts("   N N N            RRRR");
        puts("   N  NN            R   R");
        puts("   N   N U O V O    R   R E C O R D ! ! ! !\n");
        aggiorna_profilo(p);
    }
}

char imposta_sesso()
{
    int i;
    do
    {
        printf(" Sei [m]aschio o [f]emmina? ");
        scanf("%1s", &sesso);
        fflush(stdin);

        switch (sesso)
        {
            case 'f':
            case 'm':
                i = 0;
                return sesso;
            default:
                i = 1;
        }
    }
    while (i);
}

char imposta_finale_sesso(char s)
{
    switch (s)
    {
        case 'f':
            return 'a';
            break;
        case 'm':
            return 'o';
            break;
    }
}

void gestisci_profilo()
{
    do
    {
        printf(" Inserisci nickname (massimo 3 lettere [a-z,A-Z]): ");
        scanf("%3s", nickname);
        fflush(stdin);
    }
    while( !controlla_nick(nickname) );

    normalizza_nome(nickname);

    crea_filepath(filepath, nickname);

    if ( !file_exists(filepath) )
    {
        sesso = imposta_sesso();
       // scrivi_sesso(sesso);

        aggiorna_profilo(primato);
        finale_sesso = imposta_finale_sesso(sesso);

        printf("  > Benvenut%c!\n\n", finale_sesso);
        //puts("File creato con il primato di default e sesso inserito.");
    }
    else if ( !strcmp(nickname, "GIO") )
    {
        carica_profilo();
        finale_sesso = imposta_finale_sesso(sesso);
        if (primato == -1000000)
            printf("  > Bentornat%c! Ancora nessun record.\n\n", finale_sesso);
        else
            printf("  > Bentornat%c! Il tuo record è %i.\n\n", finale_sesso, primato);
        puts("                           /    ");
        puts("                          *    ");
        puts("       /     O O O___O O O______");
        puts("      *    O       O    /  O______");
        puts("          O     /      *    O______");
        puts("          O    *            O______");
        puts("           O       /       O____/_");
        puts("            O     *       O____*_");
        puts("         /    O         O______");
        puts("        *       O     O______");
        puts("           /       O______");
        puts("          *         \n");
    }
    else
    {
        carica_profilo();
        finale_sesso = imposta_finale_sesso(sesso);
        if (primato == -1000000)
            printf("  > Bentornat%c! Ancora nessun record.\n\n", finale_sesso);
        else
            printf("  > Bentornat%c! Il tuo record è %i.\n\n", finale_sesso, primato);
    }
}

/////////////////////////////////////////////////////////////////
///////////////////  GIOCO  /////////////////////////////////////
/////////////////////////////////////////////////////////////////

void inizializza_menu()
{
    modalita_gioco = -1;
    moltiplicando_max = -1;
    difficolta = -1;
}

void domanda_ritorno_menu()
{
    printf(" Premi [Invio] per tornare al menu oppure [c] per uscire: ");
    risposta_ritorno_menu = getchar();
    fflush(stdin);
}

void inizializza_partita()
{
    carica_profilo();
    domanda = -1;           //comincia a contare le domande da -1 perchè, quando si esce con 'c', l'ultima domanda non deve essere contata
    risposta_corretta = 0;
    risposta_corretta_nei_tempi = 0;
    risposta_errata = 0;
    flag_sfida_interrotta = 0; //1 -> interrotta
    flag_tempo_scaduto = 0; //1 -> scaduto
}

int controlla_risultato(int f1, int f2, int p)
{
    if (f1 * f2 == p)
        return 1;
    else
        return 0;
}

int genera_casuale(int max)
{
    int casuale = 0;
    casuale = rand() % max;     //numero intero casuale da [0] a [max-1]
    return casuale;
}

int calcola_tempo(int a, int b)
{
    int t = b - a;
    return t;
}

void attendi(int secondi_attesa)
{
   int t_0 = time(&now);
   int t_1;

   do
   {
        t_1 = time(&now);
        //printf("%d",calcola_tempo(t_0, t_1));
   }
   while (calcola_tempo(t_0, t_1) < secondi_attesa);
}

void saluta()
{
    puts("\n  > Ciao, a presto :)");
    attendi(2);
}

void motore_domanda()
{
	do
	{
		moltiplicatore = pseudo_r_moltiplicatore[genera_casuale(19)];   //estrae lo pseudo random moltiplicatore tra 0 e 10
																		//in pratica genera casualmente l'indice dell'array
		moltiplicando = genera_casuale(moltiplicando_max) + 1;  //estrae a caso il moltiplicando tra 1 e il valore inserito come moltiplicando_max
		domanda++;
		printf("\n #%d: quanto fa %dx%d? ", domanda + 1, moltiplicando, moltiplicatore);

		t_inizio_domanda = time(&now);
		//printf("Inizio: %d\n", t_inizio_domanda);

		scanf("%4s", input);
		while(getchar() != '\n');
		risposta = atoi(input);

		t_fine_domanda = time(&now);
		//printf("Fine: %d\n", t_fine_domanda);

		t_totale_partita = calcola_tempo(t_inizio_partita, time(&now));
		//printf("Totale partita: %dsec\n", t_totale_partita);

		if (t_totale_partita > DURATA_SFIDA && modalita_gioco == 2)
		{
			input[0] = 'c';
			flag_tempo_scaduto = 1;
			puts("\n TEMPO SCADUTO!!! TEMPO SCADUTO!!!");
		}

		if (input[0] != 'c')
		{
			if (controlla_risultato(moltiplicando, moltiplicatore, risposta))
			{
				printf(" Brav%c!\n", finale_sesso);
				risposta_corretta++;

				t_totale_domanda = calcola_tempo(t_inizio_domanda, t_fine_domanda);

				if (t_totale_domanda < secondi_difficolta)
					risposta_corretta_nei_tempi++;
			}
			else
			{
				printf(" No, fa %d.\n", moltiplicando * moltiplicatore);
				risposta_errata++;
			}
		}
		else if (modalita_gioco == 2 && flag_tempo_scaduto == 0)
		{
		   flag_sfida_interrotta = 1;
		   puts("\n SFIDA INTERROTTA!!!");
		}
	}
	while (input[0] != 'c');
}

void svolgimento_partita()
{
	do
	{
		inizializza_partita();
		t_inizio_partita = time(&now);
		//printf("Inizio partita: %d\n", t_inizio_partita);

		motore_domanda();

		if (input[0] == 'c'
				&& domanda > 0 && flag_sfida_interrotta == 0)
			puts("\n N.B. L'ultima domanda non viene conteggiata.");

		punteggio = risposta_errata * (moltiplicando_max - 11)
				+ moltiplicando_max * risposta_corretta_nei_tempi * fattore_difficolta;

		percentuale = (float)risposta_corretta / domanda * 100;

		if (domanda != 0 && flag_sfida_interrotta == 0)
		{
			printf("\n >>>Riepilogo: tabellina del %d, livello %s<<<\n", moltiplicando_max, descrizione_difficolta);

			if (risposta_corretta == 1 && domanda == 1)
				printf(" %d risposta corretta su %d domanda, percentuale: %.1f%%\n", risposta_corretta, domanda, percentuale);
			else if (risposta_corretta == 1)
				printf(" %d risposta corretta su %d domande, percentuale: %.1f%%\n", risposta_corretta, domanda, percentuale);
			else if (domanda == 1)
				printf(" %d risposte corrette su %d domanda, percentuale: %.1f%%\n", risposta_corretta, domanda, percentuale);
			else
				printf(" %d risposte corrette su %d domande, percentuale: %.1f%%\n", risposta_corretta, domanda, percentuale);

			printf(" Risposte corrette entro i %d secondi: %d\n", secondi_difficolta - 1, risposta_corretta_nei_tempi);

			if (modalita_gioco == 2)
			{
                puts(" +---------------------------+");
                printf(" | Punteggio: %10d     |\n", punteggio);
                puts(" +---------------------------+");

                check_primato(primato, punteggio);

                switch (punteggio)
                {
                    case 6:
                    case 66:
                    case 666:
                    devil();
                }
   			}
		}

		if (modalita_gioco == 2)
		{
			printf(" Premi [Invio] per un'altra sfida oppure [c] per uscire da questa modalità: ");
			risposta_altra_sfida = getchar();
			fflush(stdin);
		}
	}
	while (risposta_altra_sfida != 'c' && modalita_gioco == 2);
}

void avvio_menu()
{
	do
    {
		inizializza_menu();

        do
        {
            printf("\n Scegli la modalità di gioco tra [1]Allenamento, [2]Sfida a tempo(%i secondi): ", DURATA_SFIDA);
            scanf("%2s", input);
            while(getchar() != '\n');
            modalita_gioco = atoi(input);
        }
        while ((modalita_gioco < 1 || modalita_gioco > 2) && input[0] != 'c');

		if (input[0] == 'c')
        {
            domanda_ritorno_menu();
            if (risposta_ritorno_menu != 'c')
                continue;
            else
                break;
        }

        do
        {
            printf("\n Scegli una tabellina tra 1 e %i: ", CAP_TABELLINA);
            scanf("%3s", input);
            while(getchar() != '\n');
            moltiplicando_max = atoi(input);
        }
        while ((moltiplicando_max < 1 || moltiplicando_max > CAP_TABELLINA) && input[0] != 'c');

        if (input[0] == 'c')
        {
            domanda_ritorno_menu();
            if (risposta_ritorno_menu != 'c')
                continue;
            else
                break;
        }

        do
        {
            printf("\n Scegli un livello di difficoltà tra [1]Facile, [2]Medio, [3]Difficile: ");
            scanf("%2s", input);
            while(getchar() != '\n');
            difficolta = atoi(input);
        }
        while ((difficolta < 1 || difficolta > 3) && input[0] != 'c');

        if (input[0] == 'c')
        {
            domanda_ritorno_menu();
            if (risposta_ritorno_menu != 'c')
                continue;
            else
                break;
        }

        switch(difficolta)
        {
        case 1:
            secondi_difficolta = 7;
            fattore_difficolta = 1.0;
            strcpy(descrizione_difficolta, "Facile"); //funzione per copiare una stringa in un array di char
            break;
        case 2:
            secondi_difficolta = 5;
            fattore_difficolta = 1.5;
            strcpy(descrizione_difficolta, "Medio");
            break;
        case 3:
            secondi_difficolta = 3;
            fattore_difficolta = 2.0;
            strcpy(descrizione_difficolta, "Difficile");
            break;
        }

        printf("\n >>>Hai scelto fino alla tabellina del %d, livello %s<<<\n", moltiplicando_max, descrizione_difficolta);
        if (modalita_gioco == 2)
        {
            puts("                  +------------------+");
            puts("                  | SFIDA A TEMPO!!! |");
            puts("                  +------------------+");
        }
        printf(" Quando sei pront%c per iniziare, premi [Invio].", finale_sesso);
        getchar(); // wait for ENTER
        fflush(stdin); // option ONE to clean stdin
        //while(getchar()!='\n'); // option TWO to clean stdin

		svolgimento_partita();
        domanda_ritorno_menu();
    }
    while (risposta_ritorno_menu != 'c');
}

/////////////////////////////////////////////////////////////////
///////////////////  DEVIL  /////////////////////////////////////
/////////////////////////////////////////////////////////////////

void devil()
{
    puts("\n                                           :-+*############*+-:                             ");
    puts("                                      .=*##+=====+++=-::.   .-=***=:                        ");
    puts("                                   :+#*==+#@@%*=:.                :+#*-                     ");
    puts("                  +              +%+::*@@@%=.                        .=%+.           :%+    ");
    puts("                 *#%+.        .*#- .#@@@*:                              -#*.       .*+@@    ");
    puts("                 @+ -##=:.  .**:   #@@*.                                  -%=    -**: @@=   ");
    puts("                -@#    -+#@@@%+=+: .-.                                   =##@#*#*=   =@@*   ");
    puts("                -@@=        ..::-@.                                      %=         .@@@*   ");
    puts("                :@@@+            %=                                      @:        .%@@@+   ");
    puts("                 @@@@@=.         #+                                      @=      .+@@@@@.   ");
    puts("                 =@@@@@@%*=-:..  %-                                      *%:::-+#@@@@@@+    ");
    puts("                  *@@@@@@@@@@@@@@%                                       .@@@@@@@@@@@@+     ");
    puts(" #@#+-.            =@@@@@@@@@@@@@:                                        :@@@@@@@@@#:      ");
    puts(" :%@=:*++++-:.      .*@@@@@@@@@@.                                          :%@@@@@#:        ");
    puts("   #@==#=. .:-=@+      *@@@@@@@@+**#######*+=:.           :=+*######*+-.     ++.-@#         ");
    puts("    #@= =%%+. *#       %@@@@@@@@@@#+=:....:-=+#%*-    -*%@#+-:.      :=*%=      .@#         ");
    puts("     *@-  .-..*=.      @@@@@@@@%=..:.           :+#*#@*=.          .::.  +%-     @#         ");
    puts("      #@: -#@*:.+*.    @@@@@@@=   =++*#*-.         .-           -**=::-=- .%-   :@+         ");
    puts("      .@@%+..*@*..*+   *@@@@@-         .-*#=                 :+%*:         -@   +@.         ");
    puts("       .-     .*@- =%. .@@@@*              =#*-          :*#%#=.        -   @- .@*          ");
    puts("                +@- -%  =@@@=   :       ===-..+%#.        -.  =*%@*++*#@+   @- %%.          ");
    puts("                 %@  #-  +@@+    +#+-.     :+#-             =@*.-@*  :@#-: -@-%%.           ");
    puts("                 *@: *+   =@@.    *%%@@%#*+==-            .++@#=*@=  =@=:  %@@*             ");
    puts("                 @@  %:    :%#       *@%#%@@@@%:         :%+:.:--..-##-=  #@%-              ");
    puts("                #@- +*       ##   ::...::=-.....   .-:.     :=====-..:   #@*                ");
    puts("              .#%- +*         +%-  .:--:          +#++#=               :%%-                 ");
    puts("             =@+ .*-           .**.                                  .*%=                   ");
    puts("            *@- -#.              :#*.        ::..=+++**+-:-++      .+%=                     ");
    puts("           +@+ :%                  .+*:      =#@@@@@@@@@@@#+:    .*#=                       ");
    puts("           @@  ++                     -++:     .@*  .%@@@+     =#+:                         ");
    puts("           @@. =%         .=*%%%#+-.     -+=:   .+##@@%*:   -##-    .=*%%@@@%*-             ");
    puts("           #@*  +%-      +@@%%%@@@@@@*=.    -++-.       :=##=   .-*%#=:     :=*@*:          ");
    puts("            %@*  .+#+: .#%=     .-+#%@@@%*=: .@@@@%%#%%@@#@#=+##*=.             =%#-        ");
    puts("             *@%-   .-*@#             :=+#%@@@@@@@@@@@#=. .--:                    +@#.      ");
    puts("              :*@@+:  %#                     .:::::..                              .%@+     ");
    puts("                 -*%@@#          #:                                    :#*           *@#.   ");
    puts("                    #%          +@@+.                   .:-=+**########@@@#:          *@@-  ");
    puts("                   +@.      .:=%@@@@@#+-..    ..:-=+*%@%*+==-::........:::--.          @@@- ");
    puts("                  -@-     =***+++++**##%@@@@@@@@@@@@@@#*+=-:                           @@@@ ");
    puts("                  #@                      .:-=+*#%@@@@@@@@@@@@#*+-:                  :#@@@% ");
    puts("                  #@*:                             :*@@@@@@@@@@@@@@@@%#**+===---==+*%@@@@%. ");
    puts("                  .#@@@@#*=-:.              .:-+*%@@@@@@#=: .:-+*#%@@@@@@@@@@@@@@@@@@@@#-   ");
    puts("                    .=*@@@@@@@@@@%%%%%%%%@@@@@@@@@@#+-.              .:--=++++*+++==:.      ");
    puts("                        .:=+*#%@@@@@@@@@@@%#*+=-:                                           \n");
    puts("                               YOU SUMMONED THE WINKING DEVIL!\n");
}
