#include "base.h"
#define ARR_SIZE 10

extern void adder (int *arr_in, int *arr_out, int arr_size, int *sum) ;

int main (void) 
{   
		int arr_in[ARR_SIZE] = {1,2,3,4,5,6,7,8,9,10};
		int arr_out[ARR_SIZE];
		int sum=0;
	
		printArr(arr_in, ARR_SIZE);
		
	  adder(arr_in, arr_out, ARR_SIZE, &sum);
	
    printArr(arr_out, ARR_SIZE);
    printDecimal(sum);
	  _sys_exit(0);
}
