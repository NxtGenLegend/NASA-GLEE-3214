#include <ArduinoEigen.h>
#include "eigen.h"
#include <Eigen/LU> 
using namespace Eigen;

const int NUM_SENSORS = 10;
const int READINGS_PER_SENSOR = 100; // Change this to the appropriate number of readings per sensor

Eigen::MatrixXd data(READINGS_PER_SENSOR, NUM_SENSORS);

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);

    // Set up columns in data matrix for each sensor
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        data.col(i).setZero();
    }
}

void loop()
{
    // Read data from each sensor and add it to the appropriate column in the data matrix
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        for (int j = 0; j < READINGS_PER_SENSOR; j++)
        {
            // Read sensor value
            int sensor_value = analogRead(i);

            // Add sensor value to appropriate column in data matrix
            data(j, i) = sensor_value;
        }
    }

    // Print out the data matrix (for testing purposes)
    //Serial.println(data);

    // Wait for a short period before taking the next set of readings
    delay(100);
}