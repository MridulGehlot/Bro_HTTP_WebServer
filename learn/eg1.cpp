#include<iostream>
using namespace std;
#define whatever(e,f) cout<<e<<","<<f<<endl;
int main()
{
int x,y;
x=10;
y=20;
whatever(x,y);
return 0;
}