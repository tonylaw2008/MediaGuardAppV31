#include <iostream>
#include <cstring>
#ifdef _WIN32
#include <winsock2.h>  // Windows socket
#include <ws2tcpip.h>  // Windows TCP/IP
#else
#include <arpa/inet.h> // POSIX socket
#include <unistd.h>    // POSIX standard
#endif

#include "Common.h" 
#include "Config/DeviceConfig.h"

const char* STUN_SERVER_IP = "stun.l.google.com"; // STUN 伺服器 IP
const int STUN_SERVER_PORT = 19302; // STUN 伺服器端口

class NatHeartBean {

public:
	explicit NatHeartBean();
	~NatHeartBean();
public:
	 
	//獲取 IP and PORT
	int get_stun_ip();

};