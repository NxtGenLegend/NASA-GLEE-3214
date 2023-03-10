#include "CAP.h"
#include <GLEE_Radio.h>
#include "TMP117.h"
#include "MLX90395.h"
#include "MPU6000.h"
#include "TPIS1385.h"
#include <Wire.h>

// Standard V4 Lunasat I2C Sensors
//AK09940 - Magnetometer
//TMP117 - Temperature Sensor
//TPIS1385 - Thermopile
//ICM20602 - Accelerometer
//sensor-wise measurement uncertainty, process noise

// I2C Setup
// Measurement Uncertainty = Estimated Uncertainty & Process Variance Addition, 
// Subsequent Calibration & Testing

//flow of code: defining initial state+state covariance+noise covariance+measurement covariance, kalman gains,
//collecting sensor data, q process noise covariance, r , predict and update to output noise-accounted vs original values

//NOTE: VALUES NEED TO BE TUNED!!!
//ALSO NOTE: I don't like the thermometer / temp sensor inconsistency, need to fix 

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

// Initial state est. (taken as 0 assuming equilibrium, need to tune)
float thermometer_init=0;
float accelerometer_init =0;
float magnetometer_init = 0;
float thermopile_init = 0;
float capacitive_init= 0;
float solarpanel_init=0;

//initial state covariances (p)
float p_thermometer=0.1;
float p_accelerometer =0.1;
float p_magnetometer = 0.1;
float p_thermopile = 0.1;
float p_capacitive = 0.1;
float p_solarpanel=0.1;

//Process noise covariance (q)
float q_thermometer=0.1;
float q_accelerometer =0.1;
float q_magnetometer = 0.1;
float q_thermopile = 0.1;
float q_capacitive = 0.1;
float q_solarpanel=0.1;

// Measurement noise covariances (r) //no tuning required, only if necessary, just rechecking w sensor datasheets 
float r_thermometer=0.1;
float r_accelerometer =0.1;
float r_magnetometer = 0.1;
float r_thermopile = 0.1;
float r_capacitive = 0.1;
float r_solarpanel=0.1;

// Kalman gain (k)
float k_thermometer;
float k_accelerometer;
float k_magnetometer;
float k_thermopile;
float k_capacitive;
float k_solarpanel;

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

}

void loop () {

  // read sensor values 
    float thermopile_real_value = thermopile.getSample();
    float tempsensor_real_value = thermometer.getTemperatureC();
    float accelerometer_real_value = accelerometer.getSample();
    float magnetometer_real_value = magnetometer.getSample ();
    float capsensor_real_value = capacitive.getRawData();
    float solarpanel_real_value = analogRead(A1)/1024.0*100.0;

//note: 2 options for flow of data, sensor-wise updating or operation-wise filtering, I've done the latter as of rn

//Prediction of values
    //thermometer (temp sensor)
    float thermometer_pred= thermometer_init;
    float p_pred_thermometer= p_thermometer + q_thermometer;
    //thermopile
    float thermopile_pred= thermopile_init;
    float p_pred_thermopile= p_thermopile + q_thermopile;
    //accelerometer
    float accelerometer_pred= accelerometer_init;
    float p_pred_accelerometer= p_accelerometer + q_accelerometer;
    //magnetometer
    float magnetometer_pred= magnetometer_init;
    float p_pred_magnetometer= p_magnetometer + q_magnetometer;
    //cap sensor
    float capacitive_pred = capacitive_init;
    float p_pred_capacitive= p_capacitive + q_capacitive;
    //solar panel
    float solarpanel_pred= solarpanel_init;
    float p_pred_solarpanel= p_solarpanel + q_solarpanel;
 
//Updation of values based on predictions
    //temp sensor
    k_thermometer= p_pred_thermometer / (p_pred_thermometer + r_thermometer);
    thermometer_init= thermometer_pred + k_thermometer * (thermopile_real_value-thermometer_pred);
    p_final_thermometer= (1- k_thermometer)* p_pred_thermometer; 


}
