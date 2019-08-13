#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<math.h>
#include<locale.h>

#define BUFF_SIZE 128
#define WORDS_MAX 3

#define separators " "
#define floatseparators ".,"
#define operations "+-/*^"
#define filename "calc.dat"

typedef int bool;
#define false 0
#define true 1

//specific
bool isNeg(char **);
bool solveBrackets(char []);
char getOper(char **, char *);
float getNum(char **);
int getPow(int, int);
float doOperation(float, float, char);
float getResult(char *);

//general functions
bool haschar(char *, char);
char *getLine(FILE *, char *, bool *, int);
int xassert(bool , char *, char *, char *);
void mvPtrBwd(char **, char *);
void mvPtrFwd(char **, char *);

void main() 
{
  char str[BUFF_SIZE];
  FILE *fp = fopen(filename, "r");
  printf("Hello, this program works with file streams:\n");
  {
   bool endFile = false;
   for(;;){
    getLine(fp, str, &endFile, BUFF_SIZE);
    if (endFile) {
      break;
    }
    while(solveBrackets(str));
printf("-> %s\n", str);
      float res = getResult(str);
      //int res = getResult(str);
printf("Your result: %.6f\n", res);
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
  
 while( *str != '\0'){
  if ( *(str++) == ch  )
    return true;
 }
 return false;
}

void mvPtrFwd(char **src, char *str)
{
 while(haschar(str, **src)){
         (*src)++;
    }
}

void mvPtrBwd(char **src, char *str)
{
  while(haschar(str, **src)){
    (*src)--;
  }
}

char getOper(char **src, char *opers)
{ 
 mvPtrFwd(src, separators);
 if (**src != '\0'){    
  char oper = *(*src)++;
  return oper;
 }
 return '\0';
} // update haschar opers?

bool isNeg(char **src)
{
  if (**src != '\0' && **src =='-'){
    (*src)++;
    return true;
  }
  return false;
}

float getNum(char **src)
{
  mvPtrFwd(src, separators);
  bool neg = isNeg(src);
  mvPtrFwd(src, separators);
  float Num = 0; 
  while(isdigit(**src)){
	  Num = Num * 10 + *(*src)++ - '0'; 
  }
  float divisor = 10;
  if (haschar(floatseparators, **src)){
    *(*src)++;
    while(isdigit(**src)){
      Num += (*(*src)++ - '0') / divisor;
      divisor *= 10;
    }
  }
  if (neg)
    return -Num;
  else 
    return Num;
}


bool solveBrackets(char src[BUFF_SIZE])
{ 
  if(haschar(src, '(')){
printf("-> %s\n", src);
    int i = 0;
    while(src[i] != '\0'){
      i++;
    }
    while(src[i] != '('){
      i--;
    }
    int j = ++i;
    while(src[i] != ')'){
      i++;
    }
    int num = getResult(&src[j]);
    do{
      int divisor = 10;
      src[i--] = num % divisor + '0';
      num /= divisor;
    }while( num != 0 );
    
    do {
      src[i--] = ' ';
    } while(i != j - 2);
    return true;
  }else{
    return false;
  }
}

int getPow(int num, int n)
{
  if (n <= 1){
    return num;
  }
  return num*getPow(num, n - 1);
}

float doOperation(float num1, 
                  float num2, 
                  char operation)
{
	switch (operation) {
		case '+': return num1 + num2;  break;
		case '-': return num1 - num2;  break;
		case '*': return num1 * num2;  break;
		case '/': 
    xassert(num2!=0, " division by zero", "", ""); 
    return num1 / num2; break;
    case '^': return getPow(num1, num2); break;
    case '(': return num1 + num2;  break;
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

float getResult(char *src)
{
  float num1 = getNum(&src);
  while(*src != '\0' && *src != ')'){
    char op = getOper(&src, "+-/*^");
    float num2 = getNum(&src);
    mvPtrFwd(&src, separators);
    if (*src != '\0' && *src != ')'){
      for(;;){
        char op2 = getOper(&src, operations);
        if ( ! haschar("/*^", op2)){
          mvPtrBwd(&src, separators);
          break;
        }
        float num3 = getNum(&src);
printf("\t%.6f %c %.6f = ", num2, op2, num3);
        num2 = doOperation(num2, num3, op2);
printf("%.6f\n", num2);
      }
    }  
printf("\t%.6f %c %.6f = ", num1, op, num2);
    num1 = doOperation(num1, num2, op); 
printf("%.6f\n", num1);
    mvPtrFwd(&src, separators);
  }
  return num1;
}