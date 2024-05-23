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

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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


int dimensione_parole_uniche = 5000;
int dimensione_tutto_file = 5000;
int dimensione_parole_che_seguono2 = 1000;

int main(int argc, char const *argv[]) {
    setlocale(LC_ALL, "");  
    setlocale(LC_NUMERIC, "C");
    int status;
    
    /* int exit = 1;
    while(exit != 0){
        wprintf(L"\n<-Cosa si desidera effettuare: ");
        wchar_t input[100];
        fgetws(input, 100, stdin);

        if(wcslen(input) != 2){   
            wprintf(L"\n-->È stato digitato un comando non valido, premere h per la lista comandi\n");
            continue;
        }
        wchar_t carattere = input[0];
        if(carattere == L'a'){ */

    pid_t p1, p2;

    p1 = fork();

    if (p1 < 0) {
        printf("Errore\n");
    } else if (p1 == 0) {
        alloca_memoria(dimensione_tutto_file, &tutto_il_file);
        alloca_memoria(dimensione_parole_uniche, &parole_uniche);

        FILE *pF = apertura_file_analisi(1);
        
        while((lettera = fgetwc(pF)) != WEOF){

            if(indice_tutto_file == dimensione_tutto_file){
                dimensione_tutto_file *= 2;
                rialloca_array(dimensione_tutto_file, &tutto_il_file);
            }
            if(indice_parole_uniche == dimensione_parole_uniche ) {
                dimensione_parole_uniche *= 2;
                rialloca_array(dimensione_parole_uniche, &parole_uniche);
            }
            if(controllo_lettera(lettera) == 0 ){

                concatena_lettera();
            }else if(controllo_lettera(lettera)==2){

                concatena_lettera();
                inserisci_parola(0);
            }else if(indice_parola > 0){

                inserisci_parola(0);
            }
            if (controllo_lettera(lettera) == 1){
                inserisci_parola(1);


            }
        }
        fclose(pF);
        
    } else {

        waitpid(p1, &status, 0);
        p2 = fork();

        for(int j = 0; j < 30; j++){
            wprintf(L"C%ls\n",tutto_il_file[j]);
        }
        wprintf(L"HELO\n");
        
        if (p2 < 0) {
            printf("Errore\n");
        } else if (p2 == 0) {
    
            wcscpy(tutto_il_file[0], tutto_il_file[indice_tutto_file-1]);
            int conteggio_parola_unica[indice_parole_uniche];

            int dimensione_parole_che_seguono = 5000;

            wchar_t **parole_che_seguono; 
            parole_che_seguono = malloc(sizeof(wchar_t*) * dimensione_parole_che_seguono);
            for(int i = 0; i < dimensione_parole_che_seguono; i++){
                parole_che_seguono[i] = malloc(sizeof(wchar_t) * 31);
            } 
            int indice_parole_che_seguono = 0;
            int *conteggio_parole_che_seguono = malloc(sizeof(int) * dimensione_parole_che_seguono);

            FILE *fP2 = apertura_file_scrittura(2);

            for(int i = 0; i < indice_parole_uniche; i++){   
                int conteggio_parole = 0;
                for (int j = 0; j < indice_tutto_file; j++){
                
                    if(indice_parole_che_seguono == dimensione_parole_che_seguono) {
                        dimensione_parole_che_seguono *= 2;
                        rialloca_array(dimensione_parole_che_seguono, &parole_che_seguono);
                    }
                    if (wcscmp(parole_uniche[i], tutto_il_file[j]) == 0){
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
            //Chiusura file .csv
            fclose(fP2);
            wprintf(L"-->Il file csv è stato generato correttamente.\n");


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

        } else {
            waitpid(p2, &status, 0);
        }
    }
/*         }else if(carattere == L'h'){
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
    }  */
    //Chiusura programma senza errori
    return 0; 
}