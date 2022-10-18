#include <MAIN/config.h>
#include "Adafruit_VL53L0X.h"


/*
------------------   ------------------   ------------------
|  HC-SR04 (1)   |   |  HC-SR04 (2)   |   |  HC-SR04 (3)   |
------------------   ------------------   ------------------
|  VCC   |  +5V  |   |  VCC   |  +5V  |   |  VCC   |  +5V  |
|  GND   |  GND  |   |  GND   |  GND  |   |  GND   |  GND  |
|  TRIG  |  D25  |   |  TRIG  |  D27  |   |  TRIG  |  D12  |
|  ECHO  |  D36  |   |  ECHO  |  D34  |   |  ECHO  |  D32  |

*/

// address we will assign if dual sensor is present
#define LASER1_ADDRESS 0x30
#define LASER2_ADDRESS 0x31

// objects for the vl53l0x
Adafruit_VL53L0X laser1 = Adafruit_VL53L0X();
Adafruit_VL53L0X laser2 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

/*
    Reset all sensors by setting all of their XSHUT pins low for delay(10), then set all XSHUT high to bring out of reset
    Keep sensor #1 awake by keeping XSHUT pin high
    Put all other sensors into shutdown by pulling XSHUT pins low
    Initialize sensor #1 with lox.begin(new_i2c_address) Pick any number but 0x29 and it must be under 0x7F. Going with 0x30 to 0x3F is probably OK.
    Keep sensor #1 awake, and now bring sensor #2 out of reset by setting its XSHUT pin high.
    Initialize sensor #2 with lox.begin(new_i2c_address) Pick any number but 0x29 and whatever you set the first sensor to
 */

void setID() {

  digitalWrite(XSHUNT_1, LOW);    
  digitalWrite(XSHUNT_2, LOW);
  delay(10);
  
  digitalWrite(XSHUNT_1, HIGH);
  digitalWrite(XSHUNT_2, HIGH);
  delay(10);

  digitalWrite(XSHUNT_1, HIGH);
  digitalWrite(XSHUNT_2, LOW);

  // initing LOX1
  if(!laser1.begin(LASER1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(10);

  digitalWrite(XSHUNT_2, HIGH);
  delay(10);

  //initing LOX2
  if(!laser2.begin(LASER2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
}

void read_dual_sensors() {
  
  laser1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  laser2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!

  // print sensor one reading
  Serial.print(F("1: "));
  if(measure1.RangeStatus != 4) {     // if not out of range
    Serial.print(measure1.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }
  
  Serial.print(F(" "));

  // print sensor two reading
  Serial.print(F("2: "));
  if(measure2.RangeStatus != 4) {
    Serial.print(measure2.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }
  
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) { delay(1); }

  pinMode(XSHUNT_1, OUTPUT);
  pinMode(XSHUNT_2, OUTPUT);

  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(XSHUNT_1, LOW);
  digitalWrite(XSHUNT_2, LOW);

  Serial.println(F("Both in reset mode...(pins are low)"));
  
  
  Serial.println(F("Starting..."));
  setID();
 
}

void loop() {
   
  read_dual_sensors();
  delay(100);
}