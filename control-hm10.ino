/*
  control-hm10.ino
  ------------------------------------------------------------
  Recibe comandos del panel HTML (control-hm10.html) a través
  del módulo Bluetooth HM-10 y mueve dos motores reductores
  mediante un driver L298N.

  Comandos esperados (un solo carácter, coinciden con el HTML):
    'F' -> adelante   (ambos motores hacia adelante)
    'B' -> atrás      (ambos motores en reversa)
    'L' -> izquierda  (gira en el lugar: motor A en reversa, motor B adelante)
    'R' -> derecha    (gira en el lugar: motor A adelante, motor B en reversa)
    'S' -> stop       (ambos motores detenidos)

  Asume que el motor A (IN1/IN2) es la rueda IZQUIERDA y el motor B
  (IN3/IN4) es la rueda DERECHA. Si al probar "izquierda" el coche
  gira hacia la derecha (o viceversa), no hace falta tocar cables:
  solo intercambia el contenido de las funciones izquierda() y
  derecha() más abajo.

  ------------------------------------------------------------
  CONEXIONES

  HM-10  (comunicación serie, 9600 baudios por defecto)
    HM-10 TXD -> Arduino pin 2 (RX del SoftwareSerial)
    HM-10 RXD -> Arduino pin 3 (TX del SoftwareSerial)
      * El HM-10 trabaja a 3.3V lógicos. Si tu módulo NO tiene
        regulador/adaptador de nivel integrado, usa un divisor
        de voltaje en la línea RXD del HM-10 (la que recibe
        desde el Arduino) para no dañarlo. Muchas placas HM-10
        ya traen ese adaptador — revisa la tuya.
    HM-10 VCC -> 5V (o 3.3V según tu módulo)
    HM-10 GND -> GND

  Driver L298N -> Arduino
    IN1 -> pin 4   (motor A, terminal +)
    IN2 -> pin 5   (motor A, terminal -)
    IN3 -> pin 6   (motor B, terminal +)
    IN4 -> pin 7   (motor B, terminal -)
    ENA / ENB -> puente/jumper puesto (velocidad fija al 100%)
                 Si lo quitas, conecta ENA/ENB a pines PWM
                 (p. ej. 9 y 10) para controlar velocidad con
                 analogWrite() más adelante.

  Alimentación
    L298N GND       -> GND común con el Arduino (¡importante!)
    L298N +12V/VMOT -> batería/pila para los motores
                        (NO uses el 5V del Arduino para los motores)
    L298N +5V salida -> puede alimentar el Arduino si el driver
                        lo permite, o déjalo sin usar si alimentas
                        el Arduino por separado.

  Si un motor gira al revés de lo esperado, simplemente invierte
  el par de cables de ese motor en el L298N (no hace falta tocar
  el código).
  ------------------------------------------------------------
*/

#include <SoftwareSerial.h>

SoftwareSerial hm10(2, 3);   // RX, TX del Arduino

// Pines del L298N
const int IN1 = 4;
const int IN2 = 5;
const int IN3 = 6;
const int IN4 = 7;

void setup() {
  Serial.begin(9600);        // Monitor serie por USB, solo para depurar
  hm10.begin(9600);          // Baudios de fábrica del HM-10 (ajusta si el tuyo es distinto)

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  detener();
  Serial.println(F("Listo. Esperando comandos del HM-10..."));
}

void loop() {
  if (hm10.available()) {
    char comando = hm10.read();

    // Diagnóstico: muestra CUALQUIER byte que llegue, aunque no sea F/B/S.
    // Si esto nunca aparece, el Arduino no está recibiendo nada del HM-10.
    // Si aparece con un código raro (no 70/66/83), es un problema de baud rate.
    Serial.print(F("Byte recibido: '"));
    Serial.print(comando);
    Serial.print(F("' (código "));
    Serial.print((int)comando);
    Serial.println(F(")"));

    switch (comando) {
      case 'F':
        adelante();
        Serial.println(F("-> Adelante"));
        break;
      case 'B':
        atras();
        Serial.println(F("-> Atrás"));
        break;
      case 'L':
        izquierda();
        Serial.println(F("-> Izquierda"));
        break;
      case 'R':
        derecha();
        Serial.println(F("-> Derecha"));
        break;
      case 'S':
        detener();
        Serial.println(F("-> Stop"));
        break;
      default:
        // Cualquier otro carácter (saltos de línea, comandos manuales, etc.) se ignora
        break;
    }
  }
}

void adelante() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void atras() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void izquierda() {
  // Gira en el lugar: rueda A (izquierda) en reversa, rueda B (derecha) adelante
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void derecha() {
  // Gira en el lugar: rueda A (izquierda) adelante, rueda B (derecha) en reversa
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void detener() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
