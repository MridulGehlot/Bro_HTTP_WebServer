#include<iostream>
using namespace std;
int main()
{
/*
int x;
10=x;
*/

int y;
int &x=y;
x=100;
cout<<y<<endl;

/*
int &x=50;
*/

/*
compiling and running
const int &x=50;
cout<<x;
*/

/*
This cannot happen technically
compiling and running
int &&x=50; //x is a ref variable bound to an rvalue
//temp variable created x will become its alias
cout<<x;
*/


return 0;
}