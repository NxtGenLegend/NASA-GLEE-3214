#include "SimpleKalmanFilter.h"
#include "CAP.h"
#include <GLEE_Radio.h>
#include "TMP117.h"
#include "MLX90395.h"
#include "MPU6000.h"
#include "TPIS1385.h"
#include <Wire.h>

// Measurement Uncertainty = Estimated Uncertainty & Process Variance Addition,
// Subsequent Calibration & Testing

TMP117 thermometer(1);
MPU6000 accelerometer(2);
MLX90395 magnetometer(3);
TPIS1385 thermopile(4);
CAP capacitive(5);

TPsample_t thermoSample; // 2
float tempSample;
sensor_float_vec_t accSample; //3
mlx_sample_t magSample; //3
float capSample;
float solarVoltageValue;

// Thermopile Object, Thermopile Ambient, Temperature, Acceleration X, Acceleration Y, Acceleration Z, 
// Magnetometer X, Magnetometer Y, Magnetometer Z, Capacitive, Solar
float KalmanG[11];
float EU[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
float MU[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
float lastEstimate[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float currentEstimate[11];
float proVar[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
float rms [6];
float finalcurrentEst[11];



void setup()
{
    // Sensor Initializations

    // Temperature Sensor - TMP117 (Nothing)

    // Accelerometer - MPU6000
    accelerometer.begin();                          // Begin (check connection)
    accelerometer.initialize();                     // Initialize (setup)
    accelerometer.setAccelRange(MPU6000_RANGE_2_G); // Make sure acceleration range is set to 2G

    // Magnetometer - MLX90393
    magnetometer.begin_I2C();
    magnetometer.setGain(8); // default value is 8, can range from 0-15
    magnetometer.setResolution(MLX90395_RES_17, MLX90395_RES_17, MLX90395_RES_17);
    magnetometer.setOSR(MLX90395_OSR_4);       // not default. default is MLX90395_OSR_1 and is equivalent to 0
    magnetometer.setFilter(MLX90395_FILTER_8); // default. MLX90395_FILTER_1 is equivalent to 0

    // Capacitive Sensor CAP
    capacitive.begin(); // Begin

    // Thermopile - TPIS1385
    thermopile.begin();      // Begin
    thermopile.readEEprom(); // Check Registers
}

void loop()
{
    thermoSample = thermopile.getSample();
    tempSample = thermometer.getTemperatureC();
    accSample = accelerometer.getSample();
    magSample = magnetometer.getSample();
    capSample = capacitive.getRawData();
    solarVoltageValue = analogRead(A1) / 1024.0 * 100.0;

    float Samples[11] = { thermoSample.object,
                          thermoSample.ambient,
                          tempSample,
                          accSample.x,
                          accSample.y,
                          accSample.z,
                          magSample.magnetic.x,
                          magSample.magnetic.y,
                          magSample.magnetic.z,
                          capSample,
                          solarVoltageValue }; // Multiple dimensions?

    float RMS [11] = {sqrt(pow(accSample.x, 2)),
                      sqrt(pow(accSample.y, 2)),
                      sqrt(pow(accSample.z, 2)), 
                      sqrt(pow(magSample.magnetic.x, 2)),
                      sqrt(pow(magSample.magnetic.y, 2)),
                      sqrt(pow(magSample.magnetic.z, 2)),
                      sqrt(pow(thermoSample.object, 2)),
                      sqrt(pow(thermoSample.ambient, 2)),
                      sqrt(pow(tempSample, 2)),
                      sqrt(pow(capSample, 2)),
                      sqrt(pow(solarVoltageValue, 2)) };
    //or     float RMS [6] = { sqrt(pow(accSample.x, 2) + pow(accSample.y, 2) + pow(accSample.z, 2)), //compute together or separately?
                      //sqrt(pow(magSample.magnetic.x, 2) + pow(magSample.magnetic.y, 2) + pow(magSample.magnetic.z, 2)),
                      //sqrt(pow(thermoSample.object, 2) + pow(thermoSample.ambient, 2)),
                      //pow(tempSample, 2),
                      //pow(capSample, 2),
                      //pow(solarVoltageValue, 2) };

    
  
    // Simple Kalman Filter 
    
    for (int i = 0; i < 11; i++)
    {
       KalmanG[i] = EU[i] / (EU[i] + MU[i]);
        currentEstimate[i] = lastEstimate[i] + KalmanG[i] * (RMS[i] - lastEstimate[i]);
        EU[i] = (1.0f - KalmanG[i]) * EU[i] + fabsf(lastEstimate[i] - currentEstimate[i]) * proVar[i];
        lastEstimate[i] = currentEstimate[i];
    }
    //create global array for current estimate values (instead of return currentEstimate function)
        for (int i = 0; i < 11; i++) {
        finalcurrentEst[i] = currentEstimate[i];
    }

    Serial.println("Filtered values: ");

    for (int i = 0; i < 11; i++) {
        Serial.print(finalcurrentEst[i]);
        Serial.print(" ");
    }
    
}