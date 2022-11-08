#include <SoftwareSerial.h>
//libreria che mi fa accedere alla classe SoftwareSerial
//attributi: pinTx e pin Rx
//metodi: begin(), available(), read(), print(), println()
//begin() usato per configurare la velocità della porta seriale
//print() e println() usati per inviare i dati da Arduino a modulo WIFI
//available() usato per verificare presenza dati trasmessi da moduloWIFI per Arduino
//read() usato per ricevere i dati provenienti da moduloWIFI

#define rxPin 10
#define txPin 8
// Set up a new SoftwareSerial object
SoftwareSerial serialModuloWIFI =  SoftwareSerial(rxPin, txPin);

// restituisce la risposta del modulo WIFI
String check4answer(){
    String str = ""; 
    while (serialModuloWIFI.available() > 0) { // 
        char c = serialModuloWIFI.read();
        Serial.print(c);
        str += String(c);
    }
    return str;
}

//Invia comandi al modulo WIFI (esp01)
//riceve il comando da inviare (nella stringa cmd)
//aspetta e restituisce la risposta del modulo WIFI
String esp01cmd(String cmd) {
  Serial.println("sending: " + cmd); //stampo su Monitor Seriale il comando che viene inviato al modulo WIFI (DEBUG)
  serialModuloWIFI.println(cmd); //invio il comando al moduloWIFI
  delay(10);
  String risposta = check4answer(); //verifico la risposta del modulo WIFI
  return risposta; //restituisco la risposta del modulo WIFI
}

void setup()  {
    // Define pin modes for TX and RX
    // pinMode(rxPin, INPUT);
    // pinMode(txPin, OUTPUT);
    
    // Set the baud rate for the SoftwareSerial object
    serialModuloWIFI.begin(115200);
    Serial.begin(9600);
    delay(1000);
    
    //COMANDI "AT" per configurare il modulo WIFI
    esp01cmd("AT"); //verifico che il moduloWIFI sia attivo e comunicante 
    delay(1000);
    esp01cmd("AT+CWMODE=2"); // seleziona modalità Access Point
    delay(1000);
    esp01cmd("AT+CWSAP=\"robottino\",\"robottino\",1,4"); // impostiamo SSID e PWD del Access Point
    delay(1000);
    esp01cmd("AT+CIFSR"); //show Access Point IP address
    esp01cmd("AT+CIPMUX=1"); //allow up to 1 connections at the time
    
    
    Serial.println("ESP-01 Configuration Completed");
}

void loop() {
    
    Serial.println("loop...");
    
    //AT+CWLIF restituisce IP del client(telefono) connesso al modulo WIFI
    // loop che persiste fino alla connessione di un Client
    while(esp01cmd("AT+CWLIF").substring(11,18) != "192.168") { // Quando il Client è connesso è presente il suo indirizzo IP
      Serial.println("no connections so far... still waiting");
      delay(1000);
    }

    Serial.println("Connection from remote device was Established!!!");

    //Socket ID: 3
    //accept packets from any IP address/devices
    //Listen to local port 4567
    //outgoing packets could go to any remote host without restrictions...
    esp01cmd("AT+CIPSTART=3,\"UDP\",\"0.0.0.0\",0,4567,2"); //starting UDP Socket Server 
    
    
//    String str = ;    
//    Serial.println("received: "+esp01cmd("AT+CWLIF").substring(11,18));
    delay(3000);
    while(true) {
      String str = serialModuloWIFI.readString();
      if(str != "") {
        int startOfSTR = str.indexOf(":",10)+1;
        Serial.println("Received: "+str.substring(startOfSTR));
        //Serial.println("Received: "+str);
        //Serial.println(startOfSTR);
      }
    }

}
