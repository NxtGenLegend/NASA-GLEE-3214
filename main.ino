#include <GLEE_Radio.h>
#include "TMP117.h"
#include "MLX90395.h"
#include "MPU6000.h"
#include "CAP.h"
#include "TPIS1385.h"
#include <Wire.h>

// For Reference
// Standard V4 Lunasat I2C Sensors
// AK09940 - Magnetometer
// TMP117 - Temperature Sensor
// TPIS1385 - Thermopile
// ICM20602 - Accelerometer

// Standard V4 Lunasat I2C Address Configuration (7-Bit Addresses)

// AK09940:   0x0C
// TMP117:    0x48
// TPIS1385:  0x0D
// ICM20602:  0x69

byte sensorsAddrs[4] = {0x0C, 0x48, 0x0D, 0x69};

String sensorNames[4] = {"Mag", "Temp", "Therm", "Accelerometer"};

byte addr;      // Byte representation of I2C addresses
byte err;       // Byte to store error responses
int numDevices; // Accumulator for tracking number of found devices

// Initiate LunaSat Radio (SX1272 Transciever)
LunaRadio Rad;

// Sensor Objects (Debugging defaults to false)
TMP117 thermometer(1);
MPU6000 accelerometer(2);
MLX90395 magnetometer(3);
TPIS1385 thermopile(4);
CAP capacitive(5);

// Sensor Data Variables
float tempSample;
sensor_float_vec_t accSample;
mlx_sample_t magSample;
TPsample_t thermSample;
int capSample;

// Individual string buffers for memory optimization
char timeStamp_str[8] = {' '};
char temp_str[8] = {' '};
char accX_str[8] = {' '};
char accY_str[8] = {' '};
char accZ_str[8] = {' '};
char magX_str[7] = {' '};
char magY_str[7] = {' '};
char magZ_str[7] = {' '};
char thermObj_str[6] = {' '};
char thermAmb_str[6] = {' '};
char cap_str[5] = {' '};

// Transmission Buffer
char data_buffer[80];

// Output Formatting Variables
String printOut;       // QUESTIONABLE
const char *sep = ","; // QUESTIONABLE
unsigned long timeStamp;

// RED Led Pin
int LED = 4;

void setup()
{
    // Initialization
    Wire.begin();          // Begin Serial Communications
    Wire.setClock(100000); // Set Standard 100Mhz i2c speed
    Serial.begin(9600);    // Set Default

    if (!Serial)
    {
        delay(1000); // Delay if issue with serial
    }

    Serial.println("LunaSat Connection Succsessful");

    // Initalize Thermopile Sensor
    Wire.beginTransmission(0x00); // Tx buffer
    Wire.write(0x04);             // Add Register Address to Tx buffer
    Wire.write(0x00);             // Add initialization data to Tx buffer
    Wire.endTransmission();       // Send

    delay(50);

    Serial.println("Sensor Initialization Successful\n");

    pinMode(A3, OUTPUT);
    digitalWrite(A3, HIGH);

    // Start up Serial (wired) Communications
    Serial.begin(9600);

    // SENSOR INITIALIZATIONS

    // Temperature Sensor - TMP117
    // (Nothing)

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

    // Initilaize Radio, turn up power (max 17) to improve range
    Rad.initialize_radio(915.0, 7, 250.0, 12, 8);

    // Set RED led pin to output
    pinMode(LED, OUTPUT);
}

void loop()
{

    byte addr;          // Current address for scanning
    int numDevices = 0; // Number of devices found

    // Loop through expected addresses of sensors
    for (int i = 0; i < 4; i++)
    {

        addr = sensorsAddrs[i];
        Wire.beginTransmission(addr);
        err = Wire.endTransmission();

        if (!err)
        { // Check for error
            numDevices++;
            Serial.print(sensorNames[i]);
            Serial.print(" sensor found at ");

            // Hex address formating "0x00"
            Serial.print("0");
            Serial.print(addr, HEX);
            Serial.println("  !");
        }
        else if (err == 4)
        {
            Serial.print("Error at ");
            Serial.print("0");
            Serial.println(addr, HEX);
        }
    }

    Serial.print("Scan Complete: ");
    Serial.print(numDevices);
    Serial.println(" devices found\n");

    delay(1000);

    // Save Timestamp
    timeStamp = millis();

    // Get samples from each sensor
    tempSample = thermometer.getTemperatureC();
    accSample = accelerometer.getSample();
    magSample = magnetometer.getSample();
    thermSample = thermopile.getSample();
    capSample = capacitive.getRawData();

    // Decimal TO STRing Functions to load decimal (float) samples as strings into individual buffers
    // see https://www.programmingelectronics.com/dtostrf/ for more info
    dtostrf(tempSample, 3, 3, temp_str);

    dtostrf(accSample.x, 1, 4, accX_str);
    dtostrf(accSample.y, 1, 4, accY_str);
    dtostrf(accSample.z, 1, 4, accZ_str);

    dtostrf(magSample.magnetic.x, 2, 2, magX_str);
    dtostrf(magSample.magnetic.y, 2, 2, magY_str);
    dtostrf(magSample.magnetic.z, 2, 2, magZ_str);

    dtostrf(thermSample.object, 3, 2, thermObj_str);
    dtostrf(thermSample.ambient, 3, 2, thermAmb_str);

    // Using sprintf to load integers and unsigned longs (time) into buffers as strings
    sprintf(timeStamp_str, "%lu", timeStamp);
    sprintf(cap_str, "%i", capSample);

    // Using STRing CoPY to construction of transmission buffer with ID for transmission filtering

    ///////////////////////////////////////////////
    strcpy(data_buffer, "11,"); // CHANGE TO YOUR TEAM NUMBER
    ///////////////////////////////////////////////

    // Using STRing ConCaT to append indiviual character arrays (psudo strings) to
    strcat(data_buffer, timeStamp_str);
    strcat(data_buffer, ",");
    strcat(data_buffer, temp_str);
    strcat(data_buffer, ",");
    strcat(data_buffer, accX_str);
    strcat(data_buffer, ",");
    strcat(data_buffer, accY_str);
    strcat(data_buffer, ",");
    strcat(data_buffer, accZ_str);
    strcat(data_buffer, ",");
    strcat(data_buffer, magX_str);
    strcat(data_buffer, ",");
    strcat(data_buffer, magY_str);
    strcat(data_buffer, ",");
    strcat(data_buffer, magZ_str);
    strcat(data_buffer, ",");
    strcat(data_buffer, thermObj_str);
    strcat(data_buffer, ",");
    strcat(data_buffer, thermAmb_str);
    strcat(data_buffer, ",");
    strcat(data_buffer, cap_str);

    // Print data (already in string format) to serial monitor(wired)
    Serial.println(data_buffer);

    // Transmit string data (already in string format) to
    Rad.transmit_data(data_buffer);

    // Note: Optimized transmission of data is in progress... String formatting for demo purposes

    // Blink after transmit (Remove for faster datarate)
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);

    // Repeate every ~500ms INCREASE TO CHANGE SAMPLE RATE
    delay(500);
}