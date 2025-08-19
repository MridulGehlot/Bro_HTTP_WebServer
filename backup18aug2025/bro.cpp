#include<sys/stat.h>
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
class FileSystemUtility
{
private:
FileSystemUtility(){}
public:
static bool fileExists(const char *path)
{
int x;
struct stat s;
x=stat(path,&s);
if(x!=0) return false;
if(s.st_mode & S_IFDIR) return false;
return true;
}
static bool directoryExists(const char *path)
{
int x;
struct stat s;
x=stat(path,&s);
if(x!=0) return false;
if(s.st_mode & S_IFDIR) return true;
return false;
}
};
class StringUtility
{
private: StringUtility();
public:
static void toLowerCase(char *str)
{
if(str==NULL) return;
while(*str)
{
if(*str>=65 && *str<=97) *str+=32;
str++;
}
}
};
class HttpErrorStatusUtility
{
private: HttpErrorStatusUtility(){};
public:
static void sendHttpVersionNotSupportedError(int clientSocketDescriptor,char *httpVersion)
{
//not yet implemented
}
static void sendBadRequestError(int clientSocketDescriptor)
{
//not yet implemented
}
static void sendMethodNotAllowedError(int clientSocketDescriptor,char *methodType,char *requestURI)
{
//not yet implemented
}
static void sendNotFoundError(int clientSocketDescriptor,char *requestURI)
{
//will optimize later on
char header[200],content[1000],response[1200];
sprintf(content,"<!DOCTYPE html><html><head><meta charset='utf-8'><title>404 Not Found</title></head><body><h1 style='color:red'>Error: 404 Not Found</h1><h3>The Resource [%s] Not Found</h3></body></html>",requestURI);
int contentLength=strlen(content);
sprintf(header,"HTTP/1.1 404 Not Found\r\nContent-Type: text/html\nContent-Length : %d\nConnection: close\r\n\r\n",contentLength);
strcpy(response,header);
strcat(response,content);
send(clientSocketDescriptor,response,strlen(response),0);
}
};
class Validator
{
private:
Validator(){}
public:
static bool isValidStaticResourceFolder(string folder)
{
return FileSystemUtility::directoryExists(folder.c_str());
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
private:
char *methodType;
char *requestURI;
char *httpVersion;
public:
Request(char *methodType,char *requestURI,char *httpVersion)
{
this->methodType=methodType;
this->requestURI=requestURI;
this->httpVersion=httpVersion;
}
friend class Bro;
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
friend class HttpResponseUtility;
};
class HttpResponseUtility
{
private: HttpResponseUtility(){}
public:
static void sendResponse(int clientSocketDescriptor,Response& response)
{
char header[200];
sprintf(header,"HTTP/1.1 200 ok\r\nContent-Type: %s\nContent-Length : %d\nConnection: close\r\n\r\n",response.contentType.c_str(),response.contentLength);
send(clientSocketDescriptor,header,strlen(header),0);
auto it=response.content.begin();
while(it!=response.content.end())
{
string str=*it;
send(clientSocketDescriptor,str.c_str(),str.length(),0);
++it;
}
}
};
enum __request_method__{__GET__,__POST__,__PUT__,__DELETE__,__HEAD__,__OPTIONS__,__TRACE__,__CONNECT__};
typedef struct __url_mapping__
{
__request_method__ methodType;
void (*mappedFunction)(Request&,Response&);
}URLMapping;
class Bro
{
private:
string staticResourcesFolder;
map<string,URLMapping> urlMappings;
public:
void setStaticResourcesFolder(string folder)
{
if(Validator::isValidStaticResourceFolder(folder))
{
this->staticResourcesFolder=folder;
}
else
{
string exception="Invalid Resources Folder Path : "+string(folder);
throw exception;
}
}
bool serveStaticResource(int clientSocketDescriptor,const char *requestURI)
{
if(this->staticResourcesFolder.length()==0) return false;
if(!FileSystemUtility::directoryExists(this->staticResourcesFolder.c_str())) return false;
string resourcePath=this->staticResourcesFolder+string(requestURI);
if(!FileSystemUtility::fileExists(resourcePath.c_str())) return false;
FILE *f=fopen(resourcePath.c_str(),"rb");
if(f==NULL) return false;
long fileSize;
fseek(f,0,SEEK_END);
fileSize=ftell(f);
if(fileSize==0)
{
fclose(f);
return false;
}
rewind(f); //to move internal pointer to start of file
char header[200];
sprintf(header,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %ld\r\nConnection: close\r\n\r\n",fileSize); 
send(clientSocketDescriptor,header,strlen(header),0);
long bytesLeftToRead=fileSize;
int bytesToRead=4096;
char buffer[4096];
while(bytesLeftToRead>0)
{
if(bytesLeftToRead<bytesToRead) bytesToRead=bytesLeftToRead;
fread(buffer,bytesToRead,1,f);
if(feof(f)) break; //this won't happen in our case
send(clientSocketDescriptor,buffer,bytesToRead,0);
bytesLeftToRead-=bytesToRead;
}
fclose(f);
return true;
}
void get(string url,void (*callBack)(Request&,Response&))
{
if(Validator::isValidURLFormat(url))
{
this->urlMappings.insert(pair<string,URLMapping>(url,{__GET__,callBack}));
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
requestLength=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer)-sizeof(char),0);
if(requestLength==0 || requestLength==-1)
{
closesocket(clientSocketDescriptor);
continue;
}
requestBuffer[requestLength]='\0';
//code to parse first line starts here
//METHOD space REQUEST_URI space HTTP_VERSION CRLF
int i;
char *methodType,*requestURI,*httpVersion;
i=0;
methodType=requestBuffer;
while(requestBuffer[i] && requestBuffer[i]!=' ') i++;
if(requestBuffer[i]=='\0')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
closesocket(clientSocketDescriptor);
continue;
}
requestBuffer[i]='\0';
i++;
if(requestBuffer[i]==' ' || requestBuffer[i]=='\0')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
closesocket(clientSocketDescriptor);
continue;
}
StringUtility::toLowerCase(methodType);
if(!(strcmp(methodType,"get")==0 ||
strcmp(methodType,"post")==0 ||
strcmp(methodType,"put")==0 ||
strcmp(methodType,"delete")==0 || 
strcmp(methodType,"trace")==0 ||
strcmp(methodType,"options")==0 ||
strcmp(methodType,"head")==0 ||
strcmp(methodType,"connect")==0))
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
closesocket(clientSocketDescriptor);
continue;
}
requestURI=requestBuffer+i;
while(requestBuffer[i] && requestBuffer[i]!=' ') i++;
if(requestBuffer[i]=='\0')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
closesocket(clientSocketDescriptor);
continue;
}
requestBuffer[i]='\0';
i++;
if(requestBuffer[i]==' ' || requestBuffer[i]=='\0')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
closesocket(clientSocketDescriptor);
continue;
}
httpVersion=requestBuffer+i;
while(requestBuffer[i] && requestBuffer[i]!='\r' && requestBuffer[i]!='\n') i++;
if(requestBuffer[i]=='\0')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
closesocket(clientSocketDescriptor);
continue;
}
if(requestBuffer[i]=='\r' && requestBuffer[i+1]!='\n')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
closesocket(clientSocketDescriptor);
continue;
}
if(requestBuffer[i]=='\r')
{
requestBuffer[i]='\0';
i=i+2;
}
else
{
requestBuffer[i]='\0';
i=i+1;
}
StringUtility::toLowerCase(httpVersion);
if(strcmp(httpVersion,"http/1.1")!=0)\
{
HttpErrorStatusUtility::sendHttpVersionNotSupportedError(clientSocketDescriptor,httpVersion);
closesocket(clientSocketDescriptor);
continue;
}
cout<<"Request URI - "<<requestURI<<endl;
auto urlMappingsIterator=this->urlMappings.find(requestURI);
if(urlMappingsIterator==this->urlMappings.end())
{
if(!serveStaticResource(clientSocketDescriptor,requestURI))
{
HttpErrorStatusUtility::sendNotFoundError(clientSocketDescriptor,requestURI);
}
closesocket(clientSocketDescriptor);
continue;
}
URLMapping urlMapping=urlMappingsIterator->second;
if(urlMapping.methodType==__GET__ && strcmp(methodType,"get")!=0)
{
HttpErrorStatusUtility::sendMethodNotAllowedError(clientSocketDescriptor,methodType,requestURI);
closesocket(clientSocketDescriptor);
continue;
}
//code to parse first line ends here
//code to parse header and payload starts here
//code to parse header and payload ends here
Request request(methodType,requestURI,httpVersion);
Response response;
urlMapping.mappedFunction(request,response);
HttpResponseUtility::sendResponse(clientSocketDescriptor,response);
closesocket(clientSocketDescriptor);
//lot of code here
}//Infinite Loop ends Here
WSACleanup();
}
};
//User of our server
int main()
{
try
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
response.setContentType("text/html");
response<<html;
});
bro.listen(6060,[](Error& error){
if(error.hasError())
{
cout<<error.getError()<<endl;
return;
}
cout<<"Bro HTTP server is listening on port 6060...."<<endl;
});
}catch(string exception)
{
cout<<exception<<endl;
}
return 0;
}