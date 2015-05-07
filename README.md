# ACS
###Project: Time Zone Application using SCTP protocol
###Abstract: SCTP is a reliable transport layer protocol that provides stable, ordered delivery of data between two end points and also preserves data message boundaries. Unlike TCP and UDP, SCTP provides advantages like multi-homing and multi-streaming capabilities. Time Zone application makes use of SCTP protocol and helps in providing the date & time based on specific time zone. 
###Modules: Implemented two modules
###Multistreaming and Multithreading
###The final folder has client and server programs.
###How to execute
####Server Program: ./server ipv4 7777
####Client Program: ./client ipv4 localhost 7777
##Description: 
Protocol Configuration:
SCTP protocol configuration includes:
1)	Configuring SCTP Authentication Parameters 
2)	Displaying Information About SCTP Associations and Parameters 
3)	Troubleshooting SCTP Associations and Parameters
SCTP is not explicitly configured on routers but includes the configuration of network, hosts and port numbers. (On a local machine hostname will be ‘local host’ (127.0.0.1)).
Project Description: This application supports multi-streaming within an association unlike TCP. The application prints out the current date and time to a particular user at the client side. This is implemented as a single threaded and multi-threaded server. The server sends out the local time and other timezone time on two different streams and the client expects two messages from the peer and then closes the connection. The client and server programs contain functions like socket (), Connect (), Bind (), Listen (), Accept () and Close ().
Implementation: The project implementation consists of two modules:
1)	Multistreaming
2)	Multithreading
Packages needed for implementing sctp protocol based application:
–lsctp, -lksctp and libsctp tools
Multistreaming: Time zone application is a best example for implementing the SCTP protocol features. One of them is “Multistreaming”. The server.cpp file should contain the code for server side functionality. Firstly, server side programming is written using the basic server socket API calls and then the logic for time zone application is written which includes sending date & time of two time zones on different streams (0&1) and used Sctp_Sendmsg to send time service from server to the client. Server code contains the specifications for maximum number of streams for a socket connection. Then, the client side accepts the request from the server and prints the date & time onto screen if the connection is successful. Moreover, the client side contains the implementation of events&notifications to get the assoc id, state, instreams and outstreams.
Multithreading: The time zone application is extended to know how it works for multiple clients and one server basically can be considered as many to one association. In our application we have implemented multithreading using threads concept where we created thread using “pthread_create”. Number of clients depends on user providing in the code (n=3). Only server is executed once and waits until clients are connected. Once the connection is successful, the client gets the service from the server. Server gets disconnected once all the three clients are executed. And, all the threads will not be in active, as it leads to ambiguity. 
###Used in much bigger applications like SS7, PSTN.
