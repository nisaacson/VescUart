/*
  Name:    setMcConfValues.ino
  Created: 31-03-2023
  Author:  billwurles
  Description:  Get the MCCONF values from vesc and then set them to a user defined value.
                This can be used to create speedlimiting profiles that do not persist on a restart, for example by setting
                the default ERPM limit to be effectively 25km/h an ebike will be legal on EU roads, but after a user input
                the COMM_SET_MCCONF_TEMP command will be sent, which can set it to a higher speed limit or unrestrict it.

                This example is made using an Arduino Pro Micro (Atmega32u4)
*/

#include <VescUart.h>

/** Initiate VescUart class */
VescUart UART;

const int SWITCH_PIN = 7; // Digital output pin the physical switch is connected to
boolean prevState = false; // We only send data when the state of the switch changes
double speedFact = 0; // The factorial which when multipled with ERPM, returns the speed

void setup() {

  /** Setup UART port (Serial1 on Atmega32u4) */
  Serial1.begin(19200);

  while (!Serial1) {;}

  /** Define which ports to use as UART */
  UART.setSerialPort(&Serial1);

  pinMode( SWITCH_PIN, INPUT_PULLUP );

  UART.getMcConfValues();

  speedFact = ((double(UART.mcconf.motorPoles) / 2.0) * 60.0 *
            UART.mcconf.gearRatio) /
            (UART.mcconf.wheelDiameter * M_PI);
}

void loop() {

  int switchState = digitalRead(SWITCH_PIN);

  if (switchState == HIGH) {
    if(!prevState) { // Previous state was LOW, so we change the vesc settings
        /** Set the Maximum ERPM to a very large number to effectively unrestrict the VESC */
      UART.mcconf.erpmMax = 1500000000;

        /** Call the function setMcConfValues to send the current MCCONF values to the VESC */
      UART.setMcConfValues();
      
      prevState = true; // Set state to current switch state to prevent further UART calls
    }
  } else {
    if(prevState) { // Previous state was HIGH, so we change the vesc settings
        /** Set the Maximum ERPM to a number that will limit it's road speed to 25 KM/h */
      UART.mcconf.erpmMax = 25 * speedFact;

        /** Call the function setMcConfValues to send the current MCCONF values to the VESC */
      UART.setMcConfValues();
      
      prevState = false; // Set state to current switch state to prevent further UART calls
    }
  }

}