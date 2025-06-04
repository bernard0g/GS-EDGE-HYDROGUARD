
// HYDROGUARD - Sistema de Monitoramento de Enchentes (com média de leitura)
// Grupo: 
// Bernardo Lozorio Gomes Y Gomes
// Lucas Salviato Marson | RM: 563975
// Pedro Alves Simão | RM: 564065

#include <LiquidCrystal_I2C.h>

const int sensorNivel = A0;     // Sensor de nível de água
const int ledVerde = 2;         // LED verde
const int ledAmarelo = 3;       // LED amarelo
const int ledVermelho = 4;      // LED vermelho
const int buzzer = 5;           // Buzzer

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD I2C

// Função que lê 5 vezes o sensor e retorna média em metros
float lerAlturaMedia() {
  long soma = 0;
  for (int i = 0; i < 5; i++) {
    soma += analogRead(sensorNivel);
    delay(10);
  }
  float media = soma / 5.0;
  float altura = map(media, 0, 1023, 0, 300); // até 300 cm
  return altura / 100.0; // retorna em metros
}

// Os valores variam porque a leitura do sensor no simulador não é 100% fixa
// Por isso representam bem o comportamento real de sensores

void setup() {
  pinMode(sensorNivel, INPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  float alturaAgua = lerAlturaMedia();

  Serial.print("Altura da água: ");
  Serial.print(alturaAgua, 2);
  Serial.println(" m");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nivel: ");
  lcd.print(alturaAgua, 2);
  lcd.print("m");

  if (alturaAgua <= 1.0) {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(buzzer, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Status: Seguro");
  } else if (alturaAgua > 1.0 && alturaAgua <= 1.5) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(buzzer, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Status: Alerta");
  } else if (alturaAgua > 1.5 && alturaAgua <= 2.0) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(buzzer, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Nivel Elevado");
  } else {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(buzzer, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("RISCO DE ENCHENTE");
  }

  delay(2000);
}
