# 開發手冊 DEV MANUAL

#  

## 開發環境 

### 	**安裝組件指導文件 **

​	 **./DEVELOP_LOG/README_組件安裝.md**
​	(備份版本: ./MediaGuardAppV31/MediaGuard_DOC/README_**組件安裝.md 或 README_組件安裝.pdf)

​	包括Windows 與 Linux 的組件安裝.



## 功能與業務

### 	項目中的任務常量

```
//用與定義業務功能開發種類
enum class TaskType
{
	/// <summary>
	/// 任務類型: UNDEFINED 未定義的任務
	/// </summary> 
	UNDEFINED = 0,

	///// <summary>
	///// 任務類型:CAMERA_GUARD 桌面人臉識別系統
	///// </summary> 
	//CAMERA_GUARD = 1,

	///// <summary>
	///// 任務類型: CAMERA_DVR 桌面錄像系統
	///// </summary> 
	//CAMERA_DVR = 2,

	///// <summary>
	///// 任務類型: HIK_DATA_RETRIVE 桌面海康設備數據獲取系統
	///// </summary> 
	//DESKTOP_HIK_DATA_RETRIVE = 3,

	///// <summary>
	///// 任務類型: HIK_DATA_ANDROID_RETRIVE  手機版的海康設備數據獲取系統
	///// </summary> 
	//ANDROID_HIK_DATA_RETRIVE = 4,

	///// <summary>
	///// 任務類型: ANDROID_CIC_DATA_RETRIVE  手機版的 CIC的NFC拍卡數據獲取系統
	///// </summary> 
	//ANDROID_CIC_DATA_RETRIVE = 5,

	/// <summary>
	/// 任務類型: 车牌识别 POST到Python处理后返回
	/// </summary> 
	CAR_PLATE_RECOGNITION = 6,

	/// <summary>
	/// 任務類型: 人脸识别
	/// </summary>
	FACE_RECOGNITION = 7,

	/// <summary>
	/// 任務類型: 工程着装识别
	/// </summary> 
	WORK_CLOTHES_RECOGNITION = 8,

	/// <summary>
	/// 任務類型: 佩戴头盔识别
	/// </summary> 
	WEARING_HELMET_RECOGNITION = 9,

	/// <summary>
	/// 任務類型: 有人闖入
	/// </summary> 
	SOMEONE_BROKE_IN_ENABLE = 10
};

```

### 	

 

## 客戶端 HTTP API

​	客戶端 HTTP API : 不是雲端的API, 而是 [MEDIAGUARD] 提供的對外API 與web請求.

​	客戶端 URL : http://192.168.0.128:180/web/index.html
