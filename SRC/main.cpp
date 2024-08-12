#include "../INC/Server.hpp"

/**
 * Verifica se a porta fornecida é válida.
 * 
 * @param port Uma string representando o número da porta.
 * @return Retorna true se a porta é um número entre 1024 e 65535, caso contrário, retorna false.
 * @exception Nenhuma exceção é lançada por esta função.
 */
bool isPortValid(std::string port){
	return (port.find_first_not_of("0123456789") == std::string::npos && \
	std::atoi(port.c_str()) >= 1024 && std::atoi(port.c_str()) <= 65535);
}

/**
 * Função principal do programa.
 * 
 * @param ac Número de argumentos passados para o programa.
 * @param av Array de argumentos passados para o programa.
 * @return Retorna 1 em caso de erro ou se a quantidade de argumentos for inválida, 
 *         caso contrário, a execução normal do programa pode não retornar valor.
 * @exception std::exception Captura exceções que podem ser lançadas durante a inicialização do servidor.
 */
int main(int ac, char **av)
{
	Server ser;
	if (ac != 3)
		{std::cout << "Usage: " << av[0] << " <port number> <password>" << std::endl; return 1;}
	std::cout << "---- SERVER ----" << std::endl;
	try
	{
		signal(SIGINT, Server::SignalHandler);
		signal(SIGQUIT, Server::SignalHandler);
		if(!isPortValid(av[1]) || !*av[2] || std::strlen(av[2]) > 20)
			{std::cout << "invalid Port number / Password!" << std::endl; return 1;}
		ser.init(std::atoi(av[1]), av[2]);
	}
	catch(const std::exception& e)
	{
		ser.closeFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}
