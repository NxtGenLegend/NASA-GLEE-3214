#include <math.h>

// Make separate implementation without math.h and with the mathematical operators as part of the encoding 
// Adding math operators & removing math.h

String key = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`~!@#$%&_|?";
const int data_length = 256;
float input[data_length];
String output[data_length];

void setup()
{
    Serial.begin(9600);
    for (int i=0; i<data_length; i++)
    {
        if (fmod(input[i], 1.0)==0)
        {output[i] = b10_to_bX(floor(input[i]), key) + "." + b10_to_bX(fmod(input[i], 1.0), key);}
        else
        {output[i] = b10_to_bX(input[i], key);}
        Serial.println(input[i]);
        Serial.print(" : ");
        Serial.print(output[i]);
    }

}

void loop()
{

}

String b10_to_bX(float input, String key)
{
    float next_val = input;
    String encoded_string = "";
    //bool flag = false;
    while(true)
    {
        float cur_digit = fmod(next_val, key.length());
        encoded_string += key[cur_digit];
        next_val = floor(next_val/key.length());
        if (next_val==0)
        {
            break;
        }
    }
    return encoded_string;

}
