#include<iostream>
#include<chrono>

using namespace std;
int main()
{
auto start=chrono::system_clock::now();
int x=1;
while(x<=2000)
{
cout<<x<<endl;
x++;
}
auto end=chrono::system_clock::now();
cout<<chrono::duration_cast<chrono::milliseconds>(end-start).count()<<" milliseconds"<<endl;
return 0;
}