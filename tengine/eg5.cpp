#include<iostream>
#include<sys/stat.h>
using namespace std;
int main()
{
struct stat attributes;
stat("test1.chtml",&attributes);
printf("%u\n",attributes.st_mtime);
return 0;
}