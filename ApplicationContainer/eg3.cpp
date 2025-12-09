#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<map>
using namespace std;
class Container
{
typedef struct _bag
{
void *ptr;
int size;
}Bag;
map<string,Bag> dataSet;
public:
template<class whatever>
void set(string keyName,whatever something)
{
cout<<sizeof(something)<<endl;
void *ptr;
ptr=malloc(sizeof(something));
memcpy(ptr,&something,sizeof(something));
Bag bag;
bag.ptr=ptr;
bag.size=sizeof(something);
dataSet.insert(pair<string,Bag>(keyName,bag));
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
Bag bag;
bag=iterator->second;
if(bag.size!=sizeof(*anything))
{
if(success) *success=false;
return;
}
memcpy(anything,bag.ptr,sizeof(*anything));
if(success) *success=true;
}
};
//No Error Prone Code

class Bulb
{
private:
int w;
public:
Bulb()
{
this->w=0;
}
Buld(const Bulb &other)
{
this->w=other.w;
}
void setWattage(int w)
{
this->w=w;
}
int getWattage()
{
return this->w;
}
};

void extractAndPrint(Container &c)
{
Bulb *k;
bool success;
c.get("philips",&k,&success);
if(success) cout<<"wattage - "<<k->getWattage()<<endl;
else cout<<"Philips Does not exist"<<endl;
}
void keepData(Container &c)
{
Bulb *g;
g=new Bulb;
g->setWattage(60);
c.set("philips",g);
g->setWattage(100);
//the applicaiton will fall apart if the following line is introduced 
//delete g;
}
int main()
{
Container c;
keepData(c);
extractAndPrint(c);
return 0;
}

/*
void keepData(Container &c)
{
Bulb g;
g.setWattage(60);
c.set("philips",g);
g.setWattage(100);
}
answer - 60 only
*/