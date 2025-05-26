#include <SparkFun_Auth_3_CP.h> // Click here to get the library: http://librarymanager/All#SparkFun_Authentication_Coprocessor

SparkFunAuth3CPArdI2C myDevice;

void setup()
{
    delay(250);

    // Start serial
    Serial.begin(115200);
    Serial.println("Authentication Coprocessor - Basic Uasge");

    Wire.begin();

    myDevice.begin();

}

void loop()
{
}