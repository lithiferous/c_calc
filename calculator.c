#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<locale.h>

#define BUFF_SIZE 128
#define MAX_WORDS 100


typedef int bool;
#define false 0
#define true 1


int *orderOperations(char input_str[], int n);
int performMath(int num1, int num2, char operation);
int readLine(char str[], int n);
int readLines(char str[], int n);
int splitWords(char str[], char str_split[MAX_WORDS][BUFF_SIZE]);
int strToNum(char input_str[]);
int xassert(bool trueCondition, char *s1, char *s2, char *s3);


void main() 
{

 setlocale(0, "rus");
 char str[BUFF_SIZE];
 printf("Hello, waiting for input --> ");
 
 int slen;
 slen = readLines(str, BUFF_SIZE);
 char text[LINES][MAX_WORDS
}
/* 
 slen = readLine(str, BUFF_SIZE);
 char words[MAX_WORDS][BUFF_SIZE];
 int iWordsNum;
 iWordsNum = splitWords(str, words);
 int i = 0;
 while (i < iWordsNum ){
  printf("%s\n", words[i++]);
 }
*/

 int readLines(char str[], int n)
{
  char ch;
  int i = 0;
  while ((ch = getchar()) != EOF){
    if (i < n-1){
      str[i++] = ch;
    }
  }
  str[i] = '\0';
  return i;
}

 int readLine(char str[], int n)
{
  char ch;
  int i = 0;
  while ((ch = getchar()) != '\n'){
    if (i < n-1){
      str[i++] = ch;
    }
  }
  str[i] = '\0';
  return i;
}

int splitWords(char str[], char str_split[MAX_WORDS][BUFF_SIZE])
{
 int i = 0, j = 0, n_words = 0;
 char word[BUFF_SIZE];
 do{
  if (str[i] < 33 || str[i] > 127 || str[i] == '\0'){
   word[j] = '\0';
   strcpy(str_split[n_words++], word); 
   j = 0;
  } else {
   word[j++] = str[i]; 
  }
  i++;
 }while (str[i-1] != '\0');
 return n_words;
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

/*
int splitWords(char str[], char str_split[MAX_WORDS][BUFF_SIZE])
{
 int i = 0, j = 0, n_words = 0;
 char word[BUFF_SIZE];
 for(;;){
  if (str[i] == ' ' && str[i+1] != ' ' || str[i] == '\0'){
   word[j] = '\0';
   strcpy(str_split[n_words++], word); 
   printf("%s\n", word);
   if ( str[i] == '\0' ){
    break;
   }
   j = 0;
  } else {
   word[j++] = str[i]; 
  }
  i++;
 }
 return n_words;
}
*/

