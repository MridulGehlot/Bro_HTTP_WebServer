#include<iostream>
#include<queue>
using namespace std;
int main()
{
priority_queue<int> pq;
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