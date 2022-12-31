
/*
  IMU Capture

  This example uses the on-board IMU to start reading acceleration and gyroscope
  data from on-board IMU and prints it to the Serial Monitor for one second
  when the significant motion is detected.

  You can also use the Serial Plotter to graph the data.

  The circuit:
  - Arduino Nano 33 BLE or Arduino Nano 33 BLE Sense board.

  Created by Don Coleman, Sandeep Mistry
  Modified by Dominic Pajak, Sandeep Mistry

  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>
#include <string>  
const float accelerationThreshold = 2.5; // threshold of significant in G's
const int numSamples = 119;

int samplesRead = numSamples;
String data;
void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // print the header
  Serial.println("aX,aY,aZ,gX,gY,gZ");
   Wire.begin(); 
}

void loop() {
  float aX, aY, aZ, gX, gY, gZ;

  // wait for significant motion
  while (samplesRead == numSamples) {
    if (IMU.accelerationAvailable()) {
      // read the acceleration data
      IMU.readAcceleration(aX, aY, aZ);

      // sum up the absolutes
      float aSum = fabs(aX) + fabs(aY) + fabs(aZ);

      // check if it's above the threshold
      if (aSum >= accelerationThreshold) {
        // reset the sample read count
        samplesRead = 0;
        break;
      }
    }
  }

  // check if the all the required samples have been read since
  // the last time the significant motion was detected
  while (samplesRead < numSamples) {
    // check if both new acceleration and gyroscope data is
    // available
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      // read the acceleration and gyroscope data
      IMU.readAcceleration(aX, aY, aZ);
      IMU.readGyroscope(gX, gY, gZ);

      samplesRead++;

      // print the data in CSV format
      Serial.print(aX, 3);
      Serial.print(',');
      Serial.print(aY, 3);
      Serial.print(',');
      Serial.print(aZ, 3);
      Serial.print(',');
      Serial.print(gX, 3);
      Serial.print(',');
      Serial.print(gY, 3);
      Serial.print(',');
      Serial.print(gZ, 3);
      Serial.println();
      //data est un string declare plus haut , qui contiendra les informations d'acceleration et du gyroscope
      //la transimition se fait ligne par ligne 
      data += String(aX,3);
      data += ",";
      data += String(aY,3);
      data += ",";
      data += String(aZ,3);
      data += ",";
      data += String(gX,3);
      data += ",";
      data += String(gY,3);
      data += ",";
      data += String(gZ,3);
      data += " \n";
      Wire.beginTransmission(9); // debut de la transmition  
      Wire.write(data.c_str());  //formatage des donnes envoyees 
      Wire.endTransmission();    // fin de la transmition

      if (samplesRead == numSamples) {
        // add an empty line if it's the last sample
        Serial.println();
      }

      data= "";
    }
  }
}
