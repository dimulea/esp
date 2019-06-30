#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "XXXXXX";
const char* password = "12345678";

byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192,168,0,35);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

int LED_0 = 2;
int LED_1 = 13;
int LED_2 = 12;
int LED_3 = 14;

void handlePage();
void handlePost();
String webPage();
String checked();
String state;

void setup(void){
  pinMode(LED_0, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  digitalWrite(LED_0, HIGH);
  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
  digitalWrite(LED_3, HIGH);
 
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Angela", "12345678");
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED) { delay(500); }

  server.on("/", HTTP_POST, handlePost);
  server.on("/", HTTP_GET, handlePage);
  server.begin();
}
 
void loop(void){ server.handleClient(); }

void handlePost() {
  if (server.arg("b0") == "on") { digitalWrite(LED_0, LOW); } else { digitalWrite(LED_0, HIGH); }
  if (server.arg("b1") == "on") { digitalWrite(LED_1, LOW); } else { digitalWrite(LED_1, HIGH); }
  if (server.arg("b2") == "on") { digitalWrite(LED_2, LOW); } else { digitalWrite(LED_2, HIGH); }
  if (server.arg("b3") == "on") { digitalWrite(LED_3, LOW); } else { digitalWrite(LED_3, HIGH); }
  handlePage();
}

void handlePage() { server.send(200, "text/html", webPage()); }

String checked(int LED){
  if (digitalRead(LED) != 1) {
    state = "pulse";
    return("checked");
  } else {
    state = "grey";
    return("");
  }
}

String webPage(){
  return ("\
  <!DOCTYPE html><html lang=\"en\"><meta charset=\"UTF-8\"><title>ESP-12</title>\
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=1\">\
  <meta name=\"description\" content=\"ESP-12\">\
  <meta property=\"og:image\" content=\"esp12.png\">\
  <link rel=\"icon\" href=\"//cdn.esp12.tk/ico.png\" type=\"image/png\">\
  <link rel=\"stylesheet\" href=\"//cdn.esp12.tk/style.css\">\
  <body>\
    <nav class=\"blue darken-2\">\
      <div class=\"nav-wrapper container\">\
        <ul><li><a href=\"/\"><i class=\"material-icons left\">all_inclusive</i>ESP-12</a></li></ul>\
      </div>\
    </nav>\
    <div class=\"container center\">\
      <h4>Status of Indicators:</h1>\
      <form method=\"POST\">\
        <table class=\"highlight\">\
          <tbody><tr>\
            <td>LED_BUILTIN</td>\
            <td class=\"switch\"><label>Off<input type=\"checkbox\" name=\"b0\" " + checked(LED_0) + ">\
            <span class=\"lever\"></span>On</label></td>\
            <td><a class=\"btn-floating blue " + state + "\"></a></td>\
          </tr><tr>\
            <td>LED_1</td>\
            <td class=\"switch\"><label>Off<input type=\"checkbox\" name=\"b1\" " + checked(LED_1) + ">\
            <span class=\"lever\"></span>On</label></td>\
            <td><a class=\"btn-floating red " + state + "\"></a></td>\
          </tr><tr>\
            <td>LED_2</td>\
            <td class=\"switch\"><label>Off<input type=\"checkbox\" name=\"b2\" " + checked(LED_2) + ">\
            <span class=\"lever\"></span>On</label></td>\
            <td><a class=\"btn-floating red " + state + "\"></a></td>\
          </tr><tr>\
            <td>LED_2</td>\
            <td class=\"switch\"><label>Off<input type=\"checkbox\" name=\"b3\" " + checked(LED_3) + ">\
            <span class=\"lever\"></span>On</label></td>\
            <td><a class=\"btn-floating green " + state + "\"></a></td>\
          </tr></tbody>\
        </table><br>\
        <button class=\"btn deep-orange\" type=\"submit\"><i class=\"material-icons right\">send</i>change state</button>\
      </form><br>\
      <a href=\"/\" class=\"btn green\"><i class=\"material-icons right\">refresh</i>refresh</a>\
    </div>\
  </body>\
  </html>\
  ");
}
