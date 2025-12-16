#include<iostream>
#include<queue>
#include<vector>
using namespace std;
int main()
{
auto comp=[](int e,int f){return !(e<f);};
priority_queue<int,vector<int>,decltype(comp)> pq(comp);
pq.push(25);
pq.push(26);
pq.push(1);
pq.push(2);
pq.push(33);
pq.push(987);
pq.push(763);
pq.push(46);
while(!pq.empty())
{
cout<<pq.top()<<endl;
pq.pop();
}
return 0;
}