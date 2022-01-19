#include <iostream>
//#include <windows.h>
#include "Client.h"

void listenForServerMessage(ClientClass*, std::string*); 
void moveMouse(int, int); 
void click(); 
POINT examineMes(std::string&);

void mouseMovementLoop() {

	ClientClass moveClient("77.20.133.201", 8081); 

	if (!moveClient.socket->is_open()) {
		std::cout << "Mouse_Move_Client Error: " << moveClient.ec->message() << std::endl; 
		return; 
	}
	std::string mes; 

	while (true) {

		listenForServerMessage(&moveClient, &mes); 

		std::cout << "recieved: " << mes << std::endl;


		if (mes[0] == '0') {
			moveClient.socket->close(); 
			moveClient.clientCleanup(); 
			return; 
		}

		if (mes[0] == '1') {

			mes = mes.substr(1); 

			POINT pos = examineMes(mes);

			std::cout << "moving |" << pos.x << "| |" << pos.y << "|" << std::endl;

			moveMouse(pos.x, pos.y);  // x, y

		}
		else {
			if (mes[0] == '2') {
				click(); 
			}
		}
	}

}

POINT examineMes(std::string& mes) {
	POINT p; 
	size_t pos = mes.find(',');
	p.x = std::stol(mes.substr(0, pos)); 
	p.y = std::stol(mes.substr(pos+1)); 
	return p; 
}

void moveMouse(int x, int y) {

	long screenWidth = GetSystemMetrics(SM_CXSCREEN) - 1; // 3840; //GetSystemMetrics(SM_CXSCREEN) - 1; 
	long screenHeight = GetSystemMetrics(SM_CYSCREEN) - 1; //2160;  //GetSystemMetrics(SM_CYSCREEN) - 1;	

	INPUT inp = { 0 };

	inp.type = INPUT_MOUSE;
	inp.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

	inp.mi.dx = x * (long)(65535.0f / (double)screenWidth);
	inp.mi.dy = y * (long)(65535.0f / (double)screenHeight);

	SendInput(1, &inp, sizeof(INPUT));

}

void click() {
	INPUT inp = { 0 };

	inp.type = INPUT_MOUSE;
	inp.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &inp, sizeof(INPUT));
	Sleep(50);
	ZeroMemory(&inp, sizeof(INPUT));

	inp.type = INPUT_MOUSE;
	inp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &inp, sizeof(INPUT));
}
