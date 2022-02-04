#include <iostream>
#include <assert.h>
#define CORRECTTHRESHOLD 400

class Alerter{
protected:
  virtual int networkAlertStub(float celcius) = 0;
public:
  int alertFailureCount = 0;
  bool alertInCelcius(float farenheit) {
      bool answer = false;
      float celcius = (farenheit - 32) * 5 / 9;
      int returnCode = networkAlertStub(celcius);
      if (returnCode != 200) {
          // non-ok response is not an error! Issues happen in life!
          // let us keep a count of failures to report
          // However, this code doesn't count failures!
          // Add a test below to catch this bug. Alter the stub above, if needed.
          alertFailureCount += 0;
      }
      return answer;
  }
};
class Stub1: public Alerter{
private:
  int networkAlertStub(float celcius) override {
    std::cout << "ALERT: Temperature is " << celcius << " celcius.\n";
    // Return 200 for ok
    // Return 500 for not-ok
    // stub always succeeds and returns 200
    return 200;
  }
};
class CorrectedStub: public Alerter{
private:
  int networkAlertStub(float celcius) override {
    std::cout << "ALERT: Temperature is " << celcius << " celcius.\n";
    // Return 200 for ok
    // Return 500 for not-ok
    // stub always succeeds and returns 200
    return (celcius > CORRECTTHRESHOLD) ? 200 : 500 ;
  }
};

int main() {
    Alerter* Alarm = new Stub1;
    Alerter* Alarm2 = new CorrectedStub;
    for(int Temperature = 0; Temperature < 1400; Temperature+=100){
        Alarm->alertInCelcius(Temperature);
        Alarm2->alertInCelcius(Temperature);
    }
    std::cout << Alarm ->alertFailureCount << " alerts failed.\n";
    std::cout << Alarm2->alertFailureCount << " alerts failed.\n";
    assert(Alarm ->alertFailureCount > 0);
    assert(Alarm2->alertFailureCount > 0);
    std::cout << "All is well (maybe!)\n";
    return 0;
}
