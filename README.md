# FEM-MUSina
Repository che contiene informazioni sui componenti e strumentazione
necessari per costruire, programmare e gestire l'irrigatore FEM-MUSina.

L'intero progetto riportato è soggetto alla licenza GPL V3 (vedi file
[LICENSE](https://github.com/musefablab/FEM-MUSina/blob/master/LICENSE)).

G.Berti, D.Caset, S.Corradini, T.Dallapè, N.Oradini, M.Perini, G.Toller

FEM-MUSina: a seguito di un'idea partita all'interno della 
[Fondazione Edmund Mach](http://www.fmach.it), il 
[MUSE FabLab](http://fablab.muse.it), laboratorio di fabbricazione
digitale del Museo delle Scienze di Trento, ha contribuito a realizzare
un prototipo di macchina per l'irrigazione domestica di piante in vaso.
Questo progetto è nato con lo scopo di riuscire ad annaffiare svariate
piante solo quando queste ne hanno bisogno. Per fare questo ogni vaso è
dotato di un sensore dell’umidità del suolo e di una propria linea
irrigua. Il sistema, governato da un hardware e da un software creati a
partire dalla piattaforma informatica Arduino, dosa l’acqua in modo
adeguato alle esigenze di ogni pianta. Molti pezzi speciali del
prototipo, ad esclusione di quelli che richiedevano tornitura, sono
stati realizzati con gli strumenti in dotazione al FabLab: taglio laser,
stampante 3D, fresatrice. Nella progettazione si è puntato ad abbassare
i costi riducendo al massimo il numero di elettrovalvole/pompe. Nella
versione attuale, basata su un distributore rotante, una sola
elettrovalvola/pompa può alimentare sedici linee, ma è previsto di
espanderne il numero. Il controller scansiona periodicamente i sensori
di umidità nei vasi e, in base alla risposta, dosa la quantità d’acqua
somministrata a ciascuna pianta.

Gli elementi previsti per la costruzione completa della macchina
comprendono:

- [scheda Arduino](https://store.arduino.cc/product/A000066)
- [shield (autocostruito)](https://github.com/musefablab/FEM-MUSina/tree/master/schematics/pcb)
- [motor shield Arduino](https://www.arduino.cc/en/Main/ArduinoMotorShieldR3)
- componenti elettronici necessari per il circuito Arduino documentati
nella cartella [DATASHEET](https://github.com/musefablab/FEM-MUSina/tree/master/datasheet)
- switch di fine corsa
- servo
- motorino passo passo (Nema 17) 200 passi/giro
- elettrovalvola / pompa (12V)
- cablaggio per i sensori d'umidità
- tubi per flusso acqua
- alimentatore 12V collegato a presa di corrente

I dati e le tavole dei componenti sono reperibili nelle varie
cartelle:

- visualizzazione del modello solido della macchina dalle varie
prospettive - [IMAGES](https://github.com/musefablab/FEM-MUSina/tree/master/images)
- disegni tecnici dei componenti in compensato spesso 8mm da tagliare ed
assemblare ad incastro per il supporto esterno della macchina - 
[LASER](https://github.com/musefablab/FEM-MUSina/tree/master/laser)
- modelli 3D dei componenti di supporto fabbricati con stampa 3D - 
[STL](https://github.com/musefablab/FEM-MUSina/tree/master/stl)
- disegni tecnici delle parti meccaniche in alluminio e in gomma (parte
inferiore e superiore del disco forato, base di supporto al disco,
supporto rotazionale per giunto elastico, cuscinetto, cinghia, sezione
frontale della macchina) - [DRAWING](https://github.com/musefablab/FEM-MUSina/tree/master/drawings)
- schemi dei collegamenti elettronici, breadboard, PCB Arduino - 
[SCHEMATICS](https://github.com/musefablab/FEM-MUSina/tree/master/schematics)
- codice di programmazione dell'Arduino - [SRC](https://github.com/musefablab/FEM-MUSina/tree/master/src/FEM-MUSina)


Assemblare le parti meccaniche tornite e stampate inserendo il tutto nel
case dell'irrigatore completo dei supporti e dei componenti di fissaggio
come mostrato dalle immagini dei particolari nelle cartelle
[DRAWING](https://github.com/musefablab/FEM-MUSina/tree/master/drawings),
[IMAGES](https://github.com/musefablab/FEM-MUSina/tree/master/images),
e [PICTURES](https://github.com/musefablab/FEM-MUSina/tree/master/pictures).
Realizzare lo [shield](https://github.com/musefablab/FEM-MUSina/tree/master/schematics/pcb)
costituito dal [multiplexer](https://github.com/musefablab/FEM-MUSina/blob/master/datasheet/74HC4051.pdf)
ed i morsetti che controllano l'accensione dell'elettrovalvola/pompa, lo
[switch di fine corsa](https://github.com/musefablab/FEM-MUSina/blob/master/datasheet/micro-switch.pdf)
e i sensori di umidità. Per terminare lo shield effettuare gli opportuni
collegamenti e saldature (servo) tra dispositivi e pin preimpostati.
Una volta collegato il motorino passo passo al motor shield, unire lo
shield e il motor shield tra loro ed ai piedini della scheda Arduino
predisposti.
Per i vari cablaggi basarsi sugli schemi
[FEM-MUSina-schematic.pdf](https://github.com/musefablab/FEM-MUSina/blob/master/schematics/FEM-MUSina-schematic.pdf)
e [FEM-MUSina-breadboard.pdf](https://github.com/musefablab/FEM-MUSina/blob/master/schematics/FEM-MUSina-breadboard.pdf).
Collegare il controllore tramite cavo USB al computer con installato il
software [Arduino](https://www.arduino.cc/en/Main/Software). Dopo aver
scaricato il file [FEM-MUSina.ino](https://github.com/musefablab/FEM-MUSina/blob/master/src/FEM-MUSina/FEM-MUSina.ino)
aprirlo con Arduino IDE e caricare il codice. A questo punto il 
microcontrollore Arduino sarà programmato. È comunque possibile
personalizzare il codice in base delle proprie esigenze modificando una
serie di costanti all'interno della sezione iniziale "*objects and
variables declarations and definitions list*" sotto la voce "*user
customizable variables*" del software FEM-MUSina.ino: numero dei vasi da
irrigare, soglie del valore di umidità rilevate da Arduino in ogni vaso,
numero di passi del tipo di motore in utilizzo tra due fori, tempo di
innaffiamento, tempo d'attesa per lo svuotamento del tubicino di
irrigazione, tempo che intercorre tra la fine di un ciclo di irrigazione
ed il successivo.
In ultimo collegare i tubicini per il passaggio dell'acqua
dall'elettrovalvola/pompa al foro d'immissione nel revolver e i tubicini
diretti ai vasi ai fori che si vuole considerare per l'annaffiatura.

Enjoy watering!
