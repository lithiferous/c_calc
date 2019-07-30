#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<locale.h>

#define BUFF_SIZE 128
#define WORDS_MAX 3

#define separators " ,."
#define operations "+-*/"
#define filename "calc.dat"

typedef int bool;
#define false 0
#define true 1

int getNum(char **src);
char getOperation(char **src);
int doOperation(int num1, int num2, char operation);
int getStreamResult(char *src);

bool haschar(char *str, char ch);
char *getLine(FILE *fp, char *src, bool *isEnd_file, int SIZE_BUFF);
char *writeResult(FILE *fp, char *dst, int result);
int xassert(bool trueCondition, char *s1, char *s2, char *s3);

void main() 
{
  setlocale(0, "rus");
  char str[BUFF_SIZE];
  FILE *fp = fopen(filename, "r");
  printf("Hello, this program works with file streams\n");
  {
   bool endFile = false;
   for(;;){
    getLine(fp, str, &endFile, BUFF_SIZE);
    if (!endFile) {
      getStreamResult(str);
    } else {
      break;
    }
   }
  }
}

char *getWord(char *src, 
              char *dst) 
{
 while(haschar(separators, *src)){
    src++;
 }
 while(*src != '\0' &&
       ! haschar(separators, *src)){
    *dst++ = *src++;
 }
 *dst = '\0';
 return src;
}

char *getLine(FILE *fp,
              char *src,
              bool *isEnd_file,
              int SIZE_BUFF)
{
 *isEnd_file = false;
 char ch;
 int i = 0;
 while ( i < SIZE_BUFF - 1 ) {
   ch = getc(fp);
   *src = ch;
   if (ch == EOF || ch == '\n'){ 
    *isEnd_file = ch == EOF;
    break;
   }
   src++;
 }
 *src = '\0';
 return src;
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

bool haschar(char *str, char ch)
{
 while( *str != '\0' ){
  if ( *(str++) == ch  )
    return true;
 }
 return false;
}

char getOperation(char **src)
{
 while(!haschar(operations, **src)){
   (*src)++;
 }
 char oper = *(*src)++;
 return oper;
}

int getNum(char **src)
{
  while(haschar(separators, **src)){
    (*src)++;
  }
  int Num = 0; 
  while(( **src != '\0') &&
       ( isdigit(**src) )){
	  Num = Num * 10 + *(*src)++ - '0'; 
  }
  return Num;
}

int doOperation(int num1, int num2, char operation)
{
	switch (operation) {
		case '+': return num1 + num2;  break;
		case '-': return num1 - num2;  break;
		case '*': return num1 * num2;  break;
		case '/': return num1 / num2; break;
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

int getStreamResult(char *src)
{ 
  char prior_opers[] = "*/";
  int num1;
  while(*src != '\0'){
    num1 = getNum(&src);
    char op = getOperation(&src);
    int num2 = getNum(&src);
    while(haschar(separators, *src)){
      src++;
    }
    char op2 = getOperation(src);
    if (haschar(prior_opers, op2)){
      op2 = getOperation(&src);
      while (haschar(prior_opers, op2)){
        int num3 = getNum(&src);
        num2 = doOperation(num2, num3, op2);
        op2 = getOperation(&src);
      }
      num1 = num1 + num2;
    } 
    printf("%d %c %d = ", num1, op, num2);
    num1 = doOperation(num1, num2, op); 
    printf("%d\n", num1);
    while(haschar(separators, *src)){
      src++;
    }
  }
}
/*
  char prior_opers[] = "/";
  int num1 = getNum(&src);
  int num2;
  while(*src != '\0'){
    char op = getOperation(&src);
    if (haschar(prior_opers, op)){
      src--;
      int tmp_res = num1;
      num1 = 0;
      while(!haschar(prior_opers, op)){
        op = getOperation(&src);
        num2 = getNum(&src);
        num1 = doOperation(num1, num2, op); 
      }

    }
*/
