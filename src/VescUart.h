#ifndef _VESCUART_h
#define _VESCUART_h

#include <Arduino.h>

#include "buffer.h"
#include "crc.h"
#include "datatypes.h"

class VescUart {
  /** Struct to store the telemetry data returned by the VESC */
  struct dataPackage {
    float avgMotorCurrent;
    float avgInputCurrent;
    float dutyCycleNow;
    float rpm;
    float inpVoltage;
    float ampHours;
    float ampHoursCharged;
    float wattHours;
    float wattHoursCharged;
    long tachometer;
    long tachometerAbs;
    float tempMosfet;
    float tempMotor;
    float pidPos;
    uint8_t id;
    mc_fault_code error;
  };

  /** Struct to hold the nunchuck values to send over UART */
  struct nunchuckPackage {
    int valueX;
    int valueY;
    bool upperButton;  // valUpperButton
    bool lowerButton;  // valLowerButton
  };

  struct FWversionPackage {
    uint8_t major;
    uint8_t minor;
  };

  /** Struct to hold the McConf values to send over UART */
  struct McConfPackage {
    float currentMin;
    float currentMax;
    float erpmMin;
    float erpmMax;
    float dutyMin;
    float dutyMax;
    float wattMin;
    float wattMax;
    float inCurrentMin;
    float inCurrentMax;
    int motorPoles;
    float gearRatio;
    float wheelDiameter;
  };

  struct ValuesSetupPackage {
    float tempMosfet;
    float tempMotor;
    float motorCurrent;
    float inputCurrent;
    float dutyCycleNow;
    float rpm;
    float speed;
    float inpVoltage;
    float batteryLevel;
    float ampHours;
    float ampHoursCharged;
    float wattHours;
    float wattHoursCharged;
    float distance;
    float distanceAbs;
    float pidPos;
    mc_fault_code error;
    uint8_t id;
    uint8_t numVescs;
    float wattHoursLeft;
    uint32_t odometer;
    uint32_t uptimeMs;
  };

  struct DecodedAdcPackage {
    int32_t decodedLevel;  // 0-1000000
    int32_t voltage;       // µV?
    int32_t decodedLevel2;
    int32_t voltage2;
  };

  // Timeout - specifies how long the function will wait for the vesc to respond
  const uint32_t _TIMEOUT;

 public:
  /**
   * @brief      Class constructor
   */
  VescUart(uint32_t timeout_ms = 100);

  /** Variable to hold measurements returned from VESC */
  dataPackage data;

  /** Variable to hold nunchuck values */
  nunchuckPackage nunchuck;

  /** Variable to hold firmware version */
  FWversionPackage fw_version;

  /** Variable to hold McConf values */
  McConfPackage mcconf;

  /** Variable to hold ValuesSetup values */
  ValuesSetupPackage valuesSetup;

  /** Variable to hold ValuesSetup values */
  DecodedAdcPackage decodedAdc;

  /**
   * @brief      Set the serial port for uart communication
   * @param      port  - Reference to Serial port (pointer)
   */
  void setSerialPort(Stream* port);

  /**
   * @brief      Set the serial port for debugging
   * @param      port  - Reference to Serial port (pointer)
   */
  void setDebugPort(Stream* port);

  /**
   * @brief      Populate the firmware version variables
   *
   * @return     True if successfull otherwise false
   */
  bool getFWversion(void);

  /**
   * @brief      Populate the firmware version variables
   *
   * @param      canId  - The CAN ID of the VESC
   * @return     True if successfull otherwise false
   */
  bool getFWversion(uint8_t canId);
  /**

* @brief      Set a profile
* @param
*/
  void setLocalProfile(bool store, bool forward_can, bool ack,
                       bool divide_by_controllers, float current_min_rel,
                       float current_max_rel, float speed_max_reverse,
                       float speed_max, float duty_min, float duty_max,
                       float watt_min, float watt_max);

  /**
   * @brief      Sends a command to VESC and stores the returned data
   *
   * @return     True if successfull otherwise false
   */
  bool getVescValues(void);

  /**
   * @brief      Sends a command to VESC and stores the returned data
   * @param      canId  - The CAN ID of the VESC
   *
   * @return     True if successfull otherwise false
   */
  bool getVescValues(uint8_t canId);

  /**
   * @brief      Sends a command to VESC and stores the VALUES_SETUP data
   *
   * @return     True if successfull otherwise false
   */
  bool getSetupValues(void);

  /**
   * @brief      Sends a command to VESC and stores the VALUES_SETUP data
   * @param      canId  - The CAN ID of the VESC
   *
   * @return     True if successfull otherwise false
   */
  bool getSetupValues(uint8_t canId);

  /**
   * @brief      Sends a command to VESC and stores the DECODED_ADC data
   *
   * @return     True if successfull otherwise false
   */
  bool getDecodedAdcValues(void);

  /**
   * @brief      Sends a command to VESC and stores the DECODED_ADC data
   * @param      canId  - The CAN ID of the VESC
   *
   * @return     True if successfull otherwise false
   */
  bool getDecodedAdcValues(uint8_t canId);

  /**
   * @brief      Sends values for joystick and buttons to the nunchuck app
   */
  void setNunchuckValues(void);
  /**
   * @brief      Sends values for joystick and buttons to the nunchuck app
   * @param      canId  - The CAN ID of the VESC
   */
  void setNunchuckValues(uint8_t canId);

  /**
   * @brief      Sends a command to VESC and stores the MCCONF_TEMP data
   *
   * @return     True if successfull otherwise false
   */
  bool getMcConfValues(void);
  /**
   * @brief      Sends a command to VESC and stores the MCCONF_TEMP data
   * @param      canId  - The CAN ID of the VESC
   *
   * @return     True if successfull otherwise false
   */
  bool getMcConfValues(uint8_t canId);
  /**
   * @brief      Sends values for MCCONF_TEMP to VESC for temporary overriding
   * of settings
   */
  void setMcConfValues(void);
  /**
   * @brief      Sends values for MCCONF_TEMP to VESC for temporary overriding
   * of settings
   * @param      canId  - The CAN ID of the VESC
   */
  void setMcConfValues(uint8_t canId);

  /**
   * @brief      Set the current to drive the motor
   * @param      current  - The current to apply
   */
  void setCurrent(float current);

  /**
   * @brief      Set the current to drive the motor
   * @param      current  - The current to apply
   * @param      canId  - The CAN ID of the VESC
   */
  void setCurrent(float current, uint8_t canId);

  /**
   * @brief      Set the current to brake the motor
   * @param      brakeCurrent  - The current to apply
   */
  void setBrakeCurrent(float brakeCurrent);

  /**
   * @brief      Set the current to brake the motor
   * @param      brakeCurrent  - The current to apply
   * @param      canId  - The CAN ID of the VESC
   */
  void setBrakeCurrent(float brakeCurrent, uint8_t canId);

  /**
   * @brief      Set the rpm of the motor
   * @param      rpm  - The desired RPM (actually eRPM = RPM * poles)
   */
  void setRPM(float rpm);

  /**
   * @brief      Set the rpm of the motor
   * @param      rpm  - The desired RPM (actually eRPM = RPM * poles)
   * @param      canId  - The CAN ID of the VESC
   */
  void setRPM(float rpm, uint8_t canId);

  /**
   * @brief      Set the duty of the motor
   * @param      duty  - The desired duty (0.0-1.0)
   */
  void setDuty(float duty);

  /**
   * @brief      Set the duty of the motor
   * @param      duty  - The desired duty (0.0-1.0)
   * @param      canId  - The CAN ID of the VESC
   */
  void setDuty(float duty, uint8_t canId);

  /**
   * @brief      Send a keepalive message
   */
  void sendKeepalive(void);

  /**
   * @brief      Send a keepalive message
   * @param      canId  - The CAN ID of the VESC
   */
  void sendKeepalive(uint8_t canId);

  /**
   * @brief      Help Function to print struct dataPackage over Serial for Debug
   */
  void printVescValues(void);

  void setLocalProfile(bool store, bool forward_can, bool divide_by_controllers, float current_min_rel, float current_max_rel, float speed_max_reverse, float speed_max, float duty_min, float duty_max, float watt_min, float watt_max);

 private:
  /** Variable to hold the reference to the Serial object to use for UART */
  Stream* serialPort = NULL;

  /** Variable to hold the reference to the Serial object to use for debugging.
   * Uses the class Stream instead of HarwareSerial */
  Stream* debugPort = NULL;

  /**
   * @brief      Packs the payload and sends it over Serial
   *
   * @param      payload  - The payload as a unit8_t Array with length of int
   * lenPayload
   * @param      lenPay   - Length of payload
   * @return     The number of bytes send
   */
  int packSendPayload(uint8_t* payload, int lenPay);

  /**
   * @brief      Receives the message over Serial
   *
   * @param      payloadReceived  - The received payload as a unit8_t Array
   * @return     The number of bytes receeived within the payload
   */
  int receiveUartMessage(uint8_t* payloadReceived);

  /**
   * @brief      Verifies the message (CRC-16) and extracts the payload
   *
   * @param      message  - The received UART message
   * @param      lenMes   - The lenght of the message
   * @param      payload  - The final payload ready to extract data from
   * @return     True if the process was a success
   */
  bool unpackPayload(uint8_t* message, int lenMes, uint8_t* payload);

  /**
   * @brief      Extracts the data from the received payload
   *
   * @param      message  - The payload to extract data from
   * @return     True if the process was a success
   */
  bool processReadPacket(uint8_t* message);

  /**
   * @brief      Help Function to print uint8_t array over Serial for Debug
   *
   * @param      data  - Data array to print
   * @param      len   - Lenght of the array to print
   */
  void serialPrint(uint8_t* data, int len);

  /**
   * @brief      Helper function to send a "get values" command to the VESC
   *
   * @param      canId                    - The CAN ID of the VESC
   * @param      packetId                 - Packet ID to request
   * @param      expectedMessageLength    - Minimum number of bytes needed to be
   * received for parsing
   * @param      packetIdStr              - String of packetId
   */
  bool getValues(uint8_t canId, COMM_PACKET_ID packetId,
                 int expectedMessageLength, const char* packetIdStr);
  		/**
       *
		 * @brief      Set a profile
		 * @param
		 */
};


#endif
