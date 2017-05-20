# Lamport
Mutual distributed exclusion Lamport application in C

To compile :
`make -f Makefile.observer`

To clean (necessary after each run to clean the socket files):
`make -f Makefile.observer clean`

Then, run the server :
`./observer_server`

You can then start the clients (5 clients max)
`./observer_client localhost`

All the clients must be launched approximatly at the same time to avoid any issues when running with the stamp counter.

# Possible issues

Rpc fails to connect
---------------------
When starting the server for the first time, the following error can appear :
`Cannot register service: RPC: Unable to receive; errno = Success` to solve this issue, just type `rpcinfo` then the command to launch the server.

Socket fails to connect
------------------------
When starting the client once again, without running the cleaning command, the following error can appear : `Failed to bind server socket`, in this case, just run the makefile clean command (then the make command to recompile the binaries) to get rid of the socket files.

# Explanations
This project is an example of the mutual exclusion, implemented using Lamport mutal exclusion.

Each process has an internal clock and increments this clock for each action it does. This clock (stamp) is used to know what process can enter in critical section first. In this project, the critical section does nothing, but in a real-life example it would work on a common resource with the other processes (like a database query or a huge calculus)
Each process has a socket server to receive the messages from the others processes. The server is running on its own thread.

The main server (called 'observer') knows what process is running (also knows when a process quit)
and returns the list of process to the client whenever it needs it

The processes can access to the observer thanks to rpc calls.
