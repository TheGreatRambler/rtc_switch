#include "communicateWithSwitch.hpp"

// Based on this https://github.com/TheGreatRambler/nx-TAS/blob/dev/source/communicate_with_pc.cpp
void CommunicateWithPC::unserializeData(uint8_t* buf, uint16_t bufSize, DataFlag flag) {
	// The buffer itself
	// https://github.com/niXman/yas/blob/master/include/yas/buffers.hpp#L67
	// https://github.com/niXman/yas/blob/master/examples/one_func_cond/main.cpp
	if(flag == DataFlag::RETURN_MEMORY_INFO) {
		// Do whatever
	}
}

CommunicateWithPC::CommunicateWithPC() {
	// https://gist.github.com/browny/5211329
	// THIS IS THE SERVER
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		// Socket creation failed
	}

	// Set some options
	serv_addr.sin_family      = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port        = htons(SERVER_PORT);

	// Set socket to be non blocking when accepting connections
	fcntl(listenfd, F_SETFL, fcntl(listenfd, F_GETFL) | O_NONBLOCK);

	// This puts the options of serv_addr on the socket itself
	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
		// Binding failed
	}

	// 10 is the max number of queued client connections
	if(listen(listenfd, 10)) {
		// Listen failed
	}

	// No socket connected yet
	socketConnected = false;
}

bool CommunicateWithPC::readSocketHelper(void* data, uint16_t size) {
	// Small helper for a commonly used operation
	int count;
	// Check if socket has any data
	ioctl(listenfd, FIONREAD, &count);
	if(count == 0) {
		// No data yet
		// This shouldn't occur if waiting was done beforehand
		return 1;
	} else if(count == -1) {
		// There was an error
		handleZedNetError();
		return 1;
	}
	int res = recv(listenfd, data, size, MSG_WAITALL);
	if(res == -1) {
		// An error occured
		handleZedNetError();
		return 1;
	} else {
		// Data was recieved
		if(res == size) {
			// Data was the right size
			return 0;
		} else {
			// Data was the wrong size
			handleZedNetError();
			return 1;
		}
	}
}

bool CommunicateWithPC::blockUntilReady() {
	fd_set writefd;
	timeval timeout;
	FD_ZERO(&writefd);
	FD_SET(listenfd, &writefd);
	// Initialize timeout for socket blocking
	// 1 second timeout
	timeout.tv_sec  = 1;
	timeout.tv_usec = 0;
	if(select(FD_SETSIZE, &writefd, NULL, NULL, &timeout)) {
		handleZedNetError();
		return 1;
	}
	return 0;
}

void CommunicateWithPC::listenForPCCommands() {
	if(socketConnected) {
		// Socket connected, do things
		// Some info: MSG_WAITALL is needed to make sure the socket waits for the specified amount of
		// 	data, so this is changed in zed_net
		// The format works by preceding each message with a uint16_t with the size of the message, then the message right after it
		uint8_t keepGoing = true;
		while(keepGoing) {
			uint16_t dataSize;
			if(readSocketHelper(&dataSize, sizeof(dataSize)))
				break;

			// Get the number back to the correct representation
			// https://linux.die.net/man/3/ntohl
			dataSize = ntohs(dataSize);

			// Get the flag now
			// Blocking because committed to data
			if(blockUntilReady())
				break;
			DataFlag flag;
			if(readSocketHelper(&flag, sizeof(flag)))
				break;
			// Flag now tells us the data we expect to recieve

			// The message worked, so get the data
			uint8_t* dataToRead;
			if(readSocketHelper(dataToRead, dataSize))
				break;

			// Have the data now, unserialize with YAS
			unserializeData(dataToRead, dataSize, flag);
			// Have the data, TODO something with it
		}
	} else {
		// Accept socket if avaliable, else, do nothing
		// Specifically non blocking
		int res = accept(listenfd, NULL, NULL);
		if(res == -1) {
			if(errno == EWOULDBLOCK) {
				// There are no connections, so it's fine
				return;
			} else {
				// Genuine error
				handleZedNetError();
				return;
			}
		} else {
			// Connection recieved and ready
			socketConnected = true;
			connfd          = res;
			// Recursively call again because the socket is now open
			listenForPCCommands();
		}
	}
}

CommunicateWithPC::~CommunicateWithPC() {
	// Close socket
	close(connfd);
}