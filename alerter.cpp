#include <iostream>
#include <cmath>
#include <assert.h>
#define CORRECTTHRESHOLD 410
#define CORRECTTHRESHOLD2 550

class Alerter{
protected:
    virtual int networkAlertStub(float celcius) = 0;
public:
    size_t alertFailureCount = 0;//if we make this private it could be a way to prevent this from ever resetting
    bool alertInCelcius(float farenheit) {
        bool SomethingWentWrong = true;
        float celcius = (farenheit - 32) * 5 / 9;
        int returnCode = networkAlertStub(celcius);
        if (returnCode == 500) {
            // non-ok response is not an error! Issues happen in life!
            // let us keep a count of failures to report
            // However, this code doesn't count failures!
            // Add a test below to catch this bug. Alter the stub above, if needed.
            alertFailureCount ++;
            SomethingWentWrong = false;
        }
        else if(returnCode != 200){
            SomethingWentWrong = false;
        }
        return SomethingWentWrong;
    }
};
class Stub1Corrected: public Alerter{
private:
    int networkAlertStub(float celcius) override {
        std::cout << "ALERT: Temperature is " << celcius << " celcius.\n";
        // Return 200 for ok
        // Return 500 for not-ok
        // stub always succeeds and returns 200
        return (celcius < CORRECTTHRESHOLD2) ? 200 : 500 ;
    }
};
class CorrectedStub: public Alerter{
private:
    int networkAlertStub(float celcius) override {
        std::cout << "ALERT: Temperature is " << celcius << " celcius.\n";
        // Return 200 for ok
        // Return 500 for not-ok
        // stub always succeeds and returns 200
        return (celcius < CORRECTTHRESHOLD) ? 200 : 500 ;
    }
};

int main() {
    size_t TotalAlerts = 0;
    const float MaxTestTemp = 1400.00;
    Alerter* Alarm  = new Stub1Corrected;
    Alerter* Alarm2 = new CorrectedStub;
    //Design the test to go from 0°f to 1400°f with 100° increments
    for(float Temperature = 0; Temperature < MaxTestTemp; Temperature+=100){
        Alarm ->alertInCelcius(Temperature);
        Alarm2->alertInCelcius(Temperature);
        TotalAlerts++;
    }
    std::cout << Alarm ->alertFailureCount << "/" << TotalAlerts << " alerts failed.\n";
    std::cout << Alarm2->alertFailureCount << "/" << TotalAlerts << " alerts failed.\n";
    assert(Alarm ->alertFailureCount == std::floor(TotalAlerts * (1 - (CORRECTTHRESHOLD2*9/5+32) / MaxTestTemp)));
    assert(Alarm2->alertFailureCount == std::floor(TotalAlerts * (1 - (CORRECTTHRESHOLD *9/5+32) / MaxTestTemp)));
    std::cout << "All is well (maybe!)\n";
    return 0;
}
