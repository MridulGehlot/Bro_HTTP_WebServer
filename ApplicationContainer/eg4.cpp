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
//Not an Error Prone Code
class IntCollection
{
int *ptr;
unsigned int size;
public :
IntCollection()
{
this->ptr=NULL;
this->size=0;
}
IntCollection(unsigned int size)
{
if(size<0)
{
this->size=0;
this->ptr=NULL;
}
else
{
this->size=size;
this->ptr=new int[this->size]; //allocation failed
if(this->ptr==NULL) this->size=0;
}
}
IntCollection(const IntCollection &other)
{
if(other.size==0)
{
this->size=0;
this->ptr=NULL;
return;
}
this->size=other.size;
this->ptr=new int[this->size];
if(this->ptr==NULL)
{
this->size=0;
return;
}
int x;
for(x=0;x<other.size;x++) this->ptr[x]=other.ptr[x];
}

void insertAt(int index,int value)
{
if(index>=0 && index<this->size)
{
ptr[index]=value;
}
}

int getValueAt(int idx)
{
if(idx>=0 && idx<this->size) return this->ptr[idx];
else return 0;
}
int getSize()
{
return this->size;
}

};

void extractAndPrint(Container &c)
{
IntCollection k;
bool succ;
c.get("myCollection",&k,&succ);
for(int i=0;i<k.getSize();i++)
{
cout<<k.getValueAt(i)<<endl;
}
}
void keepData(Container &c)
{
IntCollection k(5);
k.insertAt(0,1000);
k.insertAt(1,2000);
k.insertAt(2,404);
k.insertAt(3,9410);
k.insertAt(4,600);
c.set("myCollection",k);
}
//if write Destructor code will halt
int main()
{
Container c;
keepData(c);
extractAndPrint(c);
return 0;
}