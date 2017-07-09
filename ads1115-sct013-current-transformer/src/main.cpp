
#include <Arduino.h>
#include <Wire.h>
#include <ADS1115.h>



class RmsConverter
{
public:
    RmsConverter(ADS1115* adc_ptr, uint8_t mux)
        : adc_(adc_ptr), mux_(mux), crossings_(100), timeout_ms_(5000), start_time_ms_(millis())
    {}

    bool
    read_sample()
    {
        //adc_->setMultiplexer(mux_);

        num_samples_++;
        double sample = adc_->getConversion() * ADS1115_MV_1P024;
        sum_ += sample * sample;

        if (signbit(prev_sample_) != signbit(sample))
        {
            crossings_--;
        }
        prev_sample_ = sample;

        return (crossings_ == 0) || ((millis() - start_time_ms_) > timeout_ms_);
    }

    double
    get_rms_value()
    {
        double rms_voltage = sqrt(sum_ / num_samples_);

        Serial.println(num_samples_);

        crossings_ = 100;
        sum_ = 0;
        prev_sample_ = 0;
        start_time_ms_ = millis();
        num_samples_ = 0;

        return rms_voltage;
    }

private:
    ADS1115* adc_;
    uint8_t mux_;
    unsigned int crossings_;
    unsigned int timeout_ms_;
    unsigned long start_time_ms_;
    double sum_;
    unsigned int num_samples_;
    double prev_sample_;

};


ADS1115 adc1(ADS1115_ADDRESS_ADDR_GND);
ADS1115 adc2(ADS1115_ADDRESS_ADDR_VDD);

RmsConverter c1(&adc1, ADS1115_MUX_P0_N1);
RmsConverter c2(&adc1, ADS1115_MUX_P2_N3);
RmsConverter c3(&adc2, ADS1115_MUX_P0_N1);


void
setup()
{
    Serial.begin(115200);

    Wire.begin();
    adc1.initialize();

    Serial.println(adc1.testConnection() ? "ADS1115 connection successful" : "ADS1115 connection failed");

    adc1.setMode(ADS1115_MODE_CONTINUOUS);
    adc1.setGain(ADS1115_PGA_1P024);  // range +-1024 mV
    adc1.setRate(ADS1115_RATE_860);  // 860 samples per second
    adc1.setMultiplexer(ADS1115_MUX_P0_N1);
}

void
loop()
{
    if (c1.read_sample())
    {
        Serial.println(c1.get_rms_value());
    }

    // if (c2.read_sample())
    // {
    //     c2.get_rms_value();
    // }

    // double millivolts = calc_rms(100, 5000);
    // Serial.println(millivolts);
    //Serial.println(0.03*millivolts);
}
