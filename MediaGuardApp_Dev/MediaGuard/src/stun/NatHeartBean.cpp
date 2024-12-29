#include "NatHeartBean.h"

NatHeartBean::~NatHeartBean()
{
    // 析構函數
}

NatHeartBean::NatHeartBean()
{
    // 構造函數
} 
 

/*
* 開發說明: 目前
*/


// 獲取本地Internet IP 和端口 // STUN NAT 目前有點問題
void NatHeartBean::get_local_internet_ip_and_port(char* &local_ip, int &local_port) {
     
    char local_internet_ip[INET_ADDRSTRLEN]; // 定義 local_internet_ip
    unsigned short local_internet_port; // 定義 local_port
    
    // 創建一個 UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return;
    }

    // 設置本地地址結構
    struct sockaddr_in local_internet_addr;
    memset(&local_internet_addr, 0, sizeof(local_internet_addr));
    local_internet_addr.sin_family = AF_INET;
    local_internet_addr.sin_port = 0; // 讓系統自動分配端口
    local_internet_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 綁定到所有可用接口
     
    // 獲取綁定的端口
    socklen_t addr_len = sizeof(local_internet_addr);
    if (bind_socket(sockfd, (struct sockaddr*)&local_internet_addr, sizeof(local_internet_addr)) < 0) {
        std::cerr << "Failed to bind socket." << std::endl;

#ifdef _WIN32
        closesocket(sockfd); // 在 Windows 中使用 closesocket
#else
        close(sockfd); // 在 Linux 中使用 close
#endif
        return;
    }
     
    // 獲取本機 IP 地址和端口
    inet_ntop(AF_INET, &local_internet_addr.sin_addr, local_internet_ip, sizeof(local_internet_ip));
    local_internet_port = ntohs(local_internet_addr.sin_port);

#ifdef DEBUG
    std::cout << "local_internet_ip: " << local_internet_ip << std::endl;
    std::cout << "local_internet_port: " << local_internet_port << std::endl;
#endif // DEBUG
     
    // 檢查是否是有效的互聯網 IP
    if (strcmp(local_internet_ip, "127.0.0.1") == 0 ||
        strncmp(local_internet_ip, "10.", 3) == 0 ||
        strncmp(local_internet_ip, "172.", 4) == 0 ||
        strncmp(local_internet_ip, "192.168.", 8) == 0) {
        std::cerr << "Obtained a private or loopback IP: " << local_internet_ip << std::endl;
        // 根據需求處理私有或回環地址
    }
    else {
        // 分配內存並拷貝 INTERNET IP 地址 
        strcpy(local_ip, local_internet_ip);
        local_port = local_internet_port;
    }
     
    // 關閉 socket
#ifdef _WIN32
    closesocket(sockfd);
#else
   close(sockfd); // 關閉 socket 句柄
#endif
}

// 包裝 bind 函數 避免socket的bind函數名稱衝突
int  NatHeartBean::bind_socket(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    return ::bind(sockfd, addr, addrlen);
}

// 獲取 STUN 伺服器的 IP 和端口
void NatHeartBean::get_server_internet_ip(char*& stun_server_ip, int& stun_server_port) {

    // 創建 STUN 請求
    create_stun_request();

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return;
    }

    struct addrinfo hints, * res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP

    // 解析主機名
    if (getaddrinfo(STUN_SERVER_IP, nullptr, &hints, &res) != 0) {
        std::cerr << "Failed to resolve hostname." << std::endl;
        return;
    }

    // 將解析的地址設置到 server_addr 中
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(STUN_SERVER_PORT);
    server_addr.sin_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr;
    // 釋放資源
    freeaddrinfo(res);
  
	// 將 STUN 伺服器的 IP 和端口設置到 server_addr 中
    inet_pton(AF_INET, STUN_SERVER_IP, &server_addr.sin_addr);
     
    // 發送請求 
    sendto(sockfd, reinterpret_cast<const char*>(stun_request), request_length,  0, (struct sockaddr*)&server_addr, sizeof(server_addr));
     
    // 接收響應
    char buffer[1024];
    socklen_t addr_len = sizeof(server_addr);

    struct timeval timeout;
    timeout.tv_sec = 5; // 超時時間，單位為秒
    timeout.tv_usec = 0; // 微秒
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)); 
    size_t recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &addr_len);

    if (recv_len < 0) {
        perror("recvfrom failed");
        // 處理錯誤
    }
    else {
        // 打印接收到的 buffer 內容
        std::cout << "[Stun Protocol] Receive the socket data: recv_len = "<< recv_len <<"\n" << std::endl;
		//內容太大 不打印
       /* for (size_t i = 0; i < recv_len; ++i) {
            printf("%02x ", static_cast<unsigned char>(buffer[i]));
        }*/
    }

    // 假設 buffer 是接收到的響應
    if (recv_len > 0) {
         
        char c_server_ip[INET_ADDRSTRLEN]; 
       
        // 確保接收到的數據足夠長
        if (recv_len >= 20) { // STUN 消息的最小長度
            // 檢查消息類型和標誌
            uint16_t message_type = (buffer[0] << 8) | buffer[1];
            if (message_type == 0x0101) { // 0x0101 是成功響應的消息類型
                // 提取 Mapped Address
                uint16_t attribute_type = (buffer[28] << 8) | buffer[29]; // 假設 Mapped Address 在第 28 和 29 字節
                uint16_t address_length = (buffer[30] << 8) | buffer[31]; // 地址長度
                uint32_t client_ip_addr = *(uint32_t*)&buffer[32]; // IP 地址
                unsigned short u_server_port = ntohs(*(uint16_t*)&buffer[36]); // 端口 
                inet_ntop(AF_INET, &client_ip_addr, c_server_ip, sizeof(c_server_ip));
                std::cout << "stun_server_ip: " << c_server_ip << std::endl;
                std::cout << "stun_server_port: " << u_server_port << std::endl;
            }
        }
    }
     

#ifdef _WIN32
    closesocket(sockfd);
#else
    close(sockfd); // 關閉 socket 句柄
#endif

    return;
}

// ...
void NatHeartBean::create_stun_request() {
    // 清空請求
    memset(stun_request, 0, STUN_REQUEST_SIZE);

    // 設置請求的標頭
    // stun_request 的結構設置如下：
    // stun_request[0]：版本和類型
    // stun_request[1]：方法碼（Binding Request）
    // stun_request[2] 和 stun_request[3]：消息長度（此處設置為 0，因為沒有額外屬性）
    // stun_request[8]：識別符的開始位置
	//--------------------------------------------------------------------------------
     // 設置請求的標頭
    stun_request[0] = 0x00; // 版本和類型
    stun_request[1] = 0x01; // 方法碼（Binding Request）

    // 設置消息長度（這裡為 0，因為沒有額外的屬性）
    stun_request[2] = 0x00;
    stun_request[3] = 0x00;
     
    // 設置識別符（隨機生成，這裡簡化處理）
    uint32_t transaction_id = rand(); // 隨機生成一個識別符
    memcpy(&stun_request[8], &transaction_id, sizeof(transaction_id)); // 將識別符放入請求中

    // 設置請求長度
    request_length = STUN_REQUEST_SIZE; 
}

//獲取公網IP,並返回IP OK ★★★★★ 2024-12-29
std::string NatHeartBean::get_public_ip_by_curl() {
    // 使用 curl 命令獲取公共 IP
    system("curl -s http://ifconfig.me > public_ip_by_curl.txt");

    // 讀取 ip.txt 文件中的內容
    FILE* file = fopen("public_ip_by_curl.txt", "r");
    if (!file) {
        std::cerr << "Failed to open ip.txt" << std::endl;
        return "127.0.0.1";
    }

    char ip[16]; // IPv4 地址最大長度
    if (fgets(ip, sizeof(ip), file) != nullptr) {
        fclose(file);
        return std::string(ip);
    }

    fclose(file);
    return "127.0.0.1"; //默認失敗的值而不是 string.empty
}

// 通過內存獲取 curl客戶端命令結果 必須安裝curl(windows 內置的,Linux需要安裝package)
// curl 命令獲得公網IP OK  ★★★★★ 2024-12-29
std::string NatHeartBean::get_public_ip_by_curl_memory() {

    // 使用 curl 命令獲取公共 IP
    const char* command = "curl -s http://ifconfig.me";
     
    // 打開管道以執行命令
#ifdef WIN32
    int _pclose(FILE * stream);
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command, "r"), _pclose);
#endif

#ifdef _linux_
    int pclose(FILE * stream);
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);
#endif
    

    if (!pipe) {
        throw std::runtime_error("_popen() failed!");
    }

    char buffer[128];
    std::string result;

    // 讀取命令的輸出
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }
    
    return result; 
}

