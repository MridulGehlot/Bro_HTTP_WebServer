#include<bits/stdc++.h>
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
class BroUtility
{
private:
BroUtility(){}
public:
static bool isHexChar(int w)
{
if(w>=48 && w<=57) return true;
if(w>='a' && w<='z') return true;
if(w>='A' && w<='Z') return true;
return false;
}
static void decode(char *encodedString,char *decodedString)
{
char *ptr=encodedString;
char *d=decodedString;
int i,m;
i=0;
while(*ptr!='\0')
{
if(*ptr=='+')
{
d[i]=' ';
ptr++;
i++;
continue;
}
if(*ptr!='%')
{
d[i]=*ptr;
ptr++;
i++;
continue;
}
ptr++;
if(isHexChar(*ptr) && isHexChar(*(ptr+1)))
{
sscanf(ptr,"%2x",&m);
d[i]=m;
i++;
ptr+=2;
}
else
{
i=0;
break;
}
}
d[i]='\0';
}
static void loadMIMETypes(map<string,string> &mimeTypesMap)
{
FILE *file;
file=fopen("bro-data/mime.types","r");
if(file==NULL) return;
char *mimeType,*extension;
char line[200];
int x;
while(true)
{
fgets(line,200,file);
if(feof(file)) break;
if(line[0]=='#') continue;
//login to remove \r\n from the end of line starts here
x=strlen(line)-1;
while(true)
{
if(line[x]=='\r' || line[x]=='\n')
{
line[x]='\0';
x--;
}
else break;
}
//login to remove \r\n from the end of line ends here
mimeType=&line[0];
for(x=0;line[x]!='\t';x++);
line[x]='\0';
x++;
while(line[x]=='\t') x++;
while(true)
{
extension=line+x;
while(line[x]!=' ' && line[x]!='\0') x++;
if(line[x]=='\0')
{
//add Entry to map and break the loop
mimeTypesMap.insert(pair<string,string>(string(extension),string(mimeType)));
//cout<<extension<<" , "<<mimeType<<endl;
break;
}
else
{
//place \0 on the xth index, add entry to map and increment value of x
line[x]='\0';
x++;
mimeTypesMap.insert(pair<string,string>(string(extension),string(mimeType)));
//cout<<extension<<" , "<<mimeType<<endl;
}
}//parsing ends here
}
fclose(file);
}
};
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
static string getFileExtension(const char *path)
{
if(path==NULL) return string("");
int x=strlen(path)-1;
while(x>=0 && path[x]!='.') x--;
if(x==-1 || path[x]!='.') return string("");
return string(path+x+1);
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
map<string,string> dataMap;
Request(char *methodType,char *requestURI,char *httpVersion,char *dataInRequest)
{
this->methodType=methodType;
this->requestURI=requestURI;
this->httpVersion=httpVersion;
if(dataInRequest!=NULL && strcmp(methodType,"get")==0)
{
createDataMap(dataInRequest,dataMap);
}
}
void createDataMap(char *str,map<string,string> &dataMap)
{
int keyLength,valueLength;
char *ptr1,*ptr2;
char *decoded;
ptr1=str;
ptr2=str;
while(true)
{
while(*ptr2!='\0' && *ptr2!='=') ptr2++;
if(*ptr2=='\0') return;
*ptr2='\0';
keyLength=ptr2-ptr1;
decoded=new char[keyLength+1];
BroUtility::decode(ptr1,decoded);
string key=string(decoded);
delete [] decoded;
ptr2++;
ptr1=ptr2;
while(*ptr2!='\0' && *ptr2!='&') ptr2++;
if(*ptr2=='\0')
{
valueLength=ptr2-ptr1;
decoded=new char[valueLength+1];
BroUtility::decode(ptr1,decoded);
dataMap.insert(pair<string,string>(key,string(decoded)));
delete [] decoded;
break;
}
else
{
*ptr2='\0';
valueLength=ptr2-ptr1;
decoded=new char[valueLength+1];
BroUtility::decode(ptr1,decoded);
dataMap.insert(pair<string,string>(key,string(decoded)));
delete [] decoded;
ptr2++;
ptr1=ptr2;
}
}//end of infinite loop
}
public:
string operator[](string key)
{
auto iterator=dataMap.find(key);
if(iterator==dataMap.end()) return string("");
return iterator->second;
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
map<string,string> mimeTypes;
public:
Bro()
{
BroUtility::loadMIMETypes(mimeTypes);
if(mimeTypes.size()==0) throw string("bro-data folder has been tampered");
}
~Bro()
{
}
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
string extension,mimeType;
extension=FileSystemUtility::getFileExtension(resourcePath.c_str());
if(extension.size()>0)
{
transform(extension.begin(),extension.end(),extension.begin(),::tolower);
auto mimeTypesIterator=mimeTypes.find(extension);
if(mimeTypesIterator!=mimeTypes.end())
{
mimeType=mimeTypesIterator->second;
}
else mimeType=string("text/html");
}
else mimeType=string("text/html");
char header[200];
//cout<<"Serving Static Resources - - - - - -"<<endl;
//cout<<resourcePath<<" , "<<extension<<" , "<<mimeType<<endl;
sprintf(header,"HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\nConnection: close\r\n\r\n",mimeType.c_str(),fileSize); 
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
void post(string url,void (*callBack)(Request&,Response&))
{
if(Validator::isValidURLFormat(url))
{
this->urlMappings.insert(pair<string,URLMapping>(url,{__POST__,callBack}));
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
char *methodType,*requestURI,*httpVersion,*dataInRequest;
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
dataInRequest=NULL;
i=0;
while(requestURI[i]!='\0' && requestURI[i]!='?') i++;
if(requestURI[i]=='?')
{
requestURI[i]='\0';
dataInRequest=requestURI+i+1;
}
//cout<<"Request URI - "<<requestURI<<endl;
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
Request request(methodType,requestURI,httpVersion,dataInRequest);
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
bro.get("/save_data_test1",[](Request& request,Response& response)->void {

string name=request["nm"];
string city=request["ct"];
cout<<"Name - "<<name<<endl;
cout<<"City - "<<city<<endl;

const char *html=R""""(
<!DOCTYPE HTML>
<html>
<head>
<meta chartset='utf-8'>
<title>Testing</title>
</head>
<body>
<h1>Test Case 1 : GET With Query String</h1>
<h3>Saved Data At the Server Side</h3>
<a href='index.html'>Home</a>
</body>
</html>
)"""";
response.setContentType("text/html");
response<<html;
});
bro.post("/save_data_test2",[](Request& request,Response& response)->void {
const char *html=R""""(
<!DOCTYPE HTML>
<html>
<head>
<meta chartset='utf-8'>
<title>Testing</title>
</head>
<body>
<h1>Test Case 2 : POST With Form Data</h1>
<h3>Saved Data At the Server Side</h3>
<a href='index.html'>Home</a>
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