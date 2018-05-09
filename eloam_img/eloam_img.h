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
	HELOAMDEVICE hDev;				//�豸���
	BSTR displayName;				//�豸��ʾ����
	BSTR friendlyName;				//�豸�Ѻ�����
	int devType;					//�豸���� 1=������ͷ 2��3=��������ͷ
	int devIndex;					//�豸����
	std::vector<int> subtypes;		//֧�ֵĳ�ͼ���� 0x01=YUY2,0x02=MJPG,0x03=UYVY,0x04=RGB24,0x05=H264
	std::vector<EloamSize> sizes;	//֧�ֵķֱ���
	int resIndex;					//ѡ��ķֱ�������
	int channels;					//ͼ��ͨ����
	HELOAMVIEW viewWnd;				//Ԥ������
	HELOAMVIDEO video;				//��Ƶ����
	DevInfo()
	{
		hDev=NULL;
		displayName=NULL;
		friendlyName=NULL;
		devType=-1;
		devIndex=-1;
		resIndex=-1;
		channels=3;//Ĭ�ϲ�ɫ
		viewWnd=NULL;
		video=NULL;
	}
};
//�豸�Ƿ��ʼ��
bool g_isInit=false;
//����ͷ����豸ID
int g_maxDeviceNum=-1;
//�豸ȫ�ֱ���
std::vector<DevInfo> g_cameras;

//�豸�β�ص�����
VOID ELOAMAPI DevChangeCallback(LONG type, LONG idx, LONG dbt, LPVOID userData);

//��ȡ֧�ֵĳ�ͼ��ʽ
void GetSubtype(long subtype,std::vector<int>& subtypeIndex);

//�����豸������ȡ�豸��Ϣ
DevInfo *GetDevInfo(int iDeviceNum,int &index);

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