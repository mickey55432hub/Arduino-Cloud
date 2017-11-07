#include <Bridge.h>
#include <BridgeClient.h>
#include <HttpClient.h>

void setup() 
{
  // Bridge takes about two seconds to start up
  // it can be helpful to use the on-board LED
  // as an indicator for when it has initialized
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  Bridge.begin();
  digitalWrite(12, HIGH);

  Serial.begin(9600);

}

void loop()
{
  // Initialize the client library
  HttpClient client;

  // Make a HTTP request:
  client.get("http://www.arduino.cc/asciilogo.txt");

  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  
  delay(5000);
}
