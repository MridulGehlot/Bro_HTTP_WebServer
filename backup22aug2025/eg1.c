#include<stdio.h>
int main()
{
char m;
m=127;
printf("%d\n",m==127);
m++;
printf("%d\n",m==128);
printf("%d\n",m==-128);
m++;
printf("%d\n",m==-127);
printf("range -128-127");
return 0;
}