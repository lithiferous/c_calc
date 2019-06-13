#include<stdio.h>
#include<stdlib.h> 
#include<string.h>

#define BUFF_SIZE 128

void main() {
	char input_str[BUFF_SIZE];
	printf("Enter operation: ");
	fgets(input_str, BUFF_SIZE, stdin);
	
	int opers_size = 10, nums_size = 2 * opers_size;
	int *oper_p = NULL, *num_p = NULL;

	int *numbers = malloc(opers_size * sizeof(*numbers));
	int *operations = malloc(opers_size * sizeof(*operations));

	if(numbers == NULL) {
		printf("error allocating mem\n");
		exit(1);
	}{
	int i = 0, j = 0, k = 0;
	while (input_str[i] != '\n') {
		if (input_str[i] == '+' || input_str[i] == '-' || input_str[i] == '/' || input_str[i] == '*') {
			operations[j] = input_str[i];
			printf("%d) oper: %d at pos %d\n", i, operations[j], j);
			j++, i++;
		} else { 
			int res = 0;
			while(input_str[i] - '0' >= 0 && input_str[i] - '0' <= 9) {
				res = input_str[i++] - '0' + res * 10; 
				numbers[k] = res;
			}
			printf("%d) num: %d at pos %d\n", i, numbers[k], k);
			k++;
		}
	}
	printf("old size num: %d oper: %d\n", sizeof(operations)/sizeof(int), sizeof(numbers)/sizeof(int));
	int *tmp_num, *tmp_oper;
	tmp_num = realloc(numbers, k * sizeof(int));
	tmp_oper = realloc(operations, j * sizeof(int));
	if(tmp_num == NULL || tmp_oper == NULL) {
		printf("error allocating mem\n");
		exit(1);
	}
	numbers = tmp_num;
	operations = tmp_oper;
	printf("val for k: %d, j: %d\n", k, j);
	}
	printf("new size num: %d oper: %d\n", sizeof(operations)/sizeof(int), sizeof(numbers)/sizeof(int));
	long unsigned int i = 0;
	for(i; i <= sizeof(numbers)/sizeof(int) ; i++) {
		printf("%d) num: %d, oper: %d\n", i, numbers[i], operations[i]);
		/*if (i < sizeof(operations)/sizeof(int) - 1) {
			printf("%d) num: %d, oper: %d\n", i, numbers[i], operations[i]);
		} else {
			printf("%d) num: %d, oper: empty\n", i, numbers[i]);
		}*/	
	}
}
/*	
	int operations[2][BUFF_SIZE];
	int i = 0, j = 0;
	while (input_str[i] != '\n') {
		if (input_str[i] == '+' || input_str[i] == '-' || input_str[i] == '/' || input_str[i] == '*') {
			operations[0][j] = i; 
			operations[1][j++] = input_str[i];
			printf("Your num: %d at pos %d\n", input_str[i], i);
		}
		i++;
	}
	
	printf(" sizeof %d\n", sizeof(operations[0])/sizeof(operations[0][0]));
	i = 0, j = 0;
	int cur_pos = 0, end_pos;
	int oper_size = sizeof(operations)/sizeof(operations[0]);
	while (i <= oper_size + 1) {
		if (i == oper_size + 1) {
			numbers[i] = get_number(input_str, oper_size + 1, input_size);
		} else if (i == 0) {
			numbers[i] = get_number(input_str, 0, operations[][0]);
		} else {
			numbers[i] = get_number(input_str, operations[i][j], operations[i+1][j]);
		}
		i++;
		j++;
	int oper_size = (strlen(s) - 1) / 2;
	int *operations = NULL;
	operations = malloc(oper_size + sizeof(int));


    	int numbers[slen];
	int i =0, j = 0;
	while (input_str[i] != slen) {
		int res = 0;
		while (input_str[j] - '0' >= 0 && input_str[j] - '0' <= 9) {
			res += (input_str[j] - '0') * 10;
			j++;}	
		j += 1;
		numbers[i] == res;
		i++;
	}	
*/	

/*
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
int perform_math(int num1, int num2, int operation) {
	switch (operation) {
		case '+': printf("sum");  return num1 + num2;  break;
		case '-': printf("diff"); return num1 - num2;  break;
		case '*': printf("mult"); return num1 * num2;  break;
		case '/': printf("div");  return num1 / num2;  break;
		default : printf("err");  return 99999999999;  break;
	}
}*/
