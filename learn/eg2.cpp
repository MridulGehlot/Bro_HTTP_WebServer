#include<iostream>
using namespace std;
#define _forward_(request,url) \
request.forwardTo(url); \
return;
class Request
{
string _forwardTo;
public:
void forwardTo(string _forwardTo)
{
this->_forwardTo=_forwardTo;
}
string forwardToWhichResource()
{
return this->_forwardTo;
}
};
void doSomething(Request &req)
{
cout<<"This is line one"<<endl;
_forward_(req,string("/coolThings"));
cout<<"This is line two"<<endl;
}
int main()
{
Request request;
doSomething(request);
cout<<"Request has been forwarded to : "<<request.forwardToWhichResource()<<endl;
return 0;
}