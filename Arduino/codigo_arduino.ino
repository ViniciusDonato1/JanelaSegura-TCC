#include <Servo.h>

#define leituraAnalogica A5
#define leituraDigital 7

int valorDigital;
int estadoAnterior = -1;
int angulo = 0;

Servo meuServoEsquerda;
Servo meuServoDireita;

void setup() {
  Serial.begin(9600);
  pinMode(leituraAnalogica, INPUT);
  pinMode(leituraDigital, INPUT);

  meuServoEsquerda.attach(9);
  meuServoDireita.attach(10);

  meuServoEsquerda.write(angulo);
  meuServoDireita.write(180 - angulo);  
}

void loop() {
  valorDigital = digitalRead(leituraDigital);

  if (valorDigital != estadoAnterior) {
    estadoAnterior = valorDigital;

    if (valorDigital == 0) {
      Serial.println("FECHADO");
      angulo = 100;
    } else {
      Serial.println("ABERTO");
      angulo = 180;
    }

    meuServoEsquerda.write(angulo);
    meuServoDireita.write(180 - angulo);
  }

  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando == "abrir") {
      angulo = 180;
      meuServoEsquerda.write(angulo);
      meuServoDireita.write(180 - angulo);
      Serial.println("ABERTO");
    } else if (comando == "fechar") {
      angulo = 90;
      meuServoEsquerda.write(angulo);
      meuServoDireita.write(180 - angulo);
      Serial.println("FECHADO");
    } else {
      Serial.println("INVALIDO");
    }
  }

  delay(20);
}
