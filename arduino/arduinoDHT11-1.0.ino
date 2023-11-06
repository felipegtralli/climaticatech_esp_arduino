#include <DHT11.h>

#define pinoUmidade A2
#define pinoLDR A1
#define pinoTMP 2

const int janela = 5;

DHT11 dht11(pinoTMP);
int umidade, luminosidade;
float temperatura, somaTemp, somaUmidade, somaLum;

void setup(){
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pinoUmidade, INPUT);
  pinMode(pinoLDR, INPUT);
  pinMode(pinoTMP, INPUT);
}

void loop(){
  somaTemp = somaUmidade = somaLum = 0;

  int i = 0;
  while(i < janela){
    temperatura = dht11.readTemperature();
    umidade = analogRead(pinoUmidade);
    luminosidade = analogRead(pinoLDR);

    mediaMovel(i);
    i++;
    
    delay(3000);
  }

    Serial.println(String(umidade) + "," + String(temperatura) + "," + String(luminosidade));
}

int calcUmidade(){
  umidade = map(umidade, 1023, 315, 0, 100);
  return umidade;
}

int calcLum(){
  luminosidade = map(luminosidade, 1023, 0, 0, 100);
  return luminosidade;
}

void mediaMovel(int i){
    somaTemp += temperatura;
    somaUmidade += calcUmidade();
    somaLum += calcLum();

    if((i+1) == janela){
      temperatura = somaTemp/janela;
      umidade = somaUmidade/janela;
      luminosidade = somaLum/janela;
    }
}