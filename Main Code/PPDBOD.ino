// Include dependencies (libraries)
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "TMP117.h"

// Instantiate TMP117 class with id of one and debugging set to false
float data[256];
float data_prime[255];
float dir = 0;
float tolerance = 7; //HYPERPARAM

// Mean that you are comparing difference of datapoints with is mean of datapoints, not mean of difference between datapoints, it will always be outside of 3SDs

void setup()
{
    Serial.begin(9600);

    for (int i=0; i<255; i++)
    {
        data_prime[i] = data[i+1]-data[i];
    }

    float sum=0;
    for (int i=0; i<255; i++)
    {
        sum+=data_prime[i];
    }

    float mean = sum/255;
    sum=0;

    for (int i=0; i<255; i++)
    {
        sum += pow(data_prime[i]-mean,2);
    }
    float stdDev = sqrt(sum/255);

    for (int i=0; i<255; i++)
    {
        if (data_prime[i]>(mean+(3*stdDev)))
        {
            dir++;
            Serial.println(data[i+1]);
            Serial.println("OUTLIER");
        }
        else if (data_prime[i]<(mean-(3*stdDev)))
        {
            dir--;
            Serial.println(data[i+1]);
            Serial.println("OUTLIER");
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
