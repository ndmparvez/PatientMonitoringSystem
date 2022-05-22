//#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
//Initialize the library with the numbers of the interface pins.
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
float val, voltage, temp;
//This is the Arduino Pin that will read the sensor output.
//int sensePin = A0;
//The variable we will use to store the sensor input.
//int sensorInput;
//The variable we will use to store temperature in degrees.
//double temp;
String ssid     = "Simulator Wifi";  // SSID to connect to
String password = ""; //virtual wifi has no password 
String host     = "api.thingspeak.com"; // Open Weather Map API
const int httpPort   = 80;
String url     = "/update?api_key=S6HJQ0GAP30FCE4M&field1=";
//Replace XXXXXXXXXXXXXXXX by your ThingSpeak Channel API Key

void setupESP8266(void) 
{
  
  // Start our ESP8266 Serial Communication
  //Serial.begin(115200);   // Serial connection over USB to computer
  Serial.println("AT");   // Serial connection on Tx / Rx port to ESP8266
  delay(10);        // Wait a little for the ESP to respond
  if (Serial.find("OK"))
    Serial.println("ESP8266 OK!!!");
    
  // Connect to Simulator Wifi
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);        // Wait a little for the ESP to respond
  if (Serial.find("OK"))
    Serial.println("Connected to WiFi!!!");
  
  // Open TCP connection to the host:
  //ESP8266 connects to the server as a TCP client. 

  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(50);        // Wait a little for the ESP to respond
  if (Serial.find("OK")) 
   Serial.println("ESP8266 Connected to server!!!") ;
 
}

void anydata(void) 
{
  
  val=analogRead(A0);
  voltage=val*0.0048828125; 
  temp = (voltage - 0.5) * 100.0;
  
  //Set the cursor to column 0, line 1
  lcd.setCursor(0, 1);
  lcd.print(temp);
  
  lcd.print(" Celsius");
  if (temp > 38)
  {
    //INPUT - FREQUENCY - TIME THAT LASTS
    tone(8, 800, 300);
    delay(250);
    
    digitalWrite(13, HIGH);
    delay(500); // Wait for 500 millisecond(s)
    digitalWrite(13, LOW);
    delay(500); // Wait for 500 millisecond(s)
  }
  
  else if (temp < 32)
  {
    //INPUT - FREQUENCY - TIME THAT LASTS
    tone(8, 500, 300);
    delay(500);

    digitalWrite(9, HIGH);
    delay(500); // Wait for 500 millisecond(s)
    digitalWrite(9, LOW);
    delay(500); // Wait for 500 millisecond(s)
  }
  else
  {
    digitalWrite(10, HIGH);
    delay(500); // Wait for 500 millisecond(s)
    digitalWrite(10, LOW);
    delay(500); // Wait for 500 millisecond(s)  
  }
  

  // Construct our HTTP call
  String httpPacket = "GET " + url + String(temp) + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int length = httpPacket.length();
  
  // Send our message length
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10); // Wait a little for the ESP to respond if (!Serial.find(">")) return -1;

  // Send our http request
  Serial.print(httpPacket);
  delay(10); // Wait a little for the ESP to respond
  if (Serial.find("SEND OK\r\n"))
    Serial.println("ESP8266 sends data to the server");
    
}


void setup() 
{
  Serial.begin(115200);
  
  //Initialize the LCD's number of columns and rows.
  lcd.begin(16, 2);
  lcd.print("IoT Project");
  lcd.setCursor(0,1);
  lcd.print("Temp Level");
  pinMode(A0, INPUT);
  //Pin of the led.
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  delay(1000);
  lcd.clear();  
  delay(1000);
  lcd.print("Temperature: ");
  setupESP8266();
             
}

void loop() 
{
  
 anydata();
  
  delay(4000); // delay changed for faster analytics
}
