#include "Conveyor.h"



int main(int argc, char const *argv[])
{
    // Enable GPIO PIN
    for (int i = 0; i < NUMBER_MACHINE*2; i++) {
        if (-1 == GPIOExport(arr_pin[i]))
		    return(1);
    }
    
	
    //Set GPIO directions
    for (int i = 0; i < NUMBER_MACHINE*2; i++) {
        if (-1 == GPIODirection(arr_pin[i], OUT))
            return(2);
    }

    Conveyor conveyor;

    // for (int i = 0; i < NUM_TEST; i++)
    // {
    //     std::cout << "(TYPE) " << randomType()  <<  ", ";
    // }
        
    conveyor.run();

    
    return 0;
}
