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

```IRC
git clone git@github:Thfirmin/ft-irc.git
cd ft_irc
make
```


## Usation

### Starting

To start the server:

```IRC
./ircserv <PORT_NUMBER> <PASSWORD>
```

### Connecting

At this point, the server'll be in execution and ready for connections. You can use any IRC client or simply use the `nc` (NetCat) tool like this:

```IRC
nc localhost <PORT_NUMBER>
```

After this, authenticate your clinet using the following commands:

```IRC
PASS <PASSWORD>
NICK <NICKNAME>
USER <USERNAME> 0 * <REAL_NAME>
```

> The `0` and `*` arguments are needed and constant, don't change them!!!

After start the server and connect to it, you can use many commands to intercat with other users and manage channels.


### Changing Nickname

to change your nickname, use:

```IRC
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

```IRC
QUIT [:<MESSAGE>]
```

> The message is optional

### Channel Commands

#### Join/Create a Channel
---

To join in a channel, use `JOIN` command:

```IRC
JOIN #<CHANNEL_NAME> [<PASSWORD>]
```

> If the channel doesn't exist, the server'll cretae one and insert you as ther first participant
> Use the third argument if channel have password

After entering the channel, you'll see a welcome message followed by a user list on the channel.

#### Invite to the Channel
---

To invite a user to the channel, use `INVITE` command:

```IRC
INVITE <NICKNAME> #<CHANNEL_NAME>
```

#### Change Topic of Channel
---

To change the topic of channel use `TOPIC` command:

```IRC
TOPIC #<CHANNEL_NAME> <NEW_TOPIC>
```

#### Change Mode Setting of Channel
---

To change mode of setting the channel, use the command `MODE` command:

```IRC
MODE #<CHANNEL_NAME> +/-<MODE> [<PARAMETER>]
```

> Use `+` to apply the mode and `-` to exclude the mode
> Some modes need aditional parameter

- i: Just invited user can join the channel
- k: Appy password to enter the channel (Need argument: <PASSWORD>)
- o: Give operator (ADM) access to a user (Need argument: <NICKNAME>)
- t: Only operators can change topic of the channel
- l: Give Channel maximum number of users (Need argument: <MAX_SIZE>)

#### Kick From the Channel
---

To kick a user from the channel, you can use `KICK` command:

```bash
KICK #<CHANNEL_NAME> <NICKNAME> [:<REASON>]
```

> Reason is optional

#### Leave the Channel
---

To leave the channel, you can use `PART` command:

```bash
PART #<CHANNEL_NAME> [:<REASON>]
```

> Reason is optional

Or `KICK` command using your own nickname as argument:

```bash
KICK #<CHANNEL_NAME> <MY_NICKNAME> [:<REASON>]
```


## Conclusion

O ft_irc permite a criação e gerenciamento de um servidor IRC com suporte para várias funcionalidades essenciais do protocolo IRC. Você poderá iniciar o servidor, conectar-se, autenticar usuários, participar de canais, trocar mensagens e muito mais.
