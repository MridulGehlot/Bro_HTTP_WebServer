#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<map>
using namespace std;
class Container
{
map<string,void *> dataSet;
public:
template<class whatever>
void set(string keyName,whatever something)
{
cout<<sizeof(something)<<endl;
void *ptr;
ptr=malloc(sizeof(something));
memcpy(ptr,&something,sizeof(something));
dataSet.insert(pair<string,void *>(keyName,ptr));
}
template<class Cool>
void get(string keyName,Cool anything,bool *success)
{
cout<<sizeof(*anything)<<endl;
auto iterator=dataSet.find(keyName);
if(iterator==dataSet.end())
{
if(success) *success=false;
return;
}
memcpy(anything,iterator->second,sizeof(*anything));
if(success) *success=true;
}
};
//remaining key existance and memory leak
void extractAndPrint(Container &c)
{
int a;
unsigned int b;
char cc;
unsigned char d;
float e;
double f;
long double g;
short h;
unsigned short i;
long int j;
unsigned long int k;
long long int l;
unsigned long long int m;
wchar_t n;

bool asuccess;
bool bsuccess;
bool ccsuccess;
bool dsuccess;
bool esuccess;
bool fsuccess;
bool gsuccess;
bool hsuccess;
bool isuccess;
bool jsuccess;
bool ksuccess;
bool lsuccess;
bool msuccess;
bool nsuccess;

c.get("int",&a,&asuccess);
c.get("uint",&b,&bsuccess);
c.get("char",&cc,&ccsuccess);
c.get("uchar",&d,&dsuccess);
c.get("float",&e,&esuccess);
c.get("double",&f,&fsuccess);
c.get("long double",&g,&gsuccess);
c.get("short",&h,&hsuccess);
c.get("ushort",&i,&isuccess);
c.get("lint",&j,&jsuccess);
c.get("ulint",&k,&ksuccess);
c.get("llint",&l,&lsuccess);
c.get("ullint",&m,&msuccess);
c.get("wchar",&n,&nsuccess);
cout<<"88888888888888888888888888"<<endl;
if(asuccess) cout<<a<<endl;
if(bsuccess) cout<<b<<endl;
if(ccsuccess) cout<<cc<<endl;
if(dsuccess) cout<<d<<endl;
if(esuccess) cout<<e<<endl;
if(fsuccess) cout<<f<<endl;
if(gsuccess) cout<<g<<endl;
if(hsuccess) cout<<h<<endl;
if(isuccess) cout<<i<<endl;
if(jsuccess) cout<<j<<endl;
if(ksuccess) cout<<k<<endl;
if(lsuccess) cout<<l<<endl;
if(msuccess) cout<<m<<endl;
if(nsuccess) cout<<n<<endl;
}
int main()
{
Container c;
c.set("int",10);
c.set("uint",(unsigned int)2147483655);
c.set("char",'A');
c.set("uchar",(unsigned char)200);
c.set("float",2.335f);
c.set("double",55.443);
c.set("long double",(long double)554.3213);
c.set("short",(short int)43);
c.set("ushort",(unsigned short int)543);
c.set("lint",(long int)-777);
c.set("ulint",(unsigned long int)54345);
c.set("llint",(long long int)-4838);
c.set("ullint",(unsigned long long int)30304);
c.set("wchar",(wchar_t)'A');
cout<<"88888888888888888888888888"<<endl;
extractAndPrint(c);
return 0;
}