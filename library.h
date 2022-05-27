#include <random>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>



#ifndef LIBRARY_H
#define LIBRARY_H


#define NUMBER_SLOT     5       // Số slot trên băng chuyền
#define NUMBER_MACHINE  3       // Số Worker
#define LINE_TIME       1000000   // Thời gian dịch chuyển của băng chuyền
#define PICK_TIME       500000    // Thời gian gắp của mỗi cánh tay
#define ASSEMBLE_TIME   600000    // Thời gian ghép sản phẩm của 2 cánh tay
#define ALARM_TIME      20000000  
#define THRESHOLD_MIN   4       // Số lượng component tối thiểu trong 1 khoảng thời gian cho trước
#define SPEED_UNIT      00000   // Đơn vị thời gian xử lý
#define NUM_TEST        60      // Số sản phẩm đưa vào mỗi testcase


// #define IN  0
// #define OUT 1

// #define LOW  0
// #define HIGH 1


enum TYPE {
    NONE        = 0,
    COMPONENT_A = 1,
    COMPONENT_B = 2,
    PRODUCT     = 3
};

/**=====================================================================================================
 *                             XÓA COMMENT ĐỂ CHẠY CÁC TEST CASE KHÁC
 ======================================================================================================*/

// static TYPE arr1[NUM_TEST] = {(TYPE) 1, (TYPE) 2, (TYPE) 2, (TYPE) 2, (TYPE) 2, (TYPE) 0, (TYPE) 2, (TYPE) 0, (TYPE) 0, (TYPE) 1, (TYPE) 1, (TYPE) 0, (TYPE) 0, (TYPE) 2, (TYPE) 1, (TYPE) 1, (TYPE) 1, (TYPE) 0, (TYPE) 2, (TYPE) 2, (TYPE) 0, (TYPE) 0, (TYPE) 1, (TYPE) 1, (TYPE) 0, (TYPE) 2, (TYPE) 2, (TYPE) 1, (TYPE) 1, (TYPE) 1, (TYPE) 1, (TYPE) 0, (TYPE) 0, (TYPE) 1, (TYPE) 0, (TYPE) 0, (TYPE) 0, (TYPE) 2, (TYPE) 2, (TYPE) 1, (TYPE) 2, (TYPE) 1, (TYPE) 0, (TYPE) 2, (TYPE) 0, (TYPE) 1, (TYPE) 0, (TYPE) 0, (TYPE) 2, (TYPE) 0, (TYPE) 0, (TYPE) 1, (TYPE) 1, (TYPE) 0, (TYPE) 2, (TYPE) 2, (TYPE) 2, (TYPE) 2, (TYPE) 1, (TYPE) 1};
// static TYPE arr1[NUM_TEST] = {(TYPE) 2, (TYPE) 0, (TYPE) 1, (TYPE) 2, (TYPE) 0, (TYPE) 1, (TYPE) 2, (TYPE) 1, (TYPE) 2, (TYPE) 1, (TYPE) 1, (TYPE) 0, (TYPE) 0, (TYPE) 2, (TYPE) 0, (TYPE) 0, (TYPE) 2, (TYPE) 2, (TYPE) 1, (TYPE) 0, (TYPE) 0, (TYPE) 0, (TYPE) 1, (TYPE) 2, (TYPE) 1, (TYPE) 1, (TYPE) 0, (TYPE) 1, (TYPE) 2, (TYPE) 0, (TYPE) 1, (TYPE) 2, (TYPE) 2, (TYPE) 0, (TYPE) 1, (TYPE) 0, (TYPE) 0, (TYPE) 1, (TYPE) 0, (TYPE) 0, (TYPE) 2, (TYPE) 1, (TYPE) 0, (TYPE) 2, (TYPE) 0, (TYPE) 2, (TYPE) 1, (TYPE) 0, (TYPE) 2, (TYPE) 2, (TYPE) 0, (TYPE) 2, (TYPE) 0, (TYPE) 1, (TYPE) 2, (TYPE) 2, (TYPE) 0, (TYPE) 1, (TYPE) 2, (TYPE) 1};
static TYPE arr1[NUM_TEST] = {(TYPE) 2, (TYPE) 0, (TYPE) 1, (TYPE) 2, (TYPE) 2, (TYPE) 2, (TYPE) 1, (TYPE) 1, (TYPE) 0, (TYPE) 1, (TYPE) 1, (TYPE) 0, (TYPE) 2, (TYPE) 1, (TYPE) 0, (TYPE) 1, (TYPE) 0, (TYPE) 0, (TYPE) 1, (TYPE) 0, (TYPE) 1, (TYPE) 2, (TYPE) 0, (TYPE) 2, (TYPE) 0, (TYPE) 2, (TYPE) 2, (TYPE) 2, (TYPE) 2, (TYPE) 2, (TYPE) 2, (TYPE) 2, (TYPE) 1, (TYPE) 1, (TYPE) 0, (TYPE) 1, (TYPE) 0, (TYPE) 1, (TYPE) 1, (TYPE) 0, (TYPE) 0, (TYPE) 1, (TYPE) 0, (TYPE) 1, (TYPE) 1, (TYPE) 0, (TYPE) 1, (TYPE) 0, (TYPE) 0, (TYPE) 2, (TYPE) 0, (TYPE) 2, (TYPE) 1, (TYPE) 1, (TYPE) 1, (TYPE) 0, (TYPE) 1, (TYPE) 2, (TYPE) 1, (TYPE) 2};


/**=====================================================================================================
 *                                      PIN CUẢ PI
 ======================================================================================================*/

static int arr_pin[NUMBER_MACHINE*2] = {7, 15, 0, 1, 3, 4};



#endif