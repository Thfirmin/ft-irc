#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <netdb.h>
#include <sstream>
#include <ctime>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fstream>
#include <cstring>
#include <string>

#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"

class Bot
{
private:
	bool Loggedin;
	std::string botnick;
	const char* host;
    const char* path;
    const int port;

public:
	Bot();
	Bot(const Bot& other);
	Bot& operator=(const Bot& other);
	~Bot();

	//init methods
	void	init(int ircsock);
	//send methods
	void	sendMessage(std::string message, int fd);
	void	sendPrivmsg(std::string message, std::string UserNick, int ircsock);
	//parse methods
	void	getCommand(std::string received, std::string& nick , std::string &command);
	void	setNick(std::string nick);

	std::string removeFirstWord(const std::string& str);
	bool curlGPT(std::string prompt, std::string Nickname, int ircsock);
	std::string extractJsonValue(const std::string& json, const std::string& key);
};
