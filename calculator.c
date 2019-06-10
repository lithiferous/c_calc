#include<stdio.h>
#include<stdlib.h> 
#include<string.h>

#define BUFF_STR 128

void order_seq(char input_arr[], int n);
void perform_arithmetics(int num1, int num2, int operation); 

void main() {
	char input_str[BUFF_STR];
	printf("Enter operation: ");
	fgets(input_str, BUFF_STR, stdin);
	int slen = strlen(s) - 1;

	int ordered_opers[slen];
	ordered_opers = order_seq(input_str, slen);
	
	
	}

void order_seq(char input_arr[], int n) {
	int inp_digs[n];
	int opers[n-1];

	int i = 0;
	while(i < n) {
		if( (i % 2) == 0 )
			inp_digs[i] = input_arr[i] - '0';
		else
			opers[i] = input_arr[i];
		printf("%d\n", input_arr[i]0');
		i++;
	}
	
	int ord_opers[n-1];
	int j;
	for (i = -(n-1), j = 0; i < n - 1, j < n - 1; i++, j++) {
		if ( (i < 0) && (opers[abs(i)] == '+') || (opers[abs(i)] == '-') )
			ord_opers[abs(i)] == j;
		else if ( (i > 0) && (opers[abs(i)] == '/' || opers[abs(i)] == '*') )
			ord_opers[abs(i)] == j;
		else if ( (i == 0) && (opers[abs(i)] == '/' || opers[abs(i)] == '*') )
			ord_opers[abs(i)] == j;
		else if ( (i == 0) && (opers[abs(i)] == '+' || opers[abs(i)] == '-') )
			ord_opers[abs(i)] == j;
	}
	for (j = 0; j < n - 1; j++) {
		printf("%d", ord_opers[j]);
	}
}	

int perform_arithmetics(int num1, int num2, int operation) {
	switch (operation) {
		case '+': printf("sum");  return num1 + num2;  break;
		case '-': printf("diff"); return num1 - num2;  break;
		case '*': printf("mult"); return num1 * num2;  break;
		case '/': printf("div");  return num1 / num2;  break;
		default : printf("err");  return 99999999999;  break;
	}
}
