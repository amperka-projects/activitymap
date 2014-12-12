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

    if(analogRead(A0) > 100)
    {
        if(++n1 >= 20)
        {
            if(n1 > 20+1) n1--;

            if(n1 == 20) sensor1();
        }
    }
    else n1 = 0;
    
    if(analogRead(A1) > 100)
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

