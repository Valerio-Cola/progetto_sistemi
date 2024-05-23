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
#include <errno.h>
int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "");  
    //Codifica inserita per stampare successivamente le variabili nel file double con il punto eg. 1.15 invece che la virgola eg. 3,14
    setlocale(LC_NUMERIC, "C");
    
    wchar_t input_lunghezza_frase[100]; 
    int lunghezza_frase_generata = 0;

    while(lunghezza_frase_generata <= 0){
        

        wprintf(L"\n<-Inserisci lunghezza frase generata: ");
        fgetws(input_lunghezza_frase, sizeof(input_lunghezza_frase)/sizeof(wchar_t), stdin);

        lunghezza_frase_generata = wcstol(input_lunghezza_frase, NULL, 10);
        wprintf(L"Lunghezza frasen %d\n", lunghezza_frase_generata);
        wchar_t c;
        if ((c = getwchar()) != WEOF) {
            ungetwc(c, stdin);
        }
    }
    return 0;

}
