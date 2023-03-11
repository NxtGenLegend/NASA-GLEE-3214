#include "MPU6000.h"
#include "Wire.h"

MPU6000 accelerometer(1, false);
<<<<<<< Updated upstream
const int calibrationSize = 40;
sensor_float_vec_t calibrationPoints[calibrationSize];
float correctedX[calibrationSize];
float correctedY[calibrationSize];
float correctedZ[calibrationSize];
float baseX, baseY, baseZ;
float modeX, modeY, modeZ;
=======
 float modeX = 0.0;
 float modeY = 0.0;
 float modeZ = 0.0;
 float baseX = 0.0;
 float baseY = 0.0;
 float baseZ = 0.0;
 float correctedX = 0.0;
 float correctedY = 0.0;
 float correctedZ = 0.0;
 sensor_float_vec_t acc;
float xAngle = 0.0;
float yAngle = 0.0;
float zAngle = 0.0;
float avgX = 0.0;
float avgY = 0.0;
float avgZ = 0.0;

float calibrationSize = 40;
float count;
float yzAcceleration;
float xzAcceleration;
float xyAcceleration;


//convert from C++ to arduino
sensor_float_vec_t MPU6000::getMPSAccel(sensor_float_vec_t GAccel){
	sensor_float_vec_t accelMPS;
	accelMPS.x = GAccel.x * MPU_ONE_G;
	accelMPS.y = GAccel.y * MPU_ONE_G;
	accelMPS.z = GAccel.z * MPU_ONE_G;
	//return accelMPS;    


 float modeX (float array[], int n);
 sort(arr, arr + n); //sort is not a thing??? define n
   float max_count = 1, res = arr[0], count = 1; 
   
   for (float i = 1; i < n; i++) 
        if (arr[i] == arr[i - 1]) 
            count++; 
        else { 
            if (count > max_count) 
                max_count = count; 
                res = arr[i - 1]; 
            
            count = 1; 
        } 
        
     if (count > max_count) 
    { 
        max_count = countX; 
        res = arr[n - 1]; 
    } 
    
    return res;
}


>>>>>>> Stashed changes

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
