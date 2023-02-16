#include <Stepper.h> //for Stepper Motor setup
#include <WiFi.h> //for WiFi setup
#include <PubSubClient.h> //for MQTT setup
#include <ArduinoJson.h> //for handle JSON objects
#include <vector> //for using an "arraylist" (vector) for the registered times 
#include <TimeLib.h> //for catching the current time from an NTP server

#define ssid "UPC0676608"
#define password "Pdrkhbwne4y6"
#define ssidm "Atis"
#define passwordm "brokkoli"
char new_topic[50] = " "; //the topic from the broker
#define mqttUser "atis941"
#define mqttPassword "kelkaposzta94"

//LED setup
int LED = 26;

//Set up MQTT
#define mqtt_broker_linux "192.168.0.26"//IP adress of notebook
WiFiClient espClient; //create a WiFi client
PubSubClient mqttClient(espClient); //create an MQTT publish/subscribe client
//define Topics
#define mqttOpen "windowBlind/open" //Topic for Open
#define mqttClose "windowBlind/close" //Topic for Close
#define mqttRefreshTime "windowBlind/refreshTime" //Topic for adding new time


//stepper Motor setup
int SPR = 2048; //steps per revolution
int motspeed = 15; //motor speed
int dt = 500; //delay time
const int PIN1 = 13;
const int PIN2 = 12;
const int PIN3 = 14;
const int PIN4 = 27;
Stepper myStepper(SPR,PIN1,PIN3,PIN2,PIN4);

//NTP setup and RTC setup
#define ntpServer "pool.ntp.org" //where the time will be requested from
const long GMTOffset_sec = 1*3600; //the offset in seconds between my time zine and GMT
const int daylightOffset_sec = 3600; //the offset in seconds for daylight saving time

//custom Time class and array
class MyTime{
  public:
    MyTime(int hour,int minute,String myposition){
      this->hour = hour;
      this->minute = minute;
      this->myposition = myposition;
    }

    MyTime(){
      this->hour = 0;
      this->minute = 0;
      this->myposition = "";
    }

    String toString(){
      return String(this->hour) + ":" + String(this->minute) + "|" + this->myposition;
    }

    int gethour(){
      return this->hour;
    }

    int getminute(){
      return this->minute;
    }

    String getmyposition(){
      return this->myposition;
    }

    
  private:
    int hour;
    int minute;
    String myposition;
};

std::vector<MyTime> mytimes; //to hold the times to be used

boolean stringcompare(char* string1, char* string2){
  if(string1 != NULL && string2 != NULL){
    for(int i = 0 ; i < strlen(string1) ; i++){
      if(string1[i] != string2[i]){
        return false;
      }
    }
    return true;
  }
}

void empty_new_topic(){
  strcpy(new_topic, " ");
}

void blinkLED(){
  digitalWrite(LED,HIGH);
  delay(1000);
  digitalWrite(LED,LOW);
}


void refreshTimes(std::vector<MyTime>* times, MyTime newtime){
  times->push_back(newtime);
}

TimeElements convert_tm_to_TimeElements(struct tm timestruct){ //function to convert the struct tm to type TimeElements, because its needed of makeTime() function in updateInnerTime()
  TimeElements timeelement;
  timeelement.Second = timestruct.tm_sec;
  timeelement.Minute = timestruct.tm_min;
  timeelement.Hour = timestruct.tm_hour;
  timeelement.Wday = timestruct.tm_wday;
  timeelement.Day = timestruct.tm_mday;
  timeelement.Month = timestruct.tm_mon;
  timeelement.Year = timestruct.tm_year;

  return timeelement;
}

void updateInnerTime(struct tm timestruct){
  Serial.println("The time yout got from server:" + String(timestruct.tm_hour) + ":" + String(timestruct.tm_min));
  Serial.println("");
  time_t EPOCH = makeTime(convert_tm_to_TimeElements(timestruct));
  setTime(EPOCH);
  switch(timeStatus()){
    case timeNotSet: Serial.println("Time's clock has not been set");
                     break;
    case timeSet: Serial.println("Time's clock has been set");
                  break;
    case timeNeedsSync: Serial.println("Time's clock has been set, but the sync has failed, so it may not be accurate");
                        break;
  }
}


struct tm getServerTime(){
  struct tm timeinfo; //inner structure that contains all the details about the time
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return timeinfo;
  }
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");

  return timeinfo;
}

void printCurrentTimes(){
  int number_of_elements = 0;
  for(MyTime fortime : mytimes){
    Serial.println(fortime.toString());
    number_of_elements++;
  }
  Serial.println("Number of elements: " + String(number_of_elements));
}

void callback(char* topic, byte* payload,unsigned int length) {
  const int CAPACITY = 256; //calculated with ArduinoJson Assistant
  Serial.println("CAPACITY:");
  Serial.println(CAPACITY);
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Current state of the client: " );
  Serial.println(mqttClient.state());
  strcpy(new_topic,topic);
  Serial.println(new_topic);
 
  
  Serial.print("Message: ");
  for(int i = 0; i<length ; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println("");
  
  if(stringcompare(topic,mqttRefreshTime)){
    mytimes.clear(); 
    StaticJsonDocument<CAPACITY> jsondoc;
    Serial.println("JsonDocument Capacity:");
    Serial.println(jsondoc.capacity());
    Serial.println("JsonDocument memory usage:");
    Serial.println(jsondoc.memoryUsage());
    DeserializationError err = deserializeJson(jsondoc, payload); //if doesnt work with byte* payload you can use the str[] array instead
    if(err){
      Serial.print("deserializeJson() failed with code:");
      Serial.println(err.c_str());
    }
    JsonArray jsonarr = jsondoc.as<JsonArray>(); //create a reference to the array stored in the incoming json document
    Serial.println("JsonArray memoryUsage:");
    Serial.println(jsonarr.memoryUsage());
    for(JsonObject jsontime : jsonarr){ //get the data from the incoming json "objects"
      String temphour = jsontime["hour"];
      String tempminute = jsontime["minute"];
      String tempposition = jsontime["position"];
      MyTime newtime((int)temphour.toInt(),(int)tempminute.toInt(),tempposition);
      refreshTimes(&mytimes,newtime);
    }
    printCurrentTimes();
  }

  
 
  Serial.println();
  Serial.println("-----------------------");
  blinkLED();
}

void Open(){
  myStepper.step(SPR);
  Serial.println("Window Open");
}

void Close(){
  myStepper.step(-SPR);
  Serial.println("Window Close");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myStepper.setSpeed(motspeed);

  //Set up WiFi
  delay(1000);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid,password); //create wifi connection with the Access Point
  while(WiFi.status() != WL_CONNECTED){ // Try connecting until it has succeeded
    delay(dt);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP Adress of ESP: ");
  Serial.println(WiFi.localIP());
  //End WiFi setup

  //LED setup
  pinMode(LED,OUTPUT);

  //Connect to the MQTT server
  mqttClient.setServer(mqtt_broker_linux,1883);
  Serial.print("Connecting to MQTT Server");
  while(!mqttClient.connected()){
    Serial.print(".");
    delay(dt);
    if(mqttClient.connect("ESP32")){//connect to the server (broker) as ESP32 other two parameters is username and password if needed
      Serial.println("");
      Serial.println("Connected - SUCCESS");
    }
  }
  
  mqttClient.subscribe(mqttOpen); //Subscribe to the Topic of opening
  mqttClient.subscribe(mqttClose); //Subscribe to the Topic of closing
  mqttClient.subscribe(mqttRefreshTime); // Subscribe to the Topic of time vector refreshing (adding and deleting new times)
  mqttClient.setCallback(callback); //a message callback function, called when a message arrives for a subscription created by this client. 

  //Init and get the time
  configTime(GMTOffset_sec, daylightOffset_sec, ntpServer); //to connect to the time server
  updateInnerTime(getServerTime()); //update the inner RTC clock of ESP32
}


void loop() {
  // put your main code here, to run repeatedly
  if(mqttClient.loop()){
    if(new_topic != NULL){
      if(stringcompare(new_topic,mqttOpen)){
        myStepper.step(SPR);
        empty_new_topic();
        delay(dt);
      }
      if(stringcompare(new_topic,mqttClose)){
        myStepper.step(-SPR);
        empty_new_topic();
        delay(dt);
      }
    }
    for(MyTime fortime : mytimes){
      if(hour() == fortime.gethour() && minute() == fortime.getminute() && second() == 0){
        if(fortime.getmyposition() == "Open"){
          Open();
        }
        if(fortime.getmyposition() == "Close"){
          Close();
        }
      }
    }
  }
}
