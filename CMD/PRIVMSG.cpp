#include "../INC/Server.hpp"

/**
 * Localiza a primeira ocorrência de uma substring específica em uma string.
 * 
 * @param cmd A string na qual a busca será realizada.
 * @param tofind A substring a ser encontrada.
 * @param str A string na qual a substring encontrada será armazenada.
 */
void FindPM(std::string cmd, std::string tofind, std::string &str)
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
 * Divide um comando PM (mensagem privada) em partes distintas.
 * 
 * @param cmd O comando PM original a ser dividido.
 * @param tmp Um vetor de strings para armazenar as partes do comando.
 * @return A mensagem privada contida no comando.
 */
std::string SplitCmdPM(std::string &cmd, std::vector<std::string> &tmp)
{
	std::stringstream ss(cmd);
	std::string str, msg;
	int count = 2;
	while (ss >> str && count--)
		tmp.push_back(str);
	if(tmp.size() != 2) return std::string("");
	FindPM(cmd, tmp[1], msg);
	return msg;
}

/**
 * Divide um comando PRIVMSG em partes distintas.
 * 
 * @param cmd O comando PRIVMSG original a ser dividido.
 * @param tmp Um vetor de strings para armazenar as partes do comando.
 * @return A mensagem contida no comando PRIVMSG.
 */
std::string SplitCmdPrivmsg(std::string cmd, std::vector<std::string> &tmp)
{
	std::string str = SplitCmdPM(cmd, tmp);
	if (tmp.size() != 2) {tmp.clear(); return std::string("");}
	tmp.erase(tmp.begin());
	std::string str1 = tmp[0]; std::string str2; tmp.clear();
	for (size_t i = 0; i < str1.size(); i++){//split the first string by ',' to get the channels names
		if (str1[i] == ',')
			{tmp.push_back(str2); str2.clear();}
		else str2 += str1[i];
	}
	tmp.push_back(str2);
	for (size_t i = 0; i < tmp.size(); i++)//erase the empty strings
		{if (tmp[i].empty())tmp.erase(tmp.begin() + i--);}
	if (str[0] == ':') str.erase(str.begin());
	else //shrink to the first space
		{for (size_t i = 0; i < str.size(); i++){if (str[i] == ' '){str = str.substr(0, i);break;}}}
	return  str;
	
}

/**
 * Verifica se os canais ou clientes mencionados estão válidos e acessíveis para o cliente atual.
 * 
 * @param tmp Um vetor de strings contendo os nomes de canais ou clientes a serem verificados.
 * @param fd O descritor de arquivo do cliente atual.
 */
void	Server::CheckForChannelsClients(std::vector<std::string> &tmp, int fd)
{
	for(size_t i = 0; i < tmp.size(); i++){
		if (tmp[i][0] == '#'){
			tmp[i].erase(tmp[i].begin());
			if(!GetChannel(tmp[i]))//ERR_NOSUCHNICK (401) // if the channel doesn't exist
				{senderror(401, "#" + tmp[i], GetClient(fd)->GetFd(), " :No such nick/channel\r\n"); tmp.erase(tmp.begin() + i); i--;}
			else if (!GetChannel(tmp[i])->GetClientInChannel(GetClient(fd)->GetNickName())) //ERR_CANNOTSENDTOCHAN (404) // if the client is not in the channel
				{senderror(404, GetClient(fd)->GetNickName(), "#" + tmp[i], GetClient(fd)->GetFd(), " :Cannot send to channel\r\n"); tmp.erase(tmp.begin() + i); i--;}
			else tmp[i] = "#" + tmp[i];
		}
		else{
			if (!GetClientNick(tmp[i]))//ERR_NOSUCHNICK (401) // if the client doesn't exist
				{senderror(401, tmp[i], GetClient(fd)->GetFd(), " :No such nick/channel\r\n"); tmp.erase(tmp.begin() + i); i--;}
		}
	}
}

/**
 * Envia uma mensagem PRIVMSG para um ou mais canais ou clientes especificados.
 * 
 * @param cmd O comando completo recebido do cliente.
 * @param fd O descritor de arquivo do cliente atual.
 */
void	Server::Privmsg(std::string cmd, int fd)
{
	std::vector<std::string> tmp;
	std::string message = SplitCmdPrivmsg(cmd, tmp);
	if (!tmp.size())//ERR_NORECIPIENT (411) // if the client doesn't specify the recipient
		{senderror(411, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :No recipient given (PRIVMSG)\r\n"); return;}
	if (message.empty())//ERR_NOTEXTTOSEND (412) // if the client doesn't specify the message
		{senderror(412, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :No text to send\r\n"); return;}
	if (tmp.size() > 10) //ERR_TOOMANYTARGETS (407) // if the client send the message to more than 10 clients
		{senderror(407, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :Too many recipients\r\n"); return;}
	CheckForChannelsClients(tmp, fd); // check if the channels and clients exist
	for (size_t i = 0; i < tmp.size(); i++){// send the message to the clients and channels
		if (tmp[i][0] == '#'){
			tmp[i].erase(tmp[i].begin());
			std::string resp = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost PRIVMSG #" + tmp[i] + " :" + message + "\r\n";
			GetChannel(tmp[i])->sendToAll(resp, fd);
		}
		else{
			std::string resp = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost PRIVMSG " + tmp[i] + " :" + message + "\r\n";
			_sendResponse(resp, GetClientNick(tmp[i])->GetFd());
		}
	}
}
