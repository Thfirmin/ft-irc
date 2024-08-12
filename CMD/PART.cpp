#include "../INC/Server.hpp"

/**
 * Encontra e retorna a parte restante de uma string após um determinado token.
 * 
 * @param cmd A string original onde será realizada a busca.
 * @param tofind O token a ser encontrado na string.
 * @param str Uma referência para armazenar a parte restante da string após o token.
 */
void FindPR(std::string cmd, std::string tofind, std::string &str)
{
	size_t i = 0;
	for (; i < cmd.size(); i++){
		if (cmd[i] != ' '){
			std::string tmp;
			for (; i < cmd.size() && cmd[i] != ' '; i++)
				tmp += cmd[i];
			if (tmp == tofind) break;
			else tmp.clear();
		}
	}
	if (i < cmd.size()) str = cmd.substr(i);
	i = 0;
	for (; i < str.size() && str[i] == ' '; i++);
	str = str.substr(i);
}

/**
 * Divide uma string de comando e preenche um vetor com os tokens resultantes.
 * 
 * @param cmd A string de comando original a ser dividida.
 * @param tmp Um vetor de strings para armazenar os tokens resultantes da divisão.
 * @return Uma string contendo a parte restante da string após a divisão.
 */
std::string SplitCmdPR(std::string &cmd, std::vector<std::string> &tmp)
{
	std::stringstream ss(cmd);
	std::string str, reason;
	int count = 2;
	while (ss >> str && count--)
		tmp.push_back(str);
	if(tmp.size() != 2) return std::string("");
	FindPR(cmd, tmp[1], reason);
	return reason;
}

/**
 * Divide uma string de comando em partes e preenche um vetor com os tokens resultantes.
 * 
 * @param cmd A string de comando original a ser dividida.
 * @param tmp Um vetor de strings para armazenar os tokens resultantes da divisão.
 * @param reason Uma referência para armazenar a parte restante da string após a divisão.
 * @param fd O descritor de arquivo associado ao cliente.
 * @return Um valor inteiro indicando o sucesso da operação (1 se bem-sucedido, 0 caso contrário).
 */
int Server::SplitCmdPart(std::string cmd, std::vector<std::string> &tmp, std::string &reason, int fd)
{
	reason = SplitCmdPR(cmd, tmp);
	if(tmp.size() < 2) {tmp.clear(); return 0;}
	tmp.erase(tmp.begin());
	std::string str = tmp[0]; std::string str1; tmp.clear();
	for (size_t i = 0; i < str.size(); i++){//split the first string by ',' to get the channels names
		if (str[i] == ',')
			{tmp.push_back(str1); str1.clear();}
		else str1 += str[i];
	}
	tmp.push_back(str1);
	for (size_t i = 0; i < tmp.size(); i++)//erase the empty strings
		{if (tmp[i].empty())tmp.erase(tmp.begin() + i--);}
	if (reason[0] == ':') reason.erase(reason.begin());
	else //shrink to the first space
		{for (size_t i = 0; i < reason.size(); i++){if (reason[i] == ' '){reason = reason.substr(0, i);break;}}}
	for (size_t i = 0; i < tmp.size(); i++){// erase the '#' from the channel name and check if the channel valid
			if (*(tmp[i].begin()) == '#')
				tmp[i].erase(tmp[i].begin());
			else
				{senderror(403, GetClient(fd)->GetNickName(), tmp[i], GetClient(fd)->GetFd(), " :No such channel\r\n"); tmp.erase(tmp.begin() + i--);}
		}
	return 1;
}

/**
 * Processa o comando PART, que indica a saída de um cliente de um ou mais canais.
 * 
 * @param cmd A string contendo o comando PART e seus argumentos.
 * @param fd O descritor de arquivo associado ao cliente que está enviando o comando.
 */
void Server::Part(std::string cmd, int fd)
{
	std::vector<std::string> tmp;
	std::string reason;
	if (!SplitCmdPart(cmd, tmp, reason, fd))// ERR_NEEDMOREPARAMS (461) // if the channel name is empty
		{senderror(461, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :Not enough parameters\r\n"); return;}
	for (size_t i = 0; i < tmp.size(); i++){
		bool flag = false;
		for (size_t j = 0; j < this->channels.size(); j++){ // search for the channel
			if (this->channels[j].GetName() == tmp[i]){ // check if the channel exist
				flag = true;
				if (!channels[j].get_client(fd) && !channels[j].get_admin(fd)) // ERR_NOTONCHANNEL (442) // if the client is not in the channel
					{senderror(442, GetClient(fd)->GetNickName(), "#" + tmp[i], GetClient(fd)->GetFd(), " :You're not on that channel\r\n"); continue;}
					std::stringstream ss;
					ss << ":" << GetClient(fd)->GetNickName() << "!~" << GetClient(fd)->GetUserName() << "@" << "localhost" << " PART #" << tmp[i];
					if (!reason.empty())
						ss << " :" << reason << "\r\n";
					else ss << "\r\n";
					channels[j].sendToAll(ss.str());
					if (channels[j].get_admin(channels[j].GetClientInChannel(GetClient(fd)->GetNickName())->GetFd()))
						channels[j].removeAdmin(channels[j].GetClientInChannel(GetClient(fd)->GetNickName())->GetFd());
					else
						channels[j].removeClient(channels[j].GetClientInChannel(GetClient(fd)->GetNickName())->GetFd());
					if (channels[j].GetClientsNumber() == 0)
						channels.erase(channels.begin() + j);
			}
		}
		if (!flag) // ERR_NOSUCHCHANNEL (403) // if the channel doesn't exist
			senderror(403, GetClient(fd)->GetNickName(), "#" + tmp[i], GetClient(fd)->GetFd(), " :No such channel\r\n");
	}
}