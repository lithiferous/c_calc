#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<locale.h>

#define BUFF_SIZE 128
#define WORDS_MAX 3

#define separators " ,."
#define operations "+-/*"
#define filename "calc.dat"

typedef int bool;
#define false 0
#define true 1

int getNum(char **src);
char getOper(char *src, char *opers);
char getOperAlias(char **src, char *opers);
int doOperation(int num1, int num2, char operation);
int getStreamResult(char *src, char *curOpers);

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
      int res = getStreamResult(str, "+-");
      printf("Your result: %d", res);
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

char getOper(char *src, char *opers)
{
 while(!haschar(opers, *src)){
   src++;
 }
 char oper = *src++;
 return oper;
}

char getOperAlias(char **src, char *opers)
{
 while(!haschar(opers, **src)){
   (*src)++;
 }
 if (**src != '\0'){
  char oper = *(*src)++;
  return oper;
 }
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

int getStreamResult(char *src, char *curOpers)
{
  int num1 = getNum(&src);
  while(*src != '\0'){
    char op = getOperAlias(&src, curOpers);
    int num2 = getNum(&src);
    while(haschar(separators, *src)){
         src++;
    }
    if (*src != '\0'){
      char op2 = getOperAlias(&src, "/*");
      if (haschar("*/", op2)){
        while(haschar("/*", op2)){
            int num3 = getNum(&src);
            printf("%d %c %d = ", num2, op2, num3);
            num2 = doOperation(num2, num3, op2);
            printf("%d\n", num2);
            op2 = getOperAlias(&src, operations);
            while(haschar(separators, *src)){
              src++;
            }
        }
        while(!haschar(curOpers, *src))
          src--;
      }
    }
    printf("%d %c %d = ", num1, op, num2);
    num1 = doOperation(num1, num2, op); 
    printf("%d\n", num1);
    while(haschar(separators, *src)){
         src++;
    }
  }
  return num1;
}

int getResult(char *src)
{
  while(*src != '\0'){
    getStreamResult;
  }
}
