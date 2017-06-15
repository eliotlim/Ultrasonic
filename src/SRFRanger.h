/*
 * SRFRanger.h
 *
 * Library for Devantech SRF08/10 I2C Ultrasonic Rangers
 *
 * created 14 June 2017
 * by Eliot Lim    (github: @eliotlim)
 *
 * Released into the MIT License.
 */

#ifndef SRFRanger_h
#define SRFRanger_h

/*
 * Values of divisors
 */
#define CM 28
#define INC 71

class SRFRanger {
  public:
    SRFRanger(byte address);
    void distanceRange();
    void distanceRange(uint8_t und);
    unsigned int distanceRead();
    void setAddress(byte newAddress);
    byte getAddress() { return addr; }
    void connect(byte address);

  private:
    byte addr;
    int reading;

    byte normalizeAddress(byte address);
};

#endif // SRFRanger_h
