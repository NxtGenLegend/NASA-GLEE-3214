#include "MPU6000.h"
#include <math.h>

// Create an instance of the MPU6000 class
MPU6000 accelerometer(1, false);

// Define the size of the calibration array
const int calibrationSize = 40;

// Create an array to store the calibration points
sensor_float_vec_t calibrationPoints[calibrationSize];

// Define arrays to store the corrected and base values for each axis
float correctedX[calibrationSize];
float correctedY[calibrationSize];
float correctedZ[calibrationSize];
float baseX, baseY, baseZ;

// Define variables to store the mode value for each axis
float modeX, modeY, modeZ;

// Define variables to store the RMS value for each axis
float rmsX, rmsY, rmsZ;

void setup() {
  Serial.begin(9600);
  
  // Initialize the accelerometer
  accelerometer.begin();
  accelerometer.initialize();
  accelerometer.setAccelRange (MPU6000_RANGE_2_G);
  
  // Take the first couple of readings and calculate the mean values for X, Y, and Z axes
  float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
  for (int i = 0; i < calibrationSize; i++) {
    calibrationPoints[i] = accelerometer.getSample();
    sumX += calibrationPoints[i].x;
    sumY += calibrationPoints[i].y;
    sumZ += calibrationPoints[i].z;
  }
  baseX = sumX / calibrationSize;
  baseY = sumY / calibrationSize;
  baseZ = sumZ / calibrationSize;
  
  // Calculate the corrected values for each calibration point
  for (int i = 0; i < calibrationSize; i++) {
    correctedX[i] = calibrationPoints[i].x - baseX;
    correctedY[i] = calibrationPoints[i].y - baseY;
    correctedZ[i] = calibrationPoints[i].z - baseZ;
  }
}

void loop() {
  // Get a new sample from the accelerometer
  sensor_float_vec_t sample = accelerometer.getSample();
  
  // Calculate the corrected values for the new sample
  float cx = sample.x - baseX;
  float cy = sample.y - baseY;
  float cz = sample.z - baseZ;
  
  // Store the corrected and base values
  correctedX[calibrationSize] = cx;
  correctedY[calibrationSize] = cy;
  correctedZ[calibrationSize] = cz;
  
  // Update the mode value for each axis
  modeX = calculateMode(correctedX, calibrationSize);
  modeY = calculateMode(correctedY, calibrationSize);
  modeZ = calculateMode(correctedZ, calibrationSize);

  // Update the RMS value for each axis
  rmsX = calculateRMS(correctedX, calibrationSize);
  rmsY = calculateRMS(correctedY, calibrationSize);
  rmsZ = calculateRMS(correctedZ, calibrationSize);

  // Print the results
  Serial.print("Corrected X: "); Serial.print(cx);
  Serial.print("  Base X: "); Serial.print(baseX);
  Serial.print("  Mode X: "); Serial.print(modeX);
  Serial.print("  RMS X: "); Serial.print(rmsX);
  Serial.println();
  
  Serial.print("Corrected Y: "); Serial.print(cy);
  Serial.print("  Base Y: "); Serial.print(baseY);
  Serial.print("  Mode Y: "); Serial.print(modeY);
  Serial
