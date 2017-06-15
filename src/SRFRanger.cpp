/*
 * SRFRanger.cpp
 *
 * Library for Devantech SRF08/10 I2C Ultrasonic Rangers
 *
 * Adapted from: https://www.arduino.cc/en/Tutorial/SFRRangerReader
 * Original Authors: Nicholas Zambetti <http://www.zambetti.com>
 * and James Tichenor <http://www.jamestichenor.net>
 *
 * created 14 June 2017
 * by Eliot Lim    (github: @eliotlim)
 *
 * Released into the MIT License.
 */

#include <Arduino.h>
#include <Wire.h>
#include "SRFRanger.h"

SRFRanger::SRFRanger(byte address) {
  addr = normalizeAddress(address);
}

void SRFRanger::distanceRange() {
  distanceRange(CM);
}

void SRFRanger::distanceRange(uint8_t und) {
  // transmit to device (address stored)
  // the default address specified in the datasheet is 224 (0xE0)
  // but i2c adressing uses the high 7 bits so it's 112
  Wire.beginTransmission(addr);
  // Set register pointer to the command register (0x00)
  Wire.write(byte(0x00));

  // Command SRF to measure using specified units (default: CM)
  byte mode = byte(0x51);
  if (und == INC)
    mode = byte(0x50);
  else if (und == CM)
    mode = byte(0x51);
  Wire.write(mode);

  // Release and free the I2C Bus
  Wire.endTransmission();
}

unsigned int SRFRanger::distanceRead() {
  // Set pointer to echo #1 register (0x02)
  Wire.beginTransmission(addr);
  Wire.write(byte(0x02));
  // TWI Restart command (for SRF10 Compatiblity)
  Wire.endTransmission(true);

  // Request 2 bytes from slave device
  Wire.requestFrom(addr, 2);

  // Receive reading from sensor
  if (2 <= Wire.available()) { // if two bytes were received
    reading = Wire.read();     // receive high byte (overwrites previous reading)
    reading = reading << 8;    // shift high byte to be high 8 bits
    reading |= Wire.read();    // receive low byte as lower 8 bits
  }

  // Release and free the I2C Bus
  Wire.endTransmission();

  return (unsigned int) reading;
}

// Set the currently connected SRF Ranger I2C Address to the specified address
void SRFRanger::setAddress(byte newAddress) {
  Wire.beginTransmission(addr);
  Wire.write(byte(0x00));
  Wire.write(byte(0xA0));
  Wire.endTransmission();

  Wire.beginTransmission(addr);
  Wire.write(byte(0x00));
  Wire.write(byte(0xAA));
  Wire.endTransmission();

  Wire.beginTransmission(addr);
  Wire.write(byte(0x00));
  Wire.write(byte(0xA5));
  Wire.endTransmission();

  Wire.beginTransmission(addr);
  Wire.write(byte(0x00));
  Wire.write(newAddress);
  Wire.endTransmission();

  addr = normalizeAddress(newAddress);
}

// Change the connection of the SRFRanger object to the specified I2C Ranger
void SRFRanger::connect(byte address) {
  addr = normalizeAddress(address);
}

// Convert valid addresses for SRF10 devices into I2C (7-bit) addresses
// bit-shifts the address correctly if out of range.
byte SRFRanger::normalizeAddress(byte address) {
  if (0x07 < address && address < 0x78)
    return address;
  return address >> 1;
}
