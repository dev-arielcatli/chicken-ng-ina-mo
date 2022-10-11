#include<ESP8266WiFi.h>
#include<SoftwareSerial.h>

const char* NETWORK_ID = "ChickenNgInaMo";      // WIFI SSID
const char* PASSWORD = "kagatinMoManokKo"; // WIFI Password

const String CODE_CONNECT = "GET /connect";
const String CODE_ADD = "GET /add";
const String CODE_ON = "GET /on?";
const String CODE_OFF = "GET /off?";
const String CODE_SET = "GET /set?";

// COMMANDS
const String COMMAND_CONNECT = "X";
const String COMMAND_ON = "O";
const String COMMAND_OFF = "F";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
//  Serial.println();
  WiFi.softAP(NETWORK_ID, PASSWORD);
  server.begin();
}

void loop() {
  int OKAY_RESPONSE = -1;
  int HAS_COMMAND = -1;
  String RESPONSE = "";
  
  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');

        if(line.startsWith(CODE_SET)){
          const int startHours = line.indexOf("hour") + 5;
          const int startMins = line.indexOf("mins") + 5;
          const String hours = line.substring(startHours, startHours+2);
          const String mins = line.substring(startMins, startMins+2);
          Serial.print("A"+hours+mins);
          RESPONSE = createNoContentResponse();
          OKAY_RESPONSE = 1;
        } else if(line.startsWith(CODE_OFF)){
          Serial.print(COMMAND_OFF);
          RESPONSE = createNoContentResponse();
          OKAY_RESPONSE = 1;
        } else if(line.startsWith(CODE_ON)){
          Serial.print(COMMAND_ON);
          RESPONSE = createNoContentResponse();
          OKAY_RESPONSE = 1;
        } else if(line.startsWith(CODE_CONNECT)) {
          HAS_COMMAND = 1;
          OKAY_RESPONSE = 1;
          RESPONSE = createHomepage();
        } else if(line.startsWith(CODE_ADD)) {
          HAS_COMMAND = 1;
          const int startCode = line.indexOf("=");
          if(startCode != -1) {
            Serial.println(line.substring(startCode + 1, startCode + 9));

            int timeout = 1;

            while(!Serial.available() && timeout < 5) {
//            Serial.println(timeout);
              delay(2000);
              timeout += 1;
            }

            if(timeout < 5) {
              String response = Serial.readStringUntil('\r');
  //            Serial.println("This is the response\r\n");
  //            Serial.println(response);
              if(response.startsWith("OK")) {
                OKAY_RESPONSE = 1;
                RESPONSE = createNoContentResponse();
              } else {
                OKAY_RESPONSE = -1;
              }
            } else {
  //            Serial.println("NO RESPONSE");
              OKAY_RESPONSE = -1;
            }            
          } else {
            OKAY_RESPONSE = -1;
            RESPONSE = createNOKResponse();
          }
        }

        
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          if(OKAY_RESPONSE == 1) {
//            Serial.println("Sending OK");
//            Serial.println(RESPONSE);
            client.println(RESPONSE);
            break;
          } else {
            if(HAS_COMMAND ==1) {
//              Serial.println("Sending NOK");
              client.println(createNOKResponse());
            }
            break;
          }
        }
      }
    }

    while (client.available()) {
      // but first, let client finish its request
      // that's diplomatic compliance to protocols
      // (and otherwise some clients may complain, like curl)
      // (that is an example, prefer using a proper webserver library)
      client.read();
    }

    // close the connection:
    client.stop();
  }
}

String createResponse(String items) {
  String response;
  response.reserve(1024);
  response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n{\"items\": [" + items + "]}\r\n";
  return response;
}

String createHomepage() {
  String response = "<!DOCTYPE html><head> <title>Chicken Feeder<\/title> <style>body{height: 100vh;}button{font-size: 2rem; width: 200px; margin: 10px; height: 100px; background-color: orange; color: white; border: none; border-radius: 20px;}.column{flex-direction: column;}.center{display: flex; align-items: center; justify-content: center}.time{font-size: 6rem; width: 10rem; height: 10rem; text-align: center; padding: 10px;}.times{margin: 20px;}h1{text-align: center;}<\/style><\/head><body class=\"column center\"> <h1 class=\"rows column\">Chicken Feeder V1.0<\/h1> <div class=\"center\"> <form method=\"GET\" action=\"\/on\"> <button type=\"submit\">ON<\/button> <\/form> <form method=\"GET\" action=\"\/off\"> <button type=\"submit\">OFF<\/button> <\/form> <\/div><form action=\"\/set\" class=\"center column\"> <p>Use 24 hour format<\/p><div class=\"times center\"> <input class=\"time\" type=\"text\" name=\"hour\" placeholder=\"HH\"> <input class=\"time\" type=\"text\" name=\"mins\" placeholder=\"MM\"> <\/div><button type=\"submit\">Add to Schedule<\/button> <\/form><\/body><\/html>";
  return response;
}

String createNoContentResponse() {
  String response;
  response.reserve(1024);
  response = "HTTP/1.1 204 No Content\r\nConnection: close\r\n";
  return response;
}

String createNOKResponse() {
  String response;
  response.reserve(1024);
  response = F("HTTP/1.1 500 Internal Server Error\r\n"
             "Content-Length: 25"
             "Content-Type: text/plain\r\n"
             "Connection: close\r\n"  // the connection will be closed after completion of the response
             "\r\n"
             "500 Internal Server Error\r\n");
  return response;
}
