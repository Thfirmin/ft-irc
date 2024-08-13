# ft-irc

This is a simple IRC server project from 42 school


## Summary

- [Description][##Description]
- [Installation][##Installation]
- [Usation][##Usation]


## Description

ft_irc is a Internet Relay Chat (IRC) server implemented in C++. IRC its a protocol for message in real time and communication on a distributed network environment.

> Compatible with IRC client (HexChat).


## Installation

```bash
git clone git@github:Thfirmin/ft-irc.git
cd ft_irc
make
```


## Usation

### Starting

To start the server:

```bash
./ircserv <PORT_NUMBER> <PASSWORD>
```

### Connecting

At this point, the server'll be in execution and ready for connections. You can use any IRC client or simply use the `nc` (NetCat) tool like this:

```bash
nc localhost <PORT_NUMBER>
```

After this, authenticate your clinet using the following commands:

```bash
PASS <PASSWORD>
NICK <NICKNAME>
USER <USERNAME> 0 * <REAL_NAME>
```

> The `0` and `*` arguments are needed and constant, don't change them!!!

After start the server and connect to it, you can use many commands to intercat with other users and manage channels.


### Changing Nickname

to change your nickname, use:

```bash
NICK <NEW_NICKNAME>
```


### Send Message

Use `PRIVMSG` to send a message

To send it to an user, insert him nickname at this form:

```IRC
PRIVMSG <NICKNAME> : <MESSAGE>
```

To send it to a channel, insert `#` with the channel name at this form:

```IRC
PRIVMSG #<CHANNEL_NAME> : <MESSAGE>
```


### Leave the Server

To disconnect the server, use `QUIT` command:

```bash
QUIT : <MESSAGE>
```


### Channel Commands

#### Join/Create a Channel
---

To join in a channel, use `JOIN` command:

```bash
JOIN #<CHANNEL_NAME>
```

> If the channel doesn't exist, the server'll cretae one and insert you as ther first participant

After entering the channel, you'll see a welcome message followed by a user list on the channel.

#### Invite to the Channel
---

To invite a user to the channel, use `INVITE` command:

```bash
INVITE <NICKNAME> #<CHANNEL_NAME>
```

#### Kick From the Channel
---

To kick a user from the channel, you can use `KICK` command:

```bash
KICK #<CHANNEL_NAME> <NICKNAME> [<REASON>]
```

> Reason is optional and may be only one word (can't have space character in there)

#### Leave the Channel
---

To leave the channel, you can use `PART` command:

```bash
PART #<CHANNEL_NAME> [<REASON>]
```

> Reason is optional and may be only one word (can't have space character in there)

Or `KICK` command using your own nickname as argument:

```bash
KICK #<CHANNEL_NAME> <MY_NICKNAME> [<REASON>]
```

1. **Set user as channel operator:**
   ```bash
   mode <#channel> o <user>
   ```

## Conclusion

ft_irc allows you to create and manage an IRC server with support for several essential features of the IRC protocol.
You can start the server, connect, authenticate users, join channels, exchange messages and much more.
