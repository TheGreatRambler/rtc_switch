#pragma once

#include <arpa/inet.h>
#include <cstdint>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define SERVER_PORT 2349

class CommunicateWithPC {
private:
	enum DataFlag : uint8_t {
		WRITE_DATA,
	};

	// Socket stuff
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;

	// Whether the socket is connected
	uint8_t socketConnected;

	void handleSocketError();

	bool blockUntilReady();

	void unserializeData(uint8_t* buf, uint16_t bufSize, DataFlag flag);

	bool readSocketHelper(void* data, uint16_t size);

public:
	CommunicateWithPC();

	void listenForPCCommands();

	~CommunicateWithPC();
}