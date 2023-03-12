#include "MLX90395.h"
#include "MPU6000.h"

MPU6000 accelerometer(2);
MLX90395 magnetometer(3);

// Set initial values for acceleration and magnetometer readings
float ax = 0.0, ay = 0.0, az = 0.0;
float mx = 0.0, my = 0.0, mz = 0.0;

// Set initial values for exponential moving average
float alpha_acc = 0.5;
float alpha_mag = 0.5;
float x_acc_filtered = 0.0, y_acc_filtered = 0.0, z_acc_filtered = 0.0;
float x_mag_filtered = 0.0, y_mag_filtered = 0.0, z_mag_filtered = 0.0;

// Set initial values for threshold
float threshold_acc = 0.0;
float threshold_mag = 0.0;

// Set initial flag for seismic activity
bool seismic_activity = false;

// Initializing variables
sensor_float_vec_t accSample; // 3
mlx_sample_t magSample;       // 3

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);
    
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

    // Set threshold_acc and threshold_mag to mean plus three standard deviations
    threshold_acc = alpha_acc * ax;
    threshold_mag = alpha_mag * mx;
}

void loop()
{
    // Read raw acceleration and magnetometer values
    accSample = accelerometer.getSample();
    magSample = magnetometer.getSample();
    ax = accSample.x;
    ay = accSample.y;
    az = accSample.z;
    mx = magSample.magnetic.x;
    my = magSample.magnetic.y;
    mz = magSample.magnetic.z;

    // Calculate exponential moving average with dynamically adjusted smoothing factor for acceleration readings
    alpha_acc = abs((ax + ay + az) - (x_acc_filtered + y_acc_filtered + z_acc_filtered)) / (ax + ay + az);
    x_acc_filtered = alpha_acc * ax + (1 - alpha_acc) * x_acc_filtered;
    y_acc_filtered = alpha_acc * ay + (1 - alpha_acc) * y_acc_filtered;
    z_acc_filtered = alpha_acc * az + (1 - alpha_acc) * z_acc_filtered;

    // Calculate exponential moving average with dynamically adjusted smoothing factor for magnetometer readings
    alpha_mag = abs((mx + my + mz) - (x_mag_filtered + y_mag_filtered + z_mag_filtered)) / (mx + my + mz);
    x_mag_filtered = alpha_mag * mx + (1 - alpha_mag) * x_mag_filtered;
    y_mag_filtered = alpha_mag * my + (1 - alpha_mag) * y_mag_filtered;
    z_mag_filtered = alpha_mag * mz + (1 - alpha_mag) * z_mag_filtered;

    // Calculate RMS acceleration
    float rms_acc = sqrt((pow(x_acc_filtered, 2) + pow(y_acc_filtered, 2) + pow(z_acc_filtered, 2)) / 3);

    // Check for seismic activity
    if (rms_acc > threshold_acc && abs(x_mag_filtered) > threshold_mag && abs(y_mag_filtered) > threshold_mag && abs(z_mag_filtered) > threshold_mag)
    {
        seismic_activity = true;
    }
    else
    {
        seismic_activity = false;
    }
}

// Useful values = RMS Acceleration (Lunar Surface Roughness) and Seismic Activity (Bool)
// Need to tune for movement (RMS Magnetometer Values)

// Smooth: < 0.02 m/s^2
// Moderately smooth: 0.02-0.04 m/s^2
// Moderately rough: 0.04-0.1 m/s^2
// Rough: 0.1-0.2 m/s^2
// Very rough: > 0.2 m/s^2