#include "MPU6000.h"
#include "Wire.h"

MPU6000 accelerometer(1, false);
const int calibrationSize = 40;
sensor_float_vec_t calibrationPoints[calibrationSize];
float correctedX[calibrationSize];
float correctedY[calibrationSize];
float correctedZ[calibrationSize];
float baseX, baseY, baseZ;
float modeX, modeY, modeZ;

void setup() {
  Serial.begin(9600);
  accelerometer.begin();
  accelerometer.initialize();
  accelerometer.setAccelRange (MPU6000_RANGE_2_G);
  
  
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
  
  
  for (int i = 0; i < calibrationSize; i++) {
    correctedX[i] = calibrationPoints[i].x - baseX;
    correctedY[i] = calibrationPoints[i].y - baseY;
    correctedZ[i] = calibrationPoints[i].z - baseZ;
  }
}

void loop() {
  
  sensor_float_vec_t sample = accelerometer.getSample();
  

  float cx = sample.x - baseX;
  float cy = sample.y - baseY;
  float cz = sample.z - baseZ;
  

  correctedX[calibrationSize] = cx;
  correctedY[calibrationSize] = cy;
  correctedZ[calibrationSize] = cz;
  

  modeX = calculateMode(correctedX, calibrationSize);
  modeY = calculateMode(correctedY, calibrationSize);
  modeZ = calculateMode(correctedZ, calibrationSize);
  
  
  Serial.print("Corrected X: "); Serial.print(cx);
  Serial.print("  Base X: "); Serial.print(baseX);
  Serial.print("  Mode X: "); Serial.print(modeX);
  Serial.println();
  
  Serial.print("Corrected Y: "); Serial.print(cy);
  Serial.print("  Base Y: "); Serial.print(baseY);
  Serial.print("  Mode Y: "); Serial.print(modeY);
  Serial.println();
  
  Serial.print("Corrected Z: "); Serial.print(cz);
  Serial.print("  Base Z: "); Serial.print(baseZ);
  Serial.print("  Mode Z: "); Serial.print(modeZ);
  Serial.println();
}


float calculateMode(float arr[], int size) {
  float modeValue = 0.0;
  int maxCount = 0
