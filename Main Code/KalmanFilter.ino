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

// Standard V4 Lunasat I2C Sensors
//AK09940 - Magnetometer
//TMP117 - Temperature Sensor
//TPIS1385 - Thermopile
//ICM20602 - Accelerometer
//sensor-wise measurement uncertainty, process noise

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

SimpleKalmanFilter thermopile(1, 0.01); //edit
SimpleKalmanFilter tempsensor (0.3,0.01); //edit
SimpleKalmanFilter accelerometer (1, 0.01); //edit
SimpleKalmanFilter magnetometer (1,0.01); //edit
SimpleKalmanFilter capsensor(1,0.01); //edit
SimpleKalmanFilter solarpanel (1, 0.01); //edit

// Serial output refresh time
const long SERIAL_REFRESH_TIME = 1000;
long refresh_time;

void setup() {
  Serial.begin(9600);

  Wire.begin();          // Begin Serial Communications
  Wire.setClock(100000); // Set Standard 100Mhz i2c speed
  Serial.begin(9600);    // Set Default

  // Sensor Initializations

  // Temperature Sensor - TMP117 (Nothing)

  // Accelerometer - MPU6000
  accelerometer.begin();                          // Begin (check connection)
  accelerometer.initialize();                     // Initialize (setup)
  accelerometer.setAccelRange(MPU6000_RANGE_2_G); // Make sure acceleration range is set to 2G

  // Magnetometer - MLX90393
  magnetometer.begin_I2C();
  // Set gain, resolution,OSR, and digital filter
  magnetometer.setGain(8); // default value is 8, can range from 0-15
  magnetometer.setResolution(MLX90395_RES_17, MLX90395_RES_17, MLX90395_RES_17);
  magnetometer.setOSR(MLX90395_OSR_4);       // not default. default is MLX90395_OSR_1 and is equivalent to 0
  magnetometer.setFilter(MLX90395_FILTER_8); // default. MLX90395_FILTER_1 is equivalent to 0

  // Capacitive Sensor CAP
  capacitive.begin(); // Begin

  // Thermopile - TPIS1385
  thermopile.begin();      // Begin
  thermopile.readEEprom(); // Check Registers

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {

  // read sensor values 
  float thermopile_real_value = thermopile.getSample();
  float tempsensor_real_value = thermometer.getTemperatureC();
  float accelerometer_real_value = accelerometer.getSample();
  float magnetometer_real_value = magnetometer.getSample ();
  float capsensor_real_value = capacitive.getRawData();
  float solarpanel_real_value = analogRead(A1)/1024.0*100.0;
  

  // calculate the estimated value with Kalman Filter
  float thermopile_estimated_value = thermopile.updateEstimate (thermopile_real_value);
  float tempsensor_estimated_value = tempsensor.updateEstimate(tempsensor_real_value);
  float accelerometer_estimated_value= accelerometer.updateEstimate(accelerometer_real_value);
  float magnetometer_estimated_value= magnetometer.updateEstimate (magnetometer_real_value);
  float capsensor_estimated_value = capsensor.updateEstimate (capsensor_real_value);
  float solarpanel_estimated_value = solarpanel.updateEstimate (solarpanel_real_value);

  

  // send to Serial output every 100ms
  // use the Serial Plotter for a good visualization
  if (millis() > refresh_time) {
    //printing thermopile values
    Serial.print(thermopile_real_value,4);
    Serial.print(",");
    Serial.print(thermopile_estimated_value,4);
    //printing temp sensor values
    Serial.print(tempsensor_real_value,4);
    Serial.print(",");
    Serial.print(tempsensor_estimated_value,4);
    //printing accelerometer values
    Serial.print(accelerometer_real_value,4);
    Serial.print(",");
    Serial.print(accelerometer_estimated_value,4);
    //printing magnetometer values
    Serial.print(magnetometer_real_value,4);
    Serial.print(",");
    Serial.print(magnetometer_estimated_value,4);
    //printing capacitive sensor values
    Serial.print(capsensor_real_value,4);
    Serial.print(",");
    Serial.print(capsensor_estimated_value,4);
    //printing solar panel values
    Serial.print(solarpanel_real_value,4);
    Serial.print(",");
    Serial.print(solarpanel_estimated_value,4);
    Serial.println();
    
    refresh_time = millis() + SERIAL_REFRESH_TIME;
  }

}
