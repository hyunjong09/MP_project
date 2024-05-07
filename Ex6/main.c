#include "base.h"
#define ARR_SIZE 10

extern int adder2 (int *arr_in, int arr_size) ;

int main (void) 
{   
		int arr_in[ARR_SIZE] = {1,2,3,4,5,6,7,8,9,10};
		int result = adder2(arr_in, ARR_SIZE);
		
	  printDecimal(result);
    
	  _sys_exit(0);
}
