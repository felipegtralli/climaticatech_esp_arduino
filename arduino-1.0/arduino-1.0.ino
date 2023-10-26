#define pinoUmidade A2
#define pinoLDR A1
#define pinoTMP A0

const int janela = 5;

int umidade, luminosidade, tmp;
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
    tmp = analogRead(pinoTMP);
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

float calcTemp(){
  temperatura = (tmp / 1024.0) * 500.0;
  return temperatura;
}

int calcLum(){
  luminosidade = map(luminosidade, 1023, 0, 0, 100);
  return luminosidade;
}

void mediaMovel(int i){
    somaTemp += calcTemp();
    somaUmidade += calcUmidade();
    somaLum += calcLum();

    if((i+1) == janela){
      temperatura = somaTemp/janela;
      umidade = somaUmidade/janela;
      luminosidade = somaLum/janela;
    }
}