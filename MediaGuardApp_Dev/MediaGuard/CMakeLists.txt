#####################################################################
#
# @brief	CMakeLists of project 
# cmake ../ -G "Visual Studio 17 2022"
# cmake ../ -G "Unix Makefiles"
# 
#####################################################################
cmake_minimum_required (VERSION 3.8)
 
# HEADER START ------------------------------------------------------------------

message("MediaGuard----------------------------------------------------------------------------------------------")
message("CMAKE_CURRENT_SOURCE_DIR = " ${CMAKE_CURRENT_SOURCE_DIR})  
message("MediaGuard----------------------------------------------------------------------------------------------")
   
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
#set project name
set(PROJECT_NAME MediaGuard)
project(${PROJECT_NAME})

set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_SOURCE_DIR})
 
# ws2def.h重定义 
if(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  /DWIN32_LEAN_AND_MEAN")
endif()

# 追加CMake查找目录
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cmake")

# 獲取項目的根路徑
message(STATUS "CMAKE ROOT: ${CMAKE_SOURCE_DIR}")
 
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/src/include/)
message(${PROJECT_NAME} "PROJECT_INCLUDE_DIR: ${PROJECT_INCLUDE_DIR}")

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)  

set(LOCAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/include)  

set(3RDPARTY_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/include)  

# HEADER END --------------------------------------------------------------------
 

# 路徑和引用系統參數
IF(WIN32) # WIN32 版本 
      
    # ffmpeg  4.2.2  改用 find_package(FFmpeg REQUIRED) 
    set(FFmpeg_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/windows/x64/ffmpeg/include)  
	set(FFmpeg_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/windows/x64/ffmpeg/lib)  

    # openssl 已經編譯版本 openssl1.1.1b
    set(Openssl_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/3rdparty/windows/x64/openssl1.1.1b/include)
    set(Openssl_LIBRARIES ${CMAKE_SOURCE_DIR}/3rdparty/windows/x64/openssl1.1.1b/lib)

     # curl  
    set(Curl_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/3rdparty/windows/x64/curl/include)
    set(Curl_LIBRARIES ${CMAKE_SOURCE_DIR}/3rdparty/windows/x64/curl/lib)
     

    # rapidjson
    set(RAPIDJSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/windows/x64/rapidjson/include)
    include_directories(${RAPIDJSON_INCLUDE_DIR}) 
 
elseif(UNIX) # LINUX 版本
   
    # ffmpeg 4.2.2 改用 find_package(FFmpeg REQUIRED) 
    set(FFmpeg_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/linux/x64/ffmpeg/include) 
	set(FFmpeg_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/linux/x64/ffmpeg/lib) 
     
    # openssl1.1.1l 改為如下 -----------------------------------------------------
    find_package(OpenSSL REQUIRED)

    # curl  
    set(Curl_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/3rdparty/linux/x64/curl/include)
    set(Curl_LIBRARIES ${CMAKE_SOURCE_DIR}/3rdparty/linux/x64/curl/lib)
     
    # rapidjson
    set(RAPIDJSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/linux/x64/rapidjson/include)
    include_directories(${RAPIDJSON_INCLUDE_DIR}) 
 
endif()
 
# 查找opencv依赖 
 # 检测操作系统 
 if(UNIX) 
    set(OpenCV_DIR "/usr/local/opencv/build")  

 # UNIX 系统上的路径 
 elseif(WIN32) 
    set(OpenCV_DIR "C:/opencv/build") 
    # 设置正确的 CUDA 工具包路径 (老是指定錯誤的12.2 需要明確指定)
    set(CUDA_TOOLKIT_ROOT_DIR "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.6")
     
    set(OpenCV_LIBRARY_DIRS "${OpenCV_DIR}/x64/vc16/lib")
   
 # Windows 系统上的路径 
 endif() 

 # 设置 opencv_contrib 路径 
 #  set(OPENCV_EXTRA_MODULES_PATH "${OpenCV_DIR}/../../opencv_contrib/modules") 


 # 查找 OpenCV 包，包括 cudev 模块 
 # find_package(OpenCV REQUIRED core imgproc highgui cudev) 
 find_package(OpenCV REQUIRED) 
 
 # 如果找不到 OpenCV，输出错误信息 
 if(NOT OpenCV_FOUND) 
    message(FATAL_ERROR "OpenCV not found at ${OpenCV_DIR}") 
 else() 
    message(STATUS "OpenCV version: ${OpenCV_VERSION}") 
    message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")
 endif()

# include local header files
include_directories(${PROJECT_INCLUDE_DIR}) 

# include 3RDPARTY_INCLUDE
include_directories(${3RDPARTY_INCLUDE}) 

# *.cpp Include directories LOCAL_SOURCES_DIR 
file(GLOB  LOCAL_SOURCES_DIR
    "./src/*.cpp"
    "./src/hmac/*.cpp"
    "./src/httpserver/*.cpp"
    "./src/httpserver/*.cc"
    "./src/interface/*.cpp"
    "./src/include/Basic/*.cpp"
    "./src/include/Common/*.cpp"
    "./src/include/Config/*.cpp"
    "./src/include/easylogging/*.cpp"
    "./src/include/ErrorInfo/*.cpp"
    "./src/include/Http/*.cpp"
    "./src/include/Httplib/*.cpp"  
    "./src/log/*.cpp" 
    "./src/util/*.cpp"
    "./src/stun/*.cpp"
)

message(STATUS "\n================ LOCAL_SOURCES_DIR ============\n")
foreach(file ${LOCAL_SOURCES_DIR})
   message(${PROJECT_NAME} "*** LOCAL_SOURCES_DIR ***: ${file}") 
endforeach() 

# *.h Include directories LOCAL_SOURCES_DIR 
file(GLOB LOCAL_HEADER_DIR 
    "./src/*.h"
    "./src//hmac/*.h"
    "./src//httpserver/*.h"
    "./src//include/Basic/*.h"
    "./src//include/Common/*.h"
    "./src//include/Config/*.h"
    "./src//include/easylogging/*.h"
    "./src//include/ErrorInfo/*.h"
    "./src//include/Http/*.h"
    "./src//include/Httplib/*.h"
    "./src//interface/*.h"
    "./src/log/*.h" 
    "./src/util/*.h"
    "./src/stun/*.h"
)
message(STATUS "\n================ LOCAL_HEADER_DIR ============\n")
foreach(file ${LOCAL_HEADER_DIR})
   message(${PROJECT_NAME} "*** GLOB  LOCAL_HEADER_DIR ***:: ${file}") 
endforeach()
 
# 排除某個目錄 這部分直接複製到 Ouput 就可以配合MediaGuard.exe 運行
list(REMOVE_ITEM SOURCES "src/web/*.*")       # web前端    
list(REMOVE_ITEM SOURCES "src/picture/*.*")   # 保存圖片文件夾 
list(REMOVE_ITEM SOURCES "src/video/*.*")     # 保存mp4文件夾 
list(REMOVE_ITEM SOURCES "src/conf/*.*")      # 配置文件

# opencv 4.8.1 來自 3rdParty的opencv的情況
include_directories(${OpenCV_INCLUDE_DIRS}) 
link_directories(${OpenCV_LIBRARY_DIRS})

# 添加opencv头文件 系統安裝opencv的情況 find_package 形式
# include_directories( ${OpenCV_INCLUDE_DIRS} )

# ffmpeg 4.2.2
include_directories(${FFmpeg_INCLUDE_DIRS}) 
link_directories(${FFmpeg_LIBRARY_DIRS})

# Openssl 
include_directories(${Openssl_INCLUDE_DIRS}) 
link_directories(${Openssl_LIBRARIES})

# Curl
include_directories(${Curl_INCLUDE_DIRS}) 
link_directories(${Curl_LIBRARIES})
 
# -------------------------------------------------------------------------------------------------------------
# 将源代码添加到此项目的可执行文件
add_executable(${PROJECT_NAME} ${LOCAL_SOURCES_DIR} ${LOCAL_HEADER_DIR})
# -------------------------------------------------------------------------------------------------------------

# target_compile_definitions 添加定义，这样可以防止特定宏冲突：
target_compile_definitions(${PROJECT_NAME} PRIVATE -D_SOME_MACRO_TO_PREVENT_CONFLICT)


# 以下的引用需要放在 add_executable 函數後面
 
# 设置包含目录，确保项目中可以找到头文件
target_include_directories(${PROJECT_NAME} PRIVATE ${LOCAL_INCLUDE_DIR})
  
 
# 鏈接文件 缺失的保留 以後需要的話,按照這個格式寫
# 鏈接 FFmpeg 庫  # 自動提示缺失 FFmpeg_LIBRARY_DIRS
# 鏈接 OpenCV 庫  # OpenCV_LIBS :: OK
# 鏈接 OpenSSL 庫 # 自動提示缺失 Openssl_LIBRARIES
# 鏈接 Curl 庫 # 自動提示缺失 Curl_LIBRARIES

IF(WIN32)
    message("11111111111111111 -  WIN32 | MSVC")
    # 设置 exe 输出路径(exe输出到 dll 目录下)
    SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/output)

    target_link_libraries(${PROJECT_NAME} PRIVATE
     
	   # ${FFmpeg_LIBRARY_DIRS}
	   # ${Openssl_LIBRARIES}
       # ${Curl_LIBRARIES} 
          
        # FFmpeg
        avcodec.lib;
        avformat.lib;
        avutil.lib;
        swscale.lib;
        avdevice.lib;
        avfilter.lib;
        postproc.lib;
        swresample.lib;
        postproc.lib
         
        # opencv 
        ${OpenCV_LIBS}
        opencv_world481d
        # curl 
        libcrypto.lib;
        libssl.lib; 
        libcrypto_static.lib;
        libcurl_debug.lib 
) 
elseif(UNIX)
     message("11111111111111111 - UNIX") 

    # LINUX版本
   target_link_libraries(${PROJECT_NAME} PRIVATE

	    ${FFmpeg_LIBRARY_DIRS}
	    ${Openssl_LIBRARIES}
        ${Curl_LIBRARIES} 
          
        # FFmpeg
        # FFmpeg
        avcodec;
        avformat;
        avutil;
        swscale;
        avdevice;
        avfilter;
        postproc;
        swresample;
         
        # opencv 
        ${OpenCV_LIBS}
        
        #openssl1.1.1q
        OpenSSL::SSL
        OpenSSL::Crypto

        # curl
        libcurl.so; 
    )
endif()
 

message("=================== ${PROJECT_NAME} =======CMAKE SETTING BUILD END")
