# IRC – Internet Relay Chat Server
## Overview

IRC project is a custom Internet Relay Chat server written in C++.
It implements essential features of the IRC protocol, allowing multiple clients to connect, join channels, exchange private messages, and manage channels with operator privileges.

Irssi is a cross-platform IRC client that was used as the reference client for testing and verification.

## Getting Started

1) Install irssi

	for MacOS:
  
	```
	brew install irssi
	```
	for Linux:

	```
	sudo apt install irssi
	```

2) Build
	```
	make
	```

3) Run
	```
	./ircserv <port> <password>
	```

	+ port — the port number where the server listens for connections

	+ password — the password required for clients to connect

4) Connect with Irssi
	```
	irssi -c <server_address> -p <port> -n <name> -w <password>
	```

## Server Commands

These are the main commands available to all users:
| Command | Description |
| ------ | ----------- |
| `/join <#channel>`   | Join or create a channel |
| `/part <#channel>` | Leave a channel |
| `/msg <nickname> :<message>`    | Send a private message to a user |
| `/msg <#channel> :<message>`   | Send a message to a channel |
| `/nick newnickname`   | Change your nickname |
| `/topic <#channel>`   | View channel topic |
| `/quit :<message>` | Disconnect from the server |
	
## Channel Operator Commands

Commands available only to channel operators:

| Command | Description |
| ------ | ----------- |
| `/kick <#channel> <user> :<reason>`  | Remove a user from the channel |
| `/invite <user> <#channel> ` | Invite a user to the channel |
| `/topic <#channel> :<new topic>`   | Set channel topic |
| `/mode <#channel> +i/-i`	  | Set or remove invite-only mode |
| `/mode <#channel> +t/-t`   | Restrict topic changes to operators |
| `/mode <#channel> +k/-k <key>` | Set or remove channel key (password) |
| `/mode <#channel> +o/-o <user>`  | Give or remove operator privileges |
| `/mode <#channel> +l/-l <limit>` | Set or remove the user limit |
	
## File Transfer (DCC)
```
/dcc send <nickname> <file>
```

## Team

[@Finnan Solomon](https://github.com/finye) and [@Anya Zinchenko](https://github.com/greyear)
