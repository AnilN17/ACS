//program for SCTP server - Time Zone Application

#include <netinet/in.h>
#include <future>
#include <time.h>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <cerrno>
#include <cstdlib>
#include <ctime>
#include <cstddef>
#include <chrono>
#include <iomanip>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/sctp.h>

#define LOCALTIME_STREAM 0
#define GMT_STREAM 1
#define MAXBUFFER 1024

int ret;
char buf[MAXBUFFER+1];


//Socket Configuration
struct Config
{
  int network;
  int port;
};

struct threaddata
{
int sd1;//socket(con.network, SOCK_STREAM, IPPROTO_SCTP);
std::string name1;
};

//Function Prototypes
bool args(int argc, char** argv, Config&);
void sctp_server(int sd);
int usage()
{
  std::cout << "usage: server <ipv4|ipv6> <port-number>\n\n";
  return 0;
}

//Main function
int main(int argc, char* argv[])
{
  Config con;
  time_t currentTime;
  //int ret;
  struct sctp_initmsg initmsg;
  char buf[MAXBUFFER+1];
  if (!args(argc, argv, con)) {
    usage();
    return -1;
  }
  if(atoi(argv[1]) == 1)
  {
  // Create the socket.
  int sd = socket(con.network, SOCK_STREAM, IPPROTO_SCTP);
    
      // Bind the socket to the local address. We need
  // to set up a socket address for this operation.
  if (con.network == AF_INET) {
    // Initialize the IPv4 address.
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr)); // Zero-initialize the address
    addr.sin_family = AF_INET;           // Set the address family
    addr.sin_addr.s_addr = INADDR_ANY;   // Allow messages from anywhere
    addr.sin_port = htons(con.port);

    // Bind the socket to the address.
    ret = bind (sd, (sockaddr*)&addr, sizeof(addr));
    if (ret < 0) 
    {
      std::cout << "error (bind): " << std::strerror(errno) << '\n';
      return -1;
    }     
   }
   else 
   {
    sockaddr_in6 addr;
    std::memset(&addr, 0, sizeof(addr)); // Zero-initialize the address
    addr.sin6_family = AF_INET6;         // Set the address family
    addr.sin6_addr = in6addr_any;        // Allow messages from anywhere
    addr.sin6_port = htons(con.port);    // Set the port number
   
    // Bind the socket to the address.
    ret = bind (sd, (sockaddr*)&addr, sizeof(addr));
    if (ret < 0) 
    {
      std::cout << "error (bind): " << std::strerror(errno) << '\n';
      return -1;
    }
   }

   // Specify that a maximum of 5 streams will be available per socket 
   memset( &initmsg, 0, sizeof(initmsg) );
   initmsg.sinit_num_ostreams = 5;
   initmsg.sinit_max_instreams = 5;
   initmsg.sinit_max_attempts = 4;
   ret = setsockopt( sd, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof(initmsg) );

    int ls = listen(sd,5);
    //std::cout << '\n' << "Waiting for client...." << '\n';
   if(ls < 0)
   {
    std::cout << "error (listen): " << std::strerror(errno) << '\n';
   }
    sockaddr_storage addr;           // Stores the address of the client.
    socklen_t length = sizeof(addr); // Length of the stored address.
    int cd = accept (sd, (sockaddr*)&addr, &length);
    if (cd < 0) {
      std::cout << "error (accept): " << std::strerror(errno) << '\n';
     }
    std::cout << '\n' << "server connected" << '\n';
    
    // Grab the current time 
    currentTime = time(NULL);
    
    struct tm tstruct,tstruct1;
    tstruct = *localtime(&currentTime);
    strftime(buf,sizeof(buf),"%Y-%m-%d,%X",&tstruct);
    ret = sctp_sendmsg( cd,(void *)buf, (size_t)strlen(buf),NULL, 0, 0, 0, LOCALTIME_STREAM, 0, 0 );
    tstruct1 = *(gmtime(&currentTime));
    strftime(buf,sizeof(buf),"%Y-%m-%d,%X",&tstruct1);
    ret = sctp_sendmsg( cd,(void *)buf, (size_t)strlen(buf),NULL, 0, 0, 0, GMT_STREAM, 0, 0 );
    close(cd);
    // Close the socket and exit.
    shutdown(sd, SHUT_RDWR);
    close(sd);
    return 0;
}
 else if(atoi(argv[1]) == 2)
{
  // Create the socket.
  int sd = socket(con.network, SOCK_STREAM, IPPROTO_SCTP);
    
      // Bind the socket to the local address. We need
  // to set up a socket address for this operation.
  if (con.network == AF_INET) {
    // Initialize the IPv4 address.
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr)); // Zero-initialize the address
    addr.sin_family = AF_INET;           // Set the address family
    addr.sin_addr.s_addr = INADDR_ANY;   // Allow messages from anywhere
    addr.sin_port = htons(con.port);

    // Bind the socket to the address.
    ret = bind (sd, (sockaddr*)&addr, sizeof(addr));
    if (ret < 0) 
    {
      std::cout << "error (bind): " << std::strerror(errno) << '\n';
      return -1;
    }     
   }
   else 
   {
    sockaddr_in6 addr;
    std::memset(&addr, 0, sizeof(addr)); // Zero-initialize the address
    addr.sin6_family = AF_INET6;         // Set the address family
    addr.sin6_addr = in6addr_any;        // Allow messages from anywhere
    addr.sin6_port = htons(con.port);    // Set the port number
   
    // Bind the socket to the address.
    ret = bind (sd, (sockaddr*)&addr, sizeof(addr));
    if (ret < 0) 
    {
      std::cout << "error (bind): " << std::strerror(errno) << '\n';
      return -1;
    }
   }

   // Specify that a maximum of 5 streams will be available per socket 
   memset( &initmsg, 0, sizeof(initmsg) );
   initmsg.sinit_num_ostreams = 5;
   initmsg.sinit_max_instreams = 5;
   initmsg.sinit_max_attempts = 4;
   ret = setsockopt( sd, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof(initmsg) );
   sctp_server(sd);
    // Close the socket and exit.
    shutdown(sd, SHUT_RDWR);
    close(sd);
    return 0;
}
else
{
  std::cout << "error";
}
}
  //SCTP_CLIENT
  void* sctp_client(void* threadargs)
{
    struct threaddata *mydata;
    mydata=(struct threaddata*)threadargs;
    time_t currentTime;
    // Grab the current time 
    currentTime = time(NULL);
    
    struct tm tstruct,tstruct1;
    tstruct = *localtime(&currentTime);
    strftime(buf,sizeof(buf),"%Y-%m-%d,%X",&tstruct);
    ret = sctp_sendmsg(mydata->sd1,(void *)buf, (size_t)strlen(buf),NULL, 0, 0, 0, LOCALTIME_STREAM, 0, 0 );
    tstruct1 = *(gmtime(&currentTime));
    strftime(buf,sizeof(buf),"%Y-%m-%d,%X",&tstruct1);
    ret = sctp_sendmsg(mydata->sd1,(void *)buf, (size_t)strlen(buf),NULL, 0, 0, 0, GMT_STREAM, 0, 0 );
    close(mydata->sd1);
}

// Returns a string representation of the given address.
std::string get_address_name(sockaddr_storage& addr)
{
  // Get the address field from the connected end point.
  void* ptr;     // Points to the actual address field
  if (addr.ss_family == AF_INET) {
    sockaddr_in* p = (sockaddr_in*)&addr;
    ptr = &p->sin_addr;
  } 
    else if (addr.ss_family == AF_INET6) {
    sockaddr_in6* p = (sockaddr_in6*)&addr;
    ptr = &p->sin6_addr;
  } else {
    std::cout << addr.ss_family << '\n';
    return "<unknown>";
  }
  // Get a textual representaiton of the client's address.
  char name[64]; // Will store the address string
  if (!inet_ntop(addr.ss_family, ptr, name, 64))
    return "<error>";
  else
    return name;
}
//SCTP_SERVER
void sctp_server(int sd) 
{
   pthread_t threads[SOMAXCONN];
   struct threaddata td[SOMAXCONN];
   //std::vector<std::future<void>> futures;
   int ls = listen(sd,SOMAXCONN);
   if(ls < 0)
   {
    std::cout << "error (listen): " << std::strerror(errno) << '\n';
   }
    int count = 1;
  while (count) 
  {
    sockaddr_storage addr;           // Stores the address of the client.
    socklen_t length = sizeof(addr); // Length of the stored address.
    std::string name = get_address_name(addr);
   // std::future<void> f = std::async(sctp_client, cd, name);

for(int j=0;j<3;j++)
{
td[j].sd1=accept(sd,(sockaddr*)&addr,&length);
    if (td[j].sd1 < 0) 
    {
      std::cout << "error (accept): " << std::strerror(errno) << '\n';
      continue;
    }
td[j].name1=name;
pthread_create(&threads[j],NULL,sctp_client,(void *)&td[j]);
}
  
pthread_exit(NULL);  //futures.push_back(std::move(f));
    --count;
  }
}

//Evaluating the arguments
bool args(int argc, char** argv, Config& con)
{
  // Get the network layer protocol.
  std::string network = argv[2];
  if (network == "ipv4") {
    con.network = AF_INET;
  } else if (network == "ipv6") {
    con.network = AF_INET6;
  } else {
    std::cout << "error (config): network must be 'ipv4' or 'ipv6'\n";
    return false;
  }
  // Get the port number.
  std::string port = argv[3];
  std::stringstream ss(port);
  ss >> con.port;
  if (!ss) {
    std::cout << "error (config): invalid port number\n";
    return false;
  }

  return true;
}
