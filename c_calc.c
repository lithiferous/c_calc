#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<math.h>
#include<locale.h>

#define BUFF_SIZE 128
#define WORDS_MAX 3

#define separators " ,."
#define operations "+-/*^"
#define filename "calc.dat"

typedef int bool;
#define false 0
#define true 1

//specific
bool solveBrackets(char []);
char getOper(char **, char *);
int getNum(char **);
int getPow(int, int);
int doOperation(int , int , char);
int getStreamResult(char *, char *, char *);

//general functions
bool haschar(char *, char);
bool mvPtrBwd(char **, char *);
char *getLine(FILE *, char *, bool *, int);
char *writeResult(FILE *, char *, int);
int xassert(bool , char *, char *, char *);
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
    if (!endFile) {
      while(solveBrackets(str)){
        solveBrackets(str);
      }
      printf("-> %s\n", str);
      int res = getStreamResult(str, "+-", "/*^");
      printf("Your result: %d\n", res);
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

bool mvPtrBwd(char **src, char *str)
{
 if (**src != '\0'){
  do{
          (*src)--;
    }while(haschar(str, **src));
  return haschar(str, **src);
 }
}

char getOper(char **src, char *opers)
{
 mvPtrFwd(src, separators);
 if (**src != '\0'){
  char oper = *(*src)++;
  return oper;
 }
}

int getNum(char **src)
{
  mvPtrFwd(src, separators);
  int Num = 0; 
  while(( **src != '\0') &&
       ( isdigit(**src) )){
	  Num = Num * 10 + *(*src)++ - '0'; 
  }
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
    double num = getStreamResult(&src[j], "+-", "*/^");
    while(num){
      int divisor = 10;
      double dnum = (double) num / divisor;
      double remainder = modf(dnum, &num);
      char cnum = round(remainder * divisor + 0.1) + '0';
      src[i--] = cnum;
      num = (int)dnum;
    }
    while(src[i] != '('){
      src[i--] = ' ';
    }
    src[i] = ' ';
    return true;
  }else{
    return false;
  }
}

int getPow(int num, int n)
{
  if (n == 1){
    return num;
  }
  return num*getPow(num, n - 1);
}

int doOperation(int num1, 
                int num2, 
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

int getStreamResult(char *src,
                    char *lowOpers, 
                    char *highOpers)
{
  int num1 = getNum(&src);
  while(*src != '\0' && *src != ')'){
    char op = getOper(&src, lowOpers);
    int num2 = getNum(&src);
    mvPtrFwd(&src, separators);
    if (*src != '\0' && *src != ')'){
      char op2 = getOper(&src, operations);
      if (haschar(highOpers, op2)){
        while(haschar(highOpers, op2)){
            int num3 = getNum(&src);
            printf("\t%d %c %d = ", num2, op2, num3);
            num2 = doOperation(num2, num3, op2);
            printf("%d\n", num2);
            op2 = getOper(&src, operations);
            mvPtrFwd(&src, separators);
        }
      }
      mvPtrBwd(&src, separators);
    }
    printf("\t%d %c %d = ", num1, op, num2);
    num1 = doOperation(num1, num2, op); 
    printf("%d\n", num1);
    mvPtrFwd(&src, separators);
  }
  return num1;
}