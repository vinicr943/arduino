#include <LiquidCrystal.h> // Biblioteca para controle do LCD
#include <math.h>          // Biblioteca para funções matemáticas (pow)

LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2); // Define os pinos do LCD

// ===================== CARACTERES PERSONALIZADOS =====================

// Carinha neutra
byte neutral[8] = {
  B00000, B10001, B00000, B00000,
  B01110, B10001, B00000, B00000
};

// Carinha feliz
byte smile[8] = {
  B00000, B10001, B00000, B00000,
  B10001, B01110, B00000, B00000
};

// Carinha triste/séria
byte serious[8] = {
  B00000, B10001, B00000, B00000,
  B11111, B00000, B00000, B00000
};

// ===================== SENSOR LDR =====================

const int ldr = A0; // Pino analógico do LDR
int valorldr = 0;   // Valor bruto lido
int luzPercent = 0; // Valor convertido em %

const int minLDR = 50;  // Valor mínimo (luz forte)
const int maxLDR = 970; // Valor máximo (escuro)

// Inverte leitura se necessário
bool sensorInvertido = false;

// ===================== LEDs =====================

const int ledAmarelo = 6;
const int ledVerde = 7;
const int ledVermelho = 8;

// ===================== BUZZER =====================

const int buzzer = 9;

// Controle de tempo do buzzer (sem delay)
unsigned long tempoAnterior = 0;
bool buzzerLigado = false;

// Tempo ligado e desligado
const int tempoLigado = 3000;   // 3 segundos ligado
const int tempoDesligado = 2000; // 2 segundos desligado

// ===================== SETUP =====================

void setup() {
  lcd.begin(16, 2); // Inicializa LCD
  lcd.clear();

  // Tela inicial
  lcd.setCursor(0, 0);
  lcd.print("Next Level");
  delay(2000);
  lcd.clear();

  // Boas-vindas
  lcd.setCursor(0, 0);
  lcd.print("Bem-vindo!");
  delay(2000);
  lcd.clear();

  // Criação dos caracteres
  lcd.createChar(0, neutral);
  lcd.createChar(1, smile);
  lcd.createChar(2, serious);

  pinMode(ldr, INPUT); // LDR como entrada

  // LEDs e buzzer como saída
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600); // Comunicação serial

  lcd.setCursor(0, 0);
  lcd.print("Sensor de Luz"); // Título fixo
}

// ===================== FUNÇÃO DE MÉDIA =====================

// Faz média de 10 leituras para reduzir ruído
int lerLDRMedio() {
  long soma = 0;
  for (int i = 0; i < 10; i++) {
    soma += analogRead(ldr); // Lê sensor
    delay(2); // Pequeno delay
  }
  return soma / 10; // Retorna média
}

// ===================== LOOP PRINCIPAL =====================

void loop() {

  // Leitura suavizada
  valorldr = lerLDRMedio();

  // Garante que valor fique dentro da faixa definida
  valorldr = constrain(valorldr, minLDR, maxLDR);

  // Normaliza para faixa 0–1
  float normalizado = (float)(valorldr - minLDR) / (maxLDR - minLDR);

  // Aplica curva exponencial (aumenta sensibilidade)
  normalizado = pow(normalizado, 20);

  // Converte para porcentagem
  luzPercent = normalizado * 100;

  // Inverte leitura se necessário
  if (sensorInvertido) {
    luzPercent = 100 - luzPercent;
  }

  // Garante limite entre 0 e 100
  luzPercent = constrain(luzPercent, 0, 100);

  // ===================== DEBUG =====================

  Serial.print("Raw: ");
  Serial.print(valorldr);
  Serial.print(" | %: ");
  Serial.println(luzPercent);

  // ===================== LCD =====================

  lcd.setCursor(0, 1);
  lcd.print("Luz: ");

  // Ajusta espaçamento
  if(luzPercent < 100) lcd.print(" "); 
  if(luzPercent < 10) lcd.print(" ");

  lcd.print(luzPercent);
  lcd.print("%   ");

  lcd.setCursor(15, 1); // posição da carinha

  // ===================== RESET SAÍDAS =====================

  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledVermelho, LOW);

  bool alerta = false; // define se buzzer deve tocar
  int freq = 0;        // frequência do buzzer

  // ===================== LÓGICA DE LUZ =====================

  // Muito escuro
  if (luzPercent < 5) {
    lcd.write(byte(2)); // triste
    digitalWrite(ledAmarelo, HIGH);
    alerta = true;
    freq = 1000;
  }
  // Ideal
  else if (luzPercent < 30) {
    lcd.write(byte(1)); // feliz
    digitalWrite(ledVerde, HIGH);
  } 
  // Atenção
  else if (luzPercent < 50) {
    lcd.write(byte(2)); // triste
    digitalWrite(ledAmarelo, HIGH);
    alerta = true;
    freq = 1500;
  } 
  // Luz excessiva
  else {
    lcd.write(byte(0)); // neutro
    digitalWrite(ledVermelho, HIGH);
    alerta = true;
    freq = 2000;
  }

  // ===================== CONTROLE DO BUZZER =====================

  unsigned long agora = millis(); // tempo atual

  if (alerta) {

    // Se buzzer está ligado
    if (buzzerLigado) {
      // Verifica se passou o tempo ligado
      if (agora - tempoAnterior >= tempoLigado) {
        noTone(buzzer); // desliga
        buzzerLigado = false;
        tempoAnterior = agora;
      }
    } 
    // Se buzzer está desligado
    else {
      // Espera o tempo de pausa
      if (agora - tempoAnterior >= tempoDesligado) {
        tone(buzzer, freq); // liga buzzer
        buzzerLigado = true;
        tempoAnterior = agora;
      }
    }

  } 
  // Se não está em alerta
  else {
    noTone(buzzer); // garante desligado
    buzzerLigado = false;
  }

  delay(200); // pequena pausa geral
}
