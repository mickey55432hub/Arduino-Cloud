#include <Ethernet.h>
#include <SPI.h>
#include <UbidotsEthernet.h>
#define ID  "59ea2ff7c03f975607ffcd5f"  // Put here your Ubidots variable ID
#define TOKEN  "A1E-o4LrChDYfI5jOlHGr8rbJJyu25MZqw"  

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 177);
Ubidots client(TOKEN);

void setup() 
{
    Serial.begin(9600);
    if (Ethernet.begin(mac) == 0) {
          Ethernet.begin(mac, ip);
    }
    delay(1000);
}



void loop() {
    float temperature = 78;
    for(int i=10;i>0;i--)
    {
      temperature=temperature+10;
    client.add(ID, temperature);
    client.sendAll();
    delay(10000);
    }
    
}
