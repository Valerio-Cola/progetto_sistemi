#include "funzioni.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
#include <time.h>
#include <math.h>

extern wchar_t **parole_uniche;
extern wchar_t parola[31];
extern int indice_parola;
extern int indice_tutto_file;
extern wchar_t **tutto_il_file;
extern wchar_t lettera;
extern int indice_parole_uniche;
//extern int cod_file;

//Inserisce parola/punteggiatura completa negli array tutto_il_file e parole_uniche
void inserisci_parola(int punteggiatura){    
    
    //Se flag indica che bisogna inserire una parola
    if(punteggiatura==0){

        //Inserisce carattere di fine stringa
        parola[indice_parola] = '\0';

        //Resetta l'indice per la prossima parola
        indice_parola = 0;

        //Inserisce in tutto_il_file e incrementa puntatore per prossima posizione
        wcscpy(tutto_il_file[indice_tutto_file], parola);
        indice_tutto_file++;

        //Flag per verificare se la parola è gia presente in parole_uniche o meno
        int inserisco = 0;

        //Prendo tutti gli elementi di parole_uniche
        for(int i = 0; i < indice_parole_uniche; i++){

            //Se già presente 
            if(wcscmp(parola, parole_uniche[i])){
                continue;

            }else{
                inserisco = 1;

            } 
        }

        //Non è stata trovata allora la inserisco e incremento indice per prossima parola
        if(inserisco == 0){
            wcscpy(parole_uniche[indice_parole_uniche], parola);
            indice_parole_uniche++;
        }

    //Se flag indica che bisogna inserire una punteggaitura    
    }else{
        //Inserisce in tutto_il_file e incrementa puntatore per prossima posizione
        wcscpy(tutto_il_file[indice_tutto_file], &lettera);
        indice_tutto_file++;

        //Mediante un flag verifica che non sia già presente in parole_uniche
        int inserisco = 0;

        //Prendo tutti gli elementi di parole_uniche
        for(int i = 0; i < indice_parole_uniche; i++){
            
            //Se non è presente non verrà trovata e quindi inserita successivamente
            if(((int)lettera != parole_uniche[i][0])){
                continue;

            }else{
                inserisco = 1;
            }    
        }

        //Se non è prensente viene inserita
        if(inserisco == 0){
            wcscpy(parole_uniche[indice_parole_uniche], &lettera);
            indice_parole_uniche++;
        }
    }


}

//Inserisce il nuovo carattere e incrementa il puntatore della parola per la prossima lettera. 
void concatena_lettera(){
    parola[indice_parola] = towlower(lettera);
    indice_parola++;
}

//Stampa la tabella in un'Array
wchar_t* stampa_tabella(wchar_t **parole_che_seguono, wchar_t **parole_uniche, int *conteggio_parole_che_seguono, int *conteggio_parola_unica, int i, int indice_parole_che_seguono){
    
    int size = 100000;
    wchar_t* stringa = malloc(size * sizeof(wchar_t));
    wchar_t da_concatenare[100];


    //Inserisco la parola analizzata
    swprintf(da_concatenare, 100,L"%ls",parole_uniche[i]);
    wcscat(stringa, da_concatenare);
    
    //Il for effettua n cicli affinchè possa prendere tutte la parole che seguono 
    for (int m=0; m < indice_parole_che_seguono; m++){
        //In ogni caso inserisco parola che segue e la sua frequenza (calcolata come numero di volte che segue la parola analizzata fratto 
        //numero di volte che appare la parola analizzata), gli if controllano il numero risultante per poterlo formattare correttamente 
        if ((double)conteggio_parole_che_seguono[m]/(double)conteggio_parola_unica[i] == 1){
            swprintf(da_concatenare, 100,L",%ls,1", parole_che_seguono[m]);
            wcscat(stringa, da_concatenare);
        }else if((double)conteggio_parole_che_seguono[m]/(double)conteggio_parola_unica[i] == 0.5){
            swprintf(da_concatenare, 100,L",%ls,0.5", parole_che_seguono[m]);
            wcscat(stringa, da_concatenare);
        }else{
            //Ciò mi permette di avere un numero approssimato per difetto che non faccia sforare la somma delle frequenze totali 
            double division = (double)conteggio_parole_che_seguono[m]/(double)conteggio_parola_unica[i];
            division = floor(division * 10000) / 10000;
            swprintf(da_concatenare, 100,L",%ls,%.4f", parole_che_seguono[m], division);
            wcscat(stringa, da_concatenare);
        }  
    }
    //Carattere di nuova linea per la prossima parola da analizzare
    //fwprintf(puntatore, L"\n");
    wcscat(stringa, L"\n");
    return stringa;
}

//Verifica la tipologia di carattere e ritorna un intero, che sarà utilizzato nella prima parte del codice
int controllo_lettera(wchar_t lettera){
    if((lettera >= L'A' && lettera <= L'Z') || (lettera >= L'a' && lettera <= L'z') || lettera == L'ò' || lettera == L'à' || lettera == L'è' || lettera == L'é' || lettera == L'ì' || lettera == L'ù' || lettera == L'Ò' || lettera == L'À' || lettera == L'È' || lettera == L'É' || lettera == L'Ì' || lettera == L'Ù'){
        return 0;
    }else if(lettera == L'.' || lettera == L'?' || lettera == L'!'){
        return 1;
    }else if(lettera == L'\''){
        return 2;
    }else{
        return -1;
    }
}

/*
In queste due funzioni viene ritornato un puntatore a file, entrambe si occupano di 
richiedere all'utente di inserire un file di input e verificare che sia corretto, in caso di scrittura
su file si può anche inserire il nome di un file da creare. 
Il controllo viene effettuato sull'estensione del file scritto che è legata a codice_file
1 per i .txt e 2 per i .csv, è supportata anche la creazione di file con caratteri speciali. */

FILE* apertura_file_scrittura(int codice_file){
    wchar_t file_da_cercare[100];
    char file_da_cercare_char[100];
    int file_trovato = 0;

    FILE* puntatore = NULL;
    while(file_trovato == 0) {
        //Input file
        wprintf(L"\n<-Digitare file su cui scrivere: ");
        wscanf(L"%ls",file_da_cercare);
        getwchar();

        //Conversione in multibyte 
        wcstombs(file_da_cercare_char, file_da_cercare, 100);

        //Verifica se l'estensione è presente nel nome del file
        char* estensione = strrchr(file_da_cercare_char, '.');
        if (estensione == NULL) {
            wprintf(L"-->Estensione del file non presente nell'input digitato.\n");
            continue;
        }
        estensione++;

        //Controlli per verificare che sia stato scritto correttamente il file con la sua estensione 
        if((codice_file == 1 && strcmp(estensione, "txt") != 0 ) || (codice_file == 2 && strcmp(estensione, "csv") != 0 )){
            wprintf(L"-->Impossibile creare/aprire il file, assicurarsi di averlo scritto correttamente.\n");
        } else {
            //Apre il file 
            puntatore = fopen(file_da_cercare_char,"w");
            
            file_trovato = 1;
            wprintf(L"-->Proseguo a scrivere su %ls...\n", file_da_cercare);
        }
    }
    return puntatore;
}

FILE* apertura_file_analisi(int codice_file){
    wchar_t file_da_cercare[100];
    char file_da_cercare_char[100];
    int file_trovato = 0;

    FILE* puntatore = NULL;
    while(file_trovato == 0) {

        wprintf(L"\n<-Digitare file da analizzare: ");
        wscanf(L"%ls",file_da_cercare);
        getwchar();
        
        wcstombs(file_da_cercare_char, file_da_cercare, 100);

        char* estensione = strrchr(file_da_cercare_char, '.');
        if (estensione == NULL) {
            wprintf(L"-->Estensione del file non presente nell'input digitato.\n");
            continue;
        }
        estensione++; 
        puntatore = fopen(file_da_cercare_char,"r");

        if(puntatore == NULL || (codice_file == 1 && strcmp(estensione, "txt") != 0 ) || (codice_file == 2 && strcmp(estensione, "csv") != 0 )){
            wprintf(L"-->Impossibile aprire il file, assicurarsi di averlo scritto correttamente.\n");
        } else {
            file_trovato = 1;
            wprintf(L"-->Proseguo ad analizzare %ls...\n", file_da_cercare);
        }
        
    }
    return puntatore;
}

//Libera la memoria allocata aper gli array settandola a null passando il puntatore all'array originale
void libera_array(int size, wchar_t ***array){

    //Con (*array) accedo al valore puntato dal puntatore
    for(int i = 0; i < size; i++){
        free((*array)[i]);
        (*array)[i] = NULL;
    }
    free(*array);
    *array = NULL;
}

//Alloca una quantità iniziale size * 32bit, per ogni elemento alloco altri 31 spazi per contenere la parola
void alloca_memoria(int size, wchar_t ***array){
    (*array) = calloc(size, sizeof(wchar_t*));
    for(int i = 0; i < size; i++){
        (*array)[i] = calloc(31, sizeof(wchar_t));
        memset((*array)[i], 0, sizeof(wchar_t) * 31);
    }    
}

void rialloca_array(int size, wchar_t ***array){
    (*array) = realloc((*array), sizeof(wchar_t*)*size);
    for(int i = size/2; i < size; i++){
        (*array)[i] = malloc(sizeof(wchar_t)*31);
        wcscpy((*array)[i], L"");
    }
}

