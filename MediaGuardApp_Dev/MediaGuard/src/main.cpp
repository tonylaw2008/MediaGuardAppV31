#pragma once
#include "Common.h" //全局包含文件
#include "StreamManager.h" 
#include "RtspStreamHandle.h" 
#include "ManagerController.h"
#include <openssl/crypto.h>
//test
#include "./interface/CameraMpeg.h" //CameraMpeg所有的CLOUD API基本放這裡

using namespace Stream;

//for the http server thread
std::thread m_http_server;

int main(int argc, char** argv)
{
#pragma region HeaderRegion 系統信息參考 SYSTEM INFORMATION REFERENCE

	//系統信息參考 System Information Reference
	std::cout << "\n===================== SYSTEM INFORMATION REFERENCE =====================\n" << std::endl;

	printf("OpenSSL version: %s\n", OpenSSL_version(OPENSSL_VERSION));

	// 初始化 FFmpeg 
	std::cout << "\n===================== FFMPEG AVFORMAT NETWORK INIT =====================\n" << std::endl;
	avformat_network_init();

	// 獲取 FFmpeg 的版本號
	const char* ffmpeg_version = av_version_info();
	std::cout << "\nFFmpeg Version: " << ffmpeg_version << std::endl;
	unsigned codec_version = avcodec_version();
	std::cout << "\nFFMPEG AVCODEC VERSION: "
		<< (codec_version >> 16) << "."        // 主版本號
		<< ((codec_version >> 8) & 0xFF) << "." // 次版本號
		<< (codec_version & 0xFF) << "\n" << std::endl; // 修訂版本號

	printf("avcodec_configuration details: \n%s\n\n", avcodec_configuration()); //解碼配置



	//列出支持的硬件
	std::cout << "\n===================== LIST SUPPORTED HARDWARES =====================\n" << std::endl;
	StreamMangement::list_supported_hardware();
	//列出電腦USB CAMERA
	std::cout << "\n===================== LIST DSSHOW DEVICES(USB CAMERAS) =====================\n" << std::endl;
	StreamMangement::list_dshow_device();

	std::cout << "\n\nIf it is running, you can terminate it by input exit\n" << std::endl;
	std::cout << "\n--------------------------------------------------------------------------------------\n" << std::endl;

#pragma endregion

	//--------------------------------------------------------------------------------------

	std::cout << "\n\n\nPRESS ENTER TO START .......\n\n\n"  << std::endl; 
	char c;
	std::cin.get(c);
	std::cout <<  c  << " NOW TO START......" << std::endl;
	
	std::cout <<  "Hello The World..................."  << std::endl;
	//LOG(INFO) << "Console Platform Is Required UTF8 Encoding.";
	std::cout <<  "Console Platform Is Required UTF8 Encoding." << std::endl;
	//test============================================
	/*char c;
	std::cin.get(c);
	std::cout << "your input1 is：" << c << std::endl;
	char d;
	std::cin.get(d);
	std::cout << "your input2 is：" << d << std::endl;*/

	//測試車牌配置節點
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
