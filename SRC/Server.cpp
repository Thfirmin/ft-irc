#include "../INC/Server.hpp"

/**
 * Indica se o servidor está cheio de bots.
 * 
 * Esta variável é usada para monitorar se o servidor atingiu a capacidade máxima de bots.
 */
bool Server::isBotfull = false;

/**
 * Construtor da classe Server.
 * 
 * Inicializa a instância do servidor com o descritor de socket definido como -1.
 */
Server::Server(){this->server_fdsocket = -1;}

/**
 * Destrutor da classe Server.
 * 
 * Realiza a limpeza necessária quando um objeto Server é destruído.
 */
Server::~Server(){}

/**
 * Construtor de cópia da classe Server.
 * 
 * @param src Objeto Server a ser copiado.
 * Copia os valores do objeto src para o objeto atual.
 */
Server::Server(Server const &src){*this = src;}

/**
 * Operador de atribuição para a classe Server.
 * 
 * @param src Objeto Server a ser copiado.
 * @return Referência para o objeto Server atual.
 * 
 * Realiza a cópia dos atributos do objeto src para o objeto atual, evitando auto-atribuição.
 */
Server &Server::operator=(Server const &src){
	if (this != &src){
		/*
		struct sockaddr_in add;
		struct sockaddr_in cliadd;
		struct pollfd new_cli;
		*/
		this->port = src.port;
		this->server_fdsocket = src.server_fdsocket;
		this->password = src.password;
		this->clients = src.clients;
		this->channels = src.channels;
		this->fds = src.fds;
		this->isBotfull = src.isBotfull;
	}
	return *this;
}

/**
 * Obtém o número da porta do servidor.
 * 
 * @return O número da porta do servidor.
 */
int Server::GetPort(){return this->port;}

/**
 * Obtém o descritor de socket do servidor.
 * 
 * @return O descritor de socket do servidor.
 */
int Server::GetFd(){return this->server_fdsocket;}

/**
 * Obtém um cliente pelo descritor de arquivo.
 * 
 * @param fd Descritor de arquivo do cliente.
 * @return Ponteiro para o objeto Client correspondente ao descritor de arquivo, ou NULL se não encontrado.
 */
Client *Server::GetClient(int fd){
	for (size_t i = 0; i < this->clients.size(); i++){
		if (this->clients[i].GetFd() == fd)
			return &this->clients[i];
	}
	return NULL;
}

/**
 * Obtém um cliente pelo apelido (nickname).
 * 
 * @param nickname O apelido do cliente.
 * @return Ponteiro para o objeto Client correspondente ao apelido, ou NULL se não encontrado.
 */
Client *Server::GetClientNick(std::string nickname){
	for (size_t i = 0; i < this->clients.size(); i++){
		if (this->clients[i].GetNickName() == nickname)
			return &this->clients[i];
	}
	return NULL;
}

/**
 * Obtém um canal pelo nome.
 * 
 * @param name O nome do canal.
 * @return Ponteiro para o objeto Channel correspondente ao nome, ou NULL se não encontrado.
 */
Channel *Server::GetChannel(std::string name)
{
	for (size_t i = 0; i < this->channels.size(); i++){
		if (this->channels[i].GetName() == name)
			return &channels[i];
	}
	return NULL;
}

/**
 * Define o descritor de socket do servidor.
 * 
 * @param fd O novo descritor de socket.
 */
void Server::SetFd(int fd){this->server_fdsocket = fd;}

/**
 * Define o número da porta do servidor.
 * 
 * @param port O novo número da porta.
 */
void Server::SetPort(int port){this->port = port;}

/**
 * Define a senha do servidor.
 * 
 * @param password A nova senha.
 */
void Server::SetPassword(std::string password){this->password = password;}

/**
 * Define a senha do servidor.
 * 
 * @param password A nova senha.
 */
std::string Server::GetPassword(){return this->password;}

/**
 * Adiciona um novo cliente à lista de clientes do servidor.
 * 
 * @param newClient O novo cliente a ser adicionado.
 */
void Server::AddClient(Client newClient){this->clients.push_back(newClient);}

/**
 * Adiciona um novo canal à lista de canais do servidor.
 * 
 * @param newChannel O novo canal a ser adicionado.
 */
void Server::AddChannel(Channel newChannel){this->channels.push_back(newChannel);}

/**
 * Adiciona um novo descritor de arquivo à lista de descritores monitorados pelo servidor.
 * 
 * @param newFd O novo descritor de arquivo a ser adicionado.
 */
void Server::AddFds(pollfd newFd){this->fds.push_back(newFd);}

/**
 * Remove um cliente da lista de clientes do servidor pelo descritor de arquivo.
 * 
 * @param fd O descritor de arquivo do cliente a ser removido.
 */
void Server::RemoveClient(int fd){
	for (size_t i = 0; i < this->clients.size(); i++){
		if (this->clients[i].GetFd() == fd)
			{this->clients.erase(this->clients.begin() + i); return;}
	}
}

/**
 * Remove um canal da lista de canais do servidor pelo nome.
 * 
 * @param name O nome do canal a ser removido.
 */
void Server::RemoveChannel(std::string name){
	for (size_t i = 0; i < this->channels.size(); i++){
		if (this->channels[i].GetName() == name)
			{this->channels.erase(this->channels.begin() + i); return;}
	}
}

/**
 * Remove um descritor de arquivo da lista de descritores monitorados pelo servidor.
 * 
 * @param fd O descritor de arquivo a ser removido.
 */
void Server::RemoveFds(int fd){
	for (size_t i = 0; i < this->fds.size(); i++){
		if (this->fds[i].fd == fd)
			{this->fds.erase(this->fds.begin() + i); return;}
	}
}

/**
 * Remove um cliente de todos os canais em que está presente.
 * 
 * @param fd O descritor de arquivo do cliente a ser removido dos canais.
 */
void	Server::RmChannels(int fd){
	for (size_t i = 0; i < this->channels.size(); i++){
		int flag = 0;
		if (channels[i].get_client(fd))
			{channels[i].removeClient(fd); flag = 1;}
		else if (channels[i].get_admin(fd))
			{channels[i].removeAdmin(fd); flag = 1;}
		if (channels[i].GetClientsNumber() == 0)
			{channels.erase(channels.begin() + i); i--; continue;}
		if (flag){
			std::string rpl = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost QUIT Quit\r\n";
			channels[i].sendToAll(rpl);
		}
	}
}

/**
 * Envia uma mensagem de erro para um cliente específico.
 * 
 * @param code Código de erro.
 * @param clientname Nome do cliente.
 * @param fd Descritor de arquivo do cliente.
 * @param msg Mensagem de erro adicional.
 */
void Server::senderror(int code, std::string clientname, int fd, std::string msg)
{
	std::stringstream ss;
	ss << ":localhost " << code << " " << clientname << msg;
	std::string resp = ss.str();
	if(send(fd, resp.c_str(), resp.size(),0) == -1)
		std::cerr << "send() faild" << std::endl;
}

/**
 * Envia uma mensagem de erro para um cliente específico dentro de um canal.
 * 
 * @param code Código de erro.
 * @param clientname Nome do cliente.
 * @param channelname Nome do canal.
 * @param fd Descritor de arquivo do cliente.
 * @param msg Mensagem de erro adicional.
 */
void Server::senderror(int code, std::string clientname, std::string channelname, int fd, std::string msg)
{
	std::stringstream ss;
	ss << ":localhost " << code << " " << clientname << " " << channelname << msg;
	std::string resp = ss.str();
	if(send(fd, resp.c_str(), resp.size(),0) == -1)
		std::cerr << "send() faild" << std::endl;
}

/**
 * Envia uma resposta para um cliente específico.
 * 
 * @param response A resposta a ser enviada.
 * @param fd Descritor de arquivo do cliente.
 */
void Server::_sendResponse(std::string response, int fd)
{
	if(send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() faild" << std::endl;
}

/**
 * Indica se um sinal foi recebido.
 * 
 * Esta variável é usada para monitorar se o servidor recebeu um sinal.
 */
bool Server::Signal = false;

/**
 * Manipulador de sinais para o servidor.
 * 
 * @param signum O número do sinal recebido.
 * 
 * Define a variável Server::Signal como true quando um sinal é recebido e exibe uma mensagem.
 */
void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::Signal = true;
}

/**
 * Fecha todos os descritores de arquivo associados aos clientes e ao servidor.
 * 
 * Exibe uma mensagem indicando o fechamento de cada cliente e do servidor.
 */
void	Server::closeFds(){
	for(size_t i = 0; i < clients.size(); i++){
		std::cout << RED << "Client <" << clients[i].GetFd() << "> Disconnected" << WHI << std::endl;
		close(clients[i].GetFd());
	}
	if (server_fdsocket != -1){	
		std::cout << RED << "Server <" << server_fdsocket << "> Disconnected" << WHI << std::endl;
		close(server_fdsocket);
	}
}

/**
 * Inicializa o servidor com a porta e senha especificadas.
 * 
 * @param port O número da porta para o servidor.
 * @param pass A senha do servidor.
 * 
 * Configura o socket do servidor e aguarda novas conexões. Em um loop, utiliza `poll` para monitorar eventos nos descritores
 * de arquivo. Aceita novos clientes ou recebe dados dos clientes existentes, conforme necessário.
 * 
 * @throws std::runtime_error Se ocorrer algum erro durante a configuração do socket ou na chamada a `poll`.
 */
void Server::init(int port, std::string pass)
{
	this->password = pass;
	this->port = port;
	this->setSeverSocket();

	std::cout << "Waiting to accept a connection...\n";
	while (Server::Signal == false)
	{
		if((poll(&fds[0],fds.size(),-1) == -1) && Server::Signal == false)
			throw(std::runtime_error("poll() faild"));
		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == server_fdsocket)
					this->acceptNewClient();
				else
					this->reciveNewData(fds[i].fd);
			}
		}
	}
	closeFds();
}

/**
 * Configura o socket do servidor.
 * 
 * Configura o socket do servidor, define opções de socket, torna o socket não bloqueante, associa o socket ao endereço e 
 * inicia a escuta de conexões.
 * 
 * @throws std::runtime_error Se ocorrer algum erro na criação do socket, definição de opções, associação ou escuta.
 */
void Server::setSeverSocket()
{
	int en = 1;
	add.sin_family = AF_INET;
	add.sin_addr.s_addr = INADDR_ANY;
	add.sin_port = htons(port);
	server_fdsocket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fdsocket == -1)
		throw(std::runtime_error("faild to create socket"));
	if(setsockopt(server_fdsocket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	 if (fcntl(server_fdsocket, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	if (bind(server_fdsocket, (struct sockaddr *)&add, sizeof(add)) == -1)
		throw(std::runtime_error("faild to bind socket"));
	if (listen(server_fdsocket, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() faild"));
	new_cli.fd = server_fdsocket;
	new_cli.events = POLLIN;
	new_cli.revents = 0;
	fds.push_back(new_cli);
}

/**
 * Aceita uma nova conexão de cliente.
 * 
 * Aceita um novo cliente que está tentando se conectar ao servidor. Define o descritor de arquivo do cliente,
 * torna o descritor de arquivo não bloqueante, armazena o cliente na lista de clientes e adiciona o descritor
 * de arquivo do cliente na lista de descritores monitorados.
 */
void Server::acceptNewClient()
{
	Client cli;
	memset(&cliadd, 0, sizeof(cliadd));
	socklen_t len = sizeof(cliadd);
	int incofd = accept(server_fdsocket, (sockaddr *)&(cliadd), &len);
	if (incofd == -1)
		{std::cout << "accept() failed" << std::endl; return;}
	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1)
		{std::cout << "fcntl() failed" << std::endl; return;}
	new_cli.fd = incofd;
	new_cli.events = POLLIN;
	new_cli.revents = 0;
	cli.SetFd(incofd);
	cli.setIpAdd(inet_ntoa((cliadd.sin_addr)));
	clients.push_back(cli);
	fds.push_back(new_cli);
	std::cout << GRE << "Client <" << incofd << "> Connected" << WHI << std::endl;
}

/**
 * Recebe novos dados de um cliente.
 * 
 * @param fd O descritor de arquivo do cliente.
 * 
 * Recebe dados do cliente associado ao descritor de arquivo especificado. Os dados são armazenados
 * no buffer do cliente e são processados conforme necessário. Se não houver mais dados para receber
 * ou se ocorrer um erro de recepção, o cliente é desconectado e removido das estruturas de dados do servidor.
 */
void Server::reciveNewData(int fd)
{
	std::vector<std::string> cmd;
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	Client *cli = GetClient(fd);
	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0);
	if(bytes <= 0)
	{
		std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
		RmChannels(fd);
		RemoveClient(fd);
		RemoveFds(fd);
		close(fd);
	}
	else
	{ 
		cli->setBuffer(buff);
		if(cli->getBuffer().find_first_of("\r\n") == std::string::npos)
			return;
		cmd = splitReceivedBuffer(cli->getBuffer());
		for(size_t i = 0; i < cmd.size(); i++)
			this->parseExecCmd(cmd[i], fd);
		if(GetClient(fd))
			GetClient(fd)->clearBuffer();
	}
}

/**
 * Divide uma string recebida em várias linhas.
 * 
 * @param str A string a ser dividida em linhas.
 * @return Um vetor de strings contendo as linhas da string original.
 * 
 * Divide a string recebida em várias linhas com base nos caracteres de quebra de linha '\r' e '\n'.
 */
std::vector<std::string> Server::splitReceivedBuffer(std::string str)
{
	std::vector<std::string> vec;
	std::istringstream stm(str);
	std::string line;
	while(std::getline(stm, line))
	{
		size_t pos = line.find_first_of("\r\n");
		if(pos != std::string::npos)
			line = line.substr(0, pos);
		vec.push_back(line);
	}
	return vec;
}

/**
 * Divide uma string em tokens separados por espaços.
 * 
 * @param cmd A string a ser dividida em tokens.
 * @return Um vetor de strings contendo os tokens da string original.
 * 
 * Divide a string recebida em tokens com base nos espaços em branco.
 */
std::vector<std::string> Server::splitCmd(std::string& cmd)
{
	std::vector<std::string> vec;
	std::istringstream stm(cmd);
	std::string token;
	while(stm >> token)
	{
		vec.push_back(token);
		token.clear();
	}
	return vec;
}

/**
 * Verifica se um cliente não está registrado corretamente.
 * 
 * @param fd O descritor de arquivo do cliente.
 * @return Retorna true se o cliente não estiver registrado corretamente, caso contrário, retorna false.
 * 
 * Verifica se o cliente associado ao descritor de arquivo especificado não está registrado corretamente.
 * Um cliente é considerado não registrado se não tiver um apelido (nickname) ou um nome de usuário (username) definidos,
 * ou se estiver marcado como não logado.
 */
bool Server::notRegistered(int fd)
{
	if (!GetClient(fd) || GetClient(fd)->GetNickName().empty() || GetClient(fd)->GetUserName().empty() || GetClient(fd)->GetNickName() == "*"  || !GetClient(fd)->GetLogedIn())
		return false;
	return true;
}

/**
 * Analisa e executa um comando recebido do cliente.
 * 
 * @param cmd O comando recebido do cliente.
 * @param fd O descritor de arquivo do cliente.
 * 
 * Analisa o comando recebido do cliente, executa as ações correspondentes com base no comando e no estado do cliente.
 * Os comandos possíveis incluem autenticação, definição de apelido e nome de usuário, ações em canais (como JOIN, TOPIC, MODE, PART),
 * mensagens PRIVMSG, convites (INVITE), e outros. Se o cliente não estiver registrado corretamente, as ações são limitadas.
 */
void Server::parseExecCmd(std::string &cmd, int fd)
{
	if(cmd.empty())
		return ;
	std::vector<std::string> splited_cmd = splitCmd(cmd);
	size_t found = cmd.find_first_not_of(" \t\v");
	if(found != std::string::npos)
		cmd = cmd.substr(found);
	if(splited_cmd.size() && (splited_cmd[0] == "BONG" || splited_cmd[0] == "bong"))
		return;
    if(splited_cmd.size() && (splited_cmd[0] == "PASS" || splited_cmd[0] == "pass"))
        clientAuthen(fd, cmd);
	else if (splited_cmd.size() && (splited_cmd[0] == "NICK" || splited_cmd[0] == "nick"))
		setNickname(cmd,fd);
	else if(splited_cmd.size() && (splited_cmd[0] == "USER" || splited_cmd[0] == "user"))
		setUsername(cmd, fd);
	else if (splited_cmd.size() && (splited_cmd[0] == "QUIT" || splited_cmd[0] == "quit"))
		Quit(cmd,fd);
	else if(notRegistered(fd))
	{
		if (splited_cmd.size() && (splited_cmd[0] == "KICK" || splited_cmd[0] == "kick"))
			Kick(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "JOIN" || splited_cmd[0] == "join"))
			Join(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "TOPIC" || splited_cmd[0] == "topic"))
			Topic(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "MODE" || splited_cmd[0] == "mode"))
			modeCommand(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "PART" || splited_cmd[0] == "part"))
			Part(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "PRIVMSG" || splited_cmd[0] == "privmsg"))
			Privmsg(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "INVITE" || splited_cmd[0] == "invite"))
			Invite(cmd,fd);
		else if (splited_cmd.size())
			_sendResponse(ERR_CMDNOTFOUND(GetClient(fd)->GetNickName(),splited_cmd[0]),fd);
	}
	else if (!notRegistered(fd))
		_sendResponse(ERR_notRegistered(std::string("*")),fd);
}