#include<iostream>
#include<map>
#include<forward_list>
#ifdef _WIN32
#include<windows.h>
#endif
#ifdef linux
#include<arpa/inet.h>
#include<sys/socket.h>
#endif
#include<string.h>
#include<unistd.h>
using namespace std;
//writer of Bro Http Server
class Validator
{
private:
Validator(){}
public:
static bool isValidStaticResourceFolder(string folder)
{
return true;
}
static bool isValidMIMEType(string mimeType)
{
return true;
}
static bool isValidURLFormat(string url)
{
return true;
}
};
class Error
{
string error;
public:
Error(string error)
{
this->error=error;
}
bool hasError()
{
return this->error.length()>0;
}
string getError()
{
return this->error;
}
};
class Request
{
};
class Response
{
private:
string contentType;
forward_list<string> content;
forward_list<string>::iterator contentIterator;
unsigned long contentLength;
public:
Response()
{
this->contentLength=0;
this->contentIterator=this->content.before_begin();
}
~Response()
{
}
void setContentType(string contentType)
{
if(Validator::isValidMIMEType(contentType))
{
this->contentType=contentType;
}
}
Response& operator<<(string content)
{
this->contentLength+=content.length();
this->contentIterator=this->content.insert_after(this->contentIterator,content);
return *this;
}
};
class Bro
{
private:
string staticResourcesFolder;
map<string,void(*)(Request&,Response&)> urlMappings;
public:
void setStaticResourcesFolder(string folder)
{
if(Validator::isValidStaticResourceFolder(folder))
{
this->staticResourcesFolder=folder;
}
}
void get(string url,void (*callBack)(Request&,Response&))
{
if(Validator::isValidURLFormat(url))
{
this->urlMappings.insert(pair<string,void(*)(Request&,Response&)>(url,callBack));
}
}
void listen(int portNumber,void (*callBack)(Error&))
{
WSADATA wsaData;
WORD ver;
ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData);

int serverSocketDescriptor,clientSocketDescriptor;
int x,successCode;
char requestBuffer[4097]; //1 extra for \0
int requestLength;
struct sockaddr_in serverSocketInformation,clientSocketInformation;
int lengthOfClientSocketInformation=sizeof(clientSocketInformation);

serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if(serverSocketDescriptor<0)
{
WSACleanup();
Error error("Unable to Create Socket");
callBack(error);
return;
}
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(portNumber);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
closesocket(serverSocketDescriptor);
WSACleanup();
char a[101];
sprintf(a,"Unable to Bind Socket on port number %d\n",portNumber);
Error error(a);
callBack(error);
return;
}
successCode=::listen(serverSocketDescriptor,10);
if(successCode<0)
{
closesocket(serverSocketDescriptor);
WSACleanup();
Error error("Unable to Accept Client Connections");
callBack(error);
return;
}
Error error("");
callBack(error);

//Infinite Loop Starts Here
while(true)
{
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&lengthOfClientSocketInformation);
if(clientSocketDescriptor<0)
{
//not yet decided
}
//DS To Maintain Request Reading starts here
forward_list<string> requestBufferDS;
forward_list<string>::iterator requestBufferDSIterator;
requestBufferDSIterator=requestBufferDS.before_begin();
int requestBufferDSSize=0;
int requestBufferDSCount=0;
//infinite loop to read request
while(1)
{
requestLength=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer)-sizeof(char),0);
if(requestLength==0) break;
requestBuffer[requestLength]='\0';
requestBufferDSIterator=requestBufferDS.insert_after(requestBufferDSIterator,string(requestBuffer));
requestBufferDSCount++;
requestBufferDSSize+=requestLength;
}//infinite loop to read request ends here
if(requestBufferDSCount>0)
{
char *requestData=new char[requestBufferDSSize+1];
char *p=requestData;
const char *q;
requestBufferDSIterator=requestBufferDS.begin();
while(requestBufferDSIterator!=requestBufferDS.end())
{
q=(*requestBufferDSIterator).c_str();
while(*q)
{
*p=*q;
p++;
q++;
}
++requestBufferDSIterator;
}
*p='\0';
requestBufferDS.clear();
printf("---------------------Request Data Begin--------------------\n");
printf("%s\n",requestData);
printf("---------------------Request Data End--------------------\n");
//code to parse the request
}
else
{
//something if no data is received
}
//DS To Maintain Request Reading ends here
closesocket(clientSocketDescriptor);
//lot of code here
}//Infinite Loop ends Here
WSACleanup();
}
};
//User of our server
int main()
{
Bro bro;
bro.setStaticResourcesFolder("whatever");
bro.get("/",[](Request& request,Response& response)->void {
const char *html=R""""(
<!DOCTYPE HTML>
<html>
<head>
<meta chartset='utf-8'>
<title>Homepage</title>
</head>
<body>
<h1>Welcome To Our website</h1>
<p>we are using bro server to serve your requests</p>
<a href='/someResource'>Visit Resources</a>
</body>
</html>
)"""";
response.setContentType("text/html");
response<<html;
});
bro.get("/someResource",[](Request& request,Response& response){
const char *html=R""""(
<!DOCTYPE HTML>
<html>
<head>
<meta chartset='utf-8'>
<title>Resource Page</title>
</head>
<body>
<h1>This is the Resource Page</h1>
<div>
<p>some important Resources</p>
</div>
<a href='/'>Home</a>
</body>
</html>
)"""";
});
bro.listen(6060,[](Error& error){
if(error.hasError())
{
cout<<error.getError()<<endl;
return;
}
cout<<"Bro HTTP server is listening on port 6060...."<<endl;
});
return 0;
}