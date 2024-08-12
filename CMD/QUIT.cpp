#include "../INC/Server.hpp"

/**
 * Encontra um parâmetro específico em um comando e extrai o restante do comando a partir desse ponto.
 * 
 * @param cmd O comando original a ser analisado.
 * @param tofind O parâmetro a ser encontrado no comando.
 * @param str A string em que o restante do comando será armazenado.
 */
void FindQ(std::string cmd, std::string tofind, std::string &str)
{
	size_t i = 0;
	for (; i < cmd.size(); i++){
		if (cmd[i] != ' '){
			std::string tmp;
			for (; i < cmd.size() && cmd[i] != ' '; i++)
				tmp += cmd[i];
			if (tmp == tofind)
				break;
			else tmp.clear();
		}
	}
	if (i < cmd.size())
		str = cmd.substr(i);
	i = 0;
	for (; i < str.size() && str[i] == ' '; i++);
	str = str.substr(i);
}

/**
 * Separa a mensagem de quit do comando, obtendo o motivo do quit.
 * 
 * @param cmd O comando de quit original.
 * @return O motivo do quit extraído do comando, ou uma mensagem padrão "Quit" se nenhum motivo for fornecido.
 */
std::string	SplitQuit(std::string cmd)
{
	std::istringstream stm(cmd);
	std::string reason,str;
	stm >> str;
	FindQ(cmd, str, reason);
	if (reason.empty())
		return std::string("Quit");
	if (reason[0] != ':'){ //if the message does not start with ':'
		for (size_t i = 0; i < reason.size(); i++){
			if (reason[i] == ' ')
				{reason.erase(reason.begin() + i, reason.end());break;}
		}
		reason.insert(reason.begin(), ':');
	}
	return reason;
}

/**
 * Processa o comando QUIT, removendo o cliente dos canais e desconectando-o do servidor.
 * 
 * @param cmd O comando QUIT recebido.
 * @param fd O descritor de arquivo associado ao cliente.
 */
void Server::Quit(std::string cmd, int fd)
{
	std::string reason;
	reason = SplitQuit(cmd);
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].get_client(fd)){
			channels[i].removeClient(fd);
			if (channels[i].GetClientsNumber() == 0)
				channels.erase(channels.begin() + i);
			else{
				std::string rpl = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost QUIT " + reason + "\r\n";
				channels[i].sendToAll(rpl);
			}
		}
		else if (channels[i].get_admin(fd)){
			channels[i].removeAdmin(fd);
			if (channels[i].GetClientsNumber() == 0)
				channels.erase(channels.begin() + i);
			else{
				std::string rpl = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost QUIT " + reason + "\r\n";
				channels[i].sendToAll(rpl);
			}
		}
	}
	std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
	RmChannels(fd);
	RemoveClient(fd);
	RemoveFds(fd);
	close(fd);
}