#include <prototypesS.h>

int add (int a, int b)
{
	int c = a + b;
	return c;
}

int sub (int a, int b)
{
	int c = a - b;
	return c;
}

int mult (int a, int b){
	int c = a * b;
	return c;
}

float divide (int a, int b){
	int c = 0;
	if (b == 0){
		fprintf(stderr,"\nDivision by zero is not allowed.\n");
		exit(EXIT_FAILURE);
	}else{
		int c = a / b;
	}
	return c;
}
