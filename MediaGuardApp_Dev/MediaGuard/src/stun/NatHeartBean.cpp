#pragma once
#include "NatHeartBean.h"

NatHeartBean::~NatHeartBean()
{
    // 析構函數
}
NatHeartBean::NatHeartBean()
{
	// 構造函數
}
  
//獲取STUN協議的網絡地址轉換IP
int NatHeartBean::get_stun_ip(){

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(STUN_SERVER_PORT);
    inet_pton(AF_INET, STUN_SERVER_IP, &server_addr.sin_addr);

    // 構建 STUN 請求
    // 此處省略請求的具體構建過程，請參考 RFC 5389
    // 假設 stun_request 和 request_length 已經正確設置

    // 發送請求
    sendto(sockfd, stun_request, request_length, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 接收響應
    char buffer[1024];
    socklen_t addr_len = sizeof(server_addr);
    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &addr_len);

    // 假設響應中包含了客戶端的 IP 和端口
    // 此處需要根據 STUN 響應格式解析出 IP 和端口
    // 以下代碼僅為示範，實際解析需要根據 RFC 5389 實現
    // 假設我們已經解析出 client_ip 和 client_port

    char client_ip[INET_ADDRSTRLEN];
    //IP格式轉換為字符串
    inet_ntop(AF_INET, &server_addr.sin_addr, client_ip, sizeof(client_ip));
    unsigned short client_port = ntohs(server_addr.sin_port);

    std::cout << "獲取的 IP: " << client_ip << std::endl;
    std::cout << "獲取的端口: " << client_port << std::endl;

    close(sockfd);
    return 0;
}

 
