#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;
int main()
{
char *start,*end,*ptr;
int x,toRead;
FILE *f;
char buffer[513];
long fileLength;
f=fopen("test1.chtml","r");
if(f==NULL)
{
cout<<"Incorrect File Name or File Does Not Exist"<<endl;
return 0;
}
fseek(f,0,SEEK_END);
fileLength=ftell(f);
rewind(f);
x=0;
while(1)
{
if((fileLength-x)>256) toRead=256;
else toRead=fileLength-x;
fread(buffer,toRead,1,f);
buffer[toRead]='\0';
//cout<<"To Read -> "<<toRead<<" bytes"<<endl;
//cout<<"Content Read -> "<<buffer<<endl;
x=x+toRead;
start=buffer;
while(1)
{
start=strstr(start,"${");
if(start==NULL) break;
end=strstr(start+2,"}");
if(end!=NULL)
{
for(ptr=start+1;ptr<end;ptr++) if(*ptr=='$' && *(ptr+1)=='{') break;
if(*ptr=='$') start=ptr;
for(ptr=start;ptr<=end;ptr++) printf("%c",*ptr);
cout<<endl;
start=end+1;
continue;
}
//if not found then load next 256 bytes chunk
//and store it in buffer from index 256
if(x==fileLength) break;
if((fileLength-x)>256) toRead=256;
else toRead=fileLength-x;
fread(buffer+256,toRead,1,f);
buffer[256+toRead]='\0';
x=x+toRead;
end=strstr(buffer+256,"}");
if(end==NULL) break;
for(ptr=start+1;ptr<end;ptr++) if(*ptr=='$' && *(ptr+1)=='{') break;
if(*ptr=='$') start=ptr;
for(ptr=start;ptr<=end;ptr++) printf("%c",*ptr);
cout<<endl;
strcpy(buffer,buffer+256);
end=end-256;
start=end+1;
}
if(x==fileLength) break;
}
fclose(f);
return 0;
}