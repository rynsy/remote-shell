# Restricted Remote Shell

A client and server for a simple shell program

## Features
This project is a simplified version of the User Pluggin Shell, written to use the networking functionality of Unix. The program has a thin client for connecting with the server, which runs commands supplied by the client. 

This program was not written with security in mind. Commands and login information provided by the client is not encrypted or obscured before sending the information to the server. Likewise, the responses from the server are not encrypted or obscured either. This program was used as an introduction to network programming, and leaves much to be desired in terms of security. 

## TODO
* Re-write program to encrypted data that travels between client and server.
* Merge this project with the User Pluggin Shell
* Spin off login function as a separate process

## Limitations
    Doesn't handle strings enclosed in '"' as one token, so they cannot be
    properly passed as parameters to commands.

    Doesn't handle empty commands from the user (e.g. ""), causes a SEGFAULT.

    Client won't exit if server is closed mid-session.
