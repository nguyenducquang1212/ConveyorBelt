#include "Worker.h"
#include "Sensor.h"
#include "library.h"



pthread_mutex_t mutex =  PTHREAD_MUTEX_INITIALIZER;

TYPE randomType () {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(0, 2);
    return (TYPE) distr(generator);
}


typedef struct {
    int i = 0;  
    Worker worker;
    Sensor sensor;
} machine;

class Conveyor{
private:
    // static machine mch [NUMBER_MACHINE];
    static machine mch0;
    static machine mch1;
    static machine mch2;
    static TYPE slot [NUMBER_SLOT];
    static int numberProduct;
    static bool finish;
public:
    Conveyor();
    
    ~Conveyor();

    static void setFinish(bool finish);

    static bool getFinish();

    static TYPE getSlot_i(int i);

    static void setSLot_i(int i, TYPE status);

    static std::string getSlot_i_String(int i);

    static void printSlot();

    static void *runConveyorBelt(void *ptr);

    static void *runWorker(void *ptr);

    void run();

    static void *alarm(void *ptr);

    static int sum();

    static int getNumberProduct();

    static void clearNumberProduct();

    static void increaseNumberProduct();

};

