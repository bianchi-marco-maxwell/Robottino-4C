#include <SoftwareSerial.h>// va a richiamare la libreria "SoftwareSerial.h"

#define rxPin 10
#define txPin 8
//Configura un nuovo oggetto SoftwareSerial
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

String check4answer(){
    String str = "";
    while (mySerial.available() > 0) {
        char c=mySerial.read();
        Serial.print(c);
        str += String(c);
    }
    return str;
}


String esp01cmd(String cmd) {
  Serial.println("sending: " + cmd);
  mySerial.println(cmd);
  delay(10);
  return check4answer();
}

void setup()  {
    // Definisce le modalità  di pin per TX e RX
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    
    // Imposta la velocità di trasmissione per l'oggetto SoftwareSerial
    mySerial.begin(115200);
    Serial.begin(9600);
    delay(1000);
    esp01cmd("AT");
    delay(1000);
    esp01cmd("AT+CWMODE=2"); // fa in modo che il robottino funzuìioni come acces point e quindi come server
    delay(1000);
    esp01cmd("AT+CWSAP=\"robottino\",\"robottino\",1,4"); //imposta ssid, poi la password, il canale e la crittografia 
    delay(1000);
    esp01cmd("AT+CIFSR"); //mostra l'indirizzo IP dell'AP
    esp01cmd("AT+CIPMUX=1"); //consente ad un massimo di 1 connessione alla volta
    
    
    Serial.println("ESP-01 Configuration Completed");
}

void loop() {
    Serial.println("loop...");
    while(esp01cmd("AT+CWLIF").substring(11,18) != "192.168") {
      Serial.println("no connections so far... still waiting");
      delay(1000);
    }

    Serial.println("Connection from remote device was Established!!!");
    //ID socket: 3
    //accetta pacchetti da qualsiasi indirizzo/dispositivo IP
    //Ascolta la porta locale 4567
    //i pacchetti in uscita possono essere inviati a qualsiasi host remoto senza restrizioni
    esp01cmd("AT+CIPSTART=3,\"UDP\",\"0.0.0.0\",0,4567,2"); //avvio del server socket UDP

    

    delay(3000);
    while(true) {
      String str = mySerial.readString();
      if(str != "") {
        int startOfSTR = str.indexOf(":",10)+1;
        Serial.println("Received: "+str.substring(startOfSTR));
      }
    }

}
