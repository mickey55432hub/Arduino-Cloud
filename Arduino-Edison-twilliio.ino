//https://grabduck.com/s/koc3VEVo

#include "WString.h"
#include <Utils.h>
#include <Rest.h>
#include <TwiML.h>
#include <vector>
#include <math.h>

using namespace twilio;

vector<Var> vars;

// Twilio REST API version
const char API_VERSION[] = "2010-04-01";

// SMS-enabled Twilio phone number
const char CALLER_ID[] = "1480405xxxx";

// Twilio Account SID and Auth token
const char ACCOUNT_SID[] = " AC9c55339a5a070ae81e782117xxxxxxxx ";
const char ACCOUNT_TOKEN[] = "59e8819f3f5b530b97b84baexxxxxxxx ";

// Response from Twilio
const char* responseStr;

// Temperature sensor variables in Fahrenheit
const int THRESHOLD_TEMP = 100;
const int WAIT = 1000000;

// B value of the thermistor
const int B_THERMISTOR = 3975;                  

int analogValue = 0;
float degKelvin = 0;
float degC = 0;
float degF = 0;
float resistance = 0;

// Connect temperature sensor to Analog pin 1
const int tempSensorPin = 1;

// This function initializes the serial debug interface.
void setup() {
    Serial.begin(9600);  
}

// This function builds: 
//     - The SMS message being sent
//     - URL path
//     - Send SMS message to your cell phone from SMS-enabled phone
// Input: float tempF - current temperature in Fahrenheit
void sendSMS( float tempF ) { 
 
    // Convert tempF to string
    char degFstr[20];
    sprintf(degFstr, "%2f", degF);
    
    // Convert THRESHOLD_TEMP to string
    char thresholdStr[20];
    sprintf(thresholdStr, "%d", THRESHOLD_TEMP);
    
    // Build the SMS message
    char smsStr[100] = "Current temperature is ";
    strcat(smsStr, degFstr);
    strcat(smsStr, " F greater than threshold temp ");
    strcat(smsStr, thresholdStr);
    strcat(smsStr, " F.");
        
    // Twilio Account = /API_VERSION/Accounts/ACCOUNT_SID
    char twilioAcc[70] = "/";
    strcat(twilioAcc, API_VERSION);
    strcat(twilioAcc, "/Accounts/");
    strcat(twilioAcc, ACCOUNT_SID);
    
    // URL path = /API_VERSION/Accounts/ACCOUNT_SID//SMS/Messages
    char postReqStr[150];
    strcpy(postReqStr, twilioAcc);
    strcat(postReqStr, "/SMS/Messages");
    Serial.println(postReqStr); 
   
    // Twilio REST
    Rest rest(ACCOUNT_SID, ACCOUNT_TOKEN);
    
    // Send SMS
    vars.clear();
    vars.push_back(Var("To", "480-xxx-xxxx"));
    vars.push_back(Var("From", "480-405-xxxx"));
    vars.push_back(Var("Body", smsStr));
    string response = twilioRest.request(postReqStr, "POST", vars);
}

// This function read the analog pin value, converts to Fahrenheit, then
// returns current temperature in Fahrenheit.
float getTemperature()
{
    analogValue = analogRead(tempSensorPin);
  
    //Get the resistance of the sensor;
    resistance = (float)(1023 - analogValue) * 10000/analogValue;
 
    //Convert to temperature via datasheet; 
    degKelvin = 1/(log(resistance/10000)/B_THERMISTOR + 1/298.15);
  
    // Convert Kelvin to Celcius
    degC = degKelvin - 273.15;
  
    // Celcius to Fahrenheit
    degF = (degC * 9.0) / 5.0 + 32.0;
        
    return degF;
}

// The main loop read the current temperature using the temperature sensor. If the current temperature is greater than the threshold value, send a SMS message.
void loop() { 
    degF = getTemperature();
    if(degF > THRESHOLD_TEMP)
    {
        sendSMS(degF);
        delay(WAIT);
    }
}
