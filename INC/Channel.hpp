#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Server.hpp"

class Client;
class Channel
{
private:

	int invit_only;
	int topic;
	int key;
	int limit;
	bool topicRestriction;
	std::string name;
	std::string time_creation;
	std::string password;
	std::string created_at;
	std::string topic_name;
	std::vector<Client> clients;
	std::vector<Client> admins;
	std::vector<std::pair<char, bool> > modes;
public:
	Channel();
	~Channel();
	Channel(Channel const &src);
	Channel &operator=(Channel const &src);
	//---------------//Setters
	void SetInvitOnly(int invit_only);
	void SetTopic(int topic);
	void SetKey(int key);
	void SetLimit(int limit);
	void SetTopicName(std::string topic_name);
	void SetPassword(std::string password);
	void SetName(std::string name);
	void SetTime(std::string time);
	void setTopicRestriction(bool value);
	void setModeAtindex(size_t index, bool mode);
	void setCreateiontime();
	//---------------//Getters
	int GetInvitOnly();
	int GetTopic();
	int GetKey();
	int GetLimit();
	int GetClientsNumber();
	bool GettopicRestriction() const;
	bool getModeAtindex(size_t index);
	bool clientInChannel(std::string &nick);
	std::string GetTopicName();
	std::string GetPassword();
	std::string GetName();
	std::string GetTime();
	std::string getCreationtime();
	std::string getModes();
	std::string clientChannelList();
	Client *get_client(int fd);
	Client *get_admin(int fd);
	Client *GetClientInChannel(std::string name);
	//---------------//Methods
	void addClient(Client newClient);
	void addAdmin(Client newClient);
	void removeClient(int fd);
	void removeAdmin(int fd);
	bool changeClientToAdmin(std::string& nick);
	bool changeAdminToClient(std::string& nick);
	//---------------//SendToAll
	void sendToAll(std::string rpl1);
	void sendToAll(std::string rpl1, int fd);
};

#endif
