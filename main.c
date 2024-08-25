#include "operations.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	const char key;
	const int precedence;
	operationFunc value;
} operation;



const operation operations[4] = {
	{'+', 1, &sum},
	{'-', 2, &sub},
	{'*', 3, &mult},
	{'/', 4, &divis}
};

bool hasBetterPrecedence(char toCheck, int* precedence){ //check if current index references an operator and if it has a lower precedence
	for(int i = 0; i < 4; i++){
		if(operations[i].key == toCheck && operations[i].precedence < *precedence){
			*precedence = operations[i].precedence;
			return true;
		}
	}
	return false;
}

bool getOperatorIndex(char* expression, int* toReturnOperatorIndex){
	bool hasOperator = false;
	int precedence = 5;
	int index = 0;
	while(expression[index] != '\0'){
		if(hasBetterPrecedence(expression[index], &precedence)){
			hasOperator = true;
			(*toReturnOperatorIndex) = index;
			printf("Operator index is: %o\n", *toReturnOperatorIndex);

		}
		index++;
	}
	fflush(stdout);
	return hasOperator;
}

operationFunc getOperation(char key){
	for(int i = 0; i < 4; i++){
		if(operations[i].key == key){
			return operations[i].value;
		}
	}
}

float calculateExpression(char* expression){
	printf("calculating expression %s\n", expression);
	fflush(stdout);
	int operatorIndex;
	bool hasOperator = getOperatorIndex(expression, &operatorIndex);
	if(!hasOperator) return strtof(expression, NULL);

	operationFunc toCallFunction = getOperation(expression[operatorIndex]);
	expression[operatorIndex] = '\0';
	float firstOperand = calculateExpression(expression);
	float secondOperand = calculateExpression(expression+operatorIndex+1);
	
	return (*toCallFunction)(firstOperand, secondOperand);
}



int main(int argc, char* argv[]){
	if(argc != 2){
		fprintf(stderr, "Provide the expression as an argument with no spaces\n");
		return 1;
	}
	char* expression = malloc(strlen(argv[1])*sizeof(char));
	strcpy(expression, argv[1]);
	float result = calculateExpression(expression);
	printf("Result: %f\n", result);

	return 0;
}
