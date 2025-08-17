#include<iostream>
#include<map>
#include<forward_list>
#include<windows.h>
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
char requestBuffer[4096];
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
requestLength=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer),0);
if(requestLength>0)
{
for(x=0;x<requestLength;x++) printf("%c",requestBuffer[x]);
const char *response=
"HTTP/1.1 200 OK\r\n"
"Connection: close\r\n"
"Content-Type: text/html\r\n"
"Content-Length: 146\r\n\r\n"
"<!Doctype HTML><html><head><title>MGCompanies</title></head>"
"<body><h1>Welcome To MGCompanies</h1><p>We are Leading in Tech Field</p></body></html>";
send(clientSocketDescriptor,response,strlen(response),0);
}
//lot of code here
}//Infinite Loop ends Here

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