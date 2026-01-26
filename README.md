# IRC – Internet Relay Chat Server
## Overview

IRC project is a custom Internet Relay Chat server written in C++.
It implements essential features of the IRC protocol, allowing multiple clients to connect, join channels, exchange private messages, and manage channels with operator privileges.

Irssi was used as the reference client for testing and verification.

## Getting Started
Build
```
make
```

Run
```
./ircserv <port> <password>
```

+ port — the port number where the server listens for connections

+ password — the password required for clients to connect

Connect with Irssi
```
irssi -c <server_address> -p <port> -k <password>
```

## Server Commands

These are the main commands available to all users:
| Command | Description |
| ------ | ----------- |
| `JOIN <#channel>`   | Join or create a channel |
| `PART <#channel>` | Leave a channel |
| `PRIVMSG <nickname> :<message>`    | Send a private message to a user |
| `PRIVMSG <#channel> :<message>`   | Send a message to a channel |
| `QUIT :<message>` | Disconnect from the server |
	
## Channel Operator Commands

Commands available only to channel operators:

| Command | Description |
| ------ | ----------- |
| `KICK <user>`  | Remove a user from the channel |
| `INVITE <user>` | Invite a user to the channel |
| `MODE <#channel> +i/-i`	  | Set or remove invite-only mode |
| `MODE <#channel> +t/-t`   | Restrict topic changes to operators |
| `MODE <#channel> +k/-k <key>` | Set or remove channel key (password) |
| `MODE <#channel> +o/-o <user>`  | Give or remove operator privileges |
| `MODE <#channel> +l/-l <limit>` | Set or remove the user limit |
	
## Bonus Features

File transfer support (optional extension to resemble a full IRC server)

## Contributors

@finye and @greyear
