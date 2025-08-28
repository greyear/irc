#pragma once


#define 
#define 
#define 
#define 
#define 
#define 

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

#define ERR_NOTREGISTERED 451
/*
":You have not registered"
- Returned to indicate that the client MUST be registered before the server will allow it
 to be parsed in detail.*/
 /*
 You tried the command BEFORE you've registered!
 */

#define ERR_NEEDMOREPARAMS 461
/*
"<command> :Not enough parameters"
- Returned to indicate to the client that it didn't supply enough parameters.*/

#define ERR_PASSWDMISMATCH 464
/*
":Password incorrect"
- Returned to indicate a failed attempt at registering a connection for which a password was
 required and was either not given or incorrect.*/
/*
If you send wrong pass OR you send nick/user BEFORE the pass
*/



#define 
#define 
#define 
#define 
#define 
#define 
#define 
#define 