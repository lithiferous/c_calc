#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<math.h>
#include<locale.h>

#define BUFF_SIZE 128
#define MBUFF_SIZE BUFF_SIZE / 2 + 1
#define WORDS_MAX 3
#define N_SIGN_DGTS 6

#define separators " ,."
#define operations "+-/*^()"
#define filename "calc.dat"


typedef int bool;
#define false 0
#define true 1

//specific
bool isNeg(char **);
int countTotalOpers(char*);
float getPow(float, float);
float getNum(char**);
float doOperation(float, float, char);
float getResult(char*);
float solveRec(char*, int, float*, int, int, float*);
void getNums(char *, float*, bool);
void getOpers(char*, char*, bool);

//general functions
bool haschar(char*, char);
bool getLine(FILE*, char*);
int xassert(bool, char*, char*, char*);
void mvPtrFwd(char**, char*);

void main() 
{
  char str[BUFF_SIZE];
  FILE *fp = fopen(filename, "r");
  printf("Hello, this program works with file streams:\n");
  while(getLine(fp, &str)){
printf("-> %s\n", str);
    float res = getResult(str);
    //int res = getResult(str);
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

bool isNeg(char **src)
{
  if (**src != '\0' && **src =='-'){
    (*src)++;
    return true;
  }
  return false;
}

void getOpers(char *src, 
              char *dst_opers,
              bool neg)
{ 
 mvPtrFwd(&src, separators);
 if (neg)
  mvPtrFwd(&src, "-");
 while (*src != '\0'){
  if(haschar(operations,*src))    
    *dst_opers++ = *src++;
  else if(haschar("()., ", *src) ||
          isdigit(*src))
    *src++;
  else{
    char s[2]; s[0] = *src; s[1] = '\0';
    xassert(false, "wrong math operation", s, "!");
  }
 }
 *dst_opers = '\0';
}

int countTotalOpers(char *src)
{
  int cnt = 0;
  while(*src != '\0'){
    if (!haschar(")", *src++))
      cnt++;
  }
  return cnt;
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

void getNums(char *src, 
             float *num,
             bool neg)
{
  if(neg){
    *num++ = - getNum(&src);
    mvPtrFwd(&src, " +-/*()^");
  }
  while(*src != '\0'){
    *num++ = getNum(&src);
    mvPtrFwd(&src, " +-/*()^");
  }
}

float getPow(float num, float n)
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

float solveRec(char* opers,
               int curoper,
               float* nums, 
               int curnum, 
               int n,
               float* res)
{ 
 if (n==1){
    *res = doOperation(nums[curnum], nums[curnum+1], opers[curoper]); 
    return n;
 }
  return solveRec(opers, ++curoper, nums, ++curnum, --n, res);
}

float getResult(char *src)
{
  char opers[MBUFF_SIZE];  
  float nums[MBUFF_SIZE];

  bool neg = isNeg(&src);
  getOpers(src, opers, neg);
  getNums(src, nums, neg);
  int cnt = countTotalOpers(opers);
  int *res = 0;
  nums[0] = solveRec(opers, 0, nums, 0, cnt, res);
  printf("%.6f\n", *res);
  return nums[0];
}