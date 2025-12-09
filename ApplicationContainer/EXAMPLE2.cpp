#include<iostream>
using namespace std;
class Request
{};
class Response
{};
class AppContainer
{};
class Bro
{
public:
void get(string url,void (*callBack)(Request &,Response &))
{
cout<<"2 parameters"<<endl;
}
void get(string url,void (*callBack)(Request &,Response &,AppContainer &ac))
{
cout<<"3 parameters"<<endl;
}
};
int main()
{
Bro b;
b.get("/abcd1",[](Request &rq,Response &rp){});
b.get("/abcd1",[](Request &rq,Response &rp,AppContainer &cc){});
b.get("/abcd1",[](Request &rq,Response &rp,AppContainer &cc){});
return 0;
}