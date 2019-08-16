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
float getNum(char**);
float getPow(float, float);
float getResult(char*);
float doOper(float, float, char);
void getNums(char *, float*, bool);
void getOpers(char*, char*, bool);
void solveBrackets(char*, char*, float*, int);

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
              int lim)
{
  while(lim){
    if(haschar(actual_opers, *opers)){
      if(haschar(actual_opers, *(opers+1)))
        while(haschar(actual_opers, *(opers+1))){
          *(nums+1) = doOper(*nums, *(nums+1), *opers);
          *opers = ' ';
          *nums = -1;
          opers++,nums++,lim--;
        }
      if(haschar(actual_opers, *(opers))){
        *nums = doOper(*nums, *(nums+1), *opers);
        *opers = ' ';
        *(nums+1) = -1;
      }
    }
    opers++,nums++, lim--;
  }
}

void getArrSum(char **opers,
               float **nums)
{
  int j = 1;
  while(**opers != '\0'){
      if(haschar("+-", **opers)){
        while(((*nums)+j) < 0){j++;}  
        **nums = doOper(**nums, *((*nums)+j++), **opers);
      }
      (*opers)++;
    }
}

float getResult(char *src)
{
  char opers[MBUFF_SIZE];  
  float nums[MBUFF_SIZE];

  bool neg = isNeg(&src);
  getOpers(src, opers, neg);
  getNums(src, nums, neg);
  int lim = strlen(opers);
  if (haschar(opers, '('))
    solveBrackes(src, &opers, &nums, lim);
  else{
    solveSeq(&opers, "^", &nums, lim);
    solveSeq(&opers, "/*", &nums, lim);  
    getArrSum(&opers, &nums);
  }
printf("%.6f\n", nums[0]);
  return nums[0];
}

int getIndNum(char *src, 
              char *opers,
              int posOp)
{
  int curp = 0, cnt = 0;
  while(posOp){
    if(haschar("-+/*^()", *src))
      posOp--;
    if(isdigit(*(src++)))
      cnt++;
    curp++;
  }
  return cnt;
}

void solveNonSeq(char* opers, 
                 char* actual_opers,
                 float* nums,
                 int lim)
{
  while(lim){
    if(haschar(actual_opers, *opers)){
      int inum = 0;
      while(*(nums+inum) < 0){inum++;}
      int inum2 = inum + 1;
      while(*(nums+inum2) < 0){inum2++;}
      *(nums+inum) = doOper(*(nums+inum), *(nums+inum2), *opers);
      *opers = ' ';
      *(nums+inum2) = -1;
    }
    opers++,lim--;
  }
}

void solveBrackes(char *src,
                  char *opers,
                  float *nums,
                  int lim)
{ 
  while(haschar(opers, '(')){
    int i = 0;
    while(opers[i] != ')'){
      i++;
    }
    int j = i;
    opers[i--] = ' ';
    while(opers[i] != '\0' &&
            opers[i] != '('){
      i--;
    }
    opers[i++] = ' ';
    int inum = getIndNum(src, opers, i);
    solveSeq(&opers[i], "^", &nums[inum], j-i);
    solveSeq(&opers[i], "/*", &nums[inum], j-i);
    solveNonSeq(&opers[i], "+-", &nums[inum], j-i);
  }
    //solveNonSeq(&opers[0], "/*", &nums[0], lim);
  //getArrSum(&opers, &nums);
}
