#include<iostream>
using namespace std;
class aaa
{
public:
aaa()
{
cout<<"Default Constructor"<<endl;
}
aaa(const aaa &other)
{
cout<<"Copy Constructor"<<endl;
}
aaa(int x)
{
cout<<"Parameterized Constructor"<<endl;
}
aaa & operator=(const aaa &other)
{
cout<<"Operator = got called"<<endl;
return *this;
}
};
void sam(aaa j)
{
cout<<"sam got called"<<endl;
}
int main()
{
aaa a;
sam(a);
cout<<"**********"<<endl;
aaa bb;
aaa kk=bb;
aaa rr=10;
aaa mm;
mm=kk;
return 0;
}