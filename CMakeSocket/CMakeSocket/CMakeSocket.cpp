// CMakeSocket.cpp: 定義應用程式的進入點。
//

#include "CMakeSocket.h"
#include "EasySocket.hpp"
using namespace std;
using namespace masesk;

void handleData(const std::string& data) {
	cout << "Client sent: " + data << endl;
}

int main()
{
	EasySocket socketManager;
	socketManager.socketListen("test", 8080, &handleData);
	return 0;
}
