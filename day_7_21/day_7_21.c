#include<stdio.h>
#include<endian.h>

int main(void)
{
  printf("Big_endian:\t%d \n Little_endian:\t%d \n mine \t%d \n",__BIG_ENDIAN,__LITTLE_ENDIAN,BYTE_ORDER);
  return 0;
}
