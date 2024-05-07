#include "base.h"
#define N 1<<12
	
void init_a(int a[N][N], int b[N][N]){
	
		for(int i=0;i<N;i++){
			for(int j=0;j<N;j++){
				a[i][j]=i+j;
				b[i][j]=0;
			}	
		}
}

void array_f(int a[N][N],int b[N][N]){
	
		for(int i=0;i<N;i++){
			for(int j=0;j<N;j++){
				if(j==0)
						b[i][0] = b[i][0] + a[i][0];
				else 
						b[i][j]=b[i][j-1] + a[i][j];
			}
		}
		
}
