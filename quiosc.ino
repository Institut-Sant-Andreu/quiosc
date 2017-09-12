#include <ESP8266WiFi.h>

//const char* ssid     = "docent";  // POSAR VALORS CORRECTES A AQUESTS PARAMETRES PER CONNECTAR A INTERNET
//const char* password = "contrasenya";

const char* host = "rss.bloople.net"; // Aquesta web genera titulars que són més fàcils de manipular amb l'arduino
String url[] = {"World", "Europe", "Business", "Technology", "Sports", "Science"}; 
int opcio = 0; // tipus de titular que volem veure
boolean hiHaMoneda = false;

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (digitalRead(9) == HIGH) {
    hiHaMoneda = true;
  }
  if (hiHaMoneda) {
    if (digitalRead(8) == HIGH) {
      opcio = 1;
    }
    if (digitalRead(2) == HIGH) {
      opcio = 2;
    }
    if (digitalRead(3) == HIGH) {
      opcio = 3;
    }
    if (digitalRead(4) == HIGH) {
      opcio = 4;
    }
    if (digitalRead(5) == HIGH) {
      opcio = 5;
    }
    if (digitalRead(6) == HIGH) {
      opcio = 6;
    }
    hiHaMoneda = false;
  }
  if (opcio > 0) {
    veureNoticia();
    opcio = 0;
  }
}

void veureNoticia() {
  Serial.print("connecting to ");
  Serial.println(host);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  Serial.print("Requesting URL: ");
  Serial.println(url[opcio - 1]);

  client.print(String("GET ") + "/?url=http%3A%2F%2Frss.nytimes.com%2Fservices%2Fxml%2Frss%2Fnyt%2F" + url[opcio - 1] + ".xml&limit=1&showtitle=false&type=html" + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  Serial.println("");
  String line;

  while (client.available()) {
    line = client.readStringUntil('\r');
    if (line.indexOf("_blank") != -1) {
      Serial.println(line.substring(line.indexOf("_blank") + 8, line.indexOf("</a></h4>")) + ". " + line.substring(line.indexOf("item-desc") + 11, line.indexOf("</p>")));
    }
  }
}




