#include <iostream>
#include <windows.h>



void press(char& c, INPUT& ip, HKL& kbl) {

	ip.ki.dwFlags = KEYEVENTF_UNICODE; 
	ip.ki.wVk = 0; 
	ip.ki.wScan = c; 
	ip.ki.time = 0; 
	ip.ki.dwExtraInfo = 0;

	SendInput(1, &ip, sizeof(INPUT));

	Sleep(1);

	ip.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
	SendInput(1, &ip, sizeof(INPUT));


}

void pressSpecialKey(INPUT& inp, HKL& kbl, int key) {
	inp.ki.wVk = key; 
	inp.ki.dwFlags = 0;
	SendInput(1, &inp, sizeof(INPUT));
	
	Sleep(1); 
	
	inp.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &inp, sizeof(INPUT));

}


void write(std::string s) {
	INPUT ip;
	HKL kbl = GetKeyboardLayout(0);

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	for (char& c : s) {
		if (c == '~' || c == '$') {
			if (c == '~')
				pressSpecialKey(ip, kbl, VK_RETURN);
			else
				pressSpecialKey(ip, kbl, VK_BACK);

		}
		else {
			press(c, ip, kbl);

		}

	}
}
