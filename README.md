Simple polylogue chat server. Messages sent via tcp protocol.

Here I implement it using boost beast tcp streams

Clients can connect simply with "nc <ip-address> <port>"

There are 3 options:
PING - estimate delay beetween client and server
CONNECTIONS - estimate amount of connections to the server
everything else will be interpreted as message, it will be sent to all the other connected clients.

Build: Project needs boost 1.71 installed, build it with cmake.

Launch options: type ./server -h

You can set number of working threads, set up server port and add timeout.

After timeout (in seconds) slow connections will be closed, if message hasnt been delivered.

Slow connections do not affect others.

