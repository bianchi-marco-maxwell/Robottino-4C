#include <SoftwareSerial.h>//richiama la libreria
//la classe è un'insieme di attributi e di metodi
#define rxPin 10	//serve a definire il ricevitore 
#define txPin 8		//serve a definire il trasmettitore
// Set up a new SoftwareSerial object
SoftwareSerial mySerialModuloWIFI =  SoftwareSerial(rxPin, txPin); //crea un oggetto della classe my serial, con parametri rx e tx

String check4answer(){
    String str = "";	// serve a creare una stringa vuota 
    while (mySerialModuloWIFI.available() > 0) { //crea un ciclo while che funziona fin quando gli si inviano dei dati
        char c=mySerialModuloWIFI.read();	//crea una funzione per leggere la stringa inviata(carattere per carattere) e la inserisce nella variabile c
        Serial.print(c);	//printa a schermo la stringa letta
        str += String(c);	//nella stringa vuota aggiunge la stringa letta 
    }
    return str;			//fa il return della stringa
}


String esp01cmd(String cmd) {	//
  Serial.println("sending: " + cmd);//printa sending più il contenuto di cmd
  mySerialModuloWIFI.println(cmd);//printa il contenuto di cmd
  delay(10);//pausa di 10 millisecondi
  return check4answer();//fa il return della funzione
}

void setup()  {
    // definisce la pin modes di TX e RX
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    
    // Set the baud rate for the SoftwareSerial object
    mySerialModuloWIFI.begin(115200);//specifica la velocita di trasmissione
    Serial.begin(9600);//specifica la velocita di trasmissione
    delay(1000);//pausa di 1000 millisecondi
    esp01cmd("AT");//manda il comando at per vedere se il circuito è funzionante
    delay(1000);//pausa di 1000 millisecondi
    esp01cmd("AT+CWMODE=2");//seleziona la modalità del wi-fi, in questo caso access point
    delay(1000);//pausa di 1000 millisecondi
    esp01cmd("AT+CWSAP=\"robottino\",\"robottino\",1,4");//impostiamo ssid e pasword del wi-fi
    delay(1000);//pausa di 1000 millisecondi
    esp01cmd("AT+CIFSR"); //mostra gli indirizzi AP(acess point) IP 
    esp01cmd("AT+CIPMUX=1"); //da la possibilita di mettere un numero massimo di dispositivi connessi
    
    
    Serial.println("ESP-01 Configuration Completed");//printa questa stringa
}

void loop() {
    Serial.println("loop...");//printa questa stringa
    while(esp01cmd("AT+CWLIF").substring(11,18) != "192.168") { //controlla se c'e stata la connessione di un dispositivo, confrontando l'indirizzo IP del client (tramite il comando AT) e quello inserito
      Serial.println("no connections so far... still waiting");//printa questa stringa
      delay(1000);//pausa di 1000 millisecondi
    }

    Serial.println("Connection from remote device was Established!!!");//printa questa stringa
    //Socket ID: 3
    //accept packets from any IP address/devices
    //Listen to local port 4567
    //outgoing packets could go to any remote host without restrictions...
    esp01cmd("AT+CIPSTART=3,\"UDP\",\"0.0.0.0\",0,4567,2"); //starting UDP Socket Server 
    
//    String str = ;    
//    Serial.println("received: "+esp01cmd("AT+CWLIF").substring(11,18));
    delay(3000);//pausa di 3000 millisecondi
    while(true) {
      String str = mySerialModuloWIFI.readString();//legge la stringa inserita
      if(str != "") {
        int startOfSTR = str.indexOf(":",10)+1;
        Serial.println("Received: "+str.substring(startOfSTR));
        //Serial.println(startOfSTR);
      }
    }

}   
