/*
 * SRFRanger Diagnostic Tool
 * Prints the distance read by Devantech SRF08/10 I2C Ultrasonic
 * Rangers in meters. Also provides an easy way to configure
 * I2C addresses for a sensor.
 *
 * Connections:
 * * Module HR-SC04 (four pins) or PING))) (and other with
 *   three pins), attached to digital pins as follows:
 * ----------------------      ----------------------
 * | SRF08/10 | Arduino |      | SRF08/10 |   Mega  |
 * ----------------------      ----------------------
 * |   Vcc    |   5V    |      |   Vcc    |   5V    |
 * |   SDA    |   A4    |      |   SDA    |   20    |
 * |   SCL    |   A5    |      |   SCL    |   21    |
 * |   NC     |         |      |   NC     |         |
 * |   Gnd    |   GND   |      |   Gnd    |   GND   |
 * ----------------------      ----------------------
 *
 * writeup created 3 Apr 2014
 * by Erick Simões (github: @ErickSimoes | twitter: @AloErickSimoes)
 * modified and adapted 14 June 2017
 * by Eliot Lim    (github: @eliotlim)
 *
 * This example code is released into the MIT License.
 */

#include <SRFRanger.h>
#include <Wire.h>

// Set up an SRFRanger on I2C address 0xE0 (8-bit address as per datasheet)
// Automatic normalization to 7-bit I2C address
SRFRanger ultrasoundSRF0(byte(0xE0));

// Print contents of the help message
void printHelpMessage() {
  Serial.println("SRFRanger Diagnostics Tool");
  Serial.println("=== Available Commands ===");
  Serial.println("getaddress - display 7-bit I2C address (decimal notation)");
  Serial.println("setaddress - set I2C address - e.g. setaddress 112");
  Serial.println("range      - perform ultrasonic ranging");
  Serial.println("help       - display this message again");
}

// Perform initialization and setup
void setup() {
  Wire.begin();
  Serial.begin(57600);

  printHelpMessage();
}

bool prompt = true;
char buffer[33];
int buf_len = 0, buf_size = 32;

void loop() {

  // Prompt
  if (prompt) {
    Serial.print("\nDevice: ");
    Serial.print(ultrasoundSRF0.getAddress());
    Serial.print(" > ");
    prompt = false;
  }

  // Read Serial Command
  buf_len = 0;
  buf_len = Serial.readBytes(buffer, buf_size);

  // Tokenize command / argument string
  char* command = strtok(buffer, " ");
  while (command != NULL) {
    Serial.println(buffer);

    // Match the command string
    if (strcmp(command, "getaddress") == 0) {
      // Print the 7-bit I2C address of the connection (in decimal notation)
      Serial.print("Address: ");
      Serial.println(ultrasoundSRF0.getAddress());
    } else if (strcmp(command, "setaddress") == 0) {
      // Set the 7-bit I2C address of the connection (in decimal notation)
      // Also changes the I2C address of the connected and attached sensor.
      command = strtok(NULL, " ");
      // Parse the I2C address as an integer, followed by a byte cast
      byte newAddress = (byte) atoi(command);
      // I2C address bounds check
      if (newAddress && 0x07 < newAddress && newAddress < 0xFF) {
        Serial.print("New Address: ");
        Serial.println(newAddress);
        // Set the address of the sensor
        ultrasoundSRF0.setAddress(newAddress);
      } else {
        Serial.println("Invalid Address String entered");
      }
    } else if (strcmp(command, "range") == 0) {
      // Perform ranging and display detected object range
      ultrasoundSRF0.distanceRange();
      delay(70);
      // Receive detected range in cm
      float distance = ultrasoundSRF0.distanceRead(CM)/100.0f;
      Serial.print("range: ");
      Serial.print(distance);
      Serial.println("m");
    } else if (strcmp(command, "help") == 0) {
      // Print the Help Message
      printHelpMessage();
    } else {
      // Invalid Command
      Serial.println("Invalid command. ");
    }

    // Prompt the user again for a command
    command = NULL;
    prompt = true;
  }

  // Zero the command buffer
  for (int c = 0; c <= buf_size; c++) buffer[c] = 0;

}
