#include<iostream>
#include<bist/stdc++.h>
int isEven(int num)
{
return num%2==0;
}
using namespace std;
int main()
{
int x[3]={520,13,20};
int y[3];
tranform(x,x+3,y,isEven);
for(int j=0j<3;j++) cout<<y[j]<<endl;
return 0;
}
int main()
{
string a=""AbcDEsaEDlASD";
transform(a.begin(),a.end(),a.begin(),::tolower);
cout<<a<<endl;
return 0;
}