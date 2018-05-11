#pragma  once

#include "afx.h"
#include <vector>
#include "eloam.h"
#include "./include/ZZImage.h"
#include "ZZImage_def.h"

#define CAMERADEV_API  __declspec(dllexport)

struct EloamSize
{
	int width;
	int height;
	EloamSize()
	{
		width=0;
		height=0;
	}
};

struct DevInfo
{
	HELOAMDEVICE hDev;				//设备句柄
	BSTR displayName;				//设备显示名称
	BSTR friendlyName;				//设备友好名称
	int devType;					//设备类型 1=主摄像头 2、3=辅助摄像头
	int developerIndex;				//开发者定义的设备索引
	int userIndex;					//用户定义的设备索引
	std::vector<int> subtypes;		//支持的出图类型 0x01=YUY2,0x02=MJPG,0x03=UYVY,0x04=RGB24,0x05=H264
	std::vector<EloamSize> sizes;	//支持的分辨率
	int resIndex;					//选择的分辨率索引
	int channels;					//图像通道数
	BOOL isDeskew;					//是否纠偏裁边
	HELOAMVIEW viewWnd;				//预览窗口
	HELOAMVIDEO video;				//视频对象
	DevInfo()
	{
		hDev=NULL;
		displayName=NULL;
		friendlyName=NULL;
		devType=-1;
		developerIndex=-1;
		userIndex=-1;
		resIndex=-1;
		channels=3;//默认彩色
		isDeskew=FALSE;
		viewWnd=NULL;
		video=NULL;
	}
};

//!eloamDll.dll函数指针
//设备初始化函数指针
pEloamGlobal_InitDevs g_pInitDevs=NULL;
//设备反初始化函数指针
pEloamGlobal_DeinitDevs g_pDeinitDevs=NULL;
//获取设备个数
pEloamGlobal_GetDevCount g_pGetDevCount=NULL;
//获取设备类型函数指针
pEloamGlobal_GetEloamType g_pGetEloamType=NULL;
//创建设备函数指针
pEloamGlobal_CreateDevice g_pCreateDevice=NULL;
//释放设备函数指针
pEloamDevice_Release g_pDeviceRelease=NULL;
//获取显示名称函数指针
pEloamDevice_GetDisplayName g_pGetDisplayName=NULL;
//获取友好名称函数指针
pEloamDevice_GetFriendlyName g_pGetFriendlyName=NULL;
//释放文字函数指针
pEloamGlobal_DestroyString g_pDestroyString=NULL;
//获取出图格式函数指针
pEloamDevice_GetSubtype g_pGetSubtype=NULL;
//获取分辨率个数函数指针
pEloamDevice_GetResolutionCountEx g_pGetResolutionCountEx=NULL;
//获取分辨率宽度函数指针
pEloamDevice_GetResolutionWidthEx g_pGetResolutionWidthEx=NULL;
//获取分辨率高度函数指针
pEloamDevice_GetResolutionHeightEx g_pGetResolutionHeightEx=NULL;
//创建视频窗口对象函数指针
pEloamGlobal_CreateView g_pCreateView=NULL;
//释放视频窗口对象函数指针
pEloamView_Release g_pViewRelease=NULL;
//视频窗口设置显示文字函数指针
pEloamView_SetText g_pViewSetText=NULL;
//创建视频函数指针
pEloamDevice_CreateVideo g_pCreateVideo=NULL;
//视频对象释放函数指针
pEloamVideo_Release g_pVideoRelease=NULL;
//将视频显示于窗口对象函数指针
pEloamView_SelectVideo g_pViewSelectVideo=NULL;
//创建图像函数指针
pEloamVideo_CreateImage g_pCreateImage=NULL;
//从文件获取图片函数指针
pEloamGlobal_CreateImageFromFile g_CreateImageFromFile=NULL;
//释放图像函数指针
pEloamImage_Release g_pImageRelease=NULL;
//纠偏去边函数指针
pEloamImage_Deskew g_pImageDeskew=NULL;
//自适应二值化函数指针
pEloamImage_AdaptiveThreshold g_pAdaptiveThreshold=NULL;
//图片彩色话函数指针
pEloamImage_ToColor g_pImageToColor=NULL;
//图片旋转函数指针
pEloamImage_Rotate g_pImageRotate=NULL;
//图片保存函数指针
pEloamImage_Save g_pImageSave=NULL;
//视频纠偏裁边函数指针
pEloamVideo_EnableDeskew g_pVideoEnableDeskew=NULL;
//取消视频纠偏裁边函数指针
pEloamVideo_DisableDeskew g_pVideoDisableDeskew=NULL;
//视频灰度化函数指针
pEloamVideo_EnableGray g_pVideoEnableGray=NULL;

//!ZZBase.dll函数函数指针
//纠偏裁边初始化函数指针
pZZInitSdk g_ZZInitSdk=NULL;
//纠偏裁边下载图片函数指针
pZZLoadImage g_ZZLoadImage=NULL;
//纠偏裁边获取物体区域坐标函数指针
pZZGetImageObjectRegionPos g_ZZGetImageObjectRegionPos=NULL;
//纠偏裁边从区域坐标信息提取区域信息函数指针
pZZGetImageRegionInfo g_ZZGetImageRegionInfo=NULL;
//纠偏裁边创建图像函数指针
pZZCreateImage g_ZZCreateImage=NULL;
//纠偏裁边分离物体函数指针
pZZSplitImageRegion g_ZZSplitImageRegion=NULL;
//纠偏裁边保存图片函数指针
pZZSaveImage g_ZZSaveImage=NULL;
//纠偏裁边释放图片函数指针
pZZDestroyImage g_ZZDestroyImage=NULL;
//纠偏裁边反初始化函数指针
pZZDeinitSdk g_ZZDeinitSdk=NULL;

//设备是否初始化
bool g_isInit=false;
//动态库函数是否加载
bool g_isLoadDllFun=false;
//摄像头最大设备ID
int g_deviceCnt=0;
//设备全局变量
std::vector<DevInfo> g_cameras;
//eloamDLL动态库句柄
HINSTANCE g_eloamDll=NULL;
//ZZBase动态库句柄
HINSTANCE g_ZZBaseDll=NULL;

//设备拔插回调函数
VOID ELOAMAPI DevChangeCallback(LONG type, LONG idx, LONG dbt, LPVOID userData);
//视频到达回调
VOID ELOAMAPI AttachCallback(HELOAMVIDEO video, LONG videoId, HELOAMVIEW view, LONG viewId, LPVOID userData);
//加载动态库模块
int loadDllModule();
//加载用到的函数
int LoadDllFunction();
//获取动态库函数指针的函数指针
void *GetFunctionPointer( const char* funName);
//设置当前进程环境变量
int SetCurrentEnvPath();
//获取支持的出图格式
void GetSubtype(long subtype,std::vector<int>& subtypeIndex);
//根据设备索引获取设备信息
DevInfo *GetDevInfo(int iDeviceNum,int &index);
//获取摄像头开发者索引
int GetDeveloperDevId(int userIndex);
//释放指定ID的设备资源
int ReleaseDevResource(int userIndex);
//图片纠偏裁边并保存
BOOL ImageDeskew(HELOAMIMAGE hEloamImg,const char* saveFileName);


/************************************************************************/
/* 功能:
/*		初始化摄像头
/* 输入参数:
/*		iDeviceNum:摄像头设备ID。例如：0,1,2……等
/* 输出参数:
/*		无
/* 返回值:
/*		0:成功,其他表示失败
/************************************************************************/
extern "C" CAMERADEV_API int _cdecl ggcaInit(int iDeviceNum);

/************************************************************************/
/* 功能:
/*		释放摄像头
/* 输入参数:
/*		iDeviceNum:摄像头设备ID。例如：0,1,2……等
/* 输出参数:
/*		无
/* 返回值:
/*		0:成功,其他表示失败
/************************************************************************/
extern "C" CAMERADEV_API int _cdecl ggcaRelease(int iDeviceNum);

/************************************************************************/
/* 功能:
/*		设置预览分辨率
/* 输入参数:
/*		iDeviceNum:摄像头设备ID;
/*		iWidth:宽度;
/*		iHeight:高度;
/*		iBPP:颜色深度
/* 输出参数:	
/*		无
/* 返回值:
/*		0:成功,其他表示失败
/************************************************************************/
extern "C" CAMERADEV_API int _cdecl ggcaSetResolution(int iDeviceNum, int iWidth, int iHeight, int iBPP);

/************************************************************************/
/* 功能:
/*		设置预览窗口
/* 输入参数:
/*		iDeviceNum:摄像头设备ID;
/*		hControl:预览窗口句柄
/* 输出参数:	
/*		无
/* 返回值:
/*		0:成功,其他表示失败
/************************************************************************/
extern "C" CAMERADEV_API int _cdecl ggcaSetPrvWnd(int iDeviceNum,HWND hControl);

/************************************************************************/
/* 功能:
/*		开始预览
/* 输入参数:
/*		iDeviceNum:摄像头设备ID;
/*		iType:打开方式，0：直接打开（目前只能为0）
/* 输出参数:	
/*		无
/* 返回值:
/*		0:成功,其他表示失败
/************************************************************************/
extern "C" CAMERADEV_API int _cdecl ggcaStart(int iDeviceNum,int iType);

/************************************************************************/
/* 功能:
/*		关闭预览
/* 输入参数:
/*		iDeviceNum:摄像头设备ID;
/*		iType:关闭方式，目前只能为0
/* 输出参数:	
/*		无
/* 返回值:
/*		0:成功,其他表示失败
/************************************************************************/
extern "C" CAMERADEV_API int _cdecl ggcaStop(int iDeviceNum,int iType);

/************************************************************************/
/* 功能:
/*		获取图片
/* 输入参数:
/*		iDeviceNum:摄像头设备ID;
/*		filename：保存的图像文件名，如果为空，则弹出设置文件名对话框;
/*		colorType：图片色彩,0-彩色24位，1-黑白(阈值)，2-黑白(自动);
/*		iClearBK：是否去边；0-不去边，1-去边，默认为0;
/*		iDeskew：是否纠偏，0-不纠偏，1-纠偏，默认为0;
/*		jpgCompress：jpg图像压缩比，范围为：0-100，默认为50;
/*		iRotateType：旋转，1-右转90°，2-旋转180°，3-左转90°，0-不旋转(默认)
/* 输出参数:	
/*		无
/* 返回值:
/*		成功:返回生成图像文件的文件名,失败:返回空
/************************************************************************/
extern "C" CAMERADEV_API char* _cdecl ggcaGetImage(int iDeviceNum , char *filename , int colorType , BOOL iClearBK=0, BOOL iDeskew=0, int jpgCompress=50, int iRotateType=0);

/************************************************************************/
/* 功能:
/*		动态显示裁边
/* 输入参数:
/*		iDeviceNum:摄像头设备ID;
/*		dynamicClear:FALSE不显示裁边，TRUE显示裁边
/* 输出参数:	
/*		无
/* 返回值:
/*		0:成功,其他表示失败
/************************************************************************/
extern "C" CAMERADEV_API int _cdecl ggcaSetDynamicClear(int iDeviceNum,BOOL dynamicClear);