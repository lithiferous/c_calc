#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<math.h>
#include<locale.h>

#define BUFF_SIZE 128
#define MBUFF_SIZE BUFF_SIZE / 2 + 1

#define separators " ,."
#define operations "+-/*^()"
#define filename "calc.dat"

typedef int bool;
#define false 0
#define true 1

//specific
bool isNeg(char *);
float getNum(char**);
float getPow(float, float);
float getResult(char*);
float doOper(float, float, char);
void getBrackets(char*, char*, float*, int);

//general functions
bool haschar(char*, char);
bool getLine(FILE*, char*);
int xassert(bool, char*, char*, char*);
void mvPtrFwd(char**, char*);

void main() 
{
  char str[BUFF_SIZE];
  FILE *fp = fopen(filename, "r");
printf("Hello, welcome to c-calculator:\n \"c\" stands for crazy!\n");
  while(getLine(fp, &str)){
printf("-> %s\n", str);
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

float getNum(char** src)
{
  mvPtrFwd(src, " +-/*()^");
  float num = 0; 
  while(isdigit(**src)){
	  num = num * 10 + *(*src)++ - '0'; 
  }
  float divisor = 10;
  if (haschar(".,", **src)){
    *(*src)++;
    while(isdigit(**src)){
      num += (*(*src)++ - '0') / divisor;
      divisor *= 10;
    }
  }
  return num;
}

bool isNeg(char *src)
{
  mvPtrFwd(&src, separators);
  if (*src != '\0' && *src =='-')
    return true;
  return false;
}

void putExpression(float *dst_nums,
                   char *dst_opers, 
                   char *src)
{
  if(isNeg(src))
    *dst_nums++ = 0;
  while(*src != '\0'){
    if (haschar("(", *src) && isNeg(src+1))
      *dst_nums++ = 0, *dst_opers++ = *src++;
    else if(haschar(operations,*src))    
      *dst_opers++ = *src++;
    else if(haschar(".", *src) || isdigit(*src))
      *dst_nums++ = getNum(&src);
    else if(haschar(separators, *src))
      src++;
    else {
      char s[2]; s[0] = *src; s[1] = '\0';
      xassert(false, "symbol not recognised: \"", s, "\"\n");
    }
 }
 *dst_opers = '\0';
}

float getPow(float num, float n)
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

void solveSeq(char *opers,
              char *actual_opers,
              float *nums, 
              int max_opers)
{
  bool resNeg = false;
  while(max_opers){
    if (haschar(actual_opers, *(opers))){
      int i = 1;
      while(*(nums+i) == -1){i++;}
      float tmp;
      if(resNeg)
        tmp = doOper(-(*nums), *(nums+i), *opers);
      else 
        tmp = doOper(*nums, *(nums+i), *opers);
      if (tmp < 0){
        *(nums+i) = -tmp;
        resNeg = true;
      } else {
        *(nums+i) = tmp;
        resNeg = false;
      }
      *opers = ' ';
      *nums = -1;      
      opers++,nums++,max_opers--;
    } else{
      opers++,nums++,max_opers--;
    }
  }
}

void getExpression(char *opers,
                   float *nums,
                   int max_opers)
{
  solveSeq(opers, "^", nums, max_opers);
  solveSeq(opers, "/*", nums, max_opers);    
  solveSeq(opers, "+-", nums, max_opers);   
}

float getResult(char *src)
{
  char opers[MBUFF_SIZE];  
  float nums[MBUFF_SIZE];

  putExpression(nums, opers, src);
  int max_opers = strlen(opers);
  if (haschar(opers, '('))
    getBrackets(src, opers, nums, max_opers);
  else
    getExpression(opers, nums, max_opers);
  int i = 0;
  while(nums[i] == -1){i++;}
  return nums[i];
}

void countNum(char *src, int *cnt)
{
  int sep = 1;
  do{
    if(*src == '.'){
      src++;
      sep--;
    }
    src++;
  }while(isdigit(*src));
  xassert(sep > 0, "invalid number: ", src, "");
  (*cnt)++;
}

int getIndNum(char *src, int cntOp)
{
  int cntNum = 0;
  while(cntOp){
    if(haschar("-+/*^()", *src))
      cntOp--;
    else if(isdigit(*src))
      countNum(src, &cntNum);
    src++;
  }
  return cntNum;
}

void shiftlStr(char *src, int n)
{
  while(*src != '\0'){
    *src++ = *(src+n);
  };
}

void getBrackets(char *src,
                 char *opers,
                 float *nums,
                 int max_opers)
{ 
  int bracket = 0;
  while(haschar(opers, '(')){
    int i = 0;
    while(*(opers+i) != ')'){
      i++;
    }
    int j = --i;
    while(*(opers+i) != '('){
      i--;
    }
    int inum = getIndNum(src, i+bracket);
    shiftlStr(opers+i, 1);
    getExpression(opers+i, nums+inum, j-i);
    shiftlStr(opers+j, 1);
    bracket++, max_opers-=2;
  }
  getExpression(opers, nums, max_opers);
}
