#include<stdio.h>
int main()
{
char a[21]={"%E2%82%AC"};
//char a[21]={"%E1%8F%A7"};
//char a[21]={"%E2%83%87"};
char *ptr;
int numbers[4]={0,0,0,0};
char bytes[4]={0,0,0,0};
int i,j,k;
int index;
int numberOfBytes;
unsigned char m;
unsigned int unicode;
ptr=a;
ptr++;
sscanf(ptr,"%2x",&j);
if(j>=128)
{
numberOfBytes=0;
k=128;
while(1)
{
if((j&k)!=k) break;
numberOfBytes++;
k=k/2;
}
printf("Numberof bytes involved %d\n",numberOfBytes);
m=(unsigned char)j;
m=m<<(numberOfBytes+1);
m=m>>(numberOfBytes+1);
index=4-numberOfBytes;
bytes[index]=m;
numbers[index]=j;
index++;
i=2;
while(i<=numberOfBytes)
{
ptr=ptr+3;
sscanf(ptr,"%2x",&j);
m=(unsigned char)j;
m=m<<2;
m=m>>2;
bytes[index]=m;
numbers[index]=j;
index++;
i++;
}
printf("Numbers : ");
for(i=0;i<=3;i++) printf("%d ",numbers[i]);
printf("\n");
printf("Bytes : ");
for(i=0;i<=3;i++) printf("%d ",bytes[i]);
printf("\n");
printf("Preparing Unicode Part\n");
index=4-numberOfBytes;
unicode=bytes[index];
index++;
while(index<=3)
{
m=bytes[index];
m=m<<2;
unicode=unicode<<8;
unicode=unicode | m;
unicode=unicode>>2;
index++;
}
}
else unicode=j;
printf("%d\n",unicode);
return 0;
}