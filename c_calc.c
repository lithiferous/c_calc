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
float solveSeq(char *, char*, float*, int*);
float getResult(char*);
void solveBrackes(char*);
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

void solveBrackes(char*);

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

float solveSeq(char *opers,
               char *actual_opers,
               float *nums,
               int *curpos)
{
  while(*(opers+*curpos)!='\0' &&
          *(nums+*curpos+1) > 0){
    if(haschar(actual_opers, *(opers+*curpos+1))){
      *(nums+*curpos+2) = doOperation(*(nums+*curpos+1), *(nums+*curpos+2), *(opers+*curpos+1));
      *(opers+*curpos+1) = ' ';
      *(nums+*curpos+1) = -1;
    } else if(haschar(actual_opers, *(opers+*curpos))){
      *(nums+*curpos+1) = doOperation(*(nums+*curpos), *(nums+*curpos+1), *(opers+*curpos));
      *(opers+*curpos) = ' ';
      *(nums+*curpos) = -1;
    }
    (*curpos)++;
  }
}

float getResult(char *src)
{
  char opers[MBUFF_SIZE];  
  float nums[MBUFF_SIZE];

  bool neg = isNeg(&src);
  getOpers(src, opers, neg);
  getNums(src, nums, neg);

  int i; //not following \0
  if (haschar(opers, '(')){
    while(haschar(opers, '(')){
      i = 0;
      while(opers[i] != '\0' &&
            opers[i] != ')'){
        i++;
      }
      if(opers[i] != '\0')
        opers[i] = ' ';
      while(opers[i] != '\0' &&
              opers[i] != '('){
        i--;
      }
      opers[i++] = ' ';
      while(opers[i] != '\0'){//delete
        solveSeq(&opers, "^", &nums, &i);
        solveSeq(&opers, "/*", &nums, &i);
        i++;
      }
    }
  } 
  i = 0;
  while(opers[i] != '\0'){
    solveSeq(&opers, "^", &nums, &i);
    solveSeq(&opers, "/*", &nums, &i);
    i++;
  }
  
  i = 0;
  int j = 0;
  while(opers[i] != '\0'){
    if(haschar("+-", opers[i])){
      while(nums[j] < 0){j++;}  
      nums[0] = doOperation(nums[0], nums[j++], opers[i]);
    }
    i++;
  }
  printf("%.6f\n", nums[0]);
  return nums[0];
}