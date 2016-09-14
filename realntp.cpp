#include <iostream>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <fstream>
#include <Windows.h>
//#include <conio.h>
#include <Math.h>
#define BUFFER_SIZE 1024
#define PORT 123
#define HOST "time.windows.com"  //"time.bora.net"//"time.nist.gov"
#define OFFLINE_FILENAME "offilne.tdat" 
#define OFFLINEFILE_MAGICNUMBER " OTSF  "
#define CAN_APPLY_DATA TRUE
#define SUPPORT_SETTIME_API TRUE
#define IS_KOREA_SERVER TRUE
#define REGISTRY_PATH "SOFTWARE\\HSH\\software\\SYNC WINDOW TIME"
#define REGKEY_HOST  "time server host"
#define REGKEY_OFFLINE_LONG_DATA "offline time data as long"
#define REGKEY_PORT "time server port"
#define OFFLINEFILE_SUPPORT FALSE
#define DEFAULT_TIMESTAMP_VALUE 1470923666ul

using namespace std;
//using namespace System;
bool external_offline_file_support;
unsigned long ttime;
string realPath;

class DataManager {
     private: 
     HKEY hKey;
    LONG status;
    void init(){
        DWORD dwDisposition;

        if((status=RegCreateKeyEx(HKEY_LOCAL_MACHINE, 
      TEXT(REGISTRY_PATH), 
      0, NULL, REG_OPTION_NON_VOLATILE, 
      KEY_ALL_ACCESS, NULL, 
      &hKey, NULL)) != ERROR_SUCCESS) {
    // etc..
  
         status = RegOpenKeyEx(
            HKEY_LOCAL_MACHINE ,                            // 키 값
            REGISTRY_PATH,// 서브 키값
            0,                                              // 옵션 항상 0
             KEY_ALL_ACCESS,                                  // 접근 권한
            &hKey                                           // 키 핸들
            );
          
      }
      else{
          
          status = RegOpenKeyEx(
            HKEY_LOCAL_MACHINE ,                            // 키 값
            REGISTRY_PATH,// 서브 키값
            0,                                              // 옵션 항상 0
             KEY_ALL_ACCESS,                                  // 접근 권한
            &hKey                                           // 키 핸들
            );
            cout<<"1 open success"<<endl;
      }
    }
    
    public:
    void aa(){}
    void saveCharDataToReg(char* key,char* val){
        if ( (status == ERROR_SUCCESS) && (hKey != NULL))
    {

         RegSetValueEx(     
            hKey,                           // RegOpenKeyEx에 얻어진 키 핸들
           TEXT( key),              // 키 안에 값 이름
            0,                              // 옵션 항상 0
            REG_SZ,                         // 데이터 타입
            (LPBYTE) val,            // 써 넣을 데이터
            (strlen(val)+1)                       // 데이터 크기
        );  
        //return TRUE;    
    }
   // return FALSE;
    }
    void saveIntDataToReg(char* key,int val ){
         if ( (status == ERROR_SUCCESS) && (hKey != NULL))
    {

         RegSetValueEx(     
            hKey,                           // RegOpenKeyEx에 얻어진 키 핸들
           TEXT( key),              // 키 안에 값 이름
            0,                              // 옵션 항상 0
            REG_DWORD,                         // 데이터 타입
            (PBYTE)& val,            // 써 넣을 데이터
           sizeof(DWORD)                      // 데이터 크기
        );  
      //  return TRUE;    
    }
   // return FALSE;
    }
    void saveLongDataToReg(char* key,unsigned long val ){
        if ( (status == ERROR_SUCCESS) && (hKey != NULL))
    {

       if( RegSetValueEx(     
            hKey,                           // RegOpenKeyEx에 얻어진 키 핸들
            TEXT(key),              // 키 안에 값 이름
            0,                              // 옵션 항상 0
            REG_DWORD,                         // 데이터 타입
            (PBYTE) &val,            // 써 넣을 데이터
           sizeof(DWORD)                      // 데이터 크기
        )== ERROR_SUCCESS){
        cout<<"save long data"<<endl;
        }else{cout<<"fail saving long data"<<endl;}
       // return TRUE;    
    }
    //return FALSE;
    }
    void saveBoolDataToReg(char* key,bool val ){
        if ( (status == ERROR_SUCCESS) && (hKey != NULL))
    {

         RegSetValueEx(     
            hKey,                           // RegOpenKeyEx에 얻어진 키 핸들
            key,              // 키 안에 값 이름
            0,                              // 옵션 항상 0
            REG_DWORD,                         // 데이터 타입
            (PBYTE) &val,            // 써 넣을 데이터
           sizeof(DWORD)                      // 데이터 크기
        );  
        //return TRUE;    
    }
    //return FALSE;
    }
   /*  bool saveFloatDataToReg(char* key,float val ){
        if ( (status == ERROR_SUCCESS) && (hKey != NULL))
    {

         RegSetValueEx(     
            hKey,                           // RegOpenKeyEx에 얻어진 키 핸들
            key,              // 키 안에 값 이름
            0,                              // 옵션 항상 0
            REG_DWORD,                         // 데이터 타입
            (PBYTE) val,            // 써 넣을 데이터
           sizeof(DWORD)                      // 데이터 크기
        );  
        return TRUE;    
    }
    return FALSE;
    }*/
    char* getCharDataFromReg(char* key,char* defaultVal){
      const static int size = 1024*4;
       char* result= new char[size];
       DWORD datatype;
       DWORD dataSize =size;
       RegQueryValueEx(hKey,key,0,&datatype,(BYTE*)result,(DWORD *)&dataSize);
       if(result ==NULL){
           return defaultVal;
       }
        return result;
    }
   int getIntDataFromReg(char* key,int defaultVal){
         int result=-1997;
       DWORD datatype;
       DWORD dataSize =sizeof(result);
       RegQueryValueEx(hKey,key,0,&datatype,(PBYTE)&result,(DWORD *)&dataSize);
       if(result==-1997){
           result = defaultVal;
       }
        return result;
    }
    long getLongDataFromReg(char* key,long defaultVal){
       long result;
       DWORD datatype;
       DWORD dataSize =sizeof(result);
       RegQueryValueEx(hKey,key,0,&datatype,(PBYTE)&result,(DWORD *)&dataSize);
       
        return result;
    }
 /*   float getFloatDataFromReg(char* key,float defaultVal){
        return 0;
    }*/
     bool getBoolDataFromReg(char* key,bool defaultVal){
        bool result=FALSE;
       DWORD datatype;
       DWORD dataSize =sizeof(result);
       RegQueryValueEx(hKey,key,0,&datatype,(PBYTE)&result,(DWORD *)&dataSize);
       
        return result;
    }
    void close(){
          RegCloseKey( hKey );
    }
     DataManager(){init();}
    ~DataManager(){close();}
   
};
 ::DataManager* dm;// = new ::DataManager(); 
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

BOOL IsWow64()
{
    BOOL bIsWow64 = FALSE;

    //IsWow64Process is not available on all supported versions of Windows.
    //Use GetModuleHandle to get a handle to the DLL that contains the function
    //and GetProcAddress to get a pointer to the function if available.

    fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
        GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

    if(NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
        {
            //handle error
        }
    }
    return bIsWow64;
}
time_t getTimeFromLong(const unsigned long lVal,bool minousKorTime){
    time_t stime = (time_t)lVal;
    
//stime =time(&stime);
if(minousKorTime){
stime  -= 9*60*60;
}

return stime;
}
void onCompleteRestoreLongData(unsigned long DaTa){
    cout<<"restored raw data: "<<DaTa<<endl;
    time_t t = getTimeFromLong(DaTa,FALSE);
    cout<<"formated data: "<<ctime(&t)<<endl;
    ttime = DaTa;
}
void loadDataFromFile(const char* FileFullPath){
    ifstream is(FileFullPath,ios::in|ios::binary);
    is.seekg(strlen(OFFLINEFILE_MAGICNUMBER),ios::beg);
   int fileversion = is.get();
    int SavedOsBit = is.get();
    is.seekg(0,ios::end);
    
    long fileSize = is.tellg();
    
    is.seekg(100,ios::beg);
    int tdatalength = fileSize-100;
    char* buffer = new char[tdatalength];
    is.read(buffer ,tdatalength);
    cout<<"buffer data: "<<buffer<<endl;
    unsigned long tempdata=0;
 tempdata = atol(buffer);
 
  /*  for(int i = 0;i<tdatalength;i++){
    tempdata+=  (unsigned long)(atol(&buffer[i])*(long)pow(10,tdatalength-i-1));
    }*/
     cout<<"restored(ori) raw data: "<<tempdata<<endl;
    cout<<"fileVersion: "<<fileversion<<endl<<"SavedOsBit: "<<SavedOsBit<<endl;
    is.close();

    onCompleteRestoreLongData(tempdata);
}
void saveDataToFile(time_t tIme){
	
	string fileFullPath = realPath;
	fileFullPath.append(OFFLINE_FILENAME);
	int offLineFileVersion = 1;
    int operationBit = 32;
    if(IsWow64()){
        operationBit=64;
    }
    else{
        operationBit=32;
    }
	const static int arraySize =100-(strlen(OFFLINEFILE_MAGICNUMBER)+1+1);
	char* reservationBytes = new char[arraySize];
	memset(reservationBytes,0xff,sizeof(char)*arraySize);
	//char reservationBytes[arraySize] = {0xff,};
	ofstream os(fileFullPath.c_str(),ios::out|ios::binary|ios::trunc);
	
	os<<OFFLINEFILE_MAGICNUMBER;
	os.put((char)offLineFileVersion);
    os.put((char)operationBit);
os<<	reservationBytes<<ttime;
	os.close();
    
}

int main(int argc,char** argv){
	//vector<char> 
     dm = new ::DataManager(); 
     ttime= DEFAULT_TIMESTAMP_VALUE;
    external_offline_file_support=OFFLINEFILE_SUPPORT;
	const char* excutedbinfilepath =argv[0];
    char manualhost[400]={0,};
    if(argc>1){
       if( argv[1]!=NULL){
         if(  strcmp(argv[1],"-sof") ==0){
             external_offline_file_support=TRUE;
         }
         else if(argv[1][0]=='-'&&argv[1][1]=='h'){
             int slen = strlen(argv[1]);
             for(int i =2;i<slen;i++){
                 manualhost[i-2]=argv[1][i];
             }
         }
       }
    }
    ttime = 0;
	char ebfRealPath[1024*5];
	realpath(excutedbinfilepath,ebfRealPath);
	//string realPath(ebfRealPath);
	realPath =ebfRealPath;
	realPath.erase( realPath.find('\"',0),1);
realPath.erase( realPath.find('\"',1),1);
int beforeLength = realPath.length();
int FileSlashindex =  realPath.find_last_of('/' )+1;
realPath = realPath.substr(0,FileSlashindex);
	
	int soc=-1;
	struct sockaddr_in addr;
	struct hostent *host_entry;
    if(manualhost[0]!=0){
     host_entry=gethostbyname(manualhost);   
    }
    else{
	host_entry = gethostbyname(HOST);//"time.bora.net"); //"time.nist.gov");
    }
	cout<<"init setting"<<endl;
	if(!host_entry){
		cout<<"host infomation error"<<endl;
         if(CAN_APPLY_DATA){
             if(external_offline_file_support){
              string fileFullPath = realPath;
	fileFullPath.append(OFFLINE_FILENAME);
             loadDataFromFile(fileFullPath.c_str());
             }
             else{
                 ttime = dm->getLongDataFromReg(REGKEY_OFFLINE_LONG_DATA,DEFAULT_TIMESTAMP_VALUE);
             }
             time_t t = getTimeFromLong(ttime,FALSE);
             
             struct tm* timeinfo;
timeinfo = localtime(&t);
    
    
    if(SUPPORT_SETTIME_API){SYSTEMTIME sT;
    SYSTEMTIME *st = &sT;
   
    st->wYear= (WORD)(timeinfo->tm_year+1900);
    st->wMonth=(WORD)(timeinfo->tm_mon+1);
    st->wDayOfWeek=(WORD)timeinfo->tm_wday;
    st->wDay= (WORD)timeinfo->tm_mday;
    st->wHour=(WORD)timeinfo->tm_hour;
    st->wMinute=(WORD)timeinfo->tm_min;
     st->wSecond= (WORD)timeinfo->tm_sec;
    st->wMilliseconds=(WORD)timeinfo->tm_hour;
     
    SetSystemTime(st);}
}
		exit(1);
	}
    if(PORT==123){
        struct protoent *proto;  
        proto=getprotobyname("udp");
	soc = socket(PF_INET,SOCK_DGRAM,proto->p_proto);
    }
    else if(PORT==37){
       // soc=socket(PF_INET,SOCK_STREAM,0);
    }
	if(soc == -1){
		cout<<"socket error"<<endl;
         if(CAN_APPLY_DATA){
              if(external_offline_file_support){
              string fileFullPath = realPath;
	fileFullPath.append(OFFLINE_FILENAME);
             loadDataFromFile(fileFullPath.c_str());
             }
             else{
                 ttime = dm->getLongDataFromReg(REGKEY_OFFLINE_LONG_DATA,DEFAULT_TIMESTAMP_VALUE);
             }
             time_t t = getTimeFromLong(ttime,FALSE);
             
             struct tm* timeinfo;
timeinfo = localtime(&t);
    
    
    if(SUPPORT_SETTIME_API){SYSTEMTIME sT;
    SYSTEMTIME *st = &sT;
   
    st->wYear= (WORD)(timeinfo->tm_year+1900);
    st->wMonth=(WORD)(timeinfo->tm_mon+1);
    st->wDayOfWeek=(WORD)timeinfo->tm_wday;
    st->wDay= (WORD)timeinfo->tm_mday;
    st->wHour=(WORD)timeinfo->tm_hour;
    st->wMinute=(WORD)timeinfo->tm_min;
     st->wSecond= (WORD)timeinfo->tm_sec;
    st->wMilliseconds=(WORD)timeinfo->tm_hour;
   
    SetSystemTime(st);}
}
		exit(1);
	}
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	memcpy((void*)(&addr.sin_addr),(void*)(host_entry->h_addr),sizeof(addr.sin_addr));
cout<<"connecting..."<<endl;
char buf[48]={0,};
buf[0]=0x1b;
unsigned long tempd=0;
int sendt;//=sendto(soc,buf,sizeof(buf),0,(struct sockaddr *)&addr,sizeof(addr));
struct timeval tv;
tv.tv_sec = 0;
tv.tv_usec = 140*100 *1000;
if (setsockopt(soc, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
    //perror("Error");
    cout<<"set socket timeout option fail.."<<endl;
}
	if((sendt=sendto(soc,buf,sizeof(buf),0,(struct sockaddr *)&addr,sizeof(addr)))==-1){//connect(soc,(struct sockaddr*)&addr,sizeof(addr))==-1){
	cout<<"socket connect error"<<endl;
    if(CAN_APPLY_DATA){
             if(external_offline_file_support){
              string fileFullPath = realPath;
	fileFullPath.append(OFFLINE_FILENAME);
             loadDataFromFile(fileFullPath.c_str());
             }
             else{
                 ttime = dm->getLongDataFromReg(REGKEY_OFFLINE_LONG_DATA,DEFAULT_TIMESTAMP_VALUE);
             }
             time_t t = getTimeFromLong(ttime,FALSE);
             struct tm* timeinfo;
timeinfo = gmtime(&t);
    
    if(SUPPORT_SETTIME_API){SYSTEMTIME sT;
    SYSTEMTIME *st = &sT;
   
    st->wYear= (WORD)(timeinfo->tm_year+1900);
    st->wMonth=(WORD)(timeinfo->tm_mon+1);
    st->wDayOfWeek=(WORD)timeinfo->tm_wday;
    st->wDay= (WORD)timeinfo->tm_mday;
    st->wHour=(WORD)timeinfo->tm_hour;
    st->wMinute=(WORD)timeinfo->tm_min;
     st->wSecond= (WORD)timeinfo->tm_sec;
    st->wMilliseconds=(WORD)timeinfo->tm_hour;
   
    SetSystemTime(st);}
}
	
}
else{
	if(sendt!=-1&&recv(soc,buf,sizeof(buf),0)<=0 ){
		cout<<"read error"<<endl;
        if(CAN_APPLY_DATA){
              if(external_offline_file_support){
              string fileFullPath = realPath;
	fileFullPath.append(OFFLINE_FILENAME);
             loadDataFromFile(fileFullPath.c_str());
             }
             else{
                 ttime = dm->getLongDataFromReg(REGKEY_OFFLINE_LONG_DATA,DEFAULT_TIMESTAMP_VALUE);
             }
             time_t t = getTimeFromLong(ttime,FALSE);
             
             struct tm* timeinfo;
timeinfo = localtime(&t);
    
    
    if(SUPPORT_SETTIME_API){SYSTEMTIME sT;
    SYSTEMTIME *st = &sT;
   
    st->wYear= (WORD)(timeinfo->tm_year+1900);
    st->wMonth=(WORD)(timeinfo->tm_mon+1);
    st->wDayOfWeek=(WORD)timeinfo->tm_wday;
    st->wDay= (WORD)timeinfo->tm_mday;
    st->wHour=(WORD)timeinfo->tm_hour;
    st->wMinute=(WORD)timeinfo->tm_min;
     st->wSecond= (WORD)timeinfo->tm_sec;
    st->wMilliseconds=(WORD)timeinfo->tm_hour;
   
    SetSystemTime(st);}
}
exit(1);
	}
	else{
        tempd=*((unsigned long*)&buf[4*10]);
        ttime = ntohl(tempd)- 2208988800lu;
		cout<<"raw data: "<<ttime<<endl;
		//unsigned long htime = ntohl(ttime)-2208988800lu;
        if(IS_KOREA_SERVER){
                 ttime -= 9*3600;
             }
//ttime = ntohl( ttime) - 2208988800lu;

time_t stime = (time_t)ttime;
//stime =  //time(&stime);
//stime  -= 9*60*60;
struct tm* timeinfo;
timeinfo = localtime(&stime);

cout<<"coverted time: "<<stime<<endl;
cout<<"human time: "<<ctime(&stime)<<endl;
if(CAN_APPLY_DATA){
    
    if(SUPPORT_SETTIME_API){SYSTEMTIME sT;
    SYSTEMTIME *st = &sT;
   
    st->wYear= (WORD)(timeinfo->tm_year+1900);
    st->wMonth=(WORD)(timeinfo->tm_mon+1);
    st->wDayOfWeek=(WORD)timeinfo->tm_wday;
    st->wDay= (WORD)timeinfo->tm_mday;
    st->wHour=(WORD)timeinfo->tm_hour;
    st->wMinute=(WORD)timeinfo->tm_min;
     st->wSecond= (WORD)timeinfo->tm_sec;
    st->wMilliseconds=(WORD)timeinfo->tm_hour;
   
    SetSystemTime(st);}
}
		cout<<"time with format: "<<(timeinfo->tm_hour)<<":"<<(timeinfo->tm_min)<<":"<<(timeinfo->tm_sec)<<endl;
		cout<<"date with format: "<<1900+(timeinfo->tm_year)<<"-" <<(timeinfo->tm_mon)+1 << "-"<< (timeinfo->tm_mday )<<endl;
	saveDataToFile(stime);
  //  ::DataManager* dm = new ::DataManager(); // = new DataManager;
    dm->saveCharDataToReg(REGKEY_HOST,(HOST));
 dm->saveIntDataToReg(REGKEY_PORT,PORT);
  dm->saveLongDataToReg(REGKEY_OFFLINE_LONG_DATA,(unsigned long)stime);
   // dm->close();
  // cout<<"reg data: "<< endl<<dm->getCharDataFromReg(REGKEY_HOST,"null")<<endl<< dm->getLongDataFromReg(REGKEY_OFFLINE_LONG_DATA,-1)<<endl<<dm->getIntDataFromReg(REGKEY_PORT,0)<<endl;
  //  delete dm;
	}
}
 delete dm;
char a =cin.get();
	return 0;
	
}
