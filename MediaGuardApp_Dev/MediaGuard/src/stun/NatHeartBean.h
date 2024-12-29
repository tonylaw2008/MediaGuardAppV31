#pragma once
#include <iostream> 
#include <cstring> 
#include <cstdint> // 用於 uint16_t, uint32_t 等類型
#include <io.h> 
#include <cerrno>
 
#include <stdio.h> 
#include <cstdio>    // 包含 popen 和 pclose 的定義 
#include <memory>    // 包含 std::unique_ptr
#include <stdexcept> // 包含 std::runtime_error
#include <string>    // 包含 std::string
#include <array>

#ifdef _WIN32
#include <winsock2.h>  // Windows socket
#include <ws2tcpip.h>  // Windows TCP/IP
#pragma comment(lib, "ws2_32.lib") // Windows socket library
#else
#include <arpa/inet.h> // POSIX socket
#include <unistd.h>    // POSIX standard 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 

#endif
 
//https://baike.baidu.com/item/stun/3131387?fr=ge_ala 原理
//https://www.bilibili.com/opus/727141412236165127
 

class NatHeartBean {
 
public:
	explicit NatHeartBean(); 
	~NatHeartBean();
private:
	const char* STUN_SERVER_IP = "stun.l.google.com"; // STUN 伺服器 IP
	const int STUN_SERVER_PORT = 19302; // STUN 伺服器端口 
	// STUN 請求的大小
	const int STUN_REQUEST_SIZE = 20; // 根據需要調整大小 
public:
	
	// 使用STUN 協議失敗,保留這兩個函數以後優化或者改造 結果 204.204.204.204
	// 獲取 IP and PORT 
	void get_server_internet_ip(char*& loacal_ip, int& local_port); 
	// 使用STUN 協議失敗,保留這兩個函數以後優化或者改造 結果 0.0.0.0 
	void get_local_internet_ip_and_port(char*& loacal_ip, int& local_port);
	
	int bind_socket(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

	//設置 STUN 請求的內容
	void create_stun_request();

	// 通過curl 客戶端命令 獲取外網ip的方式是可用的 ok 2024-12-29
	// 保留log文件的方式
	std::string get_public_ip_by_curl();
	// 通過curl 客戶端命令 獲取外網ip的方式是可用的 ok 2024-12-29
	// 通過內存獲取 客戶端命令結果
	std::string get_public_ip_by_curl_memory();

	char stun_request[20];
	int request_length;
};