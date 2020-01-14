#include "communicateWithSwitch.hpp"

CommunicateWithSwitch::CommunicateWithSwitch() {
	connectedToServer = false;
	// https://gist.github.com/browny/5211329
	// THIS IS THE CLIENT
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		// Socket creation failed
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port   = htons(SERVER_PORT);

	// Set socket to be non blocking when accepting connections
	fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK);

	// This puts the options of serv_addr on the socket itself
	if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
		// Binding failed
	}
}

bool CommunicateWithSwitch::sendSocketHelper(void* data, uint16_t size) { }

void CommunicateWithSwitch::setIpAddress(char* ip) {
	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
		// Inet pton error
	}

	if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		// Could not connect
	}
}

CommunicateWithSwitch::~CommunicateWithSwitch() { }