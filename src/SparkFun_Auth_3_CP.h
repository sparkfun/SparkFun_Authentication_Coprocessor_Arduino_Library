#ifndef SparkFun_Authentication_Coprocessor_H
#define SparkFun_Authentication_Coprocessor_H

#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_Toolkit.h>

class SparkFunAuth3CPDriver
{
    public:
        SparkFunAuth3CPDriver() {}

        /**
         * @brief Initializes communication with the Auth 3.0 Coprocessor.
         *
         * @param wirePort The I2C port to communicate with the coprocessor.
         *                 Defaults to `Wire`.
         *
         * @return Returns `true` if the initialization was successful, `false` otherwise.
         *
         * @note Call this method before using any other functions in the class.
         *       The coprocessor goes to sleep automatically after 4 seconds of
         *       inactivity. Call this method each time you want to communicate
         *       with the coprocessor.
         */
        bool begin(void);

        /**
         * @brief Read the certificate serial number.
         *
         * @param maxLen The maximum length of the certificate serial number.
         *               I.e. sizeof certSerial
         *
         * @param certSerial Pointer to the certificate serial number storage.
         *                   If the read is successful, this will contain the
         *                   certificate serial number.
         *
         * @param certSerialLen Pointer to the certificate serial number length.
         *                      If the read is successful, this will contain the
         *                      actual certificate serial number length.
         *
         * @return Returns `true` if the read was successful, `false` otherwise.
         *
         */
        bool getDeviceCertificateSerialNumber(uint16_t maxLeng, uint8_t *certSerial, uint16_t &certSerialLen);

        /**
         * @brief Read the certificate.
         *
         * @param maxLen The maximum length of the certificate. I.e. sizeof cert.
         *
         * @param cert Pointer to the certificate storage. If the read is successful,
         *             this will contain the certificate.
         *
         * @param certLen Pointer to the certificate length. If the read is successful,
         *                this will contain the actual certificate length.
         *
         * @return Returns `true` if the read was successful, `false` otherwise.
         *
         */
        bool getDeviceCertificate(uint16_t maxLeng, uint8_t *cert, uint16_t &certLen);

        /**
         * @brief Set (write) the challenge data.
         *
         * @param maxLen The maximum length of the certificate. I.e. sizeof cert.
         *
         * @param challenge Pointer to the challenge data.
         *
         * @param challengeLen The length of the challenge data. I.e. sizeof challenge.
         *
         * @return Returns `true` if the set was successful, `false` otherwise.
         *
         */
        bool setChallengeData(uint8_t *challenge, uint16_t challengeLen);

        /**
         * @brief Start the challenge response generation.
         *
         */
        void startChallenge();


        /**
         * @brief Read the challenge response.
         *
         * @param maxLen The maximum length of the response.
         *               I.e. sizeof response
         *
         * @param response Pointer to the challenge response storage.
         *                 If the response generation was successful,
         *                 this will contain the challenge response.
         *
         * @param responseLen Pointer to the challenge response length.
         *                    If the response generation was  successful,
         *                    this will contain the actual response length.
         *
         * @return Returns `true` if both the response generation and the
         *         read were successful, `false` otherwise.
         *
         * @note Response generation takes 500ms maximum. Wait 500 millis
         *       between startChallenge and getChallengeResponse.
         */
        bool getChallengeResponse(uint16_t maxLeng, uint8_t *response, uint16_t &responseLen);

    protected:
        /// @brief Sets the communication bus to the specified bus.
        /// @param theBus Bus to set as the communication device.
        void setCommunicationBus(sfTkArdI2C *theBus);

    private:
        sfTkArdI2C *_theBus; // Pointer to bus device.

};

class SparkFunAuth3CPArdI2C : public SparkFunAuth3CPDriver
{
  public:
    SparkFunAuth3CPArdI2C() {}

    /// @brief  Sets up Arduino I2C driver using the default I2C address then calls the super class begin.
    /// @return True if successful, false otherwise.
    bool begin(void)
    {
        if (_theI2CBus.init(0x10) != ksfTkErrOk)
            return false;

        setCommunicationBus(&_theI2CBus);

        _theI2CBus.setStop(true); // Use stops not restarts for I2C reads

        return SparkFunAuth3CPDriver::begin();
    }

    /// @brief  Sets up Arduino I2C driver using the specified I2C address then calls the super class begin.
    /// @return True if successful, false otherwise.
    bool begin(TwoWire &wirePort)
    {
        if (_theI2CBus.init(wirePort, 0x10) != ksfTkErrOk)
            return false;

        setCommunicationBus(&_theI2CBus);

        _theI2CBus.setStop(false); // Use restarts not stops for I2C reads

        return SparkFunAuth3CPDriver::begin();
    }

  private:
    sfTkArdI2C _theI2CBus;
};

#endif // #ifndef SparkFun_Authentication_Coprocessor_H