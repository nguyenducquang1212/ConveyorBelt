#include <random>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>



#ifndef LIBRARY_H
#define LIBRARY_H


#define NUMBER_SLOT     5       // Số slot trên băng chuyền
#define NUMBER_MACHINE  3       // Số Worker
#define LINE_TIME       10000   // Thời gian dịch chuyển của băng chuyền
#define PICK_TIME       5000    // Thời gian gắp của mỗi cánh tay
#define ASSEMBLE_TIME   6000    // Thời gian ghép sản phẩm của 2 cánh tay
#define ALARM_TIME      200000  
#define THRESHOLD_MIN   4       // Số lượng component tối thiểu trong 1 khoảng thời gian cho trước
#define SPEED_UNIT      00000   // Đơn vị thời gian xử lý
#define NUM_TEST        60      // Số sản phẩm đưa vào mỗi testcase


#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1


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
 *                                      CÁC HÀM CHO RASPBERRY PI
 ======================================================================================================*/

int arr_pin[NUMBER_MACHINE*2] = {7, 8, 11, 12, 15, 16};

static int
GPIOExport(int pin)
{
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int
GPIOUnexport(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int
GPIODirection(int pin, int dir)
{
	static const char s_directions_str[]  = "in\0out";

#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}

	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}

	close(fd);
	return(0);
}

static int
GPIORead(int pin)
{
#define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}

	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return(-1);
	}

	close(fd);

	return(atoi(value_str));
}

static int
GPIOWrite(int pin, int value)
{
	static const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}

	close(fd);
	return(0);
}



#endif