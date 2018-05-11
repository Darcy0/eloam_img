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
	HELOAMDEVICE hDev;				//�豸���
	BSTR displayName;				//�豸��ʾ����
	BSTR friendlyName;				//�豸�Ѻ�����
	int devType;					//�豸���� 1=������ͷ 2��3=��������ͷ
	int developerIndex;				//�����߶�����豸����
	int userIndex;					//�û�������豸����
	std::vector<int> subtypes;		//֧�ֵĳ�ͼ���� 0x01=YUY2,0x02=MJPG,0x03=UYVY,0x04=RGB24,0x05=H264
	std::vector<EloamSize> sizes;	//֧�ֵķֱ���
	int resIndex;					//ѡ��ķֱ�������
	int channels;					//ͼ��ͨ����
	BOOL isDeskew;					//�Ƿ��ƫ�ñ�
	HELOAMVIEW viewWnd;				//Ԥ������
	HELOAMVIDEO video;				//��Ƶ����
	DevInfo()
	{
		hDev=NULL;
		displayName=NULL;
		friendlyName=NULL;
		devType=-1;
		developerIndex=-1;
		userIndex=-1;
		resIndex=-1;
		channels=3;//Ĭ�ϲ�ɫ
		isDeskew=FALSE;
		viewWnd=NULL;
		video=NULL;
	}
};

//!eloamDll.dll����ָ��
//�豸��ʼ������ָ��
pEloamGlobal_InitDevs g_pInitDevs=NULL;
//�豸����ʼ������ָ��
pEloamGlobal_DeinitDevs g_pDeinitDevs=NULL;
//��ȡ�豸����
pEloamGlobal_GetDevCount g_pGetDevCount=NULL;
//��ȡ�豸���ͺ���ָ��
pEloamGlobal_GetEloamType g_pGetEloamType=NULL;
//�����豸����ָ��
pEloamGlobal_CreateDevice g_pCreateDevice=NULL;
//�ͷ��豸����ָ��
pEloamDevice_Release g_pDeviceRelease=NULL;
//��ȡ��ʾ���ƺ���ָ��
pEloamDevice_GetDisplayName g_pGetDisplayName=NULL;
//��ȡ�Ѻ����ƺ���ָ��
pEloamDevice_GetFriendlyName g_pGetFriendlyName=NULL;
//�ͷ����ֺ���ָ��
pEloamGlobal_DestroyString g_pDestroyString=NULL;
//��ȡ��ͼ��ʽ����ָ��
pEloamDevice_GetSubtype g_pGetSubtype=NULL;
//��ȡ�ֱ��ʸ�������ָ��
pEloamDevice_GetResolutionCountEx g_pGetResolutionCountEx=NULL;
//��ȡ�ֱ��ʿ�Ⱥ���ָ��
pEloamDevice_GetResolutionWidthEx g_pGetResolutionWidthEx=NULL;
//��ȡ�ֱ��ʸ߶Ⱥ���ָ��
pEloamDevice_GetResolutionHeightEx g_pGetResolutionHeightEx=NULL;
//������Ƶ���ڶ�����ָ��
pEloamGlobal_CreateView g_pCreateView=NULL;
//�ͷ���Ƶ���ڶ�����ָ��
pEloamView_Release g_pViewRelease=NULL;
//��Ƶ����������ʾ���ֺ���ָ��
pEloamView_SetText g_pViewSetText=NULL;
//������Ƶ����ָ��
pEloamDevice_CreateVideo g_pCreateVideo=NULL;
//��Ƶ�����ͷź���ָ��
pEloamVideo_Release g_pVideoRelease=NULL;
//����Ƶ��ʾ�ڴ��ڶ�����ָ��
pEloamView_SelectVideo g_pViewSelectVideo=NULL;
//����ͼ����ָ��
pEloamVideo_CreateImage g_pCreateImage=NULL;
//���ļ���ȡͼƬ����ָ��
pEloamGlobal_CreateImageFromFile g_CreateImageFromFile=NULL;
//�ͷ�ͼ����ָ��
pEloamImage_Release g_pImageRelease=NULL;
//��ƫȥ�ߺ���ָ��
pEloamImage_Deskew g_pImageDeskew=NULL;
//����Ӧ��ֵ������ָ��
pEloamImage_AdaptiveThreshold g_pAdaptiveThreshold=NULL;
//ͼƬ��ɫ������ָ��
pEloamImage_ToColor g_pImageToColor=NULL;
//ͼƬ��ת����ָ��
pEloamImage_Rotate g_pImageRotate=NULL;
//ͼƬ���溯��ָ��
pEloamImage_Save g_pImageSave=NULL;
//��Ƶ��ƫ�ñߺ���ָ��
pEloamVideo_EnableDeskew g_pVideoEnableDeskew=NULL;
//ȡ����Ƶ��ƫ�ñߺ���ָ��
pEloamVideo_DisableDeskew g_pVideoDisableDeskew=NULL;
//��Ƶ�ҶȻ�����ָ��
pEloamVideo_EnableGray g_pVideoEnableGray=NULL;

//!ZZBase.dll��������ָ��
//��ƫ�ñ߳�ʼ������ָ��
pZZInitSdk g_ZZInitSdk=NULL;
//��ƫ�ñ�����ͼƬ����ָ��
pZZLoadImage g_ZZLoadImage=NULL;
//��ƫ�ñ߻�ȡ�����������꺯��ָ��
pZZGetImageObjectRegionPos g_ZZGetImageObjectRegionPos=NULL;
//��ƫ�ñߴ�����������Ϣ��ȡ������Ϣ����ָ��
pZZGetImageRegionInfo g_ZZGetImageRegionInfo=NULL;
//��ƫ�ñߴ���ͼ����ָ��
pZZCreateImage g_ZZCreateImage=NULL;
//��ƫ�ñ߷������庯��ָ��
pZZSplitImageRegion g_ZZSplitImageRegion=NULL;
//��ƫ�ñ߱���ͼƬ����ָ��
pZZSaveImage g_ZZSaveImage=NULL;
//��ƫ�ñ��ͷ�ͼƬ����ָ��
pZZDestroyImage g_ZZDestroyImage=NULL;
//��ƫ�ñ߷���ʼ������ָ��
pZZDeinitSdk g_ZZDeinitSdk=NULL;

//�豸�Ƿ��ʼ��
bool g_isInit=false;
//��̬�⺯���Ƿ����
bool g_isLoadDllFun=false;
//����ͷ����豸ID
int g_deviceCnt=0;
//�豸ȫ�ֱ���
std::vector<DevInfo> g_cameras;
//eloamDLL��̬����
HINSTANCE g_eloamDll=NULL;
//ZZBase��̬����
HINSTANCE g_ZZBaseDll=NULL;

//�豸�β�ص�����
VOID ELOAMAPI DevChangeCallback(LONG type, LONG idx, LONG dbt, LPVOID userData);
//��Ƶ����ص�
VOID ELOAMAPI AttachCallback(HELOAMVIDEO video, LONG videoId, HELOAMVIEW view, LONG viewId, LPVOID userData);
//���ض�̬��ģ��
int loadDllModule();
//�����õ��ĺ���
int LoadDllFunction();
//��ȡ��̬�⺯��ָ��ĺ���ָ��
void *GetFunctionPointer( const char* funName);
//���õ�ǰ���̻�������
int SetCurrentEnvPath();
//��ȡ֧�ֵĳ�ͼ��ʽ
void GetSubtype(long subtype,std::vector<int>& subtypeIndex);
//�����豸������ȡ�豸��Ϣ
DevInfo *GetDevInfo(int iDeviceNum,int &index);
//��ȡ����ͷ����������
int GetDeveloperDevId(int userIndex);
//�ͷ�ָ��ID���豸��Դ
int ReleaseDevResource(int userIndex);
//ͼƬ��ƫ�ñ߲�����
BOOL ImageDeskew(HELOAMIMAGE hEloamImg,const char* saveFileName);


/************************************************************************/
/* ����:
/*		��ʼ������ͷ
/* �������:
/*		iDeviceNum:����ͷ�豸ID�����磺0,1,2������
/* �������:
/*		��
/* ����ֵ:
/*		0:�ɹ�,������ʾʧ��
/************************************************************************/
extern "C" CAMERADEV_API int _cdecl ggcaInit(int iDeviceNum);

/************************************************************************/
/* ����:
/*		�ͷ�����ͷ
/* �������:
/*		iDeviceNum:����ͷ�豸ID�����磺0,1,2������
/* �������:
/*		��
/* ����ֵ:
/*		0:�ɹ�,������ʾʧ��
/************************************************************************/
extern "C" CAMERADEV_API int _cdecl ggcaRelease(int iDeviceNum);

/************************************************************************/
/* ����:
/*		����Ԥ���ֱ���
/* �������:
/*		iDeviceNum:����ͷ�豸ID;
/*		iWidth:���;
/*		iHeight:�߶�;
/*		iBPP:��ɫ���
/* �������:	
/*		��
/* ����ֵ:
/*		0:�ɹ�,������ʾʧ��
/************************************************************************/
extern "C" CAMERADEV_API int _cdecl ggcaSetResolution(int iDeviceNum, int iWidth, int iHeight, int iBPP);

/************************************************************************/
/* ����:
/*		����Ԥ������
/* �������:
/*		iDeviceNum:����ͷ�豸ID;
/*		hControl:Ԥ�����ھ��
/* �������:	
/*		��
/* ����ֵ:
/*		0:�ɹ�,������ʾʧ��
/************************************************************************/
extern "C" CAMERADEV_API int _cdecl ggcaSetPrvWnd(int iDeviceNum,HWND hControl);

/************************************************************************/
/* ����:
/*		��ʼԤ��
/* �������:
/*		iDeviceNum:����ͷ�豸ID;
/*		iType:�򿪷�ʽ��0��ֱ�Ӵ򿪣�Ŀǰֻ��Ϊ0��
/* �������:	
/*		��
/* ����ֵ:
/*		0:�ɹ�,������ʾʧ��
/************************************************************************/
extern "C" CAMERADEV_API int _cdecl ggcaStart(int iDeviceNum,int iType);

/************************************************************************/
/* ����:
/*		�ر�Ԥ��
/* �������:
/*		iDeviceNum:����ͷ�豸ID;
/*		iType:�رշ�ʽ��Ŀǰֻ��Ϊ0
/* �������:	
/*		��
/* ����ֵ:
/*		0:�ɹ�,������ʾʧ��
/************************************************************************/
extern "C" CAMERADEV_API int _cdecl ggcaStop(int iDeviceNum,int iType);

/************************************************************************/
/* ����:
/*		��ȡͼƬ
/* �������:
/*		iDeviceNum:����ͷ�豸ID;
/*		filename�������ͼ���ļ��������Ϊ�գ��򵯳������ļ����Ի���;
/*		colorType��ͼƬɫ��,0-��ɫ24λ��1-�ڰ�(��ֵ)��2-�ڰ�(�Զ�);
/*		iClearBK���Ƿ�ȥ�ߣ�0-��ȥ�ߣ�1-ȥ�ߣ�Ĭ��Ϊ0;
/*		iDeskew���Ƿ��ƫ��0-����ƫ��1-��ƫ��Ĭ��Ϊ0;
/*		jpgCompress��jpgͼ��ѹ���ȣ���ΧΪ��0-100��Ĭ��Ϊ50;
/*		iRotateType����ת��1-��ת90�㣬2-��ת180�㣬3-��ת90�㣬0-����ת(Ĭ��)
/* �������:	
/*		��
/* ����ֵ:
/*		�ɹ�:��������ͼ���ļ����ļ���,ʧ��:���ؿ�
/************************************************************************/
extern "C" CAMERADEV_API char* _cdecl ggcaGetImage(int iDeviceNum , char *filename , int colorType , BOOL iClearBK=0, BOOL iDeskew=0, int jpgCompress=50, int iRotateType=0);

/************************************************************************/
/* ����:
/*		��̬��ʾ�ñ�
/* �������:
/*		iDeviceNum:����ͷ�豸ID;
/*		dynamicClear:FALSE����ʾ�ñߣ�TRUE��ʾ�ñ�
/* �������:	
/*		��
/* ����ֵ:
/*		0:�ɹ�,������ʾʧ��
/************************************************************************/
extern "C" CAMERADEV_API int _cdecl ggcaSetDynamicClear(int iDeviceNum,BOOL dynamicClear);