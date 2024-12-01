#include "../INC/Server.hpp"


// Verifica se a string é uma porta válida
bool isPortValid(std::string str)
{
	int	port;

	if (str.find_first_not_of("0123456789") != std::string::npos)
		return (false);
	port = std::atoi(port.c_str());
	if (port < 1024 && port > 65535)
		return (false);
	return (true);
}

int main(int ac, char **av)
{
	Server ser;
	
	// Verifica se tem o número certo de argv
	if (ac != 3)
	{
		std::cout << "Usage: " << av[0] << " <PORT_NUMBER> <PASSWORD>" << std::endl;
		return 1;
	}
	std::cout << "---- SERVER ----" << std::endl;
	try
	{
		// Configura SIGINT (CTRL + C) e SIGQUIT (CTRL + \)
		signal(SIGINT, Server::SignalHandler);
		signal(SIGQUIT, Server::SignalHandler);
		// Verifica Argvs entregues
		if(!isPortValid(av[1]) || !*av[2] || std::strlen(av[2]) > 20)
		{
			std::cout << "invalid Port number / Password!" << std::endl;
			return 1;
		}
		// Inicia programa
		ser.init(std::atoi(av[1]), av[2]);
	}
	catch(const std::exception& e)
	{
		ser.closeFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}
