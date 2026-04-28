
#include <ESP32Servo.h>
#include <PS4Controller.h>
Servo flag;  // create servo object to control a servo
//SERVO
#define SERVO 25 

//MOTORES
#define MI1 15
#define MI2 4
#define MIPWM 5
#define MD1 16
#define MD2 17
#define MDPWM 18

//LEDS
#define LED1 22
#define LED2 19
#define LED3 23
#define LED4 21

////////////////////////////////////////////////// CONFIG PWM
#define freq 2000
#define MD1_PWM 2  // canal donde se genera el PWM de MD1 del pin 5
#define MI1_PWM 3  // canal donde se genera el PWM de MI1 del pin 18


int rutina = 0;
bool DER = 1, IZQ = 0, TIMERSTART = 0;
int Rpwm = 0, Lpwm = 0;
int speed_rc = 255;
int speed_rc2 = 200;
int minspeed_rc = 30;


void setup() {
    ledcSetup(MD1_PWM, freq, 8); //config de pwm MD1
    ledcSetup(MI1_PWM, freq, 8); //config de pwm MI1
    pinMode(MD1, OUTPUT);
    pinMode(MD2, OUTPUT);
    pinMode(MI1, OUTPUT);
    pinMode(MI2, OUTPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    ledcAttachPin(MDPWM, MD1_PWM);    // asigno el pwm generado a un pin 
    ledcAttachPin(MIPWM, MI1_PWM);    // asigno el pwm generado a un pin 
    mstop(1);
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    flag.setPeriodHertz(50);// Standard 50hz servo
    flag.attach(SERVO, 500, 2400);
    PS4.attachOnConnect(onConnect);
    PS4.attachOnDisconnect(onDisConnect);
    PS4.begin();
    flag.write(90);
    mstop(80);
}

void loop() {
    if (PS4.Up() == 1)
    {
                forward(100, 100, 5);

    }

    if (PS4.Down() == 1)
    {

                backward(100, 100, 5);
 
    }

    if (PS4.Right() == 1)
    {
                right(100, 100, 5);

    }

    if (PS4.Left() == 1)
    {
                left(100, 100, 5);
  
    }

    if (PS4.LStickY() >= 10) {
        Lpwm = map(PS4.LStickY(), 11, 127, minspeed_rc, speed_rc);
        Rpwm = map(PS4.LStickY(), 11, 127, minspeed_rc, speed_rc);
        forward(Rpwm, Lpwm, 2);
    }
    if (PS4.LStickY() <= (-10)) {
        Lpwm = map(PS4.LStickY(), -11, -128, minspeed_rc, speed_rc);
        Rpwm = map(PS4.LStickY(), -11, -128, minspeed_rc, speed_rc);
        backward(Rpwm, Lpwm, 2);
    }
    if (PS4.RStickX() >= 10) {
        Lpwm = map(PS4.RStickX(), 11, 127, minspeed_rc, speed_rc2);
        Rpwm = map(PS4.RStickX(), 11, 127, minspeed_rc, speed_rc2);
        right(Rpwm, Lpwm, 2);
    }
    if (PS4.RStickX() <= (-10)) {
        Lpwm = map(PS4.RStickX(), -11, -128, minspeed_rc, speed_rc2);
        Rpwm = map(PS4.RStickX(), -11, -128, minspeed_rc, speed_rc2);
        left(Rpwm, Lpwm, 2);
    }
    if (((PS4.RStickX() < 10) && (PS4.RStickX() > -10)) && ((PS4.LStickY() < 10) && (PS4.LStickY() > -10))) {
        stop2(1);
    }


    if (PS4.Touchpad() == 1) { flag.write(180); }
    else{ flag.write(90);}

}

void onConnect()
{
    PS4.setLed(70, 0, 70);
    PS4.sendToController();
    delay(5);
    digitalWrite(LED1, 0); digitalWrite(LED2, 0); digitalWrite(LED3, 0); digitalWrite(LED4, 0);
}

void onDisConnect()
{
    digitalWrite(LED1, 1); digitalWrite(LED2, 1); digitalWrite(LED3, 1); digitalWrite(LED4, 1);
}

void forward(byte PWM1, byte PWM2, int T) {
    ledcWrite(MD1_PWM, PWM1);
    ledcWrite(MI1_PWM, PWM2);
    digitalWrite(MD1, 0);
    digitalWrite(MD2, 1);
    digitalWrite(MI1, 0);
    digitalWrite(MI2, 1);
    delay(T);
}

void backward(byte PWM1, byte PWM2, int T) {
    ledcWrite(MD1_PWM, PWM1);
    ledcWrite(MI1_PWM, PWM2);
    digitalWrite(MD1, 1);
    digitalWrite(MD2, 0);
    digitalWrite(MI1, 1);
    digitalWrite(MI2, 0);
    delay(T);
}

void right(byte PWM1, byte PWM2, int T) {
    ledcWrite(MD1_PWM, PWM1);
    ledcWrite(MI1_PWM, PWM2);
    digitalWrite(MD1, 0);
    digitalWrite(MD2, 1);
    digitalWrite(MI1, 1);
    digitalWrite(MI2, 0);
    delay(T);
}

void left(byte PWM1, byte PWM2, int T) {
    ledcWrite(MD1_PWM, PWM1);
    ledcWrite(MI1_PWM, PWM2);
    digitalWrite(MD1, 1);
    digitalWrite(MD2, 0);
    digitalWrite(MI1, 0);
    digitalWrite(MI2, 1);
    delay(T);
}

void mstop(int T) {
    ledcWrite(MD1_PWM, 0);
    ledcWrite(MI1_PWM, 0);
    digitalWrite(MD1, 0);
    digitalWrite(MD2, 0);
    digitalWrite(MI1, 0);
    digitalWrite(MI2, 0);
    delay(T);
}

void stop2(int T) {
    ledcWrite(MD1_PWM, 100);
    ledcWrite(MI1_PWM, 100);
    digitalWrite(MD1, 1);
    digitalWrite(MD2, 1);
    digitalWrite(MI1, 1);
    digitalWrite(MI2, 1);
    delay(T);
}
