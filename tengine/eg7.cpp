#include<iostream>
#include<string.h>
using namespace std;
void createVMDFileName(const char *chtmlFileName,char *vmdFileName)
{
char *dotPtr;
for(;*chtmlFileName!='\0';chtmlFileName++,vmdFileName++)
{
if(*chtmlFileName=='.') dotPtr=vmdFileName;
*vmdFileName=*chtmlFileName;
}
*vmdFileName='\0';
strcpy(dotPtr+1,"vmd");
}
int main()
{
char vmdFileName[257];
createVMDFileName("whatever.chtml",vmdFileName);
cout<<vmdFileName<<endl;
return 0;
}