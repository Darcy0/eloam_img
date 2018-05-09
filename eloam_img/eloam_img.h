#pragma  once

#include "afx.h"
#include <vector>
#include "include/eloamDll.h"

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
	int devIndex;					//设备索引
	std::vector<int> subtypes;		//支持的出图类型 0x01=YUY2,0x02=MJPG,0x03=UYVY,0x04=RGB24,0x05=H264
	std::vector<EloamSize> sizes;	//支持的分辨率
	int resIndex;					//选择的分辨率索引
	int channels;					//图像通道数
	HELOAMVIEW viewWnd;				//预览窗口
	HELOAMVIDEO video;				//视频对象
	DevInfo()
	{
		hDev=NULL;
		displayName=NULL;
		friendlyName=NULL;
		devType=-1;
		devIndex=-1;
		resIndex=-1;
		channels=3;//默认彩色
		viewWnd=NULL;
		video=NULL;
	}
};
//设备是否初始化
bool g_isInit=false;
//摄像头最大设备ID
int g_maxDeviceNum=-1;
//设备全局变量
std::vector<DevInfo> g_cameras;

//设备拔插回调函数
VOID ELOAMAPI DevChangeCallback(LONG type, LONG idx, LONG dbt, LPVOID userData);

//获取支持的出图格式
void GetSubtype(long subtype,std::vector<int>& subtypeIndex);

//根据设备索引获取设备信息
DevInfo *GetDevInfo(int iDeviceNum,int &index);

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