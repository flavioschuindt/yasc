/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Commands' handler functions                              *
 *                                                           *
 *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <arpa/inet.h>

#include <prototypesS.h>
#include <globalHeader.h>


PACKAGE mountResponsePackage(char status, int response, PACKAGE outPackage){
	outPackage.msg = status;
	sprintf(outPackage.num,"%X",response);
	return outPackage;
}


PACKAGE cmd_D (int operand[1], STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage) {
	STACK_ELEMENT *stack_el;
	MALL(stack_el,1);
	stack_el->operand = *operand;
	stack_el->next = stack_desc->first;
	stack_desc->first = stack_el;
	stack_desc->count++;
	return mountResponsePackage('V',OK,outPackage);
}


PACKAGE cmd_T (STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage) {
	if( stack_desc->count > 0 ) {
		return mountResponsePackage('V',stack_desc->first->operand,outPackage);
	} else {
		return mountResponsePackage('E',BAD_STACK,outPackage);
	}
}


PACKAGE cmd_add (STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage) {	/* if 'E' is returned, the operation was not performed; stack is unchanged */
	int sum;

	if (stack_desc->count >= 2){

		if( ((stack_desc->first->operand > 0) && (stack_desc->first->next->operand > (INT_MAX - stack_desc->first->operand))) ||
			((stack_desc->first->operand < 0) && (stack_desc->first->next->operand < (INT_MIN - stack_desc->first->operand))) ) {
			/* OVER/UNDERFLOW */
			return mountResponsePackage('E',OUT_OF_RANGE,outPackage);
		} else {
			sum = stack_desc->first->next->operand + stack_desc->first->operand;
			reorganizeStack(stack_desc,sum);
			return mountResponsePackage('V',OK,outPackage);
		}
	} else {
		/* Not enough operands */
		return mountResponsePackage('E',BAD_STACK,outPackage);
	}
}


PACKAGE cmd_sub (STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage) {	/* if 'E' is returned, the operation was not performed; stack is unchanged */
	int sub;

	if (stack_desc->count >= 2){

		if( ((stack_desc->first->operand > 0) && (stack_desc->first->next->operand < (INT_MIN + stack_desc->first->operand))) ||
			((stack_desc->first->operand < 0) && (stack_desc->first->next->operand > (INT_MAX + stack_desc->first->operand))) ) {
			/* OVER/UNDERFLOW */
			return mountResponsePackage('E',OUT_OF_RANGE,outPackage);
		} else {
			sub = stack_desc->first->next->operand - stack_desc->first->operand;
			reorganizeStack(stack_desc,sub);
			return mountResponsePackage('V',OK,outPackage);
		}
	} else {
		/* Not enough operands */
		return mountResponsePackage('E',BAD_STACK,outPackage);
	}
}


PACKAGE cmd_div (STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage) {	/* if 'E' is returned, the operation was not performed; stack is unchanged */
	int division;

	if( stack_desc->count >= 2 ) {
		if( stack_desc->first->operand != 0 ) {

			if ( (stack_desc->first->next->operand == LONG_MIN) && (stack_desc->first->operand == -1) ) {
				/* OVER/UNDERFLOW */
				return mountResponsePackage('E',OUT_OF_RANGE,outPackage);
			} else {
				division = stack_desc->first->next->operand / stack_desc->first->operand;
				reorganizeStack(stack_desc,division);
				return mountResponsePackage('V',OK,outPackage);
			}
		} else {
			/* division by 0 */
			return mountResponsePackage('E',DIV_0,outPackage);
		}
	} else {
		/* Not enough operands */
		return mountResponsePackage('E',BAD_STACK,outPackage);
	}
}


PACKAGE cmd_mult (STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage) {	/* if 'E' is returned, the operation was not performed; stack is unchanged */
	int prod;

	if( stack_desc->count >= 2 ) {
		if( stack_desc->first->next->operand > 0 ) {
			if( stack_desc->first->operand > 0 ) {
				if( stack_desc->first->next->operand > (INT_MAX / stack_desc->first->operand) ) {
					/* OVER/UNDERFLOW */
					return mountResponsePackage('E',OUT_OF_RANGE,outPackage);
				}
			}
			else {
				if( stack_desc->first->operand < (INT_MIN / stack_desc->first->next->operand) ) {
					/* OVER/UNDERFLOW */
					return mountResponsePackage('E',OUT_OF_RANGE,outPackage);
				}
			}
		}
		else {
			if( stack_desc->first->operand > 0 ) {
				if( stack_desc->first->next->operand < (INT_MIN / stack_desc->first->operand) ) {
					/* OVER/UNDERFLOW */
					return mountResponsePackage('E',OUT_OF_RANGE,outPackage);
				}
			}
			else {
				if( (stack_desc->first->next->operand != 0) && (stack_desc->first->operand < (INT_MAX / stack_desc->first->next->operand)) ) {
					/* OVER/UNDERFLOW */
					return mountResponsePackage('E',OUT_OF_RANGE,outPackage);
				}
			}
		}

		prod = stack_desc->first->next->operand * stack_desc->first->operand;
		reorganizeStack(stack_desc,prod);
		return mountResponsePackage('V',OK,outPackage);
	} else {
		/* Not enough operands */
		return mountResponsePackage('E',BAD_STACK,outPackage);
	}
}


PACKAGE cmd_reminder (STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage) {	/* if 'E' is returned, the operation was not performed; stack is unchanged */
	int rem;

	if( stack_desc->count >= 2 ) {
		if( stack_desc->first->operand != 0 ) {

			if ( (stack_desc->first->next->operand == LONG_MIN) && (stack_desc->first->operand == -1) ) {
				/* OVER/UNDERFLOW */
				return mountResponsePackage('E',OUT_OF_RANGE,outPackage);
			} else {
				rem = stack_desc->first->next->operand % stack_desc->first->operand;
				reorganizeStack(stack_desc,rem);
				return mountResponsePackage('V',OK,outPackage);
			}
		} else {
			/* division by 0 */
			return mountResponsePackage('E',DIV_0,outPackage);
		}
	} else {
		/* Not enough operands */
		return mountResponsePackage('E',BAD_STACK,outPackage);
	}
}


PACKAGE cmd_R (STACK_DESCRIPTOR *stack_desc, PACKAGE outPackage) {		/* stack is cleaned in the process regardless of errors */

	if( stack_desc->count > 0 ) {
		if( stack_desc->count == 1 ) {
			PACKAGE aux_pckg;
			aux_pckg = mountResponsePackage('V', stack_desc->first->operand, outPackage);
			resetStack(stack_desc);
			return aux_pckg;
		} else {
			/* result is only partial; stack is bigger than expected */
			return mountResponsePackage('E', BIG_STACK, outPackage);
		}
	} else {
		/* Stack is empty */
		return mountResponsePackage('E',BAD_STACK,outPackage);
	}
}


void reorganizeStack (STACK_DESCRIPTOR *stack_desc, int newValue) {
	STACK_ELEMENT *toEliminate;
	stack_desc->first->next->operand = newValue;
	toEliminate = stack_desc->first;
	stack_desc->first = stack_desc->first->next;
	stack_desc->count--;
	free(toEliminate);
}


void resetStack (STACK_DESCRIPTOR *stack_desc) {
	STACK_ELEMENT *aux, *aux_eliminate;

	aux = stack_desc->first;

	stack_desc->first = NULL;
	stack_desc->count = 0;

	while( aux != NULL ) {
		aux_eliminate = aux;
		aux = aux->next;
		free(aux_eliminate);
	}
}
