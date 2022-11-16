#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 8
// Set up a new SoftwareSerial object
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

String check4answer(){
    String str = "";
    //while (mySerial.available() == 0) {}
    while (mySerial.available() > 0) {
      char c = mySerial.read();
      str += String(c);
    }
    Serial.println(str);
    return str;
}


String esp01cmd(String cmd) {
  Serial.println("sending: " + cmd);
  mySerial.println(cmd);
  delay(10);
  return check4answer();
}

void setup()  {
    // Define pin modes for TX and RX
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    
    // Set the baud rate for the SoftwareSerial object
    Serial.begin(9600);

// nel caso in cui ESP01 sia settato alla velocità di trasmissione 11500
// che per la softserial risulta troppo veloce, possiamo andare a settare
// la velocità a 9600. basterà eseguire il seguente codice una volta solamente.
//    mySerial.begin(115200);
//    delay(100);
//    esp01cmd("AT+UART=9600,8,1,0,0");
//    delay(1000);
//    mySerial.end();
//    delay(1000);


    mySerial.begin(9600);
    delay(1000);
    esp01cmd("AT");
    delay(1000);
    esp01cmd("AT+CWMODE=2");
    delay(1000);
    esp01cmd("AT+CWSAP=\"robottino\",\"robottino\",1,4");
    delay(1000);
    esp01cmd("AT+CIFSR"); //show AP IP address
    esp01cmd("AT+CIPMUX=0"); //allow up to 1 connections at the time
    
    
    Serial.println("ESP-01 Configuration Completed");
}

void loop() {
    Serial.println("loop...");
    while(esp01cmd("AT+CWLIF").substring(11,18) != "192.168") {
      Serial.println("no connections so far... still waiting");
      delay(1000);
    }
    // cerca e stampa l'ip del dispositivo
    String str = esp01cmd("AT+CWLIF"); // il comando at CWLIF serve a richiedere al modulo wi-fi gli indirizzi ip dei dispositivi connessi
    int startOfSTR = str.indexOf(',',18);  // cerca la virgola dal diciottesimo carattere della stringa
    String cellphoneIP = str.substring(11,startOfSTR); // estrae i caratteri a partire dall'undicesimo, fino alla virgola
    Serial.println(cellphoneIP);// printa l'ip del valore ricavato 
    Serial.println("Connection from remote device was Established!!!"); // stampa questa stringa

    // AT+CIPSTART=<id>,<type>,<remote address>,<remote port>[,(<local port>),(<mode>)]
    // AT+CIPSEND=[<id>,]<length>[,<ip>,<port>]
    // AT+CIPCLOSE=<id>
    
    //Socket Server: server in ascolto, pronto a ricevere pacchetti UDP da WIFI
    //Socket ID: 3
    //accept packets from any IP address/devices
    //Listen to local port 4567
    //outgoing packets could go to any remote host without restrictions...
    esp01cmd("AT+CIPCLOSE=3"); //close socket if for any reason it was already open
    esp01cmd("AT+CIPSTART=3,\"UDP\",\"0.0.0.0\",0,4567,2"); //starting UDP Socket Server 

    
    //esp01cmd("AT+CIPSTART=1,\"UDP\",\""+cellphoneIP+"\",1234"); //starting UDP Socket Client 
    
    delay(3000);

    while(true) {

      // dati ricevuti da Modulo WIFI
       // sintassi comandi per il robottino: a(avanti),d(destra),s(sinistra),i(indietro) sarebbero i nomi dei comandi  
       // poi inseriamo un numero per indicare la durata del comando
       // a, 100 = avanti per 100 millisecondi; d, 300 = girare a destra per 300 millisecondi
      str = mySerial.readString();
      if(str != "") {
        int startOfSTR = str.indexOf(":",10)+1;
        Serial.println("Received: "+str);
        Serial.println("Message: "+str.substring(startOfSTR));
        switch(str){
        
        }
          
      }
      
      // dati ricevuti da Monitor Seriale
      str = Serial.readString(); // legge la stringa inserita nel monitor seriale
      if(str != "") { // se la stringa non è vuota esegue l'if
        Serial.println("Received from Serial Monitor: "+str); // stampa la stringa più ciò che abbiamo scritto sul monitor seriale
        //String str1 = "AT+CIPSEND=1," + str.length(); NOT WORKING??? bug???
        String str1 = "AT+CIPSEND=3,"; // questa riga serve a inserire l'id 
        str1 = str1 + str.length() + ",\"" + cellphoneIP + "\",1234"; // questa riga serve a inserire la lunghezza, l'ip e la porta locale, per ricevere i dati
        //str1.concat(str.length());
        //Serial.println(str1);
        esp01cmd(str1); // eseguiamo il comando contenuto in str1
        esp01cmd(str);  // invia la stringa al modulo wifi 
      }
    }
}
