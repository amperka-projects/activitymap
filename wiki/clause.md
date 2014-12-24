====== Карта офисной активности ======

{{ :projects:activitymap:assembled.jpg?direct&700 }}

===== Что это такое? =====

===== Что нам понадобится? =====

Мастер-устройство:
  - [[prod://arduino-leonardo | Arduino Leonardo]]
  - [[prod://arduino-rs485-shield | DF Robot RS485-шилд]]
  - [[prod://wall-plug-1a | Импульсный блок питания]]

Сенсор-устройство:
  - [[prod://arduino-uno | Arduino Uno]]
  - [[prod://arduino-rs485-shield | DF Robot RS485-шилд]]
  - [[prod://breadboard-mini | Breadboard Mini]]
  - [[prod://wire-mm | Провода «папа-папа»]]
  - [[prod://infrared-range-meter-150 | Инфракрасный дальномер Sharp (20-150 см) ×2 шт]]

Также для прокладки сети вам потребуется медный провод сечением 0,5..1 мм.
===== Как это собрать? =====
{{ :projects:activitymap:scheme.png?direct&700 |}}
===== Исходный код =====

==== Мастер-устройство ====

<code cpp master.ino>
#define STATE_BEGIN    0
#define STATE_SENDED   1
#define STATE_RECEIVED 2

void setup()
{
    Serial.begin(9600);
    Serial1.begin(1200);

    while (!Serial) ;

    pinMode(2, OUTPUT);
}

int curDev = 0;

int state = STATE_BEGIN;

void loop()
{
    static unsigned long t;
    byte msg[4] = { 0x03, 0x20, 0x00, 0x05 };

    if(state == STATE_BEGIN)
    {
        msg[1] = 0x20 + curDev;

        digitalWrite(2, HIGH);
        Serial1.write((const byte*)msg, 4);
        Serial1.flush();
        digitalWrite(2, LOW);

        state = STATE_SENDED;
        t = millis();
    }
    else if(state == STATE_SENDED)
    {
        if(Serial1.available())
        {
            static byte imsg[4];

            imsg[0] = imsg[1];
            imsg[1] = imsg[2];
            imsg[2] = imsg[3];
            imsg[3] = Serial1.read();

            if(imsg[0] == 0x03 && imsg[3] == 0x05)
            {
                Serial.print(curDev);
                Serial.print(':');
                Serial.println((int)(imsg[1]));
                Serial.flush();
                delay(1);

                state = STATE_RECEIVED;
            }
        }
        else if(millis()-t > 50) {
            state = STATE_RECEIVED;
        }
    }
    else if(state == STATE_RECEIVED)
    {
        if(++curDev == 15)
        {
            curDev = 0;
        }

        state = STATE_BEGIN;
    }
}
</code>

==== Сенсор-устройство ====

<code cpp sensor.ino>
signed long t1 = 0, t2 = 0;
unsigned char dir = 0;

#define BASE_ADDR 0x20
#define MSG_LEN 4

void sensor1()
{
    t1 = millis();

    check_motion();
}

void sensor2()
{
    t2 = millis();

    check_motion();
}

void check_motion()
{
    if(t1 == 0 || t2 == 0) return;

    if(t2-t1 > 0)
    {
        tone(9, 100, 1000);
        dir = 1;
    }
    else if(t1-t2 > 0)
    {
        tone(9, 300, 1000);
        dir = 2;
    }

    t1 = t2 = 0;
}

void dispatch_msg(byte* msg)
{
    if(msg[1] != BASE_ADDR+addr()) return;

    msg[0] = 0x03;
    msg[1] = dir;
    msg[2] = 0x00;
    msg[3] = 0x05;

    digitalWrite(2, HIGH);

    Serial.write(msg, 4);
    Serial.flush();

    digitalWrite(2, LOW);

    dir = 0;
}

void setup(void)
{
    Serial.begin(1200);

    digitalWrite(13, LOW);
    digitalWrite(4, HIGH);

    pinMode(8,  INPUT);
    pinMode(9,  INPUT);
    pinMode(10, INPUT);

    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);

    analogReference(INTERNAL);
}

unsigned char addr(void)
{
    unsigned char a = 0x00;

    if(digitalRead(8))  a |= 0x01;
    if(digitalRead(9))  a |= 0x02;
    if(digitalRead(10)) a |= 0x04;

    return a;
}

byte msg[4];

void loop(void)
{
    static unsigned long n1 = 0, n2 = 0;

    if(analogRead(A0) > 500)
    {
        if(++n1 >= 20)
        {
            if(n1 > 20+1) n1--;

            if(n1 == 20) sensor1();
        }
    }
    else n1 = 0;
    
    if(analogRead(A1) > 500)
    {
        if(++n2 >= 20)
        {
            if(n2 > 20+1) n2--;

            if(n2 == 20) sensor2();
        }
    }
    else n2 = 0;

    if(Serial.available())
    {
        int i;
        byte b;

        for(i = 0; i < MSG_LEN-1; i++)
            msg[i] = msg[i+1];

        msg[MSG_LEN-1] = Serial.read();

        if(msg[0] == 0x03 && msg[3] == 0x05)
        {
            dispatch_msg(msg);
        }
    }
}
</code>
===== Демонстрация работы устройства =====

{{youtube>eRLrCk5RlPs?large}}
===== Что можно сделать ещё? =====
