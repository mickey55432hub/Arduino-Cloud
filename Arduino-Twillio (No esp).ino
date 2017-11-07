

const char PHONE_FROM[] = "xxxxxxxxxxx"; // Your Twilio's phone number, including country code
const char PHONE_TO[] = "xxxxxxxxxxx"; // Any phone number including country code, BUT if you only have a free account this can only be your verified number
const char TWILIO_ACCOUNT_SID[] =  "xxxxxxxxxxxxxxx"; // Your Twilio's ACCOUNT SID
const char TWILIO_TOKEN[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // Your Twilios TOKEN
const char TEXT_MESSAGE_BODY[]  = "Your+house+is+on+fire"; // URL encoded text message.
char data[200]; // HTTP POST data string. Increase size as required.
               
#define HOST "iot-https-relay.appspot.com"
#define PORT 80
 
void setup() {
  char buffer[50];
 
  // This might work with other firmware versions (no guarantees)
  // by providing a string to ID the tail end of the boot message:
  
  // comment/replace this if you are using something other than v 0.9.2.4!
  wifi.setBootMarker(F("Version:0.9.2.4]\r\n\r\nready"));
 
  softser.begin(9600); // Soft serial connection to ESP8266
  Serial.begin(57600); while(!Serial); // UART serial debug
 
  Serial.println(F("Adafruit ESP8266 Phone Call"));
 
  // Test if module is ready
  Serial.print(F("Hard reset..."));
  if(!wifi.hardReset()) {
    Serial.println(F("no response from module."));
    for(;;);
  }
  Serial.println(F("OK."));
 
  Serial.print(F("Soft reset..."));
  if(!wifi.softReset()) {
    Serial.println(F("no response from module."));
    for(;;);
  }
  Serial.println(F("OK."));
 
  Serial.print(F("Checking firmware version..."));
  wifi.println(F("AT+GMR"));
  if(wifi.readLine(buffer, sizeof(buffer))) {
    Serial.println(buffer);
    wifi.find(); // Discard the 'OK' that follows
  } else {
    Serial.println(F("error"));
  }
 
  Serial.print(F("Connecting to WiFi..."));
  if(wifi.connectToAP(F(ESP_SSID), F(ESP_PASS))) {
 
    // IP addr check isn't part of library yet, but
    // we can manually request and place in a string.
    Serial.print(F("OK\nChecking IP addr..."));
    wifi.println(F("AT+CIFSR"));
    if(wifi.readLine(buffer, sizeof(buffer))) {
        Serial.println(buffer);
        wifi.find(); // Discard the 'OK' that follows
 
        Serial.print(F("Connecting to host..."));
 
        Serial.print("Connected..");
        wifi.println("AT+CIPMUX=0"); // configure for single connection, 
                                     //we should only be connected to one SMTP server
        wifi.find();
        wifi.closeTCP(); // close any open TCP connections
        wifi.find();
 
        if(wifi.connectTCP(F(HOST), PORT)) {
        Serial.print(F("OK\nRequesting page..."));
 
 
        strcat(data,"From=");
        strcat(data,PHONE_FROM);
        strcat(data,"&");
 
        strcat(data,"To=");
        strcat(data,PHONE_TO);
        strcat(data,"&");
 
        strcat(data,"sid=");
        strcat(data,TWILIO_ACCOUNT_SID);
        strcat(data,"&");
 
        strcat(data,"token=");
        strcat(data,TWILIO_TOKEN);
        strcat(data,"&");
 
        strcat(data,"Body=");
        strcat(data,TEXT_MESSAGE_BODY);
        
        wifi.httpPost("iot-https-relay.appspot.com","/twilio/Messages",data);
 
        wifi.closeTCP();
        while(1);
        } else { // TCP connect failed
        Serial.println(F("D'oh!"));
        }
        
    } else { // IP addr check failed
      Serial.println(F("error"));
    }
  } else { // WiFi connection failed
    Serial.println(F("FAIL"));
  }
}
 
void loop() {
 
}
