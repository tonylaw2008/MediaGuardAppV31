# 關於 c++ stun 局域網穿透 



## 局域網穿透步驟

在 C++ 中實現 STUN（Session Traversal Utilities for NAT）以進行局域網穿透，通常涉及以下幾個步驟：

1. **了解 STUN 協議**：STUN 是一種用於 NAT 穿透的協議，主要用於發現公網 IP 和端口。它通過向 STUN 伺服器發送請求，然後接收伺服器的響應來實現。

2. **選擇 STUN 庫**：可以使用現成的 STUN 庫來簡化開發過程。一些流行的 C++ STUN 庫包括：
   - **libnice**：一個用於實現 WebRTC 的庫，支持 STUN 和 TURN。
   - **reSIProcate**：一個 SIP 協議棧，內部也有 STUN 的實現。

3. **設置 STUN 客戶端**：
   - 使用所選庫建立 STUN 客戶端。
   - 配置 STUN 伺服器的地址（例如，`stun.l.google.com:19302`）。

4. **發送 STUN 請求**：使用庫的 API 發送 STUN 請求到伺服器，並處理響應，以獲取公網 IP 和端口。

5. **處理 NAT 類型**：根據 STUN 的響應來判斷 NAT 類型（例如，全通透、對稱、保護等），這將影響後續的連接策略。

6. **實現連接邏輯**：根據獲得的公網地址，實現與其他客戶端的連接邏輯。可能還需要考慮使用 TURN 伺服器來進一步解決 NAT 穿透問題。

下面是一個簡單的示範代碼片段，使用假設的 STUN 庫來獲取公網 IP：

```cpp
#include <iostream>
#include "stun_client.h" // 假設的 STUN 客戶端庫

int main() {
    StunClient stunClient;
    std::string stunServer = "stun.l.google.com:19302";
    
    if (stunClient.sendRequest(stunServer)) {
        std::string publicIp = stunClient.getPublicIp();
        unsigned short publicPort = stunClient.getPublicPort();
        
        std::cout << "Public IP: " << publicIp << std::endl;
        std::cout << "Public Port: " << publicPort << std::endl;
    } else {
        std::cerr << "STUN request failed." << std::endl;
    }

    return 0;
}
```

這個代碼片段僅作為示範，實際使用中需要根據所選庫的具體 API 進行調整。希望這些信息能幫助您開始使用 C++ 進行 STUN 局域網穿透！如果您有更具體的問題或需要進一步的幫助，請隨時告訴我。

CMD 命令實現 失敗

```
telnet stun.l.google.com:19302   //參考這個 https://webrtc.org.cn/real-world-webrtc-5/
正連線到 stun.l.google.com:19302...無法開啟到主機的連線， 在連接埠 23: 連線失敗
```



## 路由器通常需要開啟 NAT

在局域網（LAN）環境中，路由器通常需要開啟 NAT（Network Address Translation）功能，以便將內部網絡的私有 IP 地址轉換為公網 IP 地址。這樣做的原因包括：

1. **IP 地址節省**：NAT 允許多個設備使用同一個公網 IP 地址，從而節省 IP 地址資源。

2. **安全性**：NAT 隱藏了內部網絡的結構，外部設備無法直接訪問內部設備，提高了安全性。

3. **簡化網絡設置**：用戶不需要為每個設備配置公網 IP 地址，只需配置私有 IP 地址即可。

如果你的網絡需要與外部互聯網進行通信（例如，訪問網站、下載文件等），那麼路由器的 NAT 功能是必須開啟的。如果你只是在局域網內部進行通信，則可以不使用 NAT，但這在大多數情況下並不常見。

總結來說，對於大多數家庭和小型辦公室的路由器，開啟 NAT 是必要的，以確保正常的互聯網連接和安全性。
