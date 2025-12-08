#include<iostream>
using namespace std;
class aaa
{
static int lastGeneratedCode;
int code;
public:
aaa()
{
lastGeneratedCode++;
this->code=lastGeneratedCode;
cout<<"Default Constructor code - "<<this->code<<endl;
}
aaa(const aaa &other)
{
lastGeneratedCode++;
this->code=lastGeneratedCode;
cout<<"Copy Constructor code - "<<this->code<<endl;
cout<<"Copy Constructor received reference_code - "<<other.code<<endl;
}
aaa(int x)
{
lastGeneratedCode++;
this->code=lastGeneratedCode;
cout<<"Parameterized Constructor code - "<<this->code<<endl;
}
aaa & operator=(const aaa &other)
{
cout<<"Operator = code - "<<this->code<<endl;
cout<<"Operator = reference_code - "<<other.code<<endl;
return *this;
}
};
int aaa::lastGeneratedCode=0;
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