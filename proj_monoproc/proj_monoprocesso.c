#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
#include <time.h>
#include "funzioni.h"
#include <math.h>

/*
Array: 
parole_uniche conterrà solo una volta tutte le parole/punteggiature presenti nel file di testo in input, l'intero indice_parole_uniche fungerà da indice per scorrere l'array
tutto_il_file conterrà l'intero file diviso parola per parola, saranno considerate singolarmente anche i simboli di punteggiatura ? ! ., l'intero indice_tutto_file fungerà da indice per scorrere l'array
La memoria per questi due array sarà allocata dinamicamente per potèr gestire file di qualsiasi dimensione
*/
int indice_parole_uniche = 0;
wchar_t **parole_uniche;

int indice_tutto_file = 1;
wchar_t **tutto_il_file;

//In questo array verrà composta la parola lettera per lettera
wchar_t parola[31];
int indice_parola = 0;
    
//Variabile per immagazzinare la lettera presa dal file di testo in input 
wchar_t lettera;

//Variabili per gestire la grandezza degli array e la gestione della riallocazione
int dimensione_parole_uniche = 1000;
int dimensione_tutto_file = 1000;
int dimensione_parole_che_seguono2 = 1000;

int main(int argc, char const *argv[]){
    //Imposto codifica predefinita dell'utente
    setlocale(LC_ALL, "");  
    //Codifica inserita per stampare successivamente le variabili double nel file  con il punto eg. 1.15 invece che la virgola eg. 3,14
    setlocale(LC_NUMERIC, "C");
    

    //Il ciclo mi permette al processo di non concludersi e continuare a richiedere comandi all'utente affinchè non viene richiesto di terminare il programma
    int exit = 1;
    while(exit != 0){

        //Input per la richiesta del comando
        wprintf(L"\n<-Cosa si desidera effettuare: ");
        wchar_t input[100];
        fgetws(input, 100, stdin);

        //Se viene inserito un comando non valido ricomincia il while, include anche il carattere di newline
        if(wcslen(input) != 2){   
            wprintf(L"\n-->È stato digitato un comando non valido, premere h per la lista comandi\n");
            continue;
        }

        wchar_t carattere = input[0];
        if(carattere == L'a'){
            //Chiama la funzione per allocare memoria  
            alloca_memoria(dimensione_tutto_file, &tutto_il_file);
            alloca_memoria(dimensione_parole_uniche, &parole_uniche);

            //Apertura file .txt da analizzare
            FILE *pF = apertura_file_analisi(1);
            //Analizzo il file carattere per carattere, quando arrivo alla fine del file esce dal ciclo
            while((lettera = fgetwc(pF)) != WEOF){
                
                //Verifico se lo spazio nei due array è finito se si ne alloco altro 
                if(indice_tutto_file == dimensione_tutto_file){
                    dimensione_tutto_file *= 2;
                    rialloca_array(dimensione_tutto_file, &tutto_il_file);
                }
                if(indice_parole_uniche == dimensione_parole_uniche ) {
                    dimensione_parole_uniche *= 2;
                    rialloca_array(dimensione_parole_uniche, &parole_uniche);
                }
                //Se è una lettera allora la concateno nella variabile parola [vd. void concatena_carattere()]
                if(controllo_lettera(lettera) == 0 ){
                    
                    concatena_lettera();
                //C'è un'apostrofo, lo concateno nella parola e la salvo negli array [vd. void inserisci_parola()]
                }else if(controllo_lettera(lettera)==2){
                    
                    concatena_lettera();
                    inserisci_parola(0);
                //In tutti gli altri casi se la parola non è vuota la inserisco negli array 
                }else if(indice_parola > 0){

                    inserisci_parola(0);
                }
                //Se il carattere risulta come punteggiatura lo inserisco singolarmente negli array
                if (controllo_lettera(lettera) == 1){
                    inserisci_parola(1);

                    
                }
            }
            //Chiusura file .txt
            fclose(pF);

            //Da notare che il primo spazio dell'array è rimasto vuoto e verrà inserita l'ultima parola del file così da poter creare nel conteggio delle frequenze
            //l'illusione che a seguire l'ultima parola è la prima
            wcscpy(tutto_il_file[0], tutto_il_file[indice_tutto_file-1]);

            //In questo array di interi verranno inserite il numero di volte che la singola parola appare nel file. Infatti per ogni elemento in posizione
            // i in parole_uniche corrisponderà alla stessa posizione i di conteggio_parola_unica il numero di volte che appare nel file
            int conteggio_parola_unica[indice_parole_uniche];
 
            //Questo array conterrà tutte le parole che seguono la singola parola in parole_uniche
            int dimensione_parole_che_seguono = 1000;
            wchar_t **parole_che_seguono; 
            alloca_memoria(dimensione_parole_che_seguono, &parole_che_seguono); 

            int indice_parole_che_seguono = 0;

            //Allocando dinamicamente nell'array saranno inserite il numero di volte che una parola in parole_che_seguono segue quella in parole_uniche
            //Anche in questo caso per ogni elemento in posizione i in parole_che_seguono corrisponderà alla stessa posizione i di conteggio_parole_che_seguono   
            int *conteggio_parole_che_seguono = malloc(sizeof(int) * dimensione_parole_che_seguono);

            //Apertura/creazione file csv su cui scrivere la tabella
            FILE *fP2 = apertura_file_scrittura(2);

            //Prendo parola per parola nell'array parole_uniche
            for(int i = 0; i < indice_parole_uniche; i++){   
                int conteggio_parole = 0;
                //Prendo parola per parola in tutto_il_file
                for (int j = 0; j < indice_tutto_file; j++){
                
                    //Se lo spazio negli array è completo ne alloco altro
                    if(indice_parole_che_seguono == dimensione_parole_che_seguono) {
                        dimensione_parole_che_seguono *= 2;
                        rialloca_array(dimensione_parole_che_seguono, &parole_che_seguono);
                        conteggio_parole_che_seguono = realloc(conteggio_parole_che_seguono, sizeof(int)*dimensione_parole_che_seguono);
                    }
                    //Effettuo comparazione tra la parola in parole_uniche con tutte quelle in tutto_il_file
                    if (wcscmp(parole_uniche[i], tutto_il_file[j]) == 0){
                        //Se uguali inserisco in parole_che_seguono la parola che segue quella che stiamo analizzando, mediante un flag elem_gia_presente verifico se non sia già stata inserita
                        //e allo stesso indice incremento o setto a 1 il valore in conteggio_parole_che_seguono per verificare quante volte quella parola si trova dopo quella che si sta
                        //analizzando. Viene inoltre contato con int conteggio_parole le volte che la parola analizzata appare nel testo e inserita alla fine in conteggio_parola_unica.
                        int elem_gia_presente = 0;
                        for(int l = 0; l < indice_parole_che_seguono; l++){                    
                        
                            if(wcscmp(parole_che_seguono[l], tutto_il_file[j+1]) == 0){
                                conteggio_parole_che_seguono[l]++;
                                elem_gia_presente = 1;
                                conteggio_parole++;
                                break;
                            }
                        }
                        if(elem_gia_presente == 0 && wcscmp(tutto_il_file[j+1],L"") != 0){
                            wcscpy(parole_che_seguono[indice_parole_che_seguono], tutto_il_file[j+1]);
                            conteggio_parole_che_seguono[indice_parole_che_seguono] = 1;
                            indice_parole_che_seguono++;
                            conteggio_parole++;
                        }
                    }
                }   
                conteggio_parola_unica[i] = conteggio_parole;

                //Da questo punto si ha l'array contenente le parole che seguono la parola analizzata 
                stampa_tabella(fP2,parole_che_seguono, parole_uniche,conteggio_parole_che_seguono, conteggio_parola_unica, i, indice_parole_che_seguono);
                
                //Svuota array settando tutti i valori a 0 per la prossima iterazione
                for (int z=0; z < indice_parole_che_seguono; z++){ 
                    wcscpy(parole_che_seguono[z], L"");
                    conteggio_parole_che_seguono[z] = 0;
                }
                indice_parole_che_seguono = 0; 
            }
            //Libero lo spazio
            //Passo gli indirizzi di memoria dove sono memorizzati i puntatori
            libera_array(dimensione_parole_uniche, &parole_uniche);
            libera_array(dimensione_tutto_file, &tutto_il_file);
            libera_array(dimensione_parole_che_seguono, &parole_che_seguono);
            free(conteggio_parole_che_seguono);

            //Resetto variabili globali per prossima iterazione
            wmemset(parola, 0, sizeof(parola)/sizeof(wchar_t));
            indice_parole_uniche = 0;
            indice_tutto_file = 1;
            indice_parola = 0;
            lettera = L'\0';

            //Chiusura file .csv
            fclose(fP2);
            wprintf(L"-->Il file csv è stato generato correttamente.\n");

        }else if(carattere == L'b'){
            //Imposto il seme per avere una generazione "randomica" ad ogni esecuzione
            srand(time(0));

            //Apertura file .csv da analizzare
            FILE *fp = apertura_file_analisi(2);

            //Numero di parole che comporranno la frase da generare
            int lunghezza_frase_generata; 
            //Prende in input il numero di parole
            wprintf(L"\n<-Inserisci lunghezza frase generata: ");
            wscanf(L"%d", &lunghezza_frase_generata);
            
            //Poichè rimane nel buffer di input il carattere \n, per evitare che il prossimo wscanf() venga "saltato" pochè prende \n lo elimino dal buffer
            getwchar();

            //Calclo del numero di righe presente nel file .csv prendendo il primo carattere di ogni riga incrementando di 1 int numero_righe
            //affinchè non arriva alla fine del file
            int numero_righe = 0;
            wchar_t c;
            do{
                c = fgetwc(fp);
                if(c == '\n') numero_righe++;
            }while(c != WEOF);

            if(numero_righe < 2){
                wprintf(L"-->Formato del .csv non valido, verificarne il contenuto.\n");
                continue;
            }

            int parola_inizio_frase;
            wchar_t parola_da_cercare[31]; 
            
            //Flag per verificare se è stato scelto un criterio di selezione per la prima parola da generare, se 0 il ciclo while continua
            int inzio_selezionato = 0;
            while(inzio_selezionato == 0){
                //Flag per la gestione dell'input utente verificano se ci siano spazi, numeri e se l'input è composto da soli spazi
                int spazio_in_mezzo = 0;
                int contiene_numeri = 0;
                int solo_spazi = 1;

                //Input da parte dell'utente per iniziare la generazione con una parola a scelta
                wprintf(L"\n<-Inserisci una parola, non deve contenere numeri o spazi, lasciare vuoto se si vuole procedere con generazione casuale: ");
                fgetws(parola_da_cercare, sizeof(parola_da_cercare), stdin);

                //Rimuove il carattere di nuova linea alla fine della parola appena inserita in input, se presente
                parola_da_cercare[wcslen(parola_da_cercare) - 1] = '\0';

                //Capitalizzo la parola inserita in minuscolo
                for (int i = 0; parola_da_cercare[i]; i++) {
                    parola_da_cercare[i] = towlower(parola_da_cercare[i]);
                }
                //Scorro carattere per carattere la parola digitata
                for (int i = 0; i < wcslen(parola_da_cercare); i++) {
                    //Controlla se il carattere non è uno spazio
                    if (!iswspace(parola_da_cercare[i])) {
                        solo_spazi = 0;
                    }
                    //Verifica la presenza di spazi e aggiorno la flag
                    if (iswspace(parola_da_cercare[i])) {
                        spazio_in_mezzo = 1;
                    }
                    //Verifica la presenza di numeri e aggiorno la flag
                    if (iswdigit(parola_da_cercare[i])) {
                        contiene_numeri = 1;
                    }
                }
                //Se la parola inserita è lunga 0 caratteri o la flag solo_spazi è vera genero parola inziale casuale
                if (wcslen(parola_da_cercare) == 0 || solo_spazi) {
                    wprintf(L"-->Proseguo a generare una parola di inizio casuale\n");

                    //Seleziona casualmente una delle righe totali sotto forma di intero 
                    parola_inizio_frase = rand()%numero_righe;

                    //Aggiorno flag poichè è stato deciso di generare parola casuale 
                    inzio_selezionato = 1;

                //Se sono presenti numeri nell'input il ciclo ricomincia
                }else if(contiene_numeri || spazio_in_mezzo){
                    wprintf(L"-->Hai inserito una parola non valida.\n");
                //Se è stata inserita una parola corretta
                }else{
                    //Aggiorno flag poichè è stato deciso di generare parola casuale 
                    inzio_selezionato = 1;        
                }
            }
            //Sposto il puntatore all'inizio del file poichè bisogna scorrerlo di nuovo
            rewind(fp);

            //In questo array saranno inserite in ordine le parole generate che formeranno la frase da scrivere nel .txt
            wchar_t frase_generata[lunghezza_frase_generata][31];
            int indice_frase_generata = 0;

            //Dichiaro parole_che_seguono2 array dinamico per inserire tutte le parole che seguono quella analizzata e l'array frequenze in cui saranno inserite
            //le frequenze, per ogni elemento in posizione i avremo la parola in parole_che_seguono2 e la sua corrispondente frequenza
            wchar_t **parole_che_seguono2;
            float frequenze[100000];

            //In questo array saranno inserite le parole che seguono quella analizzata tante volte quanto è il valore della frequenza
            wchar_t selezione_casuale[10000][31];

            //Alloco memoria
            alloca_memoria(dimensione_parole_che_seguono2, &parole_che_seguono2);

            //Variabile che tiene traccia della lunghezza della frase generata
            int contatore_lunghezza_frase = 0;

            int conteggio = 1;

            //Ad ogni ciclo viene decisa una nuova parola da inserire nella frase_generata
            while(contatore_lunghezza_frase < lunghezza_frase_generata){
                //Buffer in cui inserire la linea della parola selezionata
                wchar_t line[100000];

                //Prendo la linea e la inserisco nel buffer
                while (fgetws(line, sizeof(line), fp)){
                    //Grazie a wcstok prendo ogni elemento in line[] più facilmente inserendo come separatore la virgola
                    wchar_t *state;
                    wchar_t *token = wcstok(line, L",", &state);

                    //Scorro il .csv fino ad arrivare alla riga che ha in prima posizione la parola che voglio analizzare
                    if ( conteggio == parola_inizio_frase ||  (wcscmp(parola_da_cercare, token) == 0)){
                        conteggio++;

                        //Indice per l'array parole_che_seguono2, viene resettato a ogni ciclo
                        int posiz = 0;

                        //Inserisco la parola, di cui bisogna analizzare i suoi successori, attuale nella frase_generata
                        wcscpy(frase_generata[indice_frase_generata], token);

                        //Sposto l'indice per la prossima parola
                        indice_frase_generata++;

                        //Prendo nuova parola che segue
                        token = wcstok(NULL, L",", &state);

                        while(token != NULL){
                            //Se gli array sono pieni rialloco nuova memoria
                            if(posiz == dimensione_parole_che_seguono2) {
                                dimensione_parole_che_seguono2 *= 2;
                                rialloca_array(dimensione_parole_che_seguono2, &parole_che_seguono2);
                            }
                            //inserisco la parola che segue in parole_che_seguono2
                            wcscpy(parole_che_seguono2[posiz], token);

                            //prendo la sua frequenza e la inserisco in frequenze                    
                            token = wcstok(NULL, L",", &state);
                            frequenze[posiz] = wcstof(token, NULL);

                            //incremento indice per la prossima parola che segue 
                            posiz++;
                            token = wcstok(NULL, L",", &state);
                        }

                        
                        //Converto la frequenza in modo che possa essere inserita un numero intero di volte nell'array frase_generata
                        //Ad esempio se ho una frequenza dello 0.6 la parola corrispondente verrà inserita 6000 volte e poi
                        //una frequenza da 0.4 la parola corrispondente verrà inserita 4000 volte, successivamente ci sarà una selezione randomica
                        //da 0 a 99999 e verrà presa la parola in quella posizione così da avere una pseudo selezione che tiene conto delle frequenze
                        //è stato scelto il valore 10000 per l'array frase_generata così da coprire frequenze estremamente piccole fino a 0.0001    
                        float conversione_in_intero = 0;
                        //Questa variabile tiene traccia per ogni for j dell'indice dell'array frase_generata
                        int ultima_posizione = 0;

                        //Creo l'array selezione_casuale come descritto sopra, per ogni parola in parole_che_seguono2 la inserisco in selezione_casuale tante volte quanto è conversione_in_intero
                        for(int i = 0; i < posiz; i++){
                            conversione_in_intero = (int)round(frequenze[i] * 10000);
                            for(int j = ultima_posizione; j < 10000; j++){
                                wcscpy(selezione_casuale[j], parole_che_seguono2[i]);
                            }
                            ultima_posizione = ultima_posizione+conversione_in_intero;
                        }
                        //Seleziono secondo le modalità sopra descritte la prossima parola da analizzare 
                        //Con il ciclo verifico che non venga selezionato uno elemento nullo poichè alcune eighe del csv potrebbero avere come risultato della
                        //Somma delle frequenze un numero minore di 1, ciò a causa dell'approssimazione. L'array quindi verrebbe riempito ma tralasciati pochissimi spazi vuoti.
                        while(1){
                            int selezione = rand()%9999;
                            if (selezione_casuale[selezione][0] != '\0') {
                                wcscpy(parola_da_cercare, selezione_casuale[selezione]);
                                break;
                            }
                        }
                        wcscpy(parola_da_cercare, selezione_casuale[rand()%9999]);

                        //Resetto gli array per la prossima iterazione
                        for(int i = 0; i < posiz; i++){
                            wcscpy(parole_che_seguono2[i], L"");
                            frequenze[i] = 0;
                        }
                        break;
                    }else{ 
                        conteggio++;
                        token = wcstok(NULL, L",", &state);
                    }
                }
                //Aumento l'indice per la prossima parola da inserire
                contatore_lunghezza_frase++;

                //Sposto il puntatore all'inizio del file
                rewind(fp);
            }
            //Chiusura file .csv
            fclose(fp);
            libera_array(dimensione_parole_che_seguono2, &parole_che_seguono2);
            
            //Pone la prima lettera della prima parola in maiuscolo
            frase_generata[0][0] = towupper(frase_generata[0][0]);

            //Apertura/creazione file .txt in cui inserire la frase generata
            FILE *fp2 = apertura_file_scrittura(1);
            
            //Mediante l'indice scorro tutta la frase_generata
            for(int i = 0; i < indice_frase_generata; i++){
                //Se l'elemento della frase_generata è un simbolo di punteggiatura pone l'elemento successivo con la prima lettera maiuscola
                if(wcscmp(frase_generata[i], L"!")==0 || wcscmp(frase_generata[i], L"?")==0 || wcscmp(frase_generata[i], L".")==0){
                    frase_generata[i+1][0] = towupper(frase_generata[i+1][0]);
                }
                //Stampo nel file la parola generata
                fwprintf(fp2, L"%ls ", frase_generata[i]);
            }
            //Chiusura file .txt per la generazione
            fclose(fp2);
            wprintf(L"\n-->Generazione frase completata correttamente.\n");
        
        }else if(carattere == L'h'){
            wprintf(L"\n\\------------------------------------------\\\n");
            wprintf(L"Lista comandi: \n");
            wprintf(L"    \\a Esegue l'analisi del file di testo per produrre il csv\n");
            wprintf(L"    \\b Esegue l'analisi del file csv per generare il testo\n");
            wprintf(L"    \\h Lista comandi\n");  
            wprintf(L"    \\e Termina il programma\n");
            wprintf(L"\\------------------------------------------\\\n");
        }
        //Chiusura del programma
        else if(carattere == L'e'){
            exit = 0;
        }else{
            wprintf(L"\n-->È stato digitato un comando non valido, premere h per la lista comandi\n");
        }
    }      
    //Chiusura programma senza errori
    return 0;
}
