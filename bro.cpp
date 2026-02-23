#include<bits/stdc++.h>
#include<queue>
#include<vector>
#include<sys/stat.h>
#include<iostream>
#include<map>
#include<forward_list>
#ifdef _WIN32
#include<windows.h>
const char *PATH_SEPARATOR="\\";
#endif
#ifdef linux
const char *PATH_SEPARATOR="/";
#include<arpa/inet.h>
#include<sys/socket.h>
#endif
#include<string.h>
#include<unistd.h>
using namespace std;
//writer of Bro Http Server

#define _forward_(request,url) \
request.forwardTo(url); \
return;


enum __container_operation_failure_reason__{__KEY_EXISTS__,__KEY_DOES_NOT_EXIST__,__OUT_OF_MEMORY__,__VALUE_SIZE_MISMATCH__};

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
void set(string keyName,whatever something,bool *success,__container_operation_failure_reason__ *reason)
{
auto iterator=dataSet.find(keyName);
if(iterator!=dataSet.end())
{
if(reason) *reason={__KEY_EXISTS__};
if(success) *success=false;
return;
}
void *ptr;
ptr=malloc(sizeof(something));
if(ptr==NULL)
{
if(reason) *reason={__OUT_OF_MEMORY__};
if(success) *success=false;
return;
}
memcpy(ptr,&something,sizeof(something));
Bag bag;
bag.ptr=ptr;
bag.size=sizeof(something);
dataSet.insert(pair<string,Bag>(keyName,bag));
if(success) *success=true;
}
template<class Cool>
void get(string keyName,Cool anything,bool *success,__container_operation_failure_reason__ *reason)
{
auto iterator=dataSet.find(keyName);
if(iterator==dataSet.end())
{
if(reason) *reason={__KEY_DOES_NOT_EXIST__};
if(success) *success=false;
return;
}
Bag bag;
bag=iterator->second;
if(bag.size!=sizeof(*anything))
{
if(reason) *reason={__VALUE_SIZE_MISMATCH__};
if(success) *success=false;
return;
}
memcpy(anything,bag.ptr,sizeof(*anything));
if(success) *success=true;
}

template<class Cool>
void remove(string keyName,Cool anything,bool *success,__container_operation_failure_reason__ *reason)
{
auto iterator=dataSet.find(keyName);
if(iterator==dataSet.end())
{
if(reason) *reason={__KEY_DOES_NOT_EXIST__};
if(success) *success=false;
return;
}
Bag bag;
bag=iterator->second;
if(bag.size!=sizeof(*anything))
{
if(reason) *reason={__VALUE_SIZE_MISMATCH__};
if(success) *success=false;
return;
}
memcpy(anything,bag.ptr,sizeof(*anything));
free(bag.ptr); //to release the memory allocated by Bro Server Programmer
if(success) *success=true;
}
bool contains(string keyName)
{
auto iterator=this->dataSet.find(keyName);
return iterator!=this->dataSet.end();
}
};

class ApplicationLevelContainer: public Container
{};

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
static bool createDirectory(const char *directoryName)
{
return mkdir(directoryName)==0;
}
static unsigned int getLastUpdatedTime(const char *fileName)
{
struct stat attributes;
stat(fileName,&attributes);
return attributes.st_mtime;
}
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
string _forwardTo;
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
string forwardToWhichResource()
{
return this->_forwardTo;
}
bool isToBeForwarded()
{
return this->_forwardTo.length()>0;
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
void set(string name,string value)
{
//will write implementation later on
}
void forwardTo(string _forwardTo)
{
this->_forwardTo=_forwardTo;
}
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

class Function
{
public:
virtual void doService(Request &,Response &)=0;
};

class StartupFunction
{
public:
virtual void run()=0;
virtual int getPriorityNumber()=0;
};
class SimpleStartupFunction:public StartupFunction
{
void (*startupFunction)(void);
int priorityNumber;
public:
SimpleStartupFunction(int priorityNumber,void (*startupFunction)(void))
{
this->priorityNumber=priorityNumber;
this->startupFunction=startupFunction;
}
int getPriorityNumber()
{
return this->priorityNumber;
}
void run()
{
this->startupFunction();
}
};
class ApplicationLevelContainerDependentStartupFunction:public StartupFunction
{
void (*startupFunction)(ApplicationLevelContainer &);
int priorityNumber;
ApplicationLevelContainer *p2ApplicationLevelContainer;
public:
ApplicationLevelContainerDependentStartupFunction(int priorityNumber,void (*startupFunction)(ApplicationLevelContainer &),ApplicationLevelContainer *p2ApplicationLevelContainer)
{
this->priorityNumber=priorityNumber;
this->startupFunction=startupFunction;
this->p2ApplicationLevelContainer=p2ApplicationLevelContainer;
}
int getPriorityNumber()
{
return this->priorityNumber;
}
void run()
{
this->startupFunction(*p2ApplicationLevelContainer);
}
};

typedef struct __url_mapping__
{
__request_method__ methodType;
Function *function;
//void (*mappedFunction)(Request&,Response&);
}URLMapping;

class SimpleFunction:public Function
{
private:
void (*mappedFunction)(Request&,Response&);
public:
SimpleFunction(void (*mappedFunction)(Request&,Response&))
{
this->mappedFunction=mappedFunction;
}
void doService(Request &request,Response &response)
{
this->mappedFunction(request,response);
}
};

class ApplicationLevelContainerDependentFunction:public Function
{
void (*mappedFunction)(Request &,Response &,ApplicationLevelContainer &);
ApplicationLevelContainer *p2ApplicationLevelContainer;
public:
ApplicationLevelContainerDependentFunction(void (*mappedFunction)(Request &,Response &,ApplicationLevelContainer &),ApplicationLevelContainer *p2ApplicationLevelContainer)
{
this->mappedFunction=mappedFunction;
this->p2ApplicationLevelContainer=p2ApplicationLevelContainer;
}
void doService(Request &request,Response &response)
{
this->mappedFunction(request,response,*p2ApplicationLevelContainer);
}
};

class StartupFunctionComparator
{
public:
int operator()(StartupFunction *e,StartupFunction *f)
{
return !(e->getPriorityNumber()<f->getPriorityNumber());
}
};
class TemplateEngine
{
private:
static void createVMDFileName(const char *chtmlFileName,char *vmdFileName)
{
char *dotPtr;
for(;*chtmlFileName!='\0';chtmlFileName++,vmdFileName++)
{
if(*chtmlFileName=='.') dotPtr=vmdFileName;
*vmdFileName=*chtmlFileName;
}
*vmdFileName='\0';
strcpy(dotPtr+1,"vmd");
}

static void createVMDFileAndProcessCHTMLFile(const char *chtmlFileName,const char *pathToVMDFile)
{}
static void processCHTMLFileWithoutCreatingVMDFile(const char *chtmlFileName,const char *pathToVMDFile)
{}

public:
//more parameters related to other type of containers will be added later on
static void processCHTMLFile(const char *chtmlFileName,Request &request,int clientSocketDescriptor)
{
if(!FileSystemUtility::directoryExists("vmd_files"))
{
if(!FileSystemUtility::createDirectory("vmd_files"))
{
//we will implement this later on
}
}
char vmdFileName[257];
createVMDFileName(chtmlFileName,vmdFileName);
// look for vmd_files\\whatever.vmd exists or not
string folderName=string("vmd_files");
string pathToVMDFile=folderName+string(PATH_SEPARATOR)+string(vmdFileName);
bool createVMDFile=false;
if(FileSystemUtility::fileExists(pathToVMDFile.c_str()))
{
if(FileSystemUtility::getLastUpdatedTime(chtmlFileName)>FileSystemUtility::getLastUpdatedTime(pathToVMDFile.c_str()))
{
createVMDFile=true;
}
}
if(createVMDFile) createVMDFileAndProcessCHTMLFile(chtmlFileName,pathToVMDFile.c_str());
else
{
processCHTMLFileWithoutCreatingVMDFile(chtmlFileName,pathToVMDFile.c_str());
}
}
}; //class TemplateEngine Ends here
class Bro
{
private:
string staticResourcesFolder;
map<string,URLMapping> urlMappings;
map<string,string> mimeTypes;
ApplicationLevelContainer applicationLevelContainer;
priority_queue<StartupFunction *,vector<StartupFunction *>,StartupFunctionComparator> startupFunctions;
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

void addStartupService(int priorityNumber,void (*startupFunction)(void))
{
StartupFunction *sf;
sf=new SimpleStartupFunction(priorityNumber,startupFunction);
this->startupFunctions.push(sf);
}
void addStartupService(int priorityNumber,void (*startupFunction)(ApplicationLevelContainer &))
{
StartupFunction *sf;
sf=new ApplicationLevelContainerDependentStartupFunction(priorityNumber,startupFunction,&(this->applicationLevelContainer));
this->startupFunctions.push(sf);
}

void get(string url,void (*callBack)(Request&,Response&))
{
if(Validator::isValidURLFormat(url))
{
Function *function=new SimpleFunction(callBack);
this->urlMappings.insert(pair<string,URLMapping>(url,{__GET__,function}));
}
}

void get(string url,void (*callBack)(Request &,Response &,ApplicationLevelContainer &))
{
if(Validator::isValidURLFormat(url))
{
Function *function=new ApplicationLevelContainerDependentFunction(callBack,&(this->applicationLevelContainer));
this->urlMappings.insert(pair<string,URLMapping>(url,{__GET__,function}));
}
}

void post(string url,void (*callBack)(Request&,Response&))
{
if(Validator::isValidURLFormat(url))
{
Function *function=new SimpleFunction(callBack);
this->urlMappings.insert(pair<string,URLMapping>(url,{__POST__,function}));
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

//call all startup functions
StartupFunction *startupFunction;
while(!startupFunctions.empty())
{
startupFunction=startupFunctions.top();
startupFunctions.pop();
startupFunction->run();
}
//call all startup functions ends here

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
//urlMapping.mappedFunction(request,response);

while(true)
{
Response response;
urlMapping.function->doService(request,response);
if(!request.isToBeForwarded())
{
HttpResponseUtility::sendResponse(clientSocketDescriptor,response);
break;
}
//copy pasted from above
string forwardTo=request.forwardToWhichResource();
request.forwardTo(string(""));
urlMappingsIterator=this->urlMappings.find(forwardTo);
if(urlMappingsIterator==this->urlMappings.end())
{
if(!serveStaticResource(clientSocketDescriptor,forwardTo.c_str()))
{
HttpErrorStatusUtility::sendNotFoundError(clientSocketDescriptor,requestURI);
}
break;
}
urlMapping=urlMappingsIterator->second;
if(urlMapping.methodType==__GET__ && strcmp(methodType,"get")!=0)
{
HttpErrorStatusUtility::sendMethodNotAllowedError(clientSocketDescriptor,methodType,requestURI);
break;
}
//some more if conditions for other request methods
//copy paste ends here
}//infinte loop for request forwarding

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

bro.addStartupService(2,[](){
cout<<"-------------------------"<<endl;
cout<<"Some Cool function that gets called on startup"<<endl;
cout<<"The Priority Number set for this function is 2"<<endl;
cout<<"-------------------------"<<endl;
});
bro.addStartupService(1,[](){
cout<<"-------------------------"<<endl;
cout<<"Some Great function that gets called on startup"<<endl;
cout<<"The Priority Number set for this function is 1"<<endl;
cout<<"-------------------------"<<endl;
});
bro.addStartupService(1,[](){
cout<<"-------------------------"<<endl;
cout<<"Some too too too Great function that gets called on startup"<<endl;
cout<<"The Priority Number set for this function is 1"<<endl;
cout<<"-------------------------"<<endl;
});
bro.addStartupService(3,[](ApplicationLevelContainer &alc){
cout<<"-------------------------"<<endl;
cout<<"Some awesome function that gets called on startup"<<endl;
cout<<"The Priority Number set for this function is 3"<<endl;
cout<<"-------------------------"<<endl;
});

//testing Request Forwarding Feature
bro.get("/coolOne",[](Request& request,Response& response)->void {
cout<<"Some Processing is done at server side for coolOne"<<endl;
//request.forwardTo(string("/coolTwo"));
// have to write like this
_forward_(request,string("/coolTwo"));
// if user wants to write request.forwardTo  he should not write anything below this
cout<<"This Line Should not get execute"<<endl;
});
bro.get("/coolTwo",[](Request& request,Response& response)->void {
cout<<"Some Processing is done at server side for coolTwo"<<endl;
request.forwardTo(string("/SomethingCool.html"));
});
bro.get("/coolThree",[](Request& request,Response& response)->void {
const char *html=R""""(
<!DOCTYPE HTML>
<html>
<head>
<meta chartset='utf-8'>
<title>Bro Test Cases</title>
</head>
<body>
<h1>Cool Three</h1>
</body>
</html>
)"""";
response<<html;
response.setContentType("text/html");
});

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


bro.get("/firstCartoonFilm",[](Request& request,Response& response,ApplicationLevelContainer &cc) -> void {
string *str;
str=new string("The Jungle Book");
cc.set("firstFilm",str,NULL,NULL);
const char *html=R""""(
<!DOCTYPE HTML>
<html>
<head>
<meta chartset='utf-8'>
<title>Bro Test Cases</title>
</head>
<body>
<h1>First Cartoon Film</h1>
<h3>The Jungle Book</h3>
<a href='/secondCartoonFilm'>Watch Next Movie</a>
</body>
</html>
)"""";
response.setContentType("text/html");
response<<html;
});
bro.get("/secondCartoonFilm",[](Request& request,Response& response,ApplicationLevelContainer &cc) -> void {
string *str;
cc.get("firstFilm",&str,NULL,NULL);
response.setContentType("text/html");
const char *html1=R""""(
<!DOCTYPE HTML>
<html>
<head>
<meta chartset='utf-8'>
<title>Bro Test Cases</title>
</head>
<body>
<h1>First Cartoon Film was
)"""";
response<<html1;
response<<*str;
const char *html2=R""""(
</h1>
<br><br>
<h1>Second Cartoon Film</h1>
<h3>MG's Life Story</h3>
<a href='/secondCartoonFilm'>Watch Next Movie</a>
</body>
</html>
)"""";
response<<html2;
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


bro.get("/slogan",[](Request& request,Response& response){
string slogan,line;
ifstream iFile("data/sofd.data");
while(true)
{
if(!(getline(iFile,line))) break;
if(slogan.length()>0) slogan+=string("<br>");
slogan+=line;
}
iFile.close();
response.setContentType("text/html");
const char *html=R""""(
<!DOCTYPE HTML>
<html>
<head>
<meta chartset='utf-8'>
<title>Slogan</title>
</head>
<body>
<h1>MG's Word</h1>
)"""";
response<<html;
response<<slogan.c_str();
const char *html2=R""""(
</body>
</html>
)"""";
response<<html2;
});

bro.get("/sloganOfTheDay",[](Request& request,Response& response){
string slogan,line;
ifstream iFile("data/sofd.data");
while(true)
{
if(!(getline(iFile,line))) break;
if(slogan.length()>0) slogan+=string("<br>");
slogan+=line;
}
iFile.close();
request.set("sloganOfTheDay",slogan);
_forward_(request,string("/wordsOfWisdom.chtml"));
});


bro.listen(9090,[](Error& error){
if(error.hasError())
{
cout<<error.getError()<<endl;
return;
}
cout<<"Bro HTTP server is listening on port 9090...."<<endl;
});
}catch(string exception)
{
cout<<exception<<endl;
}
return 0;
}