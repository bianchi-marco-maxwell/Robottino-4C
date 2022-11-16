# Robottino-4C by Bianchi
Robottino per la classe 4C del Maxwell

Implementa la ricezione di pacchetti UDP dal modulo WiFi (ESP01) configurato come Access Point, e l'inoltro del contenuto (testo) al monitor seriale collegato all'Arduino.

## Procedura di invio
1. collegarsi dallo smartphone alla rete wifi offerta dal robottino (ssid:robottino, password:robottino)
2. usare applicazione "UDP Monitor" (per Android) su telefonino
3. inserire Indirizzo IP dell'Access Point (192.168.4.1) e la porta del UDP Server (4567)
4. inserire testo
5. inviare testo (premendo l'appostio pulsante offerto dall'APP)

Risultato: Monitor Seriale dovrebbe mostrare il testo inviato

## Descrizione del sistema
Il robottino è controllato da una scheda arduino.

L'arduino da una parte è collegato al pc tramite un cavo usb mentre dall'altra parte è collegato a un modulo wifi.

il pc gestisce l'invio e la ricezione dei dati mediante un software chiamato Monitor Seriale.

Nel modulo wifi il software UDP Server gestisce la ricezione e l'invio di pacchetti UDP/IP. Una volta ricevuto un pacchetto UDP ne estrae il contenuto e lo inoltra all'arduino mediante la porta UART.

L'arduino viene utilizzato come tramite tra il Monitor Seriale e il modulo wifi, il modulo wifi e l'arduino comunicano attraverso una porta UART che riceve i dati  inviati dallo smartphone, che per mezzo di un'altra UART collegata ad una porta USB arrivano al Monitor Seriale. 

