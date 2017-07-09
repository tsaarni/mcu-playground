
#include <Arduino.h>
#include <SPI.h>
#include <stdint.h>


#define CS_POT_PIN D1
#define FSYNC_PIN D0



#define REG_CONTROL  0x0000
#define REG_FREQ0    0x4000
#define REG_FREQ1    0x4000
#define REG_PHASE0   0xC000
#define REG_PHASE1   0xE000

#define SPI_CLOCK_SPEED      12000000



// control register
#define CR_B28      (1<<13)
#define CR_HLB      (1<<12)
#define CR_FSELECT  (1<<11)
#define CR_PSELECT  (1<<10)
#define CR_RESET    (1<<8)
#define CR_SLEEP1   (1<<7)
#define CR_SLEEP12  (1<<6)
#define CR_OPTBITEN (1<<5)
#define CR_DIV2     (1<<3)
#define CR_MODE     (1)


void
set_frequency(double freq)
{
   uint32_t out_freq = ((freq * pow(2, 28)) / 25000000.0);

   //   SPI.beginTransaction(SPISettings(SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE2));

   digitalWrite(FSYNC_PIN, LOW);

   SPI.transfer16(REG_CONTROL | CR_B28 | CR_RESET);
   SPI.transfer16(REG_FREQ0 | (out_freq & 0x3FFF));
   SPI.transfer16(REG_FREQ0 | (out_freq >> 14 ));
   SPI.transfer16(REG_PHASE0);
   SPI.transfer16(REG_CONTROL | CR_B28);

   digitalWrite(FSYNC_PIN, HIGH);

   SPI.endTransaction();
}


void
set_pot(uint8_t val)
{
   digitalWrite(CS_POT_PIN, LOW);

   SPI.transfer(B00010001); // This tells the chip to set the pot
   SPI.transfer(val);
   digitalWrite(CS_POT_PIN, HIGH);

}


void
setup()
{
   pinMode(CS_POT_PIN, OUTPUT);
   pinMode(FSYNC_PIN, OUTPUT);
   pinMode(D2, OUTPUT);

   digitalWrite(CS_POT_PIN, HIGH);
   digitalWrite(FSYNC_PIN, HIGH);

   SPI.begin();
   //SPI.beginTransaction(SPISettings(SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE2));

   //delay(1000);
   set_frequency(4000000);

   set_pot(0xFF);
}



void
loop()
{
   // set_pot(0);
   // delay(1000);
   // set_pot(0x80);
   // delay(1000);
   // set_pot(0xFF);
   // delay(1000);


}
