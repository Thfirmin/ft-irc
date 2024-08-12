# ft-irc

Simple IRC server project from 42 school

## Descrição

ft_irc é um servidor de Internet Relay Chat (IRC) em C++. O IRC é um protocolo para mensagens em tempo real e comunicação em um ambiente de rede distribuída.

## Funcionalidades
- Arquitetura para lidar com conexões simultâneas de clientes.
- Suporte para múltiplas conexões simultâneas.
- Criação e gerenciamento de canais IRC.
- Autenticação e registro de usuários.
- Transmissão de mensagens para todos os usuários em um canal.
- Mensagens privadas entre usuários.
- Manipulação de vários comandos IRC, como join, PRIVMSG, part, etc.
- Suporte para apelidos de usuários e nomes de canais.
- participação em canais e conversas em grupo.
- Envio e recebimento de mensagens.
- Alteração de apelidos de usuários.
- Envio de mensagens privadas para outros usuários.
- Compatibilidade com cliente IRC (hexchat).

## Instalação
```bash
git clone <repository_url>
cd ft_irc
make
```

## Uso
Para iniciar o servidor:
```bash
./ircserv <número da porta> <senha>
```
Neste ponto, o servidor estará em execução e pronto para conexões. Você pode usar qualquer cliente IRC ou simplesmente usar a ferramenta `nc` (NetCat) assim:
```bash
nc localhost <número da porta>
```
Depois disso, autentique seu cliente usando os seguintes comandos:
```bash
pass <senha>
nick <apelido>
user <nome de usuário> 0 * <nome real>  # Deve ser composto por 4 argumentos
```

## Funcionalidade Extra (BONUS)
Para a parte bônus, há um bot cliente que envia requisições a um LLM local (ollama). Para usar o bot, você pode executar:
```bash
make bonus
./bot <endereço> <porta> <senha> <nome_do_bot>
```
Você pode se comunicar com o bot usando outro cliente através do comando `PRIVMSG`, como:
```bash
privmsg bot :sua pergunta
```
E o bot responderá:
```bash
:bot!~bot@localhost PRIVMSG user :resposta
```

## Exemplos de Uso do Servidor IRC

Depois de iniciar o servidor e se conectar a ele, você pode usar vários comandos para interagir com outros usuários e gerenciar canais.

### Criação e participação em Canais

#### Criar e Entrar em um Canal
```bash
join #novocanal
```
Após entrar no canal, você verá uma mensagem de boas-vindas e a lista de usuários no canal.

#### Enviar Mensagens para um Canal
Para enviar uma mensagem para todos os usuários de um canal, use:
```bash
PRIVMSG #novocanal :Olá pessoal!
```

### Mensagens Privadas

#### Enviar uma Mensagem Privada para um Usuário
Para enviar uma mensagem privada para um usuário específico, use:
```bash
PRIVMSG user :Oi, como vai?
```

### Mudança de Nickname

#### Alterar o Nickname
Para mudar seu nickname, use:
```bash
NICK novo_nickname
```

### Sair de um Canal

#### Sair de um Canal
Para sair de um canal, use:
```bash
part #novocanal
```

### Desconexão do Servidor

#### Desconectar do Servidor
Para se desconectar do servidor, use:
```bash
QUIT :Até mais!
```

## Comandos Adicionais do IRC

Aqui estão alguns comandos adicionais que você pode achar úteis:

### Listar Canais Disponíveis
```bash
LIST
```

### Obter Informações Sobre um Usuário
```bash
WHOIS apelido_usuario
```

### Enviar Notificações a Todos os Usuários
Para enviar uma notificação global (geralmente reservado para operadores):
```bash
WALLOPS :Mensagem para todos
```

## Exemplos de Sessões Completas

### Exemplo 1: Sessão Básica de Chat

1. **Iniciar o servidor:**
   ```bash
   ./ircserv 4444 mypassword
   ```

2. **Conectar ao servidor:**
   ```bash
   nc localhost 4444
   ```

3. **Autenticar e registrar:**
   ```bash
   pass mypassword
   nick usuario1
   user usuario1 0 * NomeCompleto
   ```

4. **Criar e entrar em um canal:**
   ```bash
   join #geral
   ```

5. **Enviar uma mensagem para o canal:**
   ```bash
   PRIVMSG #geral :Oi, estou testando o servidor IRC!
   ```

6. **Sair do canal e desconectar:**
   ```bash
   part #geral
   QUIT :Até a próxima!
   ```

### Exemplo 2: Sessão com Mensagem Privada e Mudança de Apelido

1. **Iniciar o servidor:**
   ```bash
   ./ircserv 4444 mypassword
   ```

2. **Conectar ao servidor:**
   ```bash
   nc localhost 4444
   ```

3. **Autenticar e registrar:**
   ```bash
   pass mypassword
   nick usuario2
   user usuario2 0 * OutroNome
   ```

4. **Enviar uma mensagem privada:**
   ```bash
   PRIVMSG usuario1 :Olá, você está aí?
   ```

5. **Mudar de apelido:**
   ```bash
   NICK novo_usuario2
   ```

6. **Desconectar:**
   ```bash
   QUIT :Tchau!
   ```

### Exemplo 3: Mudança de modo do canal

1. **Set user as channel operator:**
   ```bash
   mode <#channel> o <user>
   ```

2. **Operator command:**
   ```bash
   kick <#channel> <user> <reason - only one word>
   ```

3. **WIP:**
   ```bash
   wip
   ```

## Conclusão

O ft_irc permite a criação e gerenciamento de um servidor IRC com suporte para várias funcionalidades essenciais do protocolo IRC. Você poderá iniciar o servidor, conectar-se, autenticar usuários, participar de canais, trocar mensagens e muito mais.
