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
    esp01cmd("AT+CIPMUX=1"); //allow up to 1 connections at the time
    
    
    Serial.println("ESP-01 Configuration Completed");
}

void printlnWIFI(String str, String cellIP) {
      if(str != "") {
        Serial.println("Received from Serial Monitor: "+str);
        //String str1 = "AT+CIPSEND=1," + str.length(); NOT WORKING??? bug???
        String str1 = "AT+CIPSEND=3,";
        str1 = str1 + str.length() + ",\"" + cellIP + "\",1234";
        //str1.concat(str.length());
        //Serial.println(str1);
        esp01cmd(str1);
        esp01cmd(str);        
      }    
}

void loop() {
    Serial.println("loop...");
    while(esp01cmd("AT+CWLIF").substring(11,18) != "192.168") {
      Serial.println("no connections so far... still waiting");
      delay(1000);
    }

    String str = esp01cmd("AT+CWLIF");
    int startOfSTR = str.indexOf(',',18);  //IP finsce prima della virgola
    String cellphoneIP = str.substring(11,startOfSTR);
    Serial.println(cellphoneIP);
    Serial.println("Connection from remote device was Established!!!");

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
      str = mySerial.readString();
      if(str != "") {

        int startOfSTR = str.indexOf(":",10)+1;
        Serial.println("Received: "+str);
        Serial.println("Message: "+str.substring(startOfSTR));
        str = str.substring(startOfSTR);
          
        char cmd = str[0];
        int durata = str.substring(1).toInt();
  
        Serial.println(cmd);
  
        switch (cmd){
        case 'a': 
            printlnWIFI("Avanti di " + String(durata), cellphoneIP);
            break;
        case 'i': 
            printlnWIFI("Indietro di " + String(durata), cellphoneIP);
            break;
        default:
            printlnWIFI("Comando non esistente: ", cellphoneIP);
            break;
        }
          
      }

      // dati ricevuti da Monitor Seriale
      str = Serial.readString(); 
      printlnWIFI(str, cellphoneIP);
    }
}
