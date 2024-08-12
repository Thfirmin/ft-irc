##comandos do IRC
- `privmsg`
- `join`
- `part`
- `quit`
- `invite`
- `kick`
- `mode`
- `topic`


### iniciar o servidor
```bash
./ircserv 4444 123
```
sendo  4444 a porta
e 123 a senha do servidor

---

### conectar usuário pelo terminal
```bash
nc localhost 4444
```
sendo nc o client irc que opera via terminal e 
servidor e depois a porta

```bash
pass 123
nick thino
user thfirmin * 0 thfirmin
```
acima cada linha indica o password, nick no servidor e por fim user.

### explicando o user (devido o uso do netcat)
```bash
pass 123
nick thino
user thfirmin * 0 thfirmin
```
- * instrui o servidor a determinar o host automaticamente 
- 0 instrui o servidor a determinar automaticamente o nome do servidor

---


### envio de mensagens privadas entre usuários ou dentro de "canais"
```bash
privmsg <target> : <mensagem> 
```
para sala *não esquecer da tralha
```bash
join #<nome do canal>
privmsg #<target> : <mensagem> 
```

---

### comando join *usado para entrar em um canal
```bash
join #<nome do canal>
```

### comando invite *usado para convidar um usuário específico para um canal
```bash
INVITE <nickname> <#canal>
```
exemplo de uso:
```bash
INVITE Joao #chat
```
*O comando INVITE é útil **quando um canal possui o modo de convite** `+i`, o que significa que apenas usuários convidados podem entrar no canal.

### comando kick *usado para remover um usuário específico de um canal 
```bash
KICK <#canal> <nickname> [<motivo>]
```
`[<motivo>]`: É opcional e representa o motivo pelo qual o usuário está sendo removido do canal.

exemplo de uso:
```bash
KICK #channel Joao 
```
ou 

```bash
KICK #channel Joao : Agressivo
```

---

### comando mode
```bash
MODE #canal +/-modo parâmetros
```
- `#canal`: É o nome do canal no qual você deseja modificar os modos.
- `+/-modo`: Indica se você está adicionando (+) ou removendo (-) um modo.
- `parâmetros`: São valores específicos associados ao modo que está sendo modificado. Por exemplo, se você estiver configurando o modo de convite (`+i`) para o canal, poderá adicionar um parâmetro que consiste em uma lista de usuários autorizados a convidar outros usuários para o canal.

**Modos de Canal**
- `+t` `-t`: Adiciona/remove tópico restrito. Somente operadores do canal podem alterar o tópico.
- `+i` `-i`: Apenas usuários autorizados podem acessar o canal.
- `+k` `-k`: Adiciona/remove senha de canal.
- `+o` `-o`: Fornece/remove privilégios de operador de canal para usuários.
- `+l` `-l`: Define/remove limite de usuários para o canal.


**Modificação do Modo do Usuário**
```bash
MODE <#channel> +/-modo <nickname>
```
`+/-modo`: Indica se você está adicionando (+) ou removendo (-) um modo.

**definindo número de usuários do canal**
Adicionar restrições de número de usuários de um canal.
```bash
MODE #channel +l <number of users>
```
Remover restrições de número de usuários de um canal.
```bash
MODE #channel -l
```

---
### comando quit *usado para desconectar um servidr  
Vale dizer que em caso de uso do `nc`, será realizado a saída do servidor mas não do cliente de conexão com os servers.
```bash
QUIT : [<motivo>]
```
- `[<motivo>]`: É opcional e representa o motivo pelo qual você está saindo do canal. 

exemplo de uso:
```bash
QUIT : pegue seu banquinho e saia de mansinho
```

### comando part *usado para fazer um usuário sair de um canal específico 
Semelhante ao comando `QUIT`, que é usado para sair completamente do servidor IRC. Aqui está a sintaxe básica do comando PART
```bash
PART <#canal> [<motivo>]
```
- `<#canal>`: É o nome do canal do qual você deseja sair.
- `[<motivo>]`: É opcional e representa o motivo pelo qual você está saindo do canal. 

exemplo de uso:
```bash
PART #chat
```
ou
```bash
PART #chat : Indo embora
```

---

### comando topic *usado para definir ou obter o tópico de um canal.

```bash
TOPIC #canal : [novo_tópico]
```
`#canal`: É o nome do canal para o qual você deseja obter ou definir o tópico.
`[novo_tópico]`: É opcional e representa o novo tópico que você deseja definir para o canal.

**obtendo um tópico de um canal**
Se você deseja apenas obter o tópico atual de um canal, você pode usar o comando TOPIC sem especificar um novo tópico:
```bash
TOPIC #channel
```
**definindo um tópico de um canal**
Se você tem permissões para definir o tópico de um canal, poderá definir o tópico do canal.
```bash
TOPIC #channel : Discussões Gerais
```
**definindo tópico restrito**
Adicionar/remover permissões para definir o tópico de um canal.
```bash
MODE #channel +t/-t
```
