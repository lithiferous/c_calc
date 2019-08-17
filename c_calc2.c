#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<math.h>
#include<locale.h>

#define BUFF_SIZE 128
#define N_SIGN_DGTS 6

#define separators " "
#define floatseparators ".,"
#define operations "+-/*^"
#define filename "calc.dat"

typedef int bool;
#define false 0
#define true 1

//specific
bool isNeg(char **);
char getOper(char**, char*);
float getPow(float, int);
float getNum(char**);
float getResult(char *src);
float doOper(float, float, char);
float getExpression(char*);
void solveBrackets(char*);

//general functions
bool haschar(char*, char);
bool getLine(FILE*, char*);
int xassert(bool, char*, char*, char*);
void mvPtrBwd(char**, char*);
void mvPtrFwd(char**, char*);

void main() 
{
  char str[BUFF_SIZE];
  FILE *fp = fopen(filename, "r");
  printf("Hello, this program works with file streams:\n");
  while(getLine(fp, &str)){
    float res = getResult(str);
printf("Your result: %.6f\n", res);
  } 
}

bool getLine(FILE *fp, char *src)
{
 char ch;
 int i = 0;
 while( i < BUFF_SIZE - 1 ){
   ch = getc(fp);
   if (ch == '\n' || ch == EOF)
    break;
   *src++ = ch;
 }
 *src = '\0';
 if (ch == '\n')
  return true;
 return false;
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

void reverseStr(char *src, int len)
{
  int i = 0, j = len;
  while(i <= j){
    char tmp;
    tmp = *(src+i);
    *(src + i++) = *(src + j);
    *(src + j--) = tmp;
  }
  int diff = i - j;
  if(diff > 0){
    while(diff){
      *(src+--diff) = ' ';
    }
  }
}

char getOper(char **src, char *opers)
{ 
 mvPtrFwd(src, separators);
 if(haschar(operations, **src))
  return *(*src)++;
 return '\0';
}

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

float getPow(float num, int n)
{
  if (n <= 1){
    return num;
  }
  return num*getPow(num, n - 1);
}

float doOper(float num1, 
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
    case '(': return num1 * num2;  break;
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

int strlenRev(char *src)
{
  int counter = 0;
  while(*src != '\0'){
    counter++;
    src--;
  }
  return --counter;
}

void shiftStr(char *src,
              int n,
              int maxlen,
              int bracklen)
{ 
  xassert(maxlen + n < BUFF_SIZE - 1, \
          "string buffer exceeded\n", "", "");
  
  do{
     *(src + bracklen + n) = *(src + bracklen);
  }while(bracklen-- != 0);

  while(n){
    *(src + n--) = ' '; 
  }
}

float solveSeq(char **src,
               char *actual_opers,
               char op,
               float num1)
{
  for(;;){
    if ( ! haschar(actual_opers, op)){
      break;
    }
    float num2 = getNum(src);
    num1 = doOper(num1, num2, op);
    op = getOper(src, operations);
  }
  return num1;
}

float getExpression(char *src)
{
  float num1 = getNum(&src);
  while(*src != '\0' && *src != ')'){
    char op = getOper(&src, operations);
    float num2 = getNum(&src);
    char op2 = getOper(&src, operations);
    if(haschar("^", op2)){
      num2 = solveSeq(&src, "^", op2, num2);
    } else if(haschar("*/", op2)) {
      num2 = solveSeq(&src, "*/", op2, num2);
    }
    num1 = doOper(num1, num2, op);
  }
  return num1;
}

void getArrSum(char *src)
{
  float num1 = getNum(&src);
  while(*src != '\0'){
    char op2 = getOper(&src, "+-");
    float num2 = getNum(&src);
    num1 = doOper(num1, num2, op2);
    src++;
  }
  return num1;
}

float getResult(char *src)
{
  float res;
printf("-> %s\n", src);
  if( ! haschar(src, '(') )
    res = getExpression(&src[0]);
  else {
    solveBrackets(src);
    res = getExpression(&src[0]);
  }
  return res;
} 

void putNum(float num,
            char *src,
            int maxlen,
            int bracklen)
{
  bool neg = num < 0;
  char cNum[BUFF_SIZE];
  int i = 0;
  cNum[i++] = '\0';
  int signPart = N_SIGN_DGTS;
  num *= getPow(10, signPart);
  while (signPart){
    cNum[i++] = (int) num % 10 + '0';
    num /= 10;
    signPart--;
  }
  cNum[i++] = '.';
  if(neg)
    num *= -1;
  while((int) num!=0){
    cNum[i++] = ((int) num % 10) + '0';
    num /= 10;
  }
  if(neg)
    cNum[i++] = '-';
  int nchars = i;
  int diff = bracklen - nchars;
  if (diff >= 0){
    reverseStr(&cNum[0], nchars);
    strcpy(&src[0], &cNum);
    do{
      src[diff] = ' ';
    }while(diff-- > 0);
  } else {
    shiftStr(&src[0], -diff, maxlen, strlen(&src[0]));
    reverseStr(&cNum[0], nchars);
    strcpy(&cNum[nchars], &src[bracklen]);
    strcpy(&src[0], &cNum);
  }
}

void solveBrackets(char *src)
{ 
  int maxlen = strlen(src);
  while(haschar(src, '(')){
    int i = 0;
    while(src[i] != '\0'){
      i++;
    }
    while(src[i] != '('){
      i--;
    }
    src[i] = ' ';
    int j = i;
    while(src[j] != ')'){
      j++;
    }
    float num = getExpression(&src[i]);
    src[j++] = ' ';
    putNum(num, &src[i], maxlen, j-i);
  }
}
