// Include dependencies (libraries)
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "TMP117.h"

// Instantiate TMP117 class with id of one and debugging set to false
TMP117 thermometer(1,false);

// Observation configuration
int timeBetweenSample; // Time between samples in ms 

// Dynamic Variables
float temperatureC=0;  // Variable to store an observed temperature in Celsius
float temperatureF;  // Variable to store an observed temperature in Fahrenheit
float startTime;    // Variables for timing offset calculations
float endTime;
float prevVal=0;
float persistency = 0;
const float persistency_threshold = 5; //hyperparam to tune
float mean, stdDev;

void setup(){
    // Begin Serial Communications (Lunasat baud rate set to 9600 bits per second)
    Serial.begin(9600);

    // Set time between samples to 1000ms (1s)
    timeBetweenSample = 10;

    
    const int short_term_memory = 15; //again to tune
    float memory[short_term_memory];

    for (int i=0; i<short_term_memory; i++)
    {
       float temp = thermometer.getTemperatureC();
       memory[i] = temp-prevVal;
       delay(100000000000000000000000000000000000000); //need to do the calibration point thing like in the cap example so as to take readings in diff environments (for all sensors)
       prevVal = temp;
    }
    memory[0] = thermometer.getTemperatureC()-prevVal;

    float sum = 0;

    for (int i=0; i<short_term_memory; i++)
    {
        sum += memory[i];
    }

    mean = sum/short_term_memory;
    sum=0;

    for (int i=0; i<short_term_memory; i++)
    {
        sum += pow(memory[i]-mean,2);
    }

    stdDev = sqrt(sum/short_term_memory);
};

void loop(){
    // Record start time for letter offseting of delay to make the loop run once per second
    startTime = millis();

    // Sample temperature in C
    temperatureC = thermometer.getTemperatureC();
    
    // Sample temperature in F
    temperatureF = thermometer.getTemperatureF();
    
    // Output the temperature via serial
    Serial.print("Temperature: "); 
    Serial.print(temperatureC);
    Serial.print(" (degrees C), ");
    Serial.print(temperatureF);
    Serial.println(" (degrees F)");

    if ((temperatureC-prevVal)>=(mean+(3*stdDev)) || (temperatureC-prevVal)<=(mean-(3*stdDev)))
    {
        //use arduino STL's vector to effectively append to your list
        Serial.println("OUTLIER");
        persistency++;
        if(persistency==persistency_threshold) //== here bcz i dont want to print potential event 1000s of times for a longer event
        {
            Serial.println("POTENTIAL EVENT");
        }
    }
    else
    {
        persistency = 0;
    }


    prevVal = temperatureC;

    // Record the end time so we know how long it took to record the measurement
    endTime = millis();

    // Delay each loop by the sample rate off set by the time it took to get the temperature
    delay(timeBetweenSample - (endTime - startTime));
};
