#include<stdio.h>
#include<stdlib.h>

union word
{
  int a;
  char b;
}c;

int check_CPU(void)
{
  c.a = 1;
  return (c.a ==1);
}

int main(void)
{
  int i;
  i = check_CPU();
 if(i == 0)
 {
   printf("大端存储\n");
 }
 else{
   printf("小端存储\n");
 }


}
