#include "../INC/Server.hpp"

/**
 * Gera uma string representando um modo de operação a ser adicionado a uma cadeia de modos.
 * 
 * @param chain A cadeia de modos existente.
 * @param opera O operador (+ ou -) para adicionar à cadeia de modos.
 * @param mode O modo a ser adicionado à cadeia de modos.
 * @return Uma string representando o modo a ser adicionado.
 */
std::string Server::modeToAppend(std::string chain, char opera, char mode)
{
	std::stringstream ss;

	ss.clear();
	char last = '\0';
	for(size_t i = 0; i < chain.size(); i++)
	{
		if(chain[i] == '+' || chain[i] == '-')
			last = chain[i];
	}
	if(last != opera)
		ss << opera << mode;
	else
		ss << mode;
	return ss.str();
}

/**
 * Extrai os argumentos de um comando IRC, como nome, conjunto de modos e parâmetros.
 * 
 * @param cmd O comando IRC a ser analisado.
 * @param name Uma referência para armazenar o nome do comando.
 * @param modeset Uma referência para armazenar o conjunto de modos do comando.
 * @param params Uma referência para armazenar os parâmetros do comando.
 */
void Server::getCmdArgs(std::string cmd,std::string& name, std::string& modeset ,std::string &params)
{
	std::istringstream stm(cmd);
	stm >> name;
	stm >> modeset;
	size_t found = cmd.find_first_not_of(name + modeset + " \t\v");
	if(found != std::string::npos)
		params = cmd.substr(found);
}

/**
 * Divide uma string de parâmetros separados por vírgula em um vetor de strings.
 * 
 * @param params A string contendo os parâmetros separados por vírgula.
 * @return Um vetor de strings contendo os parâmetros separados.
 */
std::vector<std::string> Server::splitParams(std::string params)
{
	if(!params.empty() && params[0] == ':')
		params.erase(params.begin());
	std::vector<std::string> tokens;
	std::string param;
	std::istringstream stm(params);
	while (std::getline(stm, param, ','))
	{
		tokens.push_back(param);
		param.clear();
	}
	return tokens;
}

/**
 * Processa o comando MODE para um canal, realizando operações como definir restrições, limites e privilégios.
 * 
 * @param cmd O comando MODE a ser processado.
 * @param fd O descritor de arquivo do cliente que enviou o comando.
 */
void Server::modeCommand(std::string& cmd, int fd)
{
	std::string channelName;
	std::string params;
	std::string modeset;
	std::stringstream mode_chain;
	std::string arguments = ":";
	Channel *channel;
	char opera = '\0';

	arguments.clear();
	mode_chain.clear();
	Client *cli = GetClient(fd);
	size_t found = cmd.find_first_not_of("MODEmode \t\v");
	if(found != std::string::npos)
		cmd = cmd.substr(found);
	else
	{
		_sendResponse(ERR_NOTENOUGHPARAM(cli->GetNickName()), fd); 
		return ;
	}
	getCmdArgs(cmd ,channelName, modeset ,params);
	std::vector<std::string> tokens = splitParams(params);
	if(channelName[0] != '#' || !(channel = GetChannel(channelName.substr(1))))
	{
		_sendResponse(ERR_CHANNELNOTFOUND(cli->GetUserName(),channelName), fd);
		return ;
	}
	else if (!channel->get_client(fd) && !channel->get_admin(fd))
	{
		senderror(442, GetClient(fd)->GetNickName(), channelName, GetClient(fd)->GetFd(), " :You're not on that channel\r\n"); return;
	}
	else if (modeset.empty()) // response with the channel modes (MODE #channel)
	{
		_sendResponse(RPL_CHANNELMODES(cli->GetNickName(), channel->GetName(), channel->getModes()) + \
		RPL_CREATIONTIME(cli->GetNickName(), channel->GetName(),channel->getCreationtime()),fd);
		return ;
	}
	else if (!channel->get_admin(fd)) // client is not channel operator
	{
		_sendResponse(ERR_NOTOPERATOR(channel->GetName()), fd);
		return ;
	}
	else if(channel)
	{
		size_t pos = 0;
		for(size_t i = 0; i < modeset.size(); i++)
		{
			if(modeset[i] == '+' || modeset[i] == '-')
				opera = modeset[i];
			else
			{
				if(modeset[i] == 'i')//invite mode
					mode_chain << inviteOnly(channel , opera, mode_chain.str());
				else if (modeset[i] == 't') //topic restriction mode
					mode_chain << topicRestriction(channel, opera, mode_chain.str());
				else if (modeset[i] == 'k') //password set/remove
					mode_chain <<  passwordMode(tokens, channel, pos, opera, fd, mode_chain.str(), arguments);
				else if (modeset[i] == 'o') //set/remove user operator privilege
						mode_chain << operatorPrivilege(tokens, channel, pos, fd, opera, mode_chain.str(), arguments);
				else if (modeset[i] == 'l') //set/remove channel limits
					mode_chain << channelLimit(tokens, channel, pos, opera, fd, mode_chain.str(), arguments);
				else
					_sendResponse(ERR_UNKNOWNMODE(cli->GetNickName(), channel->GetName(),modeset[i]),fd);
			}
		}
	}
	std::string chain = mode_chain.str();
	if(chain.empty())
		return ;
 	channel->sendToAll(RPL_CHANGEMODE(cli->getHostname(), channel->GetName(), mode_chain.str(), arguments));
}

/**
 * Define ou remove o modo de convite apenas para um canal IRC.
 * 
 * @param channel O ponteiro para o canal onde o modo será definido ou removido.
 * @param opera O caractere que indica se o modo será definido (+) ou removido (-).
 * @param chain A cadeia de modos atual para o canal.
 * @return Uma string representando a alteração no modo de convite apenas, para ser adicionada à cadeia de modos.
 */
std::string Server::inviteOnly(Channel *channel, char opera, std::string chain)
{
	std::string param;
	param.clear();
	if(opera == '+' && !channel->getModeAtindex(0))
	{
		channel->setModeAtindex(0, true);
		channel->SetInvitOnly(1);
		param =  modeToAppend(chain, opera, 'i');
	}
	else if (opera == '-' && channel->getModeAtindex(0))
	{
		channel->setModeAtindex(0, false);
		channel->SetInvitOnly(0);
		param =  modeToAppend(chain, opera, 'i');
	}
	return param;
}

/**
 * Define ou remove o modo de restrição de tópico para um canal IRC.
 * 
 * @param channel O ponteiro para o canal onde o modo será definido ou removido.
 * @param opera O caractere que indica se o modo será definido (+) ou removido (-).
 * @param chain A cadeia de modos atual para o canal.
 * @return Uma string representando a alteração no modo de restrição de tópico, para ser adicionada à cadeia de modos.
 */
std::string Server::topicRestriction(Channel *channel ,char opera, std::string chain)
{
	std::string param;
	param.clear();
	if(opera == '+' && !channel->getModeAtindex(1))
	{
		channel->setModeAtindex(1, true);
		channel->setTopicRestriction(true);
		param =  modeToAppend(chain, opera, 't');
	}
	else if (opera == '-' && channel->getModeAtindex(1))
	{
		channel->setModeAtindex(1, false);
		channel->setTopicRestriction(false);
		param =  modeToAppend(chain, opera, 't');
	}	
	return param;
}

/**
 * Verifica se uma senha é válida de acordo com critérios específicos.
 * 
 * @param password A senha a ser verificada.
 * @return true se a senha é válida, false caso contrário.
 */
bool validPassword(std::string password)
{
	if(password.empty())
		return false;
	for(size_t i = 0; i < password.size(); i++)
	{
		if(!std::isalnum(password[i]) && password[i] != '_')
			return false;
	}
	return true;
}

/**
 * Define ou remove o modo de senha para um canal.
 * 
 * @param tokens Vetor de tokens contendo os parâmetros para o modo de senha.
 * @param channel Ponteiro para o canal em questão.
 * @param pos Posição atual no vetor de tokens.
 * @param opera Operador de modo (+ ou -).
 * @param fd Descritor de arquivo do cliente.
 * @param chain Corrente de modos para o canal.
 * @param arguments Argumentos do modo (por exemplo, a senha).
 * @return Uma string contendo a representação do modo alterado.
 */
std::string Server::passwordMode(std::vector<std::string> tokens, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string &arguments)
{
	std::string param;
	std::string pass;

	param.clear();
	pass.clear();
	if(tokens.size() > pos)
		pass = tokens[pos++];
	else
	{
		_sendResponse(ERR_NEEDMODEPARM(channel->GetName(),std::string("(k)")),fd);
		return param;
	}
	if(!validPassword(pass))
	{
		_sendResponse(ERR_INVALIDMODEPARM(channel->GetName(),std::string("(k)")),fd);
		return param;
	}
	if(opera == '+')
	{
		channel->setModeAtindex(2, true);
		channel->SetPassword(pass);
		if(!arguments.empty())
			arguments += " ";
		arguments += pass;
		param = modeToAppend(chain,opera, 'k');
	}
	else if (opera == '-' && channel->getModeAtindex(2))
	{
		if(pass == channel->GetPassword())
		{		
			channel->setModeAtindex(2, false);
			channel->SetPassword("");
			param = modeToAppend(chain,opera, 'k');
		}
		else
			_sendResponse(ERR_KEYSET(channel->GetName()),fd);
	}
	return param;
}

/**
 * Define ou remove o modo de privilégio de operador para um usuário em um canal.
 * 
 * @param tokens Vetor de tokens contendo os parâmetros para o modo de privilégio.
 * @param channel Ponteiro para o canal em questão.
 * @param pos Posição atual no vetor de tokens.
 * @param fd Descritor de arquivo do cliente.
 * @param opera Operador de modo (+ ou -).
 * @param chain Corrente de modos para o canal.
 * @param arguments Argumentos do modo (por exemplo, o nome de usuário).
 * @return Uma string contendo a representação do modo alterado.
 */
std::string Server::operatorPrivilege(std::vector<std::string> tokens, Channel *channel, size_t& pos, int fd, char opera, std::string chain, std::string& arguments)
{
	std::string user;
	std::string param;

	param.clear();
	user.clear();
	if(tokens.size() > pos)
		user = tokens[pos++];
	else
	{
		_sendResponse(ERR_NEEDMODEPARM(channel->GetName(),"(o)"),fd);
		return param;
	}
	if(!channel->clientInChannel(user))
	{
		_sendResponse(ERR_NOSUCHNICK(channel->GetName(), user),fd);
		return param;
	}
	if(opera == '+')
	{
		channel->setModeAtindex(3,true);
		if(channel->changeClientToAdmin(user))
		{
			param = modeToAppend(chain, opera, 'o');
			if(!arguments.empty())
				arguments += " ";
			arguments += user;
		}
	}
	else if (opera == '-')
	{
		channel->setModeAtindex(3,false);
		if(channel->changeAdminToClient(user))
		{
			param = modeToAppend(chain, opera, 'o');
				if(!arguments.empty())
					arguments += " ";
			arguments += user;

		}
	}
	return param;
}

/**
 * Verifica se o limite do canal é válido.
 * 
 * @param limit A string contendo o limite do canal a ser verificado.
 * @return true se o limite for válido, false caso contrário.
 */
bool Server::isvalidLimit(std::string& limit)
{
	return (!(limit.find_first_not_of("0123456789")!= std::string::npos) && std::atoi(limit.c_str()) > 0);
}

/**
 * Define ou remove o limite de usuários em um canal.
 * 
 * @param tokens Um vetor de strings contendo os tokens da operação MODE.
 * @param channel Um ponteiro para o canal onde a operação será aplicada.
 * @param pos A posição atual no vetor de tokens.
 * @param opera O operador da operação MODE ('+' para adicionar, '-' para remover).
 * @param fd O descritor de arquivo do cliente que está realizando a operação.
 * @param chain A cadeia de modos atual para o canal.
 * @param arguments Os argumentos adicionais para a operação MODE.
 * @return Uma string contendo a modificação no modo do canal.
 */
std::string Server::channelLimit(std::vector<std::string> tokens,  Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments)
{
	std::string limit;
	std::string param;

	param.clear();
	limit.clear();
	if(opera == '+')
	{
		if(tokens.size() > pos )
		{
			limit = tokens[pos++];
			if(!isvalidLimit(limit))
			{
				_sendResponse(ERR_INVALIDMODEPARM(channel->GetName(),"(l)"), fd);
			}
			else
			{
				channel->setModeAtindex(4, true);
				channel->SetLimit(std::atoi(limit.c_str()));
				if(!arguments.empty())
					arguments += " ";
				arguments += limit;
				param =  modeToAppend(chain, opera, 'l');
			}
		}
		else
			_sendResponse(ERR_NEEDMODEPARM(channel->GetName(),"(l)"),fd);
	}
	else if (opera == '-' && channel->getModeAtindex(4))
	{
		channel->setModeAtindex(4, false);
		channel->SetLimit(0);
		param  = modeToAppend(chain, opera, 'l');
	}
	return param;
}