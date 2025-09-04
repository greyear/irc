#pragma once

#define ERR_NOSUCHNICK        "401" // "<nickname> :No such nick/channel"
#define ERR_NOSUCHCHANNEL     "403" // "<channel name> :No such channel"
#define ERR_CANNOTSENDTOCHAN  "404" // "<channel name> :Cannot send to channel"
#define ERR_UNKNOWNCOMMAND "421" // "<command> :Unknown command"

#define ERR_NONICKNAMEGIVEN 431
/*
":No nickname given"
- Returned when a nickname parameter expected for a command and isn't found.*/

#define ERR_ERRONEUSNICKNAME 432
/*
"<nick> :Erroneous nickname"
- Returned after receiving a NICK message which contains characters which do not fall in the
 defined set. See section 2.3.1 for details on valid nicknames*/
 /*
 nickname   =  ( letter / special ) *8( letter / digit / special / "-" )

letter → alphabetic character (A–Z, a–z).
digit → numeric character (0–9).
special → certain special characters allowed in nicknames (like [ ] \ ^ { } |`).
"-" → the dash/hyphen is also allowed.

The nickname must start with a letter or a special character (it cannot start with a digit or a dash).
After that, you may have up to 8 more characters from the set {letter, digit, special, dash}.
So the maximum nickname length is 9 characters.*/

#define ERR_NICKNAMEINUSE 433
/*
"<nick> :Nickname is already in use"
- Returned when a NICK message is processed that results in an attempt to change to a currently
 existing nickname.*/

#define ERR_USERNOTINCHANNEL  "441" // "<nick> <channel> :They aren't on that channel"
#define ERR_NOTONCHANNEL      "442" // "<channel> :You're not on that channel"

#define ERR_NOTREGISTERED "451"
/*
":You have not registered"
- Returned to indicate that the client MUST be registered before the server will allow it
 to be parsed in detail.*/
 /*
 You tried the command BEFORE you've registered!
 */

#define ERR_NEEDMOREPARAMS "461"
/*
"<command> :Not enough parameters"
- Returned to indicate to the client that it didn't supply enough parameters.*/

#define ERR_ALREADYREGISTRED "462"
/*
":You may not reregister"

- Returned by the server to any link which tries to
 change part of the registered details (such as
 password or user details from second USER message).
*/

#define ERR_PASSWDMISMATCH 464
/*
":Password incorrect"
- Returned to indicate a failed attempt at registering a connection for which a password was
 required and was either not given or incorrect.*/
/*
If you send wrong pass OR you send nick/user BEFORE the pass
*/

#define ERR_CHANNELISFULL     "471" // "<channel> :Cannot join channel (+l)"
#define ERR_INVITEONLYCHAN    "473" // "<channel> :Cannot join channel (+i)"
#define ERR_BANNEDFROMCHAN    "474" // "<channel> :Cannot join channel (+b)"
#define ERR_NOPRIVILEGES      "481" // ":Permission Denied- You're not an IRC operator"