#include <HTTPClient.h>
#include <WiFi.h>
#define RXp2 16
#define TXp2 17

const char* ssid     = "esp32_teste";
const char* password = "espteste1234@";
const char* URL = "https://pinnatiped-cylinder.000webhostapp.com/postespdata.php";

String msgRecebida = "", umidade, temperatura, luminosidade, postData, payload;
String lista[3] = {umidade, temperatura, luminosidade};

int httpCode;

void setup(){
 Serial.begin(115200);
 Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
 connectWiFi();
}

void loop(){
  if(WiFi.status() != WL_CONNECTED) connectWiFi();
  
  msgRecebida = "";
  while(msgRecebida.isEmpty()){
    msgRecebida = Serial2.readString();
  }
  msgRecebida.remove(msgRecebida.indexOf("\n"));
  
  stringSplit();

  HTTPClient http;

  http.begin(URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  postData = "umidade=" + lista[0] + "&temperatura=" + lista[1] + "&luminosidade=" + lista[2] + "";
  httpCode = http.POST(postData);
  payload = http.getString();

  printInfo();
}

void connectWiFi(){
  WiFi.mode(WIFI_OFF);
  delay(1000);
  
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    
  Serial.println("\nconnected to : " + String(ssid));
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}

void stringSplit(){
  char* p;
  char msg[msgRecebida.length() + 1];
  p = strtok(strcpy(msg, msgRecebida.c_str()), ",");
  int i = 0;
  while(p != NULL && p != "\n"){
    String s = p;
    lista[i] = s;
    p = strtok(NULL, ",");
    i++;
  }
}

void printInfo(){
  Serial.printf("Dados: Umidade: %s%%     Temperatura: %s *C     Luminosidade: %s%%\n", lista[0], lista[1], lista[2]);
  Serial.printf("URL : %s\n", URL);
  Serial.printf("httpCode: %d\n", httpCode); 
  Serial.print("payload : " + String(payload));
  Serial.println("--------------------------------------------------");
}