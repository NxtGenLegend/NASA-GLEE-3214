// Include dependencies (libraries)
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "TMP117.h"

// Instantiate TMP117 class with id of one and debugging set to false
float data[256];
float dir = 0;
float tolerance = 7; //HYPERPARAM

void setup()
{
    Serial.begin(9600);

    float sum=0;
    for (int i=0; i<256; i++)
    {
        sum+=data[i];
    }

    float mean = sum/256;
    sum=0;

    for (int i=0; i<256; i++)
    {
        sum += pow(data[i]-mean,2);
    }
    float stdDev = sqrt(sum/256);

    for (int i=0; i<256-1; i++)
    {
        if (data[i+1]-data[i]>(mean+(3*stdDev)))
        {
            dir++;
            Serial.println(data[i+1]);
        }
        else if (data[i+1]-data[i]<(mean-(3*stdDev)))
        {
            dir--;
            Serial.println(data[i+1]);
        }
        else
        {dir=0;}

        if (abs(dir)>tolerance)
        {
            Serial.println("... CONTINUOUS OUTLIER ... POTENTIAL EVENT");
        }
    }
    

};

void loop()
{

};
