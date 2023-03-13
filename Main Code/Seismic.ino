#include "MLX90395.h"
#include "MPU6000.h"

MPU6000 accelerometer(2);
MLX90395 magnetometer(3);

// CONVERT TO ARRAYS FOR GENERALIZATION

// Set initial values for acceleration and magnetometer readings
float ax = 0.0, ay = 0.0, az = 0.0;
float mx = 0.0, my = 0.0, mz = 0.0;

// Set initial values for exponential moving average
float alpha_acc = 0.5;
float alpha_mag = 0.5;
float x_acc_filtered = 0.0, y_acc_filtered = 0.0, z_acc_filtered = 0.0;
float x_mag_filtered = 0.0, y_mag_filtered = 0.0, z_mag_filtered = 0.0;

// Set initial values for thresholds
float threshold_acc = 0.0;
float mean_ax = 0.0;
float mean_ay = 0.0;
float mean_az = 0.0;
float std_ax = 0.0;
float std_ay = 0.0;
float std_az = 0.0;

float mean_mx = 0.0;
float mean_my = 0.0;
float mean_mz = 0.0;
float std_mx = 0.0;
float std_my = 0.0;
float std_mz = 0.0;

float threshold_ax = 0.0;
float threshold_ay = 0.0;
float threshold_az = 0.0;

float threshold_mx = 0.0;
float threshold_my = 0.0;
float threshold_mz = 0.0;

// Set initial flag for seismic activity
bool seismic_activity = false;

// Initializing variables for Accelerometer & Magnetometer samples
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

    // Calculate mean and standard deviation for each axis
    mean_ax = alpha_acc * ax + (1 - alpha_acc) * mean_ax;
    mean_ay = alpha_acc * ay + (1 - alpha_acc) * mean_ay;
    mean_az = alpha_acc * az + (1 - alpha_acc) * mean_az;

    std_ax = sqrt((alpha_acc * (ax - mean_ax) * (ax - mean_ax)) + ((1 - alpha_acc) * std_ax * std_ax));
    std_ay = sqrt((alpha_acc * (ay - mean_ay) * (ay - mean_ay)) + ((1 - alpha_acc) * std_ay * std_ay));
    std_az = sqrt((alpha_acc * (az - mean_az) * (az - mean_az)) + ((1 - alpha_acc) * std_az * std_az));

    // Update threshold for each axis dynamically
    float threshold_ax = mean_ax + 3 * std_ax;
    float threshold_ay = mean_ay + 3 * std_ay;
    float threshold_az = mean_az + 3 * std_az;

    // Calculate exponential moving average with dynamically adjusted smoothing factor for magnetometer readings
    alpha_mag = abs((mx + my + mz) - (x_mag_filtered + y_mag_filtered + z_mag_filtered)) / (mx + my + mz);
    x_mag_filtered = alpha_mag * mx + (1 - alpha_mag) * x_mag_filtered;
    y_mag_filtered = alpha_mag * my + (1 - alpha_mag) * y_mag_filtered;
    z_mag_filtered = alpha_mag * mz + (1 - alpha_mag) * z_mag_filtered;

    // Calculate mean and standard deviation for each axis
    mean_mx = alpha_mag * mx + (1 - alpha_mag) * mean_mx;
    mean_my = alpha_mag * my + (1 - alpha_mag) * mean_my;
    mean_mz = alpha_mag * mz + (1 - alpha_mag) * mean_mz;

    std_mx = sqrt((alpha_mag * (mx - mean_mx) * (mx - mean_mx)) + ((1 - alpha_mag) * std_mx * std_mx));
    std_my = sqrt((alpha_mag * (my - mean_my) * (my - mean_my)) + ((1 - alpha_mag) * std_my * std_my));
    std_mz = sqrt((alpha_mag * (mz - mean_mz) * (mz - mean_mz)) + ((1 - alpha_mag) * std_mz * std_mz));

    // Update threshold for each axis dynamically
    threshold_mx = mean_mx + 3 * std_mx;
    threshold_my = mean_my + 3 * std_my;
    threshold_mz = mean_mz + 3 * std_mz;

    // Calculate RMS acceleration
    float rms_acc = sqrt((pow(x_acc_filtered, 2) + pow(y_acc_filtered, 2) + pow(z_acc_filtered, 2)) / 3);
    threshold_acc = sqrt((pow(threshold_ax, 2) + pow(threshold_ay, 2) + pow(threshold_az, 2)) / 3);

    // Check for seismic activity
    if (rms_acc > threshold_acc && abs(x_mag_filtered) > threshold_mx && abs(y_mag_filtered) > threshold_my && abs(z_mag_filtered) > threshold_mz)
    {
        seismic_activity = true;
    }
    else
    {
        seismic_activity = false;
    }
}

// Threshold must be triple axes, not single for magnetometer
// Useful values = RMS Acceleration (Lunar Surface Roughness) and Seismic Activity (Bool)
// Need to tune for movement (RMS Magnetometer Values)

// Smooth: < 0.02 m/s^2
// Moderately smooth: 0.02-0.04 m/s^2
// Moderately rough: 0.04-0.1 m/s^2
// Rough: 0.1-0.2 m/s^2
// Very rough: > 0.2 m/s^2