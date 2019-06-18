#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<locale.h>

#define BUFF_SIZE 128

typedef int bool;
#define false 0
#define true 1


int *orderOperations(char input_str[], int n);
int performMath(int num1, int num2, char operation);
int readStr(char str[], int n);
int strToNum(char input_str[]);
int xassert(bool trueCondition, char *s1, char *s2, char *s3);


void main() 
{
 setlocale(0, "rus");
 char str[BUFF_SIZE];
 printf("Hello, waiting for input --> ");
 int slen;
 slen = readStr(str, BUFF_SIZE);
 int i = 0;
 printf("%d\n", slen);
 for(i; i < slen + 1; i++){
 printf("%s", str[i]);
 }
 /*while (str[i != slen){
  printf("%s", str[i++]);
 }
 */
 printf("\n");
}
/*
 printf("2 + 36q2 = %d\n", performMath(strToNum("2"), strToNum("36q2"), '+'));
 printf("2 * 23 = %d\n", performMath(2, strToNum("23"), '*'));
 printf("2034 - 3 = %d\n", performMath(strToNum("2034"), 3, '-'));
 printf("2 / 3 = %d\n", performMath(2, 3, '/'));
 printf("2 ^ 3 = %d\n", performMath(2, 3, '^'));
 printf("2 + 3 = %d\n", performMath(2, 3, '+'));

 int *p;
 int i;
 char input_str = "2 * 4 + 5 - 2 / 4";
 p = orderOperations(input_str, strlen(input_str)); 
 
   for (i = 0; i < strlen(input_str); i++) {
   printf("2 * 4 + 5 - 2 / 4 ; operations order: %d\n", *p); 
 }
 */
 int readStr(char str[], int n)
 {
  char ch;
  int i = 0;
  while ((ch = getchar()) != '\n'){
    if (i < n){
      str[i++] = ch;
    }
  }
  str[i] = '\0';
  return i;
}

int performMath(int num1, int num2, char operation)
{
	switch (operation) {
		case '+': return num1 + num2;  break;
		case '-': return num1 - num2;  break;
		case '*': return num1 * num2;  break;
		case '/': return num1 / num2;  break;
		default : 
   {
    char s[2];
    s[0] = operation;
    s[1] = '\0';
    xassert(false, " wrong mathematical operation \"", s, "\"");  
    break;
   }
	}
}

int strToNum(char input_str[])
{
 int res = 0;
 int i = 0;
	while(input_str[i] - '0' >= 0 &&
       input_str[i] - '0' <= 9){
		res = res * 10 + input_str[i++] - '0'; 
	}
	return res;
}

int xassert(bool trueCondition, 
        char *s1, 
        char *s2, 
        char *s3)
{
 if ( ! trueCondition ){
  printf("Error:%s%s%s\n", s1, s2, s3);
  exit(3);
 }
}

int * orderOperations(char input_str[], int n) 
{
 int ord_opers[n];	
 int i = 0;
 for (i = -(n-1); input_str[abs(i)] != "\0"; i++) {
   if ( (i > 0) && (input_str[abs(i)] == '/' || input_str[abs(i)] == '*') )
     ord_opers[abs(i)] = abs(i);   
   else if ( (i < 0) && (input_str[abs(i)] == '+') || (input_str[abs(i)] == '-') )
     ord_opers[abs(i)] == abs(i);
 }
 return ord_opers;
}	

