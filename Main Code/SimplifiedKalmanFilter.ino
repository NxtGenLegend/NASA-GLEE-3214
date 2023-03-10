#include "SimpleKalmanFilter.h"
#include "CAP.h"
#include <GLEE_Radio.h>
#include "TMP117.h"
#include "MLX90395.h"
#include "MPU6000.h"
#include "TPIS1385.h"
#include <Wire.h>
#include <ArduinoEigen.h>
#include "eigen.h"

// SimpleKalmanFilter Constructor Modification, Sensor Constructor Addition, I2C Setup
// Measurement Uncertainty = Estimated Uncertainty & Process Variance Addition,
// Subsequent Calibration & Testing

TMP117 thermometer(1);
MPU6000 accelerometer(2);
MLX90395 magnetometer(3);
TPIS1385 thermopile(4);
CAP capacitive(5);

float tempSample;
sensor_float_vec_t accSample;
mlx_sample_t magSample;
TPsample_t thermSample;
int capSample;
float solarVoltageValue;

float KalmanG;
float EU;
float MU;
float lastEstimate = 0;
float currentEstimate;
float proVar;

SimpleKalmanFilter ThermoKf(1, 1, 1); // tune
SimpleKalmanFilter TempKf(1, 1, 1);   // tune
SimpleKalmanFilter AccKf(1, 1, 1);    // tune
SimpleKalmanFilter MagKf(1, 1, 1);    // tune
SimpleKalmanFilter CapKf(1, 1, 1);    // tune
SimpleKalmanFilter SolarKf(1, 1, 1);  // tune

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
    tempSample = thermometer.getTemperatureC();
    accSample = accelerometer.getSample();
    magSample = magnetometer.getSample();
    thermSample = thermopile.getSample();
    capSample = capacitive.getRawData();
    solarVoltageValue = analogRead(A1) / 1024.0 * 100.0;

    // Simplified Kalman Filter
    KalmanG = EU / (EU  + MU);
    currentEstimate = lastEstimate + KalmanG * (MU - lastEstimate);
    EU = (1.0f - KalmanG) * EU + fabsf(lastEstimate - currentEstimate) * proVar;
    lastEstimate = currentEstimate;

    // calculate the estimated value with Kalman Filter
    // float thermopile_estimated_value = ThermoKf.updateEstimate (thermSample); // Datatype Conversion Required
    float tempsensor_estimated_value = TempKf.updateEstimate(tempSample);
    // float accelerometer_estimated_value = AccKf.updateEstimate(accSample); // Datatype Conversion Required
    // float magnetometer_estimated_value = MagKf.updateEstimate(magSample);  // Datatype Conversion Required
    float capsensor_estimated_value = CapKf.updateEstimate(capSample);
    float solarpanel_estimated_value = SolarKf.updateEstimate(solarVoltageValue);
}

/*Algorithm For Implementation: (_err_estimate, = _err_measure, _q)
float SimpleKalmanFilter::updateEstimate(float mea)
{
  _kalman_gain = _err_estimate/(_err_estimate + _err_measure);
  _current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
  _err_estimate =  (1.0f - _kalman_gain)*_err_estimate + fabsf(_last_estimate-_current_estimate)*_q;
  _last_estimate=_current_estimate;

  return _current_estimate;
}
*/