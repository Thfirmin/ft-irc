#include "../INC/Server.hpp"

/**
 * Encontra a substring 'tofind' em 'cmd' e atribui o restante da string a 'str'.
 * 
 * @param cmd A string onde será realizada a busca.
 * @param tofind A substring a ser encontrada em 'cmd'.
 * @param str A string onde será armazenado o restante da string após 'tofind'.
 */
void FindK(std::string cmd, std::string tofind, std::string &str)
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
 * Separa 'cmd' em substrings usando espaços como delimitadores e armazena até 3 substrings em 'tmp'.
 * 
 * @param cmd A string a ser processada e dividida em substrings.
 * @param tmp Vetor onde serão armazenadas as substrings resultantes da divisão.
 * @return A string restante após a terceira substring em 'cmd', ou uma string vazia se a divisão não for bem-sucedida.
 */
std::string SplitCmdK(std::string &cmd, std::vector<std::string> &tmp)
{
	std::stringstream ss(cmd);
	std::string str, reason;
	int count = 3;
	while (ss >> str && count--)
		tmp.push_back(str);
	if(tmp.size() != 3) return std::string("");
	FindK(cmd, tmp[2], reason);
	return reason;
}

/**
 * Divide a string 'cmd' em substrings usando espaços como delimitadores e armazena as substrings relevantes em 'tmp'.
 * 
 * @param cmd A string contendo os dados para o comando de kick.
 * @param tmp Vetor onde serão armazenadas as substrings resultantes da divisão.
 * @param user Referência para a string onde será armazenado o nome do usuário alvo do kick.
 * @param fd O descritor de arquivo associado ao cliente que está enviando o comando.
 * @return A string contendo o motivo do kick, ou uma string vazia se a divisão não for bem-sucedida.
 */
std::string Server::SplitCmdKick(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd)
{
	std::string reason = SplitCmdK(cmd, tmp);
	if (tmp.size() < 3) // check if the client send the channel name and the client to kick
		return std::string("");
	tmp.erase(tmp.begin());
	std::string str = tmp[0]; std::string str1;
	user = tmp[1]; tmp.clear();
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
	return reason;
}

/**
 * Realiza a operação de kick em um ou mais clientes de um canal.
 * 
 * @param cmd A string contendo os dados para o comando de kick.
 * @param fd O descritor de arquivo associado ao cliente que está enviando o comando.
 */
void	Server::Kick(std::string cmd, int fd)
{
	//ERR_BADCHANMASK (476) // if the channel mask is invalid
	std::vector<std::string> tmp;
	std::string reason ,user;
	reason = SplitCmdKick(cmd, tmp, user, fd);
	if (user.empty())
		{senderror(461, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :Not enough parameters\r\n"); return;}
	for (size_t i = 0; i < tmp.size(); i++){ // search for the channel
		if (GetChannel(tmp[i])){// check if the channel exist
			Channel *ch = GetChannel(tmp[i]);
			if (!ch->get_client(fd) && !ch->get_admin(fd)) // check if the client is in the channel
				{senderror(442, GetClient(fd)->GetNickName(), "#" + tmp[i], GetClient(fd)->GetFd(), " :You're not on that channel\r\n"); continue;}
			if(ch->get_admin(fd)){ // check if the client is admin
				if (ch->GetClientInChannel(user)){ // check if the client to kick is in the channel
					std::stringstream ss;
					ss << ":" << GetClient(fd)->GetNickName() << "!~" << GetClient(fd)->GetUserName() << "@" << "localhost" << " KICK #" << tmp[i] << " " << user;
					if (!reason.empty())
						ss << " :" << reason << "\r\n";
					else ss << "\r\n";
					ch->sendToAll(ss.str());
					if (ch->get_admin(ch->GetClientInChannel(user)->GetFd()))
						ch->removeAdmin(ch->GetClientInChannel(user)->GetFd());
					else
						ch->removeClient(ch->GetClientInChannel(user)->GetFd());
					if (ch->GetClientsNumber() == 0)
						channels.erase(channels.begin() + i);
				}
				else // if the client to kick is not in the channel
					{senderror(441, GetClient(fd)->GetNickName(), "#" + tmp[i], GetClient(fd)->GetFd(), " :They aren't on that channel\r\n"); continue;}
			}
			else // if the client is not admin
				{senderror(482, GetClient(fd)->GetNickName(), "#" + tmp[i], GetClient(fd)->GetFd(), " :You're not channel operator\r\n"); continue;}
		}
		else // if the channel doesn't exist
			senderror(403, GetClient(fd)->GetNickName(), "#" + tmp[i], GetClient(fd)->GetFd(), " :No such channel\r\n");
	}
}