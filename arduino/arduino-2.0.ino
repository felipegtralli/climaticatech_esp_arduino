#include <DHT11.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

#define pinoUmidade A2
#define pinoLDR A1
#define pinoTMP 2
#define pinoReleLmpd 7
#define pinoReleFan 8

const int janela = 5;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT11 dht11(pinoTMP);
int umidade, luminosidade;
float temperatura, somaTemp, somaUmidade, somaLum;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pinoReleLmpd, OUTPUT);
  pinMode(pinoReleFan, OUTPUT);
  pinMode(pinoUmidade, INPUT);
  pinMode(pinoLDR, INPUT);
  pinMode(pinoTMP, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.display();
}

void loop() {
  somaTemp = somaUmidade = somaLum = 0;

  int i = 0;
  while(i < janela) {
    temperatura = dht11.readTemperature();
    umidade = analogRead(pinoUmidade);
    luminosidade = analogRead(pinoLDR);

    mediaMovel(i);
    i++;  
    
    printDisplay();
    delay(1000);
  }

    acendeLmpd();
    ligaFan();

    Serial.print(String(umidade) + "," + String(temperatura) + "," + String(luminosidade));
}

int calcUmidade() {
  umidade = map(umidade, 1023, 315, 0, 100);
  return umidade;
}

int calcLum() {
  luminosidade = map(luminosidade, 1023, 0, 0, 100);
  return luminosidade;
}

void mediaMovel(int i) {
    somaTemp += temperatura;
    somaUmidade += calcUmidade();
    somaLum += calcLum();

    if((i+1) == janela) {
      temperatura = somaTemp/janela;
      umidade = somaUmidade/janela;
      luminosidade = somaLum/janela;
    }
}

void acendeLmpd() {
  if(luminosidade < 50) digitalWrite(pinoReleLmpd, HIGH);
  else digitalWrite(pinoReleLmpd, LOW);
}

void ligaFan() {
  if(temperatura > 30) digitalWrite(pinoReleFan, HIGH);
  else digitalWrite(pinoReleFan, LOW);
}

void printDisplay() {
    display.clearDisplay();
    display.display();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0); display.println("CLIMATICA TECH"); display.display();
    display.println("Temperatura: " + String(temperatura) + " C"); display.display();
    display.println("Umidade: " + String(umidade) + "%"); display.display();
    display.println("Luminosidade: " + String(luminosidade) + "%"); display.display();
}