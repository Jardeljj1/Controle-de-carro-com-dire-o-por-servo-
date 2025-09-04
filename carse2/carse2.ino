#include <AFMotor.h>
#include <Servo.h>

// Motores com L293D
AF_DCMotor MotorFR(1);
AF_DCMotor MotorFL(2);
AF_DCMotor MotorBL(3);
AF_DCMotor MotorBR(4);

// Servo para direção
Servo direcao;
int pinoServo = 10;

const int buzPin = 2;
const int ledPin = A5;
int valSpeed = 255;

// Ângulos do servo
int centro = 90;
int esquerda = 60;
int direita = 120;

// Estados atuais
char estadoMotor = 'S';  // F = frente, B = tras, S = stop
char estadoServo = 'C';   // L = esquerda, R = direita, C = centro
unsigned long tempoAcaoServo = 0;  // tempo do último comando L ou R
int posicaoAlvoServo = 90;

void setup() {
    Serial.begin(9600);
    pinMode(buzPin, OUTPUT);
    pinMode(ledPin, OUTPUT);

    MotorFL.setSpeed(valSpeed);
    MotorFR.setSpeed(valSpeed);
    MotorBL.setSpeed(valSpeed);
    MotorBR.setSpeed(valSpeed);

    MotorFL.run(RELEASE);
    MotorFR.run(RELEASE);
    MotorBL.run(RELEASE);
    MotorBR.run(RELEASE);

    direcao.attach(pinoServo);
    direcao.write(centro);
    posicaoAlvoServo = centro;
}

void loop() {
    // Leitura de comandos Bluetooth
    while (Serial.available() > 0) {
        char command = Serial.read();
        Serial.println(command);

        switch (command) {
            // Movimento dos motores
            case 'F': estadoMotor = 'F'; break;
            case 'B': estadoMotor = 'B'; break;
            case 'S': estadoMotor = 'S'; break;

            // Direção do servo
            case 'L':
                estadoServo = 'L';
                tempoAcaoServo = millis();
                break;
            case 'R':
                estadoServo = 'R';
                tempoAcaoServo = millis();
                break;
            case 'C':
                estadoServo = 'C';
                break;

            // Extras
            case 'Y':
                digitalWrite(buzPin, HIGH);
                delay(200);
                digitalWrite(buzPin, LOW);
                delay(80);
                digitalWrite(buzPin, HIGH);
                delay(300);
                digitalWrite(buzPin, LOW);
                break;
            case 'X': digitalWrite(ledPin, HIGH); break;
            case 'x': digitalWrite(ledPin, LOW); break;

            // Controle de velocidade
            case '0': SetSpeed(0); break;
            case '1': SetSpeed(25); break;
            case '2': SetSpeed(50); break;
            case '3': SetSpeed(75); break;
            case '4': SetSpeed(100); break;
            case '5': SetSpeed(125); break;
            case '6': SetSpeed(150); break;
            case '7': SetSpeed(175); break;
            case '8': SetSpeed(200); break;
            case '9': SetSpeed(255); break;
        }
    }

    // Atualiza motores de acordo com estadoMotor
    if (estadoMotor == 'F') frente();
    else if (estadoMotor == 'B') tras();
    else if (estadoMotor == 'S') stopMotores();

    // Atualiza servo
    if ((estadoServo == 'L' || estadoServo == 'R') && millis() - tempoAcaoServo > 500) {
        estadoServo = 'C'; // volta automaticamente ao centro
    }

    if (estadoServo == 'L') posicaoAlvoServo = esquerda;
    else if (estadoServo == 'R') posicaoAlvoServo = direita;
    else posicaoAlvoServo = centro;

    direcao.write(posicaoAlvoServo);
}

// Funções auxiliares
void frente() {
    MotorFL.run(FORWARD);
    MotorFR.run(FORWARD);
    MotorBL.run(FORWARD);
    MotorBR.run(FORWARD);
}

void tras() {
    MotorFL.run(BACKWARD);
    MotorFR.run(BACKWARD);
    MotorBL.run(BACKWARD);
    MotorBR.run(BACKWARD);
}

void stopMotores() {
    MotorFL.run(RELEASE);
    MotorFR.run(RELEASE);
    MotorBL.run(RELEASE);
    MotorBR.run(RELEASE);
}

void SetSpeed(int val) {
    valSpeed = val;
    MotorFL.setSpeed(val);
    MotorFR.setSpeed(val);
    MotorBL.setSpeed(val);
    MotorBR.setSpeed(val);
}
