
#include <LiquidCrystal_I2C.h>

// DEFINIÇÕES DE PINOS
#define pinLdrA A0
#define pinLdrB A1

// DEFINIÇÕES
#define valA 700
#define valB 700

#define DISTANCIA 0.8 // DISTANCIA ENTRE OS SENSORES EM METROS 

// INSTANCIANDO OBJETOS
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DECLARAÇÃO DE VARIÁVEIS
int estadoLdrA;
int estadoLdrB;
int estadoLdrAAnt;
int estadoLdrBAnt;

int estadoContagem;   //0=standby 1=contandoA 2=contandoB 3=Fim contagem

unsigned long tempoA;
unsigned long tempoB;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(pinLdrA, INPUT);
  pinMode(pinLdrB, INPUT);

  estadoLdrAAnt = analogRead(pinLdrA);
  estadoLdrBAnt = analogRead(pinLdrB);

  estadoContagem  = 0;

  Serial.println("Fim do setup()");
}

void loop() {
  estadoLdrA = analogRead(pinLdrA);
  estadoLdrB = analogRead(pinLdrB);
  Serial.println(estadoLdrA);
  //Serial.println(estadoLdrB);
  
  if (estadoContagem == 2) {
    if (estadoLdrA <= valA && estadoLdrAAnt > valA) {
      tempoB = millis();
      estadoContagem = 3;
      delay(tempoB - tempoA);  //aguarda para filtrar o efeito bouncing
    }
  }

  if (estadoContagem == 1) { 
     if (estadoLdrB <= valB && estadoLdrBAnt > valB) {
       tempoB = millis();
       estadoContagem = 3;
       delay(tempoB - tempoA);  //aguarda para filtrar o efeito bouncing
     }
  }
  
  if (estadoContagem == 0) {
     if (estadoLdrA <= valA && estadoLdrAAnt > valA) {
       tempoA = millis();
       estadoContagem = 1;
     }

     if (estadoLdrB <= valB && estadoLdrBAnt > valB) {
       tempoA = millis();
       estadoContagem = 2;
     }
  }
  
  if (estadoContagem == 3) {
    float intervalo = (float)(tempoB - tempoA) / 1000.0; // INTERVALO EM SEGUNDOS
    Serial.print("Tempo: ");
    Serial.print(intervalo);
    Serial.println(" segundos");

    float velocidade = DISTANCIA / intervalo; // velocidade em METROS/SEGUNDO

    Serial.print("Vel.: ");
    Serial.print(velocidade, 2);
    Serial.println(" m/s");

    lcd.print("Vel.: ");
    lcd.print(velocidade, 2);
    lcd.print(" m/s");

    lcd.setCursor(0, 1);

    velocidade = (float)(DISTANCIA / 1000.0) / (float)(intervalo / 3600.0); // velocidade em KM/H

    Serial.print("Vel.: ");
    Serial.print(velocidade, 2);
    Serial.println(" km/h");

   delay(3000); // intervalo entre leituras;

    lcd.clear();

    tempoA = 0;
    tempoB = 0;

    estadoContagem = 0;
  }

  estadoLdrAAnt = estadoLdrA;
  estadoLdrBAnt = estadoLdrB;
}

// IMPLEMENTO DE FUNÇÕES
