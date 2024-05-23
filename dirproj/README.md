
# Progetto SO Modulo II
## Cola Valerio 2060062

Il programma permette all'utente di scegliere tramite terminale quale operazione effettuare:

## Lista comandi: 
 
```bash
    \a Esegue l'analisi del file di testo per produrre il csv
    \b Esegue l'analisi del file csv per generare il testo
    \h Lista comandi
    \e Termina il programma
```
**Qualsiasi altro input da parte dell'utente verrà ignorato.**



## Compilazione e esecuzione
Assicurarsi che ci si trovi nella stessa cartella del progetto:
```bash
cd /dirproj
``` 

Spostarsi nella cartella corretta in base al tipo di processo:
```bash
cd /proj_multiproc
cd /proj_monoproc
```

Compilare in entrambi i casi il programma con un semplice:

```bash
make
``` 

Ed eseguirlo con:

```bash
./final
```
È richiesto all'utente di portare a termine ogni comando, in caso contrario sarà necessario terminare il programma con ctrl+c.



## Funzionamento generale del programma

### Prima Parte

Verrà richiesto all'utente di inserire mediante terminale: 
- File .txt da analizzare per generare la tabella 
- File .csv su cui scriverla.

Il programma effettuerà un **controllo** per verificare che i file inseriti dall'utente siano validi, saranno controllate le estensioni e nomi. 

Ogni parola/punteggiatura presente nel file .txt verrà inserita in 2 array, uno che conterrà il file intero e un'altro che conterrà ogni parola/punteggiatura solo una volta. 

Successivamente per ogni parola/punteggiatura viene contato il numero di volte che appare e da chi è seguita, ciò viene salvato in un'array. Nel .csv per ogni parola viene stampato 'parola_che_segue,frequenza' attraverso i dati salvati precedentemente.

### Seconda Parte

Sarà richiesto all'utente di inserire mediante terminale:
- File .csv da analizzare
- Lunghezza della frase in parole da generare 
    Saranno considerati input validi tutti quelli che sono o iniziano con numeri, le cifre digitate non devono essere più di 99. 
    Eg. Validi:
        56 4 --> 56
        56dhsf3 --> 56
        56 fd54 --> 56
        123456789999fyu -->  12345678999 

- La possibilità di far iniziare la frase con una parola specifica
- File .txt su cui scrivere la frase appena generata

Anche in questo caso saranno effettuati **controlli** appositi sugli input utente riguardante correttezza nomi e estensioni dei file e correttezza della parola con cui iniziare(presenza di spazi e numeri).

Il programma esegue il conteggio del numero di righe del file .csv così da poter scegliere una parola di inizio casuale nel caso in cui l'utente decida di non inserire una parola di inizio. il file sarà considerato come non valido se non saranno presenti almeno più di due righe.
**Nel caso in cui la parola digitata non sia presente nel .csv oppure considerata come non valida non verrà stampato nulla nel file .txt oppure ne verrà generato uno vuoto**

Successivamente verrà presa la prima parola di una riga del .csv e verranno salvate le parole da cui è seguita assieme alle relative frequenze per poi selezionarne una, che sarà salvata in un array.
La parola selezionata verrà cercata nel .csv per poter immagazzinare le parole che la seguono con relative frequenze e selezionarne una. Così via affinchè non si raggiunge il numero di parole desiserate dall'utente.
L'array verrà poi stampato sul file .txt.

**Tutti i file di input/output dovranno trovarsi/verranno generati nella rispettiva cartella del programma**

