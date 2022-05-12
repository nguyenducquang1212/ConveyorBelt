#include <iostream>
#include <unistd.h>
#include "Worker.h"
#include "library.h"

Worker::Worker () {
    takeA = false;
    takeB = false;
}

Worker::~Worker() {

}

int Worker::numberProduct = 0;

bool Worker::getTakeA () {
    return takeA;
}

bool Worker::getTakeB () {
    return takeB;
}

bool Worker::getLeftPick () {
    return leftPick;
}

bool Worker::getRightPick () {
    return rightPick;
}

void Worker::setTakeA(bool takeA){
    this->takeA = takeA;
}

void Worker::setTakeB(bool takeB){
    this->takeB = takeB;
}

void Worker::setLeftPick(bool leftPick){
    this->leftPick = leftPick;
}

void Worker::setRightPick(bool rightPick){
    this->rightPick = rightPick;
}

void Worker::leftArmPickA () {
    usleep(PICK_TIME);      // Delay của việc gắp
    std::cout << "\nLeft arm picks Component A" << std::endl;
    setLeftPick(true);      // Thông báo rằng cánh tay trái 
    setTakeA(true);         // Thông báo rằng đã gắp component A
}

void Worker::leftArmPickB () {
    usleep(PICK_TIME);      // Delay của việc gắp
    std::cout << "\nLeft arm picks Component B" << std::endl;
    setLeftPick(true);      // Thông báo rằng cánh tay trái 
    setTakeB(true);         // Thông báo rằng đã gắp component B
}

void Worker::rightArmPickA () {
    usleep(PICK_TIME);      // Delay của việc gắp
    std::cout << "\nRight arm picks Component A" << std::endl;
    setRightPick(true);     // Thông báo rằng cánh tay phải
    setTakeA(true);         // Thông báo rằng đã gắp component A
}

void Worker::rightArmPickB () {
    std::cout << "\nRight arm picks Component B" << std::endl;
    usleep(PICK_TIME);      // Delay của việc gắp
    setRightPick(true);     // Thông báo rằng cánh tay phải
    setTakeB(true);         // Thông báo rằng đã gắp component B
}

void Worker::increaseNumberProduct() {
    numberProduct++;
}


void Worker::assembleProduct() {
    usleep(ASSEMBLE_TIME);
    std::cout << "\nAssemble Product" << std::endl;
}

int Worker::getNumberProduct() {
    return numberProduct;
}

void Worker::setNumberProduct(int n) {
    numberProduct = n;
}

void Worker::putProduct() {
    usleep(PICK_TIME);     // Delay của việc đặt sản phẩm
    // Sau khi đặt thành công, đưa hết tất cả các giá trị về false
    std::cout << "\nPut Product" << std::endl;
    setLeftPick(false);
    setRightPick(false);
    setTakeA(false);
    setTakeB(false);
    increaseNumberProduct();
}