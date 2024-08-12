#include "../INC/Channel.hpp"

/**
 * Construtor padrão da classe Channel.
 * 
 * Inicializa os membros da classe com valores padrão.
 */
Channel::Channel(){
	this->invit_only = 0;
	this->topic = 0;
	this->key = 0;
	this->limit = 0;
	this->topicRestriction = false;
	this->name = "";
	this->topic_name = "";
	char charaters[5] = {'i', 't', 'k', 'o', 'l'};
	for(int i = 0; i < 5; i++)
		modes.push_back(std::make_pair(charaters[i],false));
	this->created_at = "";
}

/**
 * Destrutor da classe Channel.
 */
Channel::~Channel(){}

/**
 * Construtor de cópia da classe Channel.
 * 
 * @param src O objeto Channel a ser copiado.
 */
Channel::Channel(Channel const &src){*this = src;}

/**
 * Operador de atribuição da classe Channel.
 * 
 * @param src O objeto Channel a ser atribuído.
 * @return Uma referência para o próprio objeto Channel após a atribuição.
 * 
 * Atribui os valores dos membros do objeto Channel fornecido ao próprio objeto Channel.
 */
Channel &Channel::operator=(Channel const &src){
	if (this != &src){
		this->invit_only = src.invit_only;
		this->topic = src.topic;
		this->key = src.key;
		this->limit = src.limit;
		this->topicRestriction = src.topicRestriction;
		this->name = src.name;
		this->password = src.password;
		this->created_at = src.created_at;
		this->topic_name = src.topic_name;
		this->clients = src.clients;
		this->admins = src.admins;
		this->modes = src.modes;
	}
	return *this;
}

/**
 * Define se o canal está configurado para ser somente por convite.
 * 
 * @param invit_only O valor a ser definido para o modo "somente por convite".
 */
void Channel::SetInvitOnly(int invit_only){this->invit_only = invit_only;}

/**
 * Define se o canal está configurado para ter um tópico.
 * 
 * @param topic O valor a ser definido para o modo "tópico".
 */
void Channel::SetTopic(int topic){this->topic = topic;}

/**
 * Define a data de criação do canal.
 * 
 * @param time A data de criação a ser definida para o canal.
 */
void Channel::SetTime(std::string time){this->time_creation = time;}


/**
 * Define a chave de acesso do canal (se houver).
 * 
 * @param key A chave de acesso a ser definida para o canal.
 */
void Channel::SetKey(int key){this->key = key;}

/**
 * Define o limite de usuários permitidos no canal.
 * 
 * @param limit O limite de usuários a ser definido para o canal.
 */
void Channel::SetLimit(int limit){this->limit = limit;}

/**
 * Define o nome do tópico do canal.
 * 
 * @param topic_name O nome do tópico a ser definido para o canal.
 */
void Channel::SetTopicName(std::string topic_name){this->topic_name = topic_name;}

/**
 * Define a senha de acesso do canal (se houver).
 * 
 * @param password A senha de acesso a ser definida para o canal.
 */
void Channel::SetPassword(std::string password){this->password = password;}

/**
 * Define o nome do canal.
 * 
 * @param name O nome do canal a ser definido.
 */
void Channel::SetName(std::string name){this->name = name;}

/**
 * Define se há restrição de tópico no canal.
 * 
 * @param value O valor booleano indicando se há restrição de tópico no canal.
 */
void Channel::setTopicRestriction(bool value){this->topicRestriction = value;}

/**
 * Define o modo em um índice específico na lista de modos do canal.
 * 
 * @param index O índice na lista de modos do canal.
 * @param mode O valor do modo a ser definido.
 */
void Channel::setModeAtindex(size_t index, bool mode){modes[index].second = mode;}

/**
 * Define o horário de criação do canal.
 * 
 * Define o horário de criação do canal como o horário atual do sistema em formato de string.
 */
void Channel::setCreateiontime(){
	std::time_t _time = std::time(NULL);
	std::ostringstream oss;
	oss << _time;
	this->created_at = std::string(oss.str());
}

/**
 * Obtém o status do modo "somente por convite" do canal.
 * 
 * @return O valor do modo "somente por convite" do canal.
 */
int Channel::GetInvitOnly(){return this->invit_only;}

/**
 * Obtém o status do modo "tópico" do canal.
 * 
 * @return O valor do modo "tópico" do canal.
 */
int Channel::GetTopic(){return this->topic;}

/**
 * Obtém a chave de acesso do canal.
 * 
 * @return A chave de acesso do canal.
 */
int Channel::GetKey(){return this->key;}

/**
 * Obtém o limite de usuários permitidos no canal.
 * 
 * @return O limite de usuários permitidos no canal.
 */
int Channel::GetLimit(){return this->limit;}

/**
 * Obtém o número total de clientes no canal (incluindo administradores).
 * 
 * @return O número total de clientes no canal.
 */
int Channel::GetClientsNumber(){return this->clients.size() + this->admins.size();}

/**
 * Obtém o status da restrição de tópico do canal.
 * 
 * @return true se há restrição de tópico no canal, caso contrário, retorna false.
 */
bool Channel::GettopicRestriction() const{return this->topicRestriction;}

/**
 * Obtém o valor do modo em um índice específico na lista de modos do canal.
 * 
 * @param index O índice na lista de modos do canal.
 * @return O valor do modo no índice especificado.
 */
bool Channel::getModeAtindex(size_t index){return modes[index].second;}

/**
 * Verifica se um cliente está presente no canal (incluindo administradores).
 * 
 * @param nick O apelido do cliente a ser verificado.
 * @return true se o cliente estiver presente no canal, caso contrário, retorna false.
 */
bool Channel::clientInChannel(std::string &nick){
	for(size_t i = 0; i < clients.size(); i++){
		if(clients[i].GetNickName() == nick)
			return true;
	}
	for(size_t i = 0; i < admins.size(); i++){
		if(admins[i].GetNickName() == nick)
			return true;
	}
	return false;
}

/**
 * Obtém o nome do tópico do canal.
 * 
 * @return O nome do tópico do canal.
 */
std::string Channel::GetTopicName(){return this->topic_name;}

/**
 * Obtém a senha de acesso do canal.
 * 
 * @return A senha de acesso do canal.
 */
std::string Channel::GetPassword(){return this->password;}

/**
 * Obtém o nome do canal.
 * 
 * @return O nome do canal.
 */
std::string Channel::GetName(){return this->name;}

/**
 * Obtém a data de criação do canal.
 * 
 * @return A data de criação do canal.
 */
std::string Channel::GetTime(){return this->time_creation;}

/**
 * Obtém o horário de criação do canal em formato de string.
 * 
 * @return O horário de criação do canal em formato de string.
 */
std::string Channel::getCreationtime(){return created_at;}

/**
 * Obtém os modos ativos do canal em formato de string.
 * 
 * @return Uma string representando os modos ativos do canal.
 */
std::string Channel::getModes(){
	std::string mode;
	for(size_t i = 0; i < modes.size(); i++){
		if(modes[i].first != 'o' && modes[i].second)
			mode.push_back(modes[i].first);
	}
	if(!mode.empty())
		mode.insert(mode.begin(),'+');
	return mode;
}

/**
 * Retorna uma lista de clientes e administradores presentes no canal.
 * 
 * @return Uma string contendo a lista de clientes e administradores no canal.
 */
std::string Channel::clientChannelList(){
	std::string list;
	for(size_t i = 0; i < admins.size(); i++){
		list += "@" + admins[i].GetNickName();
		if((i + 1) < admins.size())
			list += " ";
	}
	if(clients.size())
		list += " ";
	for(size_t i = 0; i < clients.size(); i++){
		list += clients[i].GetNickName();
		if((i + 1) < clients.size())
			list += " ";
	}
	return list;
}

/**
 * Obtém um ponteiro para o cliente com o descritor de arquivo (fd) fornecido.
 * 
 * @param fd O descritor de arquivo do cliente a ser obtido.
 * @return Um ponteiro para o cliente se encontrado, caso contrário, retorna NULL.
 */
Client *Channel::get_client(int fd){
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it){
		if (it->GetFd() == fd)
			return &(*it);
	}
	return NULL;
}

/**
 * Obtém um ponteiro para o administrador com o descritor de arquivo (fd) fornecido.
 * 
 * @param fd O descritor de arquivo do administrador a ser obtido.
 * @return Um ponteiro para o administrador se encontrado, caso contrário, retorna NULL.
 */
Client *Channel::get_admin(int fd){
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it){
		if (it->GetFd() == fd)
			return &(*it);
	}
	return NULL;
}

/**
 * Obtém um ponteiro para o cliente ou administrador com o nome fornecido.
 * 
 * @param name O nome do cliente ou administrador a ser obtido.
 * @return Um ponteiro para o cliente ou administrador se encontrado, caso contrário, retorna NULL.
 */
Client* Channel::GetClientInChannel(std::string name)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it){
		if (it->GetNickName() == name)
			return &(*it);
	}
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it){
		if (it->GetNickName() == name)
			return &(*it);
	}
	return NULL;
}

/**
 * Adiciona um novo cliente ao canal.
 * 
 * @param newClient O novo cliente a ser adicionado ao canal.
 */
void Channel::addClient(Client newClient){clients.push_back(newClient);}

/**
 * Adiciona um novo administrador ao canal.
 * 
 * @param newClient O novo administrador a ser adicionado ao canal.
 */
void Channel::addAdmin(Client newClient){admins.push_back(newClient);}

/**
 * Remove um cliente do canal com base no descritor de arquivo (fd) fornecido.
 * 
 * @param fd O descritor de arquivo do cliente a ser removido.
 */
void Channel::removeClient(int fd){
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it){
		if (it->GetFd() == fd)
			{clients.erase(it); break;}
	}
}

/**
 * Remove um administrador do canal com base no descritor de arquivo (fd) fornecido.
 * 
 * @param fd O descritor de arquivo do administrador a ser removido.
 */
void Channel::removeAdmin(int fd){
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it){
		if (it->GetFd() == fd)
			{admins.erase(it); break;}
	}
}

/**
 * Move um cliente para a lista de administradores do canal.
 * 
 * @param nick O apelido do cliente a ser movido para a lista de administradores.
 * @return true se a operação for bem-sucedida, caso contrário, retorna false.
 */
bool Channel::changeClientToAdmin(std::string& nick){
	size_t i = 0;
	for(; i < clients.size(); i++){
		if(clients[i].GetNickName() == nick)
			break;
	}
	if(i < clients.size()){
		admins.push_back(clients[i]);
		clients.erase(i + clients.begin());
		return true;
	}
	return false;
}

/**
 * Move um administrador para a lista de clientes do canal.
 * 
 * @param nick O apelido do administrador a ser movido para a lista de clientes.
 * @return true se a operação for bem-sucedida, caso contrário, retorna false.
 */
bool Channel::changeAdminToClient(std::string& nick){
	size_t i = 0;
	for(; i < admins.size(); i++){
		if(admins[i].GetNickName() == nick)
			break;
	}
	if(i < admins.size()){
		clients.push_back(admins[i]);
		admins.erase(i + admins.begin());
		return true;
	}
	return false;

}

/**
 * Envia uma mensagem para todos os clientes e administradores no canal.
 * 
 * @param rpl1 A mensagem a ser enviada para todos os clientes e administradores.
 */
void Channel::sendToAll(std::string rpl1)
{
	for(size_t i = 0; i < admins.size(); i++)
		if(send(admins[i].GetFd(), rpl1.c_str(), rpl1.size(),0) == -1)
			std::cerr << "send() faild" << std::endl;
	for(size_t i = 0; i < clients.size(); i++)
		if(send(clients[i].GetFd(), rpl1.c_str(), rpl1.size(),0) == -1)
			std::cerr << "send() faild" << std::endl;
}

/**
 * Envia uma mensagem para todos os clientes e administradores no canal, exceto para o cliente com o descritor de arquivo especificado.
 * 
 * @param rpl1 A mensagem a ser enviada para todos os clientes e administradores, exceto para o cliente com o descritor de arquivo especificado.
 * @param fd O descritor de arquivo do cliente que não deve receber a mensagem.
 */
void Channel::sendToAll(std::string rpl1, int fd)
{
	for(size_t i = 0; i < admins.size(); i++){
		if(admins[i].GetFd() != fd)
			if(send(admins[i].GetFd(), rpl1.c_str(), rpl1.size(),0) == -1)
				std::cerr << "send() faild" << std::endl;
	}
	for(size_t i = 0; i < clients.size(); i++){
		if(clients[i].GetFd() != fd)
			if(send(clients[i].GetFd(), rpl1.c_str(), rpl1.size(),0) == -1)
				std::cerr << "send() faild" << std::endl;
	}
}