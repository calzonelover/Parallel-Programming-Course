#include<stdio.h>
int main()
{
	int I;
	long int LI;
	float F;
	double D;
	printf("int = %ld bytes\n",sizeof(I));
	printf("long = %ld bytes\n", sizeof(LI));
	printf("float = %ld bytes\n", sizeof(F));
	printf("double = %ld bytes\n", sizeof(D));
}
