#include "../INC/Client.hpp"

/**
 * Construtor padrão da classe Client.
 * 
 * Inicializa os membros da classe com valores padrão.
 */
Client::Client()
{
	this->nickname = "";
	this->username = "";
	this->fd = -1;
	this->isOperator= false;
	this->registered = false;
	this->buffer = "";
	this->ipadd = "";
	this->logedin = false;
}

/**
 * Construtor parametrizado da classe Client.
 * 
 * @param nickname O apelido do cliente.
 * @param username O nome de usuário do cliente.
 * @param fd O descritor de arquivo associado ao cliente.
 */
Client::Client(std::string nickname, std::string username, int fd) :fd(fd), nickname(nickname), username(username){}

/**
 * Construtor padrão da classe Client.
 */
Client::~Client(){}

/**
 * Destrutor da classe Client.
 */
Client::Client(Client const &src){*this = src;}

/**
 * Construtor de cópia da classe Client.
 * 
 * @param src O objeto Client a ser copiado.
 */
Client &Client::operator=(Client const &src){
	if (this != &src){
		this->nickname = src.nickname;
		this->username = src.username;
		this->fd = src.fd;
		this->ChannelsInvite = src.ChannelsInvite;
		this->buffer = src.buffer;
		this->registered = src.registered;
		this->ipadd = src.ipadd;
		this->logedin = src.logedin;
	}
	return *this;
}

/**
 * Obtém o descritor de arquivo associado ao cliente.
 * 
 * @return O descritor de arquivo associado ao cliente.
 */
int Client::GetFd(){return this->fd;}

/**
 * Verifica se o cliente está registrado.
 * 
 * @return true se o cliente estiver registrado, caso contrário, retorna false.
 */
bool Client::getRegistered(){return registered;}

/**
 * Verifica se o cliente foi convidado para um canal específico.
 * 
 * @param ChName O nome do canal a ser verificado.
 * @return true se o cliente foi convidado para o canal especificado, caso contrário, retorna false.
 */
bool Client::GetInviteChannel(std::string &ChName){
	for (size_t i = 0; i < this->ChannelsInvite.size(); i++){
		if (this->ChannelsInvite[i] == ChName)
			return true;
	}
	return false;
}

/**
 * Obtém o apelido (nickname) do cliente.
 * 
 * @return O apelido (nickname) do cliente.
 */
std::string Client::GetNickName(){return this->nickname;}

/**
 * Verifica se o cliente está logado.
 * 
 * @return true se o cliente estiver logado, caso contrário, retorna false.
 */
bool Client::GetLogedIn(){return this->logedin;}

/**
 * Obtém o nome de usuário do cliente.
 * 
 * @return O nome de usuário do cliente.
 */
std::string Client::GetUserName(){return this->username;}

/**
 * Obtém o buffer de mensagens do cliente.
 * 
 * @return O buffer de mensagens do cliente.
 */
std::string Client::getBuffer(){return buffer;}

/**
 * Obtém o endereço IP do cliente.
 * 
 * @return O endereço IP do cliente.
 */
std::string Client::getIpAdd(){return ipadd;}

/**
 * Obtém o hostname do cliente no formato "nickname!username".
 * 
 * @return O hostname do cliente.
 */
std::string Client::getHostname(){
	std::string hostname = this->GetNickName() + "!" + this->GetUserName();
	return hostname;
}

/**
 * Define o descritor de arquivo associado ao cliente.
 * 
 * @param fd O descritor de arquivo a ser definido para o cliente.
 */
void Client::SetFd(int fd){this->fd = fd;}

/**
 * Define o apelido (nickname) do cliente.
 * 
 * @param nickName O apelido (nickname) a ser definido para o cliente.
 */
void Client::SetNickname(std::string& nickName){this->nickname = nickName;}

/**
 * Define o status de logado do cliente.
 * 
 * @param value O valor booleano indicando se o cliente está logado ou não.
 */
void Client::setLogedin(bool value){this->logedin = value;}

/**
 * Define o nome de usuário do cliente.
 * 
 * @param username O nome de usuário a ser definido para o cliente.
 */
void Client::SetUsername(std::string& username){this->username = username;}

/**
 * Adiciona dados ao buffer de mensagens do cliente.
 * 
 * @param received Os dados a serem adicionados ao buffer.
 */
void Client::setBuffer(std::string received){buffer += received;}

/**
 * Define o status de registro do cliente.
 * 
 * @param value O valor booleano indicando se o cliente está registrado ou não.
 */
void Client::setRegistered(bool value){registered = value;}

/**
 * Define o endereço IP do cliente.
 * 
 * @param ipadd O endereço IP a ser definido para o cliente.
 */
void Client::setIpAdd(std::string ipadd){this->ipadd = ipadd;}

/**
 * Limpa o buffer de mensagens do cliente.
 */
void Client::clearBuffer(){buffer.clear();}

/**
 * Adiciona um canal à lista de convites do cliente.
 * 
 * @param chname O nome do canal a ser adicionado à lista de convites.
 */
void Client::AddChannelInvite(std::string &chname){
	ChannelsInvite.push_back(chname);
}

/**
 * Remove um canal da lista de convites do cliente.
 * 
 * @param chname O nome do canal a ser removido da lista de convites.
 */
void Client::RmChannelInvite(std::string &chname){
	for (size_t i = 0; i < this->ChannelsInvite.size(); i++){
		if (this->ChannelsInvite[i] == chname)
			{this->ChannelsInvite.erase(this->ChannelsInvite.begin() + i); return;}
	}
}