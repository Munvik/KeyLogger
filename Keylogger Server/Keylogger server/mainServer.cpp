#include <time.h>
#include <iostream>
#include <cstdio>
#include <SFML/Network.hpp>
#include <fstream>

#define SLEEP 5
#define WINDOWS

#ifdef LINUX
#include <unistd.h>
#endif LINUX		

#ifdef WINDOWS
#include <Windows.h>
#endif // WINDOWS



void connect(sf::TcpListener &server, sf::SocketSelector &selector, std::vector <sf::TcpSocket *> &clients)
{

	sf::TcpSocket *newSocket = new sf::TcpSocket;
	server.accept(*newSocket);
	clients.push_back(newSocket);
	selector.add(*newSocket);
	std::cout << "Server have been conected to client with IP " << newSocket->getRemoteAddress() << std::endl;
}

void disconnect(sf::TcpSocket &socket, sf::SocketSelector &listener, std::vector <sf::TcpSocket *> &clients)
{
	std::cout << "Rozlaczanie z klientem o IP " << socket.getRemoteAddress() << " ! \n";
	sf::TcpSocket *tcpSckt = &socket;
	listener.remove(*tcpSckt);
	socket.disconnect();
	size_t pos = std::find(clients.begin(), clients.end(), &socket) - clients.begin();
	if (pos >= clients.size())
	{
		std::cout << "Nie ma takiej pozycji ! \n";
		return;
	}
	clients.erase(clients.begin() + pos);
	delete tcpSckt;
}

void receiveData(sf::TcpSocket &socket, sf::SocketSelector &listener, std::vector <sf::TcpSocket *> &clients, std::string file)
{

	const size_t bufferSize = 3000;
	char buffer[bufferSize];
	size_t received;

	if (socket.receive(buffer, bufferSize, received) == sf::Socket::Disconnected)
	{
		disconnect(socket, listener, clients);
		return;
	}

	std::fstream outputFile;
	outputFile.open(file + ".txt", std::ios::out | std::ios::app);

	if (!outputFile.is_open())
	{
		std::cout << "Plik nie otworzony poprawnie !";
		return;
	}

	std::cout << buffer;
	outputFile << buffer;

	outputFile.close();
}

std::string getSourceInfo(sf::TcpSocket *client)
{
	const size_t bufferSize = 3000;
	char buffer[bufferSize];
	size_t received;
	client->receive(buffer, bufferSize, received);
	std::cout << buffer << std::endl;

	return buffer;
}

void serverLive()
{
	std::vector <sf::TcpSocket *> clients;
	std::vector <std::string> sourceInfos;
	sf::TcpListener server;
	server.listen(2000);
	sf::SocketSelector selector;
	selector.add(server);


	while (1)
	{
		if (selector.wait())
		{
			
			if (selector.isReady(server))
			{
				sf::TcpSocket *client = new sf::TcpSocket;
				if (server.accept(*client) == sf::Socket::Done)
				{
					std::cout << "Polaczono z klientem !\n";

					clients.push_back(client);
					selector.add(*client);

					sourceInfos.push_back(getSourceInfo(client));
				}
				else
					delete client;
				//connect(server, selector, clients);
			}



			for (int i = 0; i < clients.size(); i++)
			{
				if (selector.isReady(*clients[i]))
				{
					receiveData(*clients[i], selector, clients, sourceInfos[i]);
				}
			}

		}
	}


}

int main()
{	
	std::cout << "Welcome to Server" << std::endl;

	serverLive();

	return 0;
}