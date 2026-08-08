# HTTP Overview
HTTP(HyperText Transfer Protocol)is an application layer protocol based on TCP.
Http module does not know anything about Tcp.It only converts byte streams into application-level messages.


# 2.Http request structure
An HTTP request consisted of three parts:
Request Line
Header
Body

The request uses an unordered_map to store HTTP headers.
The purpose of this design is to provide efficient access to header values.
Since Http headers are identified by their names,an unordered_map allows us to quickly find the specific header without traversing all headers.

The request line contains:
HTTP method
Request path
HTTP version


# 3.HttpParser
## Responsibility
HttpParser only cares about:
How to transform raw TCP bytes into HTTP semantic information.

The responsibility of HttpParser is parsing data ,not handling network operations.
It cooperates with HttpContext and provides the parsing result of the current HTTP message.

Parser should not:
handle TCP connection
send response 
access socket

This keeps the protocol layer independent from the network layer.


# 4.HttpContext
## Why HttpContext？
Tcp is stream-oriented.
Tcp does not preserve message boundaies.
For example:
clients send:
    GET /index.html HTTP/1.1
    Host:test
server may receive:
first time:GET/index
second time:.html HTTP/1.1
        Host:test       

Therefore we need context,Httpcontext is used to maintain the parsing state during HTTP request processing.

HttpContext save:
current parse state
current request
remaining buffer

# 5.HttpRequest
Represent HTTP request information.
It only stores request data.
It does not handle:
Network operations
Socket communication
Business logic

# 6.HttpResponse
## Responsibility:
Generate Http response.

It contains:
Status code
Headers
Response body


# 7.HttpHandler
HttpHandler is application layer logic.
It decides how to handle different request.
HttpHandler does not care about how HTTP
message are parsed or transmitted.


# 7.Keep Alive
One tcp connection can carry multiple request.
Therefore TcpConnection lifetime!=HttpRequest lifetime.



# Future Extension
possible features:
chunked encoding
gzip compression 
multipart/form-data
https
http/2
websocket