# Lamport
Mutual distributed exclusion Lamport application in C

To compile :
make -f Makefile.observer

Then, run the server : 
./observer_server

You can then start the clients (5 clients max)
./observer_client localhost


#Explanations
This project is an example of the mutual exclusion, implemented using Lamport mutal exclusion.

Each process has an internal 'clock' and increments this clock for each action it does. This clock (stamp) is used to know
what process can enter in critical section first. In this project, the critical section does nothing, but in a real-life example
it would work on a common resource with the other process (like a database query or a huge calculus)
Each process has a socker server to receive the messages from the others processes. The server is running on its own thread. 

The main server (called 'observer') knows what process is running (also knows when a process quit)
and returns the list of process to the client whenever it needs it

The processes can access to the observer thanks to rpc calls.
