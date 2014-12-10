
void setup()
{
    Serial.begin(9600);
    digitalWrite(13, LOW);
    digitalWrite(4, HIGH);
}

signed long t1 = 0, t2 = 0;

void sensor1()
{
    t1 = millis();
    Serial.print("t1=");
    Serial.println(t1);

    check_motion();
}

void sensor2()
{
    t2 = millis();
    Serial.print("t2=");
    Serial.println(t2);

    check_motion();
}

void check_motion()
{
    if(t1 == 0 || t2 == 0) return;

    Serial.println(t1-t2);

    if(t2-t1 > 0) tone(9, 100, 1000);
    else if(t1-t2 > 0) tone(9, 300, 1000);

    t1 = t2 = 0;
}

void loop()
{
    static unsigned long n1 = 0, n2 = 0;

    if(analogRead(A0) > 100)
    {
        if(++n1 >= 20)
        {
            if(n1 > 20+1) n1--;

            if(n1 == 20) sensor1()/*, tone(9, 7000, 100)*/;
        }
    }
    else n1 = 0;
    
    if(analogRead(A1) > 100)
    {
        if(++n2 >= 20)
        {
            if(n2 > 20+1) n2--;

            if(n2 == 20) sensor2()/*, tone(9, 7000, 100)*/;
        }
    }
    else n2 = 0;

    delay(1);
}

