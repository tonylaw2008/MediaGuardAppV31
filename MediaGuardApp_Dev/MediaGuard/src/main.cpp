#pragma once
#include "Common.h" //全局包含文件
#include "StreamManager.h" 
#include "RtspStreamHandle.h" 
#include "ManagerController.h"

//test
//#include "./interface/CameraMpeg.h" //CameraMpeg所有的CLOUD API基本放這裡

using namespace Stream;

//for the http server thread
std::thread m_http_server;

int main(int argc, char** argv)
{ 
	//--------------------------------------------------------------------------------------

	std::cout << "\n\n\nPRESS ENTER TO START .......\n\n\n"  << std::endl; 
	char c;
	std::cin.get(c);
	std::cout <<  c  << " NOW TO START......" << std::endl;
	
	std::cout <<  "Hello The World..................."  << std::endl;
	//LOG(INFO) << "Console Platform Is Required UTF8 Encoding.";
	std::cout <<  "Console Platform Is Required UTF8 Encoding." << std::endl;

	
	//測試車牌配置節點 test============================================ 
	/*auto permitedCamList = DEVICE_CONFIG.cfgCarPlateRecogBusiness.PermitedCamList;
	for (auto item = permitedCamList.begin(); item != permitedCamList.end(); ++item)
	{
		std::cout << "permitedCamList ..................." << item->CameraId << std::endl;
	}*/


	//================================================================================================  
	// 正式開始運行程式
	//================================================================================================  
	ManagerController::main_initialize();

	m_http_server = std::thread(std::bind(&ManagerController::http_server_start));

	//------------------------------------------------------------------
	ManagerController managerController;

	//managerController.run_media_list();  //硬數據測試
	managerController.run_media_batch_list(); //多路camera 啟動處理 

	//任意键退出 前需要关闭其他线程
	managerController.Uninit();

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));

	std::cout << "Press any key to exit...";
	std::cin.get(); 

	//close the http server thread
	if (m_http_server.joinable())
		m_http_server.join();

	return 0;
}
