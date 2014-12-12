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

