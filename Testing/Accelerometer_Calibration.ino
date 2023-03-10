/* take(find) first couple readings and mean then change the set's values according to error and store both values.
    mode after all values are processed
*/ 

#include "MPU6000.h"
#include "Wire.h"

static boolean firstX = true;
static boolean firstY = true;
static boolean firstZ = true;
MPU6000 accelerometer(1, false);
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


 float modeX ( float array[], int n);
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



void setup() {
  Serial.begin(9600);
  accelerometer.begin();
  accelerometer.initialize();
  accelerometer.setAccelRange (MPU6000_RANGE_2_G);
  sensor_float_vec_t calibrationPoints[calibrationSize];  // Array of vectors for storing calibration points  
    for(int i = 0; i < calibrationSize; i++){
        calibrationPoints[i] = accelerometer.getSample();
    }

for(int j = 0; j < calibrationSize; j++){
        avgX = avgX + calibrationPoints[j].x;
        avgY = avgY + calibrationPoints[j].y;
        
    }
    avgX = avgX/calibrationSize;
    avgY = avgY/calibrationSize;
    yzAcceleration = sqrt(pow(accMPS.y,2) + pow(accMPS.z,2)); 
    xzAcceleration = sqrt(pow(accMPS.x,2) + pow(accMPS.z,2)); 
    xyAcceleration = sqrt(pow(accMPS.x,2) + pow(accMPS.y,2)); 

}

void loop() {
  acc= accelerometer.getSample();
  accelerometer.getEvent(&event);

  float scaleX = event.acceleration.X - baseX;
  float scaleY = event.acceleration.Y - baseY;
  float scaleZ = event.acceleration.Z - baseZ;
  
  Serial.print("X : "); Serial.print(scaleX);
  Serial.print("Y : "); Serial.print(scaleY);
  Serial.print("Z : "); Serial.print(scaleZ);

  xAngle = accMPS.x/ yzAcceleration;
    xAngle = atan(xAngle)*(180/3.141);
    Serial.print("X Angle (Roll): ");
    Serial.print(xAngle);
    Serial.println("º");
  yAngle = accMPS.y/xzAcceleration;
    yAngle = atan(yAngle)*(180/3.141);
    Serial.print("Y Angle (Pitch): ");
    Serial.print(yAngle);
    Serial.println("º");
  zAngle = accMPS.z/xyAcceleration; //refer to line 35 issue
    zAngle = (atan(zAngle)*(180/3.141));
    Serial.print("Z Angle (Yaw): ");
    Serial.print(zAngle);
    Serial.println("º");
    Serial.println();
  delay(500);
  

    float baseX() ;
    float n;
    float arr[50];
    float count; "Enter the size of array: ";
    
  Serial.print("ModeX"); Serial.print(correctedX);
  
  float modeY( float array[], int n) ;
    sort(arr, arr + n); //arr vs array 
   float max_count = 1, res = arr[0], count = 1; 
   
   for (float i = 1; i < n; i++) { 
        if (arr[i] == arr[i - 1]) 
            count++; 
        else { 
            if (count > max_count) { 
                max_count = count; 
                res = arr[i - 1]; 
            } 
            count = 1; 
        } 
        
     if (count > max_count) 
    { 
        max_count = countY; 
        res = arr[n - 1]; 
    } 
    
    return res;
}
    float baseY();
    float n;
    float arr[50];
    count<<"Enter the size of array: ";
  Serial.print("ModeY"); Serial.print(correctedY);
  
  float modeZ( float array[], int n);
    sort(array, array + n);
   float max_count = 1, res = array[0], count = 1; 
   
   for (float i = 1; i < n; i++) { 
        if (arr[i] == arr[i - 1]) 
            count++; 
        else { 
            if (count > max_count) { 
                max_count = count; 
                res = arr[i - 1]; 
            } 
            count = 1; 
        } 
        
     if (count > max_count) 
    { 
        max_count = countZ; 
        res = arr[n - 1]; 
    } 
    
    return res;
}
    float baseZ();
    float n;
    float arr[50];
    count<<"Enter the size of array: ";
  Serial.print("ModeZ"); Serial.print(correctedZ);//correctedZ 
  
}