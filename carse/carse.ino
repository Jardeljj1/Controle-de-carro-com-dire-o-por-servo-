#include <AFMotor.h>
#include <Servo.h>

AF_DCMotor MotorFR(1);
AF_DCMotor MotorFL(2);
AF_DCMotor MotorBL(3);
AF_DCMotor MotorBR(4);

Servo direcao;
int pinoServo = 10;  // Servo conectado no pino 10

const int buzPin = 2;
const int ledPin = A5;
int valSpeed = 255;

int centro = 90;
int esquerda = 60;
int direita = 120;

char ultimoMovimentoMotores = 'S'; // F, B ou S
unsigned long tempoAcaoServo = 0;  // Controle de tempo do servo
int posicaoAlvoServo = 90;         // Ângulo alvo atual do servo

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
    // Lê comandos do Bluetooth
    while (Serial.available() > 0) {
        char command = Serial.read();
        Serial.println(command);

        switch(command) {
            case 'F':
                frente();
                ultimoMovimentoMotores = 'F';
                posicaoAlvoServo = centro; // Servo centralizado
                break;

            case 'B':
                tras();
                ultimoMovimentoMotores = 'B';
                posicaoAlvoServo = centro; // Servo centralizado
                break;

            case 'L':
                // Vira servo para esquerda temporariamente
                posicaoAlvoServo = esquerda;
                tempoAcaoServo = millis(); // marca o tempo atual
                if (ultimoMovimentoMotores == 'F') frente();
                else if (ultimoMovimentoMotores == 'B') tras();
                break;

            case 'R':
                // Vira servo para direita temporariamente
                posicaoAlvoServo = direita;
                tempoAcaoServo = millis();
                if (ultimoMovimentoMotores == 'F') frente();
                else if (ultimoMovimentoMotores == 'B') tras();
                break;

            case 'S':
                MotorFL.run(RELEASE);
                MotorFR.run(RELEASE);
                MotorBL.run(RELEASE);
                MotorBR.run(RELEASE);
                ultimoMovimentoMotores = 'S';
                break;

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

    // Atualiza o servo continuamente
    // Se passaram 500 ms desde a ação de virar, volta ao centro
    if ((posicaoAlvoServo == esquerda || posicaoAlvoServo == direita) && millis() - tempoAcaoServo > 500) {
        posicaoAlvoServo = centro;
    }

    direcao.write(posicaoAlvoServo);
}

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

void SetSpeed(int val) {
    valSpeed = val;
    MotorFL.setSpeed(val);
    MotorFR.setSpeed(val);
    MotorBL.setSpeed(val);
    MotorBR.setSpeed(val);
}
