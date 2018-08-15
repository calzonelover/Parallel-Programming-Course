#include<stdio.h>
int main()
{
	int num, *numPtr, b[5], *n;
	num = 1;
	printf("num = %d, &num= %p \n", num, &num);
	numPtr = &num;
	*numPtr = 2;
	printf("after pointed => num = %d\n", num);
	for (unsigned int i=0; i<5; i++){
		b[i] = i+1;
		printf("b[i] = %d \n", b[i]);
	}
	n = b; // pointer n would be assigned with b[0] address
	*n = 20;
	*(n+1) = 15;
	printf("Assigned value by passing from address \n");
	for (unsigned int i=0; i<5; i++){
		printf("b[i] = %d \n", b[i]);	
	}
}
