#include "Bot.hpp"

/**
 * Construtor da classe Bot.
 * Inicializa o estado de login como falso.
 */
Bot::Bot() : host("localhost"), path("/api/generate"), port(11434) { Loggedin = false; }

/**
 * Destrutor da classe Bot.
 */
Bot::~Bot() {}

/**
 * Construtor de cópia da classe Bot.
 * Realiza uma cópia profunda dos atributos da instância fornecida.
 *
 * @param other A instância a ser copiada.
 */
Bot::Bot(const Bot &other) : host(other.host), path(other.path), port(other.port)
{
	Loggedin = other.Loggedin;
	botnick = other.botnick;
}

/**
 * Operador de atribuição da classe Bot.
 * Realiza uma cópia profunda dos atributos da instância fornecida.
 *
 * @param other A instância a ser copiada.
 * @return Uma referência para a instância após a cópia.
 */
Bot &Bot::operator=(const Bot &other)
{
	if (this != &other)
	{
		this->Loggedin = other.Loggedin; // Copia o estado de login do bot.
		this->botnick = other.botnick;	 // Copia o nickname do bot.
	}
	return *this;
}

/**
 * Define o nickname do bot.
 *
 * @param nick O novo nickname para o bot.
 */
void Bot::setNick(std::string nick)
{
	botnick = nick;
}

/**
 * Envia uma mensagem genérica para o socket especificado.
 *
 * @param message A mensagem a ser enviada.
 * @param fd O descritor de arquivo do socket.
 */
void Bot::sendMessage(std::string message, int fd)
{
	if (send(fd, message.c_str(), message.size(), 0) == -1)
		std::cerr << "Send failed" << std::endl;
}

/**
 * Envia uma mensagem PRIVMSG para um usuário específico no servidor IRC.
 *
 * @param message A mensagem a ser enviada.
 * @param UserNick O nickname do usuário para o qual a mensagem será enviada.
 * @param ircsock O descritor de arquivo do socket IRC.
 */
void Bot::sendPrivmsg(std::string message, std::string UserNick, int ircsock)
{
	std::string msg = "PRIVMSG " + UserNick + " :" + message + "\r\n";
	if (send(ircsock, msg.c_str(), msg.size(), 0) == -1)
		std::cerr << "Send failed" << std::endl;
}

/**
 * Extrai o nickname e o comando de uma mensagem IRC recebida.
 *
 * @param received A mensagem IRC recebida.
 * @param nick Variável para armazenar o nickname extraído.
 * @param command Variável para armazenar o comando extraído.
 */
void Bot::getCommand(std::string received, std::string &nick, std::string &command)
{
	// Remove o caractere ':' do início da mensagem, se presente
	if (received[0] == ':')
		received.erase(received.begin());

	// Encontra a posição do primeiro '!' para extrair o nickname
	size_t pos = received.find('!');
	nick = received.substr(0, pos);
	pos = std::string::npos;

	// Encontra a posição do primeiro ':' para extrair o comando
	pos = received.find(':');
	command = received.substr(pos);

	// Remove o caractere ':' do início do comando, se presente
	if (!command.empty() && command[0] == ':')
		command.erase(command.begin());
}

/**
 * Remove a primeira palavra de uma string.
 *
 * @param str A string da qual a primeira palavra será removida.
 * @return A string resultante após a remoção da primeira palavra. Se a string não contiver espaços, retorna uma string vazia.
 */
std::string Bot::removeFirstWord(const std::string &str)
{
	size_t pos = str.find(' ');
	if (pos == std::string::npos)
	{
		return "";
	}
	else
	{
		return str.substr(pos + 1);
	}
}

/**
 * Extrai o valor de um campo JSON de uma string.
 *
 * @param line A string contendo o campo JSON.
 * @param field O nome do campo JSON a ser extraído.
 * @return O valor do campo JSON extraído.
 */
std::string Bot::extractJsonValue(const std::string &json, const std::string &key)
{
	std::string key_with_quotes = "\"" + key + "\":";
	size_t start = json.find(key_with_quotes);
	if (start == std::string::npos)
	{
		return "";
	}
	start += key_with_quotes.length();
	size_t end = json.find_first_of(",}", start);
	if (end == std::string::npos)
	{
		end = json.length();
	}
	std::string value = json.substr(start, end - start);
	if (value[0] == '"')
	{
		value = value.substr(1, value.length() - 2);
	}
	size_t pos = 0;
	while ((pos = value.find("\\n", pos)) != std::string::npos)
	{
		value.replace(pos, 2, "\n");
		pos += 1;
	}
	return value;
}

/**
 * Envia uma solicitação HTTP POST para um servidor remoto.
 * A função envia um prompt para um modelo de linguagem GPT e recebe uma resposta.
 * A resposta é enviada como uma mensagem para um cliente IRC.
 *
 * @param prompt O prompt a ser enviado para o modelo GPT.
 * @param Nickname O nome do cliente IRC para o qual a resposta será enviada.
 * @param ircsock O socket do cliente IRC.
 * @return Retorna true se a solicitação for enviada e a resposta for enviada com sucesso, caso contrário, retorna false.
 */
bool Bot::curlGPT(std::string prompt, std::string Nickname, int ircsock)
{
	struct addrinfo hints, *res;
	int sockfd;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;		 // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP

	if (getaddrinfo(host, "http", &hints, &res) != 0)
	{
		std::cout << "ERROR, no such host" << std::endl;
		return false;
	}

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0)
	{
		std::cout << "ERROR opening socket" << std::endl;
		return false;
	}

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	memcpy(&serv_addr.sin_addr, &((struct sockaddr_in *)res->ai_addr)->sin_addr, sizeof(struct in_addr));

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cout << "ERROR connecting" << std::endl;
		return false;
	}

	freeaddrinfo(res);

	std::string request = "POST " + std::string(path) + " HTTP/1.1\r\n";
	request += "Host: " + std::string(host) + "\r\n";
	request += "Content-Type: application/json\r\n";
	std::string body = "{\"model\" : \"phi3:mini\", \"prompt\": \"" + prompt + "\"}";
	std::stringstream ss;
	ss << body.length();
	request += "Content-Length: " + ss.str() + "\r\n";
	request += "Connection: close\r\n\r\n";
	request += body;

	int n = send(sockfd, request.c_str(), request.length(), 0);
	if (n < 0)
	{
		std::cout << "ERROR writing to socket" << std::endl;
		return false;
	}

	char buffer[4096];
	std::memset(buffer, 0, 4096);
	std::string response;
	std::string message;
	bool done = false;

	while (!done && (n = recv(sockfd, buffer, 4095, 0)) > 0)
	{
		response.append(buffer, n);
		std::memset(buffer, 0, 4096);

		std::string::size_type pos = 0;
		while ((pos = response.find("\n")) != std::string::npos)
		{
			std::string line = response.substr(0, pos);
			response.erase(0, pos + 1);

			std::string resp = extractJsonValue(line, "response");
			std::string done_str = extractJsonValue(line, "done");

			if (!line.empty())
			{
				std::cout << line;
			}
			message.append(resp);

			if (done_str == "true")
			{
				done = true;
				break;
			}
		}
	}

	if (!message.empty())
	{
		std::istringstream stream(message);
		std::string line;
		while (std::getline(stream, line))
		{
			sendPrivmsg(line, Nickname, ircsock);
			sendPrivmsg(" ", Nickname, ircsock);
		}
	}

	if (n < 0)
	{
		std::cout << "ERROR reading from socket" << std::endl;
		return false;
	}

	if (n < 0)
	{
		std::cout << "ERROR reading from socket" << std::endl;
		return false;
	}

	close(sockfd);
	return true;
}

/**
 * Inicia o bot para processar comandos recebidos do servidor IRC.
 *
 * @param ircsock O descritor de soquete para comunicação com o servidor IRC.
 */
void Bot::init(int ircsock)
{
	std::string received, nickname, command;
	ssize_t receivedBytes;

	char buff[1024];
	while (true)
	{
		memset(buff, 0, sizeof(buff));
		receivedBytes = recv(ircsock, buff, (sizeof(buff) - 1), 0);
		if (receivedBytes <= 0)
			break;
		received = buff;
		size_t pos = received.find_first_of("\n\r");
		if (pos != std::string::npos)
			received = received.substr(0, pos);
		if (received == ": 001 " + botnick + " : Welcome to the IRC server!" && !Loggedin)
			Loggedin = true;
		else if (!Loggedin)
		{
			std::cout << received << std::endl;
			return;
		}
		else if (received.find("PRIVMSG") != std::string::npos && Loggedin)
		{
			getCommand(received, nickname, command);
			std::string prompt;
			prompt = "responda em no máximo 5 linhas, ";
			prompt.append(command);
			std::cout << "Data: " << prompt << std::endl;
			curlGPT(prompt, nickname, ircsock);
			continue;
		}
	}
}
