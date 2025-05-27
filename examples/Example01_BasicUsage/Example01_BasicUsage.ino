#include <SparkFun_Auth_3_CP.h> // Click here to get the library: http://librarymanager/All#SparkFun_Authentication_Coprocessor

SparkFunAuth3CPArdI2C myDevice;

void setup()
{
    delay(250);

    // Start serial
    Serial.begin(115200);
    Serial.println("Authentication Coprocessor - Basic Usage");

    // Start I2C
    Wire.begin();
}

void loop()
{
    // Check the device is connected - and awake
    myDevice.begin(Wire);

    // Read the certificate serial number
    char certSerial[32 + 1]; // 32 ASCII characters plus NULL terminator
    if (myDevice.getDeviceCertificateSerialNumber(sizeof(certSerial), (uint8_t *)certSerial))
    {
        Serial.print("Certificate Serial Number: ");
        Serial.println(certSerial);
    }

    // Read the certificate
    uint8_t cert[609]; // Cert could be up to 609 bytes
    size_t certSize;
    if (myDevice.getDeviceCertificate(sizeof(cert), cert, certSize))
    {
        Serial.print("Certificate Size: ");
        Serial.println(certSize);

        printHexThing("Certificate", certSize, cert);
    }

    // Write the challenge data
    uint8_t challengeData[32] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
    };
    if (myDevice.setChallengeData(challengeData, sizeof(challengeData)))
        Serial.println("Challenge accepted");

    // Start the challenge response generation
    if (myDevice.startChallenge())
        Serial.println("Challenge response generation started");

    delay(500); // Wait tAUTH

    // Read the challenge response
    uint8_t challengeResponse[64];
    if (myDevice.getChallengeResponse(sizeof(challengeResponse), challengeResponse))
        printHexThing("Challenge Response", sizeof(challengeResponse), challengeResponse);

    // Repeat after 5 seconds - coprocessor will go to sleep after 4 seconds
    delay(5000);
}

void printHexThing(const char *name, size_t thingSize, uint8_t *thing)
{
    Serial.print(name);
    Serial.print(":\r\n{");
    for (size_t x = 0 ; x < thingSize ; x++)
    {
        const size_t rowLength = 32;
        if (x % rowLength == 0)
            Serial.println();
        //Serial.printf(" 0x%02X,", thing[thingSize - x - 1]); //Print in reverse order
        Serial.printf("0x%02X", thing[x]); //Normal order
        if (x < (thingSize - 1))
        {
            Serial.print(",");
            if ((x % rowLength) < (rowLength - 1))
                Serial.print(" ");
        }
    }
    Serial.println("\r\n}");
}

