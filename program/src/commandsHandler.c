#include <stdio.h>
#include <stdlib.h>

#include <prototypesS.h>
#include <globalHeader.h>

PACKAGE mountResponsePackage(char status, int response, PACKAGE outPackage){
	outPackage.msg = status;
	sprintf(outPackage.num,"%X",response);
	return outPackage;
}

void cmd_D(int operand[1], STACK_DESCRIPTOR *stack_desc){
	STACK_ELEMENT *stack_el;
	MALL(stack_el,1)
	stack_el->operand = *operand;
	stack_el->next = stack_desc->first;
	stack_desc->first = stack_el;
	stack_desc->count++;
}
int cmd_T(STACK_DESCRIPTOR *stack_desc){
	if (stack_desc->count > 0){ /*It's a very important test. If it is not done, stack_desc->first is pointing to NULL and
								  stack_desc->first->operand will cause a segmentairon fault. Ńever remove this check!*/
		return stack_desc->first->operand;
	} else { 
		return 0; /*IT CAN'T RETURN ZERO. IT SHOULD RETURN INVALID COMMAND. NEEDS TO BE REVISED!!*/
	}
}
int cmd_P(STACK_DESCRIPTOR *stack_desc){
	return stack_desc->count;
}
void cmd_add(STACK_DESCRIPTOR *stack_desc) {
	int sum;
	sum = stack_desc->first->operand + stack_desc->first->next->operand;
	reorganizeStack(stack_desc,sum);
}
void cmd_sub(STACK_DESCRIPTOR *stack_desc) {
	int sub;
	sub = stack_desc->first->operand - stack_desc->first->next->operand;
	reorganizeStack(stack_desc,sub);
}
void cmd_div(STACK_DESCRIPTOR *stack_desc) {
	int division;
	division = stack_desc->first->operand / stack_desc->first->next->operand;
	reorganizeStack(stack_desc,division);
}
void cmd_mult(STACK_DESCRIPTOR *stack_desc) {
	int prod;
	prod = stack_desc->first->operand * stack_desc->first->next->operand;
	reorganizeStack(stack_desc,prod);
}
void cmd_reminder(STACK_DESCRIPTOR *stack_desc){
	int rem;
	rem = stack_desc->first->operand % stack_desc->first->next->operand;
	reorganizeStack(stack_desc,rem);	
}

int cmd_R(STACK_DESCRIPTOR *stack_desc){
	int result = cmd_T(stack_desc);
	stack_desc->count = 0;
	free(stack_desc->first);
	return result; /*It only calls the T command to get the first value in the stack.
					*** IT NEEDS TO BE REVISED SINCE NOT NECESSARILY THE VALUE IN THE
						TOP OF STACK IS THE FINAL RESULT!!*** */
}

void reorganizeStack(STACK_DESCRIPTOR *stack_desc, int newValue){
	STACK_ELEMENT *toEliminate;
	stack_desc->first->next->operand = newValue;
	toEliminate = stack_desc->first; 
	stack_desc->first = stack_desc->first->next;
	stack_desc->count--;
	free(toEliminate);
}