#include <iostream>
#include "Client.h"
#include <windows.h>

void listenForServerMessage(ClientClass *client, std::string *message) {

	while (true) {
		// listen, save message in "message", termninate thread


		if (client->socket->is_open()) {
			
			client->socket->wait(client->socket->wait_read); 

			size_t bytes = client->socket->available(); 

			if (bytes > 0) {

				std::vector<char> inp(bytes); 

				client->socket->read_some(asio::buffer(inp.data(), inp.size()), (*client->ec)); 

				*message = std::string(inp.begin(), inp.end()); 

				std::cout << *message << std::endl; 

				break; 

			}

		}
		else {
			std::cout << "Socket closed" << std::endl;

			(*message) = "SOCKET_CLOSED"; 

			break; 
		}
	}

}