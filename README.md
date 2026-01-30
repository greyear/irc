# IRC – Internet Relay Chat Server
## Overview

The IRC project is a custom Internet Relay Chat server implementation in C++, following the IRC protocol (RFC 1459). It enables multiple clients to connect simultaneously, communicate through private messages, create and manage channels, and use operator privileges for channel administration.

This implementation features:
- **Non-blocking I/O** with epoll() for handling multiple simultaneous connections
- **Socket programming** for network communication
- **Channel management** with operator privileges and modes
- **User authentication** and nickname management
- **Private messaging** between users and channels

The server was developed and tested using **irssi**, a popular command-line IRC client.

## Getting Started

### 1) Install irssi

**macOS:**
```
brew install irssi
```

**Linux:**
```
sudo apt install irssi
```

### 2) Build the server
```
make
```

### 3) Start the server
```
./ircserv <port> <password>
```

**Arguments:**
- `port` — the port number on which the server listens for incoming connections (e.g., 6667)
- `password` — the connection password required for client authentication

### 4) Connect with irssi
```
irssi -c localhost -p <port> -n <nickname> -w <password>
```

**Arguments:**
- `-c` — server address (use `localhost` or `127.0.0.1` for local testing)
- `-p` — port number (must match the server's port)
- `-n` — your desired nickname
- `-w` — connection password (must match the server's password)

## Examples

**Starting the server:**
```
./ircserv 6667 mypassword
```

**Connecting with irssi:**
```
irssi -c localhost -p 6667 -n alice -w mypassword
```

**Basic usage in irssi:**
```
/nick alice
/join #general
/msg #general :Hello everyone!
/msg bob :Hey Bob, how are you?
/part #general
/quit :Goodbye!
```

**Channel operator commands:**
```
/join #myroom
/topic #myroom :Welcome to my room!
/mode #myroom +i
/invite bob #myroom
/mode #myroom +o bob
/kick #myroom alice :Spamming
```

## Server Commands

Basic commands available to all users:

| Command | Description |
| ------- | ----------- |
| `/nick <newnickname>` | Change your nickname |
| `/join <#channel>` | Join or create a channel |
| `/part <#channel>` | Leave a channel |
| `/msg <nickname> :<message>` | Send a private message to a user |
| `/msg <#channel> :<message>` | Send a message to a channel |
| `/topic <#channel>` | View the channel topic |
| `/quit :<message>` | Disconnect from the server |

## Channel Operator Commands

Commands available only to channel operators:

| Command | Description |
| ------- | ----------- |
| `/kick <#channel> <user> :<reason>` | Remove a user from the channel |
| `/invite <user> <#channel>` | Invite a user to an invite-only channel |
| `/topic <#channel> :<new topic>` | Set or change the channel topic |
| `/mode <#channel> +i/-i` | Enable/disable invite-only mode |
| `/mode <#channel> +t/-t` | Restrict/allow topic changes to operators only |
| `/mode <#channel> +k/-k <key>` | Set/remove channel password |
| `/mode <#channel> +o/-o <user>` | Grant/revoke operator privileges |
| `/mode <#channel> +l/-l <limit>` | Set/remove user limit for the channel |

## Channel Modes

| Mode | Description |
| ---- | ----------- |
| `i` | Invite-only — users must be invited to join |
| `t` | Topic restrictions — only operators can change the topic |
| `k` | Channel key (password) — requires password to join |
| `o` | Operator privileges — grants channel operator status |
| `l` | User limit — restricts the maximum number of users |

## Implementation Details

### Key Features
- **Non-blocking sockets** using `epoll()` for efficient connection handling
- **Command parsing** with modular command classes for each IRC command
- **Channel management** with support for multiple channels and operator privileges
- **User authentication** via PASS, NICK, and USER commands
- **Error handling** with appropriate IRC numeric replies

### Architecture
The server uses an object-oriented design with the following main components:
- **Server** — manages connections, socket operations, and main event loop
- **Client** — represents connected users and their state
- **Channel** — manages channel members, modes, and topic
- **ACommand** — abstract base class for all IRC commands
- **Command classes** — individual implementations for each IRC command (NICK, JOIN, PRIVMSG, etc.)

### Protocol Compliance
The server implements core IRC commands based on RFC 1459, including:
- Connection registration (PASS, NICK, USER)
- Channel operations (JOIN, PART, KICK, INVITE, TOPIC)
- Messaging (PRIVMSG)
- Server queries (PING)
- Channel modes (MODE)


## Testing

Test with multiple clients by opening several terminal windows:

**Terminal 1:**
```
./ircserv 6667 pass123
```

**Terminal 2:**
```
irssi -c localhost -p 6667 -n alice -w pass123
```

**Terminal 3:**
```
irssi -c localhost -p 6667 -n bob -w pass123
```

Now alice and bob can communicate through channels or private messages!

## Team

[@Finnan Solomon](https://github.com/finye) and [@Anya Zinchenko](https://github.com/greyear)
