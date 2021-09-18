# ASN1-TCP-client-server
- Use Cmake to build the project (version 3.8 required).
- server's IP address and port are currently hardcoded as 192.168.0.137:1234 since that is a test project. Change values in source code, if needed. 
- Server is running in an endless loop, accepting clients and serving each in different thread.
- Client is sending one message from console input to server and shuts down afterwards.
- Messages can have arbitrary length, length of data packet is coded in ASN1 format.
