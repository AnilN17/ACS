
//Program for SCTP client - Time Zone Application
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <sstream>

#include <netinet/sctp.h>
#include <unistd.h>
#include <netdb.h>
#define LOCALTIME_STREAM 0
#define GMT_STREAM 1
#define MAXBUFFER 1024
//Configuration of socket
struct Config
{
  int         network;    // Specifies the network layer.
  const char* host;       // The DNS name of the host.
  int         port;       // The application port number.
};

//Function Prototype
bool args(int argc, char** argv, Config&);
void sctp_client(int sd, const Config&);

//Usage Method
int usage()
{
  std::cout << "usage: client <ipv4|ipv6> <hostname> <port-number>\n\n";
  return 0;
}

// Main Function
int main(int argc, char* argv[])
{
   Config con;
     if (!args(argc, argv, con)) 
      {
        usage();
        return -1;
      }
    con.host = "localhost";
	//create a socket
    int sd = socket(con.network, SOCK_STREAM, IPPROTO_SCTP);
    sctp_client(sd,con);
    std::cout << '\n' << "Connection successful" << '\n';
    shutdown(sd, SHUT_RDWR);
    close(sd);
    return 0;
}
//Evaluating the arguments
bool args(int argc, char** argv, Config& con)
{
  // Get the network layer protocol.
  std::string network = argv[1];
  if (network == "ipv4") {
    con.network = AF_INET;
  } else if (network == "ipv6") {
    con.network = AF_INET6;
  } else {
    std::cout << "error (config): network must be 'ipv4' or 'ipv6'\n";
    return false;
  }
  //get the hostname
  con.host = argv[2];
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

//Sctp client connection logic
void sctp_client (int sd, const Config& con)
{
   char recvd[MAXBUFFER+1];
   struct sockaddr_in servaddr;
   struct sctp_status status;
   struct sctp_sndrcvinfo sndrcvinfo;
   struct sctp_event_subscribe events;
   struct sctp_initmsg initmsg;
   //struct sctp_sndrcvinfo sndrcvinfo;
   int flags, ret, in;
  // Give hints about the connection being requested.
  addrinfo hint {};
  std::memset(&hint, 0, sizeof(hint));
  hint.ai_family = con.network;     // Use the requsted network
  hint.ai_socktype = SOCK_STREAM; // Use the requested transport
  addrinfo* info;
  int result = getaddrinfo(con.host, NULL, &hint, &info);
  if (result < 0) 
  {
  std::cout << "error (getaddrinfo):" << gai_strerror(result) << '\n';
  return;
  }
   /* Specify that a maximum of 5 streams will be available per socket */
   memset( &initmsg, 0, sizeof(initmsg) );
   initmsg.sinit_num_ostreams = 5;
   initmsg.sinit_max_instreams = 5;
   initmsg.sinit_max_attempts = 4;
   ret = setsockopt( sd, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof(initmsg));

  // Connect to the remove server.
  // Bind the socket to the local address. We need to set up a socket address for this operation.
  if (con.network == AF_INET) 
  {
    // Initialize the IPv4 address.
    sockaddr_in* addr = (sockaddr_in*)info->ai_addr;
    addr->sin_port = htons(con.port);
    // Bind the socket to the address.
    ret = connect (sd, info->ai_addr, info->ai_addrlen); 
    if (ret < 0) 
    {
      std::cout << "error (connect ipv4): " << std::strerror(errno) << '\n';
      return;
    }
  } 
  else {
    // Initialize the IPv6 address.
    sockaddr_in6* addr = (sockaddr_in6*)info->ai_addr;
    addr->sin6_port = htons(con.port);
    // Connect the socket to the address.
    ret = connect (sd, info->ai_addr, info->ai_addrlen); 
    if (ret < 0) 
    {
      std::cout << "error (connect ipv6): " << std::strerror(errno) << '\n';
      return;
    }

  }
   /* Enable receipt of SCTP Snd/Rcv Data via sctp_recvmsg */
   memset( (void *)&events, 0, sizeof(events) );
   events.sctp_data_io_event = 1;
   ret = setsockopt( sd, SOL_SCTP, SCTP_EVENTS, (const void *)&events, sizeof(events) );
/* Read and emit the status of the Socket (optional step) */
   in = sizeof(status);
   ret = getsockopt( sd, SOL_SCTP, SCTP_STATUS, (void *)&status, (socklen_t *)&in );
   std::cout << "assoc id =" << status.sstat_assoc_id << "\n";
   std::cout << "state =" << status.sstat_state << "\n";
   std::cout << "instrms =" << status.sstat_instrms << "\n";
   std::cout << "outstrms =" << status.sstat_outstrms << "\n";

   for(int i=0;i<2;i++)
   {
   in = sctp_recvmsg( sd, (void *)recvd, sizeof(recvd),
                    (struct sockaddr *)NULL, 0, &sndrcvinfo, &flags );
    if (in > 0) 
    {
          recvd[in] = 0;
          if (sndrcvinfo.sinfo_stream == LOCALTIME_STREAM) 
          {
          std::cout << "Runs on stream 0" << "\n";
          std::cout << "(Local):" << recvd << "\n";
          } 
          else if(sndrcvinfo.sinfo_stream == GMT_STREAM)
          {
            std::cout <<"Runs on stream 1" << "\n";
            std::cout << "(GMT):" << recvd << "\n";
          }
          else
          {
            std::cout <<"No stream read";
          }
    }
   }
}