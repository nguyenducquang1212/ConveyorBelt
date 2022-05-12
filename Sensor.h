#include "library.h"

class Sensor {
private:
    TYPE state;
public:
    Sensor();

    ~Sensor();

    TYPE getState();

    void setState(TYPE state);
};