#include "base.h"
#define N 1<<12

extern void array_f(int a[N][N],int b[N][N]);
extern void init_a(int a[N][N], int b[N][N]);
int main (void){
	
		int a[N][N];
		int b[N][N];
		init_a(a,b);
	  printDecimal(a[N-1][N-1]);
		sendstr("\n");
		array_f(a,b);
		printDecimal(b[N-1][N-1]);
		_sys_exit(0);
}
