#include <iostream>
#include <cstring>
#include <pthread.h>
#include <unistd.h>
#include <wiringPi.h>
#include "Worker.h"
#include "Sensor.h"
#include "library.h"
#include "Conveyor.h"

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
        usleep(LINE_TIME);     // Delay 1 kho???ng th???i gian sau ???? d???ch chuy???n b??ng chuy???n
    } 
    // Khi ???? h???t s???n ph???m ????a v??o
    // Cho h??? th???ng ch???y ?????n khi ko c??n g?? tr??n b??ng
    while (sum()) {
        for (int i = 0; i < NUMBER_SLOT; i++) {
            pthread_mutex_lock(&mutex);
            for (int j = NUMBER_SLOT-1; j > 0; j--) {
                slot[j] = slot[j-1];
            }
            slot[0] = NONE;
            printSlot();
            pthread_mutex_unlock(&mutex);
            usleep(LINE_TIME);     // Delay 1 kho???ng th???i gian sau ???? d???ch chuy???n b??ng chuy???n
        } 
    }

    setFinish(true);            // G???i t??n hi???u b??o hi???u k???t th??c
    pthread_exit(NULL);
}
// KHONG THE GOI DUOC DO HAM STATIC KO THE GOI HAM NON STATIC
void *Conveyor::runWorker(void *ptr) {
    machine *mch;
    mch = (machine *) ptr;

    while (!getFinish()) {
        mch->sensor.setState(getSlot_i(mch->i));
        if ( !( mch->worker.getTakeA() | mch->worker.getTakeB() ) ) {   // N???u worker ch??a g???p ???????c component n??o
            if ( mch->sensor.getState() == COMPONENT_A ) {              // N???u detect ra component A
                digitalWrite(arr_pin[mch->i*2], HIGH);                       // B???t led i*2 t????ng ???ng v???i c??nh tay tr??i
                pthread_mutex_lock(&mutex);
                std::cout << mch->i ;
                mch->worker.leftArmPickA();                             // -> Lu??n cho c??nh tay tr??i g???p tr?????c
                setSLot_i(mch->i, NONE);
                // digitalWrite(arr_pin[mch->i*2], LOW);                       // T???t led i*2 t????ng ???ng v???i c??nh tay tr??i
                pthread_mutex_unlock(&mutex);

                while (mch->sensor.getState() != COMPONENT_B){
                    mch->sensor.setState( getSlot_i(mch->i) );          // ?????i ?????n khi detect ???????c component B
                }
                
                digitalWrite(arr_pin[mch->i*2+1], HIGH);                     // B???t led i*2 t????ng ???ng v???i c??nh tay tr??i
                pthread_mutex_lock(&mutex);
                std::cout << mch->i ;
                mch->worker.rightArmPickB();                            // -> Cho c??nh tay ph???i g???p B
                setSLot_i(mch->i, NONE);
                // digitalWrite(arr_pin[mch->i*2+1], LOW);                     // T???t led i*2 t????ng ???ng v???i c??nh tay tr??i
                pthread_mutex_unlock(&mutex);

                usleep(LINE_TIME);
                std::cout << mch->i ;
                mch->worker.assembleProduct();                          // Gh??p 2 component -> Product
                usleep(LINE_TIME);

                while (mch->sensor.getState() != NONE) {
                    mch->sensor.setState( getSlot_i(mch->i) );          // ?????i ?????n khi c?? slot tr???ng
                }

                pthread_mutex_lock(&mutex);
                std::cout << mch->i ;
                // digitalWrite(arr_pin[mch->i*2], HIGH);                       // B???t 2 led t????ng ???ng PUT PRODUCT
                // digitalWrite(arr_pin[mch->i*2+1], HIGH);                     
                mch->worker.putProduct();                               // ?????t Product
                digitalWrite(arr_pin[mch->i*2], LOW);                       // T??ts 2 led t????ng ???ng PUT PRODUCT
                digitalWrite(arr_pin[mch->i*2+1], LOW); 
                setSLot_i(mch->i, PRODUCT);
                increaseNumberProduct();
                std::cout << "\nNUMBER PRODUCT : "  << getNumberProduct() << std::endl;
                pthread_mutex_unlock(&mutex);
            }


            else if ( mch->sensor.getState() == COMPONENT_B ) {         // N???u detect ra component B
                pthread_mutex_lock(&mutex);
                std::cout << mch->i ;
                digitalWrite(arr_pin[mch->i*2+1], HIGH);                       // B???t led i*2 t????ng ???ng v???i c??nh tay tr??i
                mch->worker.leftArmPickB();                             // -> Lu??n cho c??nh tay tr??i g???p tr?????c
                setSLot_i(mch->i, NONE);
                // digitalWrite(arr_pin[mch->i*2+1], LOW);                       // T???t led i*2 t????ng ???ng v???i c??nh tay tr??i
                pthread_mutex_unlock(&mutex);

                while (mch->sensor.getState() != COMPONENT_A){
                    mch->sensor.setState( getSlot_i(mch->i) );          // ?????i ?????n khi detect ???????c component A
                }

                pthread_mutex_lock(&mutex);
                std::cout << mch->i ;
                digitalWrite(arr_pin[mch->i*2], HIGH);                     // B???t led i*2 t????ng ???ng v???i c??nh tay tr??i
                mch->worker.rightArmPickA();                            // -> Cho c??nh tay ph???i g???p A
                setSLot_i(mch->i, NONE);
                // digitalWrite(arr_pin[mch->i*2], LOW);                     // T???t led i*2 t????ng ???ng v???i c??nh tay tr??i
                pthread_mutex_unlock(&mutex);

                usleep(LINE_TIME);
                std::cout << mch->i ;
                mch->worker.assembleProduct();                          // Gh??p 2 component -> Product
                usleep(LINE_TIME);

                while (mch->sensor.getState() != NONE) {
                    mch->sensor.setState( getSlot_i(mch->i) );          // ?????i ?????n khi c?? slot tr???ng
                }

                pthread_mutex_lock(&mutex);
                std::cout << mch->i ;
                // digitalWrite(arr_pin[mch->i*2], HIGH);                       // B???t 2 led t????ng ???ng PUT PRODUCT
                // digitalWrite(arr_pin[mch->i*2+1], HIGH);                     
                mch->worker.putProduct();                               // ?????t Product
                digitalWrite(arr_pin[mch->i*2], LOW);                       // T??ts 2 led t????ng ???ng PUT PRODUCT
                digitalWrite(arr_pin[mch->i*2+1], LOW); 
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
        digitalWrite(25, LOW);
        usleep(ALARM_TIME/2);
        if (mch0.worker.getNumberProduct() < THRESHOLD_MIN)
        {  
            digitalWrite(25, HIGH);
            std::cout << "\nNUMBER PRODUCT IN " << ALARM_TIME << " us: " << getNumberProduct() << std::endl;
            std::cout << "\n=========================================";
            std::cout << "\n             MISS DEADLINE: " << std::endl;
            std::cout << "=========================================" << std::endl;
        }
        clearNumberProduct();       
        usleep(ALARM_TIME/2);
    }

    pthread_exit(NULL);
}


void Conveyor::run(){
    // Enable GPIO PIN
    wiringPiSetup();
    for (int i = 0; i < NUMBER_MACHINE*2; i++) {
        pinMode(arr_pin[i], OUTPUT);
    }
    pinMode(25, OUTPUT);
    for (int i = 0; i < NUMBER_MACHINE*2; i++) {
        digitalWrite(arr_pin[i], LOW);
    }
    digitalWrite(25, LOW);
    
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