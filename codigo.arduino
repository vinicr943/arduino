#include <LiquidCrystal.h> // Biblioteca para controle do LCD
#include <math.h>          // Biblioteca para funções matemáticas (pow)

LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2); // Define os pinos do LCD

// Caracteres personalizados (carinhas)
byte neutral[8] = { // Carinha neutra
  B00000, B10001, B00000, B00000,
  B01110, B10001, B00000, B00000
};

byte smile[8] = { // Carinha feliz
  B00000, B10001, B00000, B00000,
  B10001, B01110, B00000, B00000
};

byte serious[8] = { // Carinha triste/séria
  B00000, B10001, B00000, B00000,
  B11111, B00000, B00000, B00000
};

// LDR (sensor de luz)
const int ldr = A0; // pino analógico
int valorldr = 0;   // valor bruto lido
int luzPercent = 0; // valor convertido em porcentagem

// FAIXA AJUSTADA do sensor (mínimo = luz forte, máximo = escuro)
const int minLDR = 50;
const int maxLDR = 970;

// Define se o sensor deve inverter a leitura
bool sensorInvertido = false;

// LEDs
const int ledAmarelo = 6;
const int ledVerde = 7;
const int ledVermelho = 8;

// BUZZER
const int buzzer = 9;

void setup() {
  lcd.begin(16, 2); // Inicializa LCD 16x2
  lcd.clear();

  // Tela inicial
  lcd.setCursor(0, 0);
  lcd.print("Next Level");
  lcd.setCursor(0, 1);
  delay(2000); // espera 2 segundos
  lcd.clear();

  // Mensagem de boas-vindas
  lcd.setCursor(0, 0);
  lcd.print("Bem-vindo!");
  delay(2000);
  lcd.clear();

  // Cria caracteres personalizados no LCD
  lcd.createChar(0, neutral);
  lcd.createChar(1, smile);
  lcd.createChar(2, serious);

  pinMode(ldr, INPUT); // define LDR como entrada

  // Define LEDs e buzzer como saída
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600); // inicia comunicação serial (debug)

  lcd.setCursor(0, 0);
  lcd.print("Sensor de Luz"); // título fixo
}

// Função que calcula média de 10 leituras para evitar ruído
int lerLDRMedio() {
  long soma = 0;
  for (int i = 0; i < 10; i++) {
    soma += analogRead(ldr); // lê o sensor
    delay(2); // pequeno delay
  }
  return soma / 10; // retorna média
}

void loop() {

  valorldr = lerLDRMedio(); // leitura suavizada

  // Garante que o valor fique dentro da faixa definida
  valorldr = constrain(valorldr, minLDR, maxLDR);

  // Normaliza o valor entre 0 e 1
  float normalizado = (float)(valorldr - minLDR) / (maxLDR - minLDR);

  // Aplica curva exponencial (torna a resposta mais agressiva)
  normalizado = pow(normalizado, 20);

  // Converte para porcentagem
  luzPercent = normalizado * 100;

  // Inverte se necessário
  if (sensorInvertido) {
    luzPercent = 100 - luzPercent;
  }

  // Garante valores entre 0 e 100
  luzPercent = constrain(luzPercent, 0, 100);

  // Debug no monitor serial
  Serial.print("Raw: ");
  Serial.print(valorldr);
  Serial.print(" | %: ");
  Serial.println(luzPercent);

  // Mostra no LCD
  lcd.setCursor(0, 1);
  lcd.print("Luz: ");
  
  // Ajuste de espaçamento para não sobrar caracteres
  if(luzPercent < 100) lcd.print(" "); 
  if(luzPercent < 10) lcd.print(" ");
  
  lcd.print(luzPercent);
  lcd.print("%   ");

  lcd.setCursor(15, 1); // posição da carinha

  // Desliga todos os LEDs e buzzer antes de decidir o estado
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledVermelho, LOW);
  noTone(buzzer);

  // Lógica LEDs + buzzer

  // Muito escuro (alerta)
  if (luzPercent < 5) {
    lcd.write(byte(2)); // carinha triste
    digitalWrite(ledAmarelo, HIGH);
    tone(buzzer, 1000); // som
  }
  // Ideal (penumbra)
  else if (luzPercent < 30) {
    lcd.write(byte(1)); // feliz
    digitalWrite(ledVerde, HIGH);
  } 
  // Atenção
  else if (luzPercent < 50) {
    lcd.write(byte(2)); // triste
    digitalWrite(ledAmarelo, HIGH);
    tone(buzzer, 1500);
  } 
  // Luz excessiva
  else {
    lcd.write(byte(0)); // neutro
    digitalWrite(ledVermelho, HIGH);
    tone(buzzer, 2000);
  }

  delay(200); // pequena pausa
}
