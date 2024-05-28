#ifndef FUNZIONI_H
#define FUNZIONI_H

#include <wchar.h>

void inserisci_parola(int punteggiatura);
void concatena_lettera();
int controllo_lettera(wchar_t lettera);
FILE* apertura_file_scrittura(int codice_file);
FILE* apertura_file_analisi(int codice_file);
wchar_t* stampa_tabella( wchar_t **parole_che_seguono, wchar_t **parole_uniche, int *conteggio_parole_che_seguono, int *conteggio_parola_unica, int i, int indice_parole_che_seguono);
void libera_array(int size, wchar_t ***array);
void alloca_memoria(int size, wchar_t ***array);
void rialloca_array(int size, wchar_t***array);
#endif
