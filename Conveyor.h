#include <iostream>
#include <cstring>
#include <pthread.h>
#include <unistd.h>
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


machine Conveyor::mch0;
machine Conveyor::mch1;
machine Conveyor::mch2;
int Conveyor::numberProduct;
TYPE Conveyor::slot [NUMBER_SLOT];
bool Conveyor::finish;


Conveyor::Conveyor(){
    for (int i = 0; i < NUMBER_SLOT; i++){
        slot[i] = NONE;
    }
    mch0.i = 0;
    mch1.i = 1;
    mch2.i = 2;
    finish = false;
}

Conveyor::~Conveyor(){
    
}

void Conveyor::setFinish(bool fi) {
    finish = fi;
}

bool Conveyor::getFinish(){
    return finish;
}

TYPE Conveyor::getSlot_i(int i) {
    return slot[i];
}

void Conveyor::setSLot_i(int i, TYPE status) {
    slot[i] = status;
}

int Conveyor::getNumberProduct() {
    return numberProduct;
}

void Conveyor::clearNumberProduct() {
    numberProduct = 0;
}

void Conveyor::increaseNumberProduct() {
    numberProduct++;
}

std::string Conveyor::getSlot_i_String(int i) {
    if (slot[i] == NONE){
        return " ";
    }
    else if (slot[i] == COMPONENT_A) {
        return "A";
    }
    else if (slot[i] == COMPONENT_B) {
        return "B";
    }
    else {
        return "P";
    }
    
}

void Conveyor::printSlot() {
    std::cout << "\nConveyor Belt";
    std::cout <<"\nL: ";
    for(int i = 0; i < NUMBER_MACHINE; i++){
        std::cout << i << " | ";
    }
    std::cout << "\n ---------------------------" << std::endl;
    std::cout << " | ";
    for(int i = 0; i < NUMBER_SLOT; i++) {
        std::cout << getSlot_i_String(i) << " | ";
    }
    std::cout << "\n ---------------------------" << std::endl;
    std::cout <<"R: ";
    for(int i = 0; i < NUMBER_MACHINE; i++){
        std::cout << i << " | ";
    }
    std::cout << std::endl;
}

int Conveyor::sum(){
    int sum = 0;
    for (int i = 0; i < NUMBER_SLOT; i++) {
        sum += slot[i];
    }
    return sum;
}

void *Conveyor::runConveyorBelt(void *ptr){
    setFinish(false);
    TYPE *line;
    line = (TYPE *) ptr;
    for (int i = 0; i < NUM_TEST; i++) {
        pthread_mutex_lock(&mutex);
        for (int j = NUMBER_SLOT-1; j > 0; j--) {
            slot[j] = slot[j-1];
        }
        // std::cout << *(line+i) << std::endl;
        slot[0] = *(line + i);
        printSlot();
        pthread_mutex_unlock(&mutex);
        usleep(LINE_TIME);     // Delay 1 khoảng thời gian sau đó dịch chuyển băng chuyền
    } 
    // Khi đã hết sản phẩm đưa vào
    // Cho hệ thống chạy đến khi ko còn gì trên băng
    while (sum()) {
        for (int i = 0; i < NUMBER_SLOT; i++) {
            pthread_mutex_lock(&mutex);
            for (int j = NUMBER_SLOT-1; j > 0; j--) {
                slot[j] = slot[j-1];
            }
            slot[0] = NONE;
            printSlot();
            pthread_mutex_unlock(&mutex);
            usleep(LINE_TIME);     // Delay 1 khoảng thời gian sau đó dịch chuyển băng chuyền
        } 
    }

    setFinish(true);            // Gửi tín hiệu báo hiệu kết thúc
    pthread_exit(NULL);
}
// KHONG THE GOI DUOC DO HAM STATIC KO THE GOI HAM NON STATIC
void *Conveyor::runWorker(void *ptr) {
    machine *mch;
    mch = (machine *) ptr;

    while (!getFinish()) {
        mch->sensor.setState(getSlot_i(mch->i));
        if ( !( mch->worker.getTakeA() | mch->worker.getTakeB() ) ) {   // Nếu worker chưa gắp được component nào
            if ( mch->sensor.getState() == COMPONENT_A ) {              // Nếu detect ra component A
                pthread_mutex_lock(&mutex);
                std::cout << mch->i ;
                GPIOWrite(arr_pin[mch0->i*2], 1);                       // Bật led i*2 tương ứng với cánh tay trái
                mch->worker.leftArmPickA();                             // -> Luôn cho cánh tay trái gắp trước
                setSLot_i(mch->i, NONE);
                GPIOWrite(arr_pin[mch0->i*2], 0);                       // Tắt led i*2 tương ứng với cánh tay trái
                pthread_mutex_unlock(&mutex);

                while (mch->sensor.getState() != COMPONENT_B){
                    mch->sensor.setState( getSlot_i(mch->i) );          // Đợi đến khi detect được component B
                }
                pthread_mutex_lock(&mutex);
                std::cout << mch->i ;
                GPIOWrite(arr_pin[mch0->i*2+1], 1);                     // Bật led i*2 tương ứng với cánh tay trái
                mch->worker.rightArmPickB();                            // -> Cho cánh tay phải gắp B
                setSLot_i(mch->i, NONE);
                GPIOWrite(arr_pin[mch0->i*2+1], 0);                     // Tắt led i*2 tương ứng với cánh tay trái
                pthread_mutex_unlock(&mutex);

                usleep(LINE_TIME);
                std::cout << mch->i ;
                mch->worker.assembleProduct();                          // Ghép 2 component -> Product
                usleep(LINE_TIME);

                while (mch->sensor.getState() != NONE) {
                    mch->sensor.setState( getSlot_i(mch->i) );          // Đợi đến khi có slot trống
                }

                pthread_mutex_lock(&mutex);
                std::cout << mch->i ;
                GPIOWrite(arr_pin[mch0->i*2], 1);                       // Bật 2 led tương ứng PUT PRODUCT
                GPIOWrite(arr_pin[mch0->i*2+1], 1);                     
                mch->worker.putProduct();                               // Đặt Product
                GPIOWrite(arr_pin[mch0->i*2], 0);                       // Tăts 2 led tương ứng PUT PRODUCT
                GPIOWrite(arr_pin[mch0->i*2+1], 0); 
                setSLot_i(mch->i, PRODUCT);
                increaseNumberProduct();
                std::cout << "\nNUMBER PRODUCT : "  << getNumberProduct() << std::endl;
                pthread_mutex_unlock(&mutex);
            }


            else if ( mch->sensor.getState() == COMPONENT_B ) {         // Nếu detect ra component B
                pthread_mutex_lock(&mutex);
                std::cout << mch->i ;
                mch->worker.leftArmPickB();                             // -> Luôn cho cánh tay trái gắp trước
                setSLot_i(mch->i, NONE);
                pthread_mutex_unlock(&mutex);

                while (mch->sensor.getState() != COMPONENT_A){
                    mch->sensor.setState( getSlot_i(mch->i) );          // Đợi đến khi detect được component A
                }

                pthread_mutex_lock(&mutex);
                std::cout << mch->i ;
                mch->worker.rightArmPickA();                            // -> Cho cánh tay phải gắp A
                setSLot_i(mch->i, NONE);

                usleep(LINE_TIME);
                std::cout << mch->i ;
                mch->worker.assembleProduct();                          // Ghép 2 component -> Product
                pthread_mutex_unlock(&mutex);
                usleep(LINE_TIME);

                while (mch->sensor.getState() != NONE) {
                    mch->sensor.setState( getSlot_i(mch->i) );          // Đợi đến khi có slot trống
                }

                pthread_mutex_lock(&mutex);
                std::cout << mch->i ;
                mch->worker.putProduct();                               // Đặt Product
                setSLot_i(mch->i, PRODUCT);
                increaseNumberProduct();
                std::cout << "\nNUMBER PRODUCT : "  << getNumberProduct() << std::endl;
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    pthread_exit(NULL);
}

void *Conveyor::alarm(void *ptr){
    while (!getFinish()) { 
    usleep(ALARM_TIME);
        if (mch0.worker.getNumberProduct() < THRESHOLD_MIN)
        {  
            std::cout << "\nNUMBER PRODUCT IN " << ALARM_TIME << " us: " << getNumberProduct() << std::endl;
            std::cout << "\n=========================================";
            std::cout << "\n             MISS DEADLINE: " << std::endl;
            std::cout << "=========================================" << std::endl;
        }
        clearNumberProduct();       
    }
    pthread_exit(NULL);
}


void Conveyor::run(){
    pthread_t conveyorBeltThread;
    pthread_t threads[NUMBER_MACHINE];
    pthread_t alamrThread;

    int ret;

    ret = pthread_create(&alamrThread, NULL, alarm, NULL);
    if (ret) {
        std::cout << "\nError: Cannot creat Alarm thread! " << ret << std::endl;
        exit(-1);
    }

    // std::cout << "Create conveyorBeltThread" << std::endl;
    ret = pthread_create(&conveyorBeltThread, NULL, runConveyorBelt, (void *) &arr1);
    if (ret) {
        std::cout << "\nError: Cannot create conveyorBeltThread! " << ret << std::endl;
        exit(-1);
    } 

    // std::cout << "Tao thread " << 0 << std::endl;
    ret = pthread_create(&threads[0], NULL, runWorker, (void *) &mch0);
    if (ret) {
        std::cout << "\nError: Cannot creat threads! " << ret << std::endl;
        exit(-1);
    }

    // std::cout << "Tao thread " << 1 << std::endl;
    ret = pthread_create(&threads[1], NULL, runWorker, (void *) &mch1);
    if (ret) {
        std::cout << "\nError: Cannot creat threads! " << ret << std::endl;
        exit(-1);
    }

    // std::cout << "Tao thread " << 2 << std::endl;
    ret = pthread_create(&threads[2], NULL, runWorker, (void *) &mch2);
    if (ret) {
        std::cout << "\nError: Cannot creat threads! " << ret << std::endl;
        exit(-1);
    }

    pthread_join(conveyorBeltThread, NULL);

    pthread_join(alamrThread, NULL);

    for (int i = 0; i < NUMBER_MACHINE; i++) {
        ret = pthread_join(threads[i], NULL);
    }
}