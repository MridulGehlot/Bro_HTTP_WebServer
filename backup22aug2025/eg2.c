//range 0-255
#include<stdio.h>
int main()
{
unsigned char m=127;
m++;
printf("%d\n",m==128);
m++;
printf("%d\n",m==129);
printf("range 0-255");
return 0;
}