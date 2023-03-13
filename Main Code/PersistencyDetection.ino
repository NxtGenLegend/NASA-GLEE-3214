// Include dependencies (libraries)
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "TMP117.h"

// Instantiate TMP117 class with id of one and debugging set to false

float data[256];
float dir = 0;
float tolerance = 7; //HYPERPARAM

float packet_means[256]; //means of each successive packet
int packets_parsed = 0;

// SampleSize Addition, Variable Naming & Formatting

void setup()
{
    Serial.begin(9600);
    data[256]; //assign data a value here
}

void loop()
{
    float* primes = data_priming(data, 256);
    float* stats = data_descrtiptors(primes, 255);

    if (dbod(primes, stats[0], stats[1], 256, 7))
    {
        Serial.println("PERSISTENT OUTLIER ... POTENTIAL EVENT");
    }

    delete[] primes;
    //delete[] stats;

    if(packets_parsed==256)
    {
        //packets_parsed=0;
        delete[]stats;

        float* pack_primes = data_priming(packet_means, 256);
        float* pack_stats = data_descrtiptors(pack_primes, 255);
        if (dbod(pack_primes, pack_stats[0], pack_stats[1], 256, 2))
        {
        Serial.println("POTENTIAL MEAN SHIFT ... POTENTIAL CHANGE IN POSITION/CIRCUMSTANCE");
        }
        delete[] pack_stats;
        delete[] pack_primes;
        packets_parsed=0;
    }
    else
    {
        packet_means[packets_parsed] = stats[0];
        packets_parsed++;
    }




}

float* data_priming (float* datum, int size)
{
    float data_prime[size-1];
    for (int i = 0; i < size-1; i++)
    {
        data_prime[i] = datum[i + 1] - datum[i];
    }
    return data_prime;
}

float* data_descrtiptors(float* datum, int size)
{
    float sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += datum[i];
    }

    float mean = sum / size;
    sum = 0;

    for (int i = 0; i < size; i++)
    {
        sum += pow(datum[i] - mean, 2);
    }
    float stdDev = sqrt(sum / size);

    float data_desc_stats[2]={mean, stdDev};
    return data_desc_stats;

}

boolean dbod(float* data_prime, float mean, float stdDev, int data_size, int threshold) //have to figure out what to do about array resizing (will do before testing)
{
    // float* data_prime = data_priming(datum, 256);
    // float* stats = data_descrtiptors(data_prime, data_size-1);
    // float mean = stats[0];
    // float stdDev = stats[1];
    // delete[] stats;

    for (int i = 0; i < 254; i++)
    {
        if (data_prime[i + 1] - data_prime[i] > (mean + (3 * stdDev)))
        {
            dir++;
            Serial.println(data[i + 2]);
            Serial.println("OUTLIER");
        }
        else if (data_prime[i + 1] - data_prime[i] < (mean - (3 * stdDev)))
        {
            dir--;
            Serial.println(data[i + 2]);
            Serial.println("OUTLIER");
        }
        else
        {
            dir = 0;
        }

        if (abs(dir) > threshold)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}
