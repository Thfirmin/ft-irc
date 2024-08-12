#include "Bot.hpp"
#include <csignal>
int ircsock;

/**
 * Verifica se a porta especificada é válida.
 * A porta deve ser um número entre 1024 e 65535.
 * 
 * @param port A porta a ser verificada.
 * @return true se a porta é válida, false caso contrário.
 */
bool isPortValid(std::string port){
	return (port.find_first_not_of("0123456789") == std::string::npos && \
	std::atoi(port.c_str()) >= 1024 && std::atoi(port.c_str()) <= 65535);
}

/**
 * Envia uma mensagem para um descritor de arquivo (socket).
 * 
 * @param message A mensagem a ser enviada.
 * @param fd O descritor de arquivo (socket) para o qual a mensagem será enviada.
 */
void sendMessage(std::string message, int fd){
    if(send(fd, message.c_str(), message.size(), 0) == -1)
        std::cerr << "Send failed" << std::endl;
}

/**
 * Manipulador de sinal para os sinais SIGINT e SIGQUIT.
 * Imprime uma mensagem ao receber o sinal.
 * 
 * @param signum O número do sinal recebido.
 */
void SignalHandler(int signum){
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	sendMessage("QUIT\r\n", ircsock);
}

/**
 * Função principal do programa. Conecta o bot a um servidor IRC.
 * 
 * @param ac O número de argumentos passados na linha de comando.
 * @param av Um array de strings contendo os argumentos passados na linha de comando.
 * @return 0 se a execução foi bem-sucedida, 1 em caso de erro.
 */
int main(int ac, char **av)
{
	Bot bot;
	if (ac != 5)
	{std::cerr << "Usage: " << av[0] << " <address> <port> <password> <botnickname>" << std::endl; return 1;}
	if (!isPortValid(av[2]) || !*av[3] || std::strlen(av[3]) > 20)
		{std::cerr << "Invalid port! / password!" << std::endl; return 1;}
	signal(SIGINT, SignalHandler);
	signal(SIGQUIT, SignalHandler);
	std::string address = av[1];
	if(address == "localhost" || address == "LOCALHOST")
		address = "127.0.0.1";
	/* std::string filename = av[5];
	if (!bot.getnokat(filename))
		{std::cerr << "Failed to get nokat" << std::endl; return 1;} */
	struct sockaddr_in ircHints;
	ircsock = socket(AF_INET, SOCK_STREAM, 0);
	if (ircsock == -1)
		{std::cerr << "failed to create socket (ircsock)" << std::endl; return 1;}

    ircHints.sin_family = AF_INET;
    ircHints.sin_port = htons(std::atoi(av[2]));
	ircHints.sin_addr.s_addr = inet_addr(address.c_str());
	
    if(connect(ircsock, (struct sockaddr*)&ircHints, sizeof(ircHints)) == -1)
		{std::cerr << "connect failed" << std::endl; return 1;}
	
    // connection to irc server
    sendMessage("PASS " + std::string(av[3]) + "\r\n", ircsock);
    sendMessage("NICK " + std::string(av[4]) + "\r\n", ircsock);
    sendMessage("USER " + std::string(av[4]) + " 0 * bot\r\n", ircsock);
	bot.setNick(std::string(av[4]));
	std::cout << GRE << "BOT Is Connected!" << WHI << std::endl;
	bot.init(ircsock);
	std::cout << RED << "BOT Is Disconnected!" << WHI << std::endl;
	return 0;
}