#include "eloam_img.h"
#include <afxdlgs.h>
//#include <commdlg.h>

#pragma comment(lib,"lib/eloamDll.lib")

extern "C" CAMERADEV_API int _cdecl ggcaInit( int iDeviceNum )
{
	int iRet=-1;
	if (!g_isInit)
	{//��ʼ���豸
		g_deviceCnt=0;
		EloamGlobal_InitDevs(DevChangeCallback,NULL);
		g_isInit=true;
	}
	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (pDevInfo)
	{//������豸�ѳ�ʼ��
		iRet=0;
	}
	else
	{
		int deveIndex=GetDeveloperDevId(iDeviceNum);
		int computerIndex=-1;
		for(int i =0;i<g_deviceCnt;i++ )
		{	
			int devType=EloamGlobal_GetEloamType(1,i);
			if(devType-1 == deveIndex)//�豸����=�豸�����߶������+1
			{
				computerIndex = i;
			}	
		}
		if (computerIndex<0)
		{
			return iRet;
		}
		HELOAMDEVICE hDev = EloamGlobal_CreateDevice(1,computerIndex);
		if (NULL!=hDev)
		{
			DevInfo devInfo;
			devInfo.hDev=hDev;	
			devInfo.displayName = EloamDevice_GetDisplayName(hDev);//��ȡ�豸��ʾ����
			devInfo.friendlyName = EloamDevice_GetFriendlyName(hDev);//��ȡ�Ѻ�����
			devInfo.devType=EloamGlobal_GetEloamType(1,deveIndex);//��ȡ�豸����
			devInfo.developerIndex=deveIndex;//�����߶�����豸����
			devInfo.userIndex=iDeviceNum;//�û�������豸����
			LONG subtype = EloamDevice_GetSubtype(hDev);//��ȡ��ģʽ
			GetSubtype(subtype,devInfo.subtypes);
			if (!devInfo.subtypes.empty())
			{
				int subtypeIndex=devInfo.subtypes[0];
				int resCount = EloamDevice_GetResolutionCountEx(hDev, subtypeIndex);//��ȡ�ֱ��ʸ���
				for (int i = 0; i < resCount; ++i)
				{
					EloamSize eloamSize;
					eloamSize.width = EloamDevice_GetResolutionWidthEx(hDev, subtypeIndex, i);
					eloamSize.height = EloamDevice_GetResolutionHeightEx(hDev, subtypeIndex, i);
					devInfo.sizes.push_back(eloamSize);
				}
			}
			g_cameras.push_back(devInfo);
			iRet=0;
		}
	}
	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaRelease( int iDeviceNum )
{
	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (NULL==pDevInfo)
	{
		return -1;
	}
	EloamGlobal_DestroyString(pDevInfo->displayName);
	pDevInfo->displayName=NULL;
	EloamGlobal_DestroyString(pDevInfo->friendlyName);
	pDevInfo->friendlyName=NULL;
	pDevInfo->subtypes.clear();
	pDevInfo->sizes.clear();
	if (NULL!=pDevInfo->video)
	{//�ͷ���Ƶ����
		EloamVideo_Release(pDevInfo->video);
		pDevInfo->video=NULL;
	}
	if (NULL!=pDevInfo->viewWnd)
	{
		EloamView_Release(pDevInfo->viewWnd);
		pDevInfo->viewWnd=NULL;
	}
	EloamDevice_Release(pDevInfo->hDev);
	g_cameras.erase(g_cameras.begin()+vecIndex);

	if (g_cameras.empty())
	{
		EloamGlobal_DeinitDevs();
		g_isInit=false;
	}
	return 0;
}

extern "C" CAMERADEV_API int _cdecl ggcaSetResolution( int iDeviceNum, int iWidth, int iHeight, int iBPP )
{
	int iRet=-1;
	if (1!=iBPP&&3!=iBPP)
	{//1=8bitͼ�� 3=24bitͼ��
		return iRet;
	}
	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (NULL==pDevInfo)
	{
		return iRet;
	}
	bool isFindRes=false;
	for (int i=0;i<pDevInfo->sizes.size();i++)
	{
		EloamSize eloamSize=pDevInfo->sizes[i];
		if (iWidth==eloamSize.width && iHeight==eloamSize.height)
		{
			pDevInfo->resIndex=i;
			isFindRes=true;
			break;
		}
	}
	if (isFindRes)
	{
		pDevInfo->channels=iBPP;
		iRet=0;
	}
	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaSetPrvWnd( int iDeviceNum,HWND hControl )
{
	int iRet=-1;


	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (pDevInfo->viewWnd)
	{
		EloamView_Release(pDevInfo->viewWnd);
		pDevInfo->viewWnd=NULL;
	}
		HELOAMVIEW hView = EloamGlobal_CreateView(hControl,NULL,0);
	if (!hView)
	{
		return iRet;
	}
	if (pDevInfo)
	{

		pDevInfo->viewWnd=hView;
		EloamView_SetText(pDevInfo->viewWnd, L"�ȴ�����Ƶ��...", RGB(255, 255, 255));
		iRet=0;
	}
	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaStart( int iDeviceNum,int iType )
{
	int iRet=-1;
	if (0!=iType)
	{
		return iRet;
	}
	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (!(pDevInfo->hDev)||!(pDevInfo->viewWnd)||(pDevInfo->resIndex<0)||(pDevInfo->subtypes.empty()))
	{
		return iRet;
	}	
	if (NULL!=pDevInfo->video)
	{//�ͷ���Ƶ����
		EloamVideo_Release(pDevInfo->video);
		pDevInfo->video=NULL;
	}
	HELOAMVIDEO video = EloamDevice_CreateVideo(pDevInfo->hDev, pDevInfo->resIndex, pDevInfo->subtypes[0], NULL, NULL, NULL, NULL);	
	if (!video)
	{
		return iRet;
	}

	pDevInfo->video=video;
	if (EloamView_SelectVideo(pDevInfo->viewWnd, pDevInfo->video, NULL, NULL))
	{
		EloamView_SetText(pDevInfo->viewWnd, L"����Ƶ�У���ȴ�...", RGB(255, 255, 255));
		iRet=0;
	} 
	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaStop( int iDeviceNum,int iType )
{
	int iRet=-1;
	if (0!=iType)
	{
		return iRet;
	}
	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (!pDevInfo)
	{
		return iRet;
	}
	if (NULL!=pDevInfo->video&&NULL!=pDevInfo->viewWnd)
	{
		EloamVideo_Release(pDevInfo->video);//�ͷ���Ƶ����
		pDevInfo->video = NULL;
		EloamView_SetText(pDevInfo->viewWnd, L"���ź�����...", 0);
		iRet=0;
	}
	return iRet;
}

extern "C" CAMERADEV_API char* _cdecl ggcaGetImage( int iDeviceNum , char *filename , int colorType , BOOL iClearBK/*=0*/, BOOL iDeskew/*=0*/, int jpgCompress/*=50*/, int iRotateType/*=0*/ )
{
	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (!pDevInfo)
	{
		return NULL;
	}
	if (NULL==pDevInfo->video||NULL==pDevInfo->viewWnd)
	{
		return NULL;
	}
	if (0==strcmp(filename,"")||NULL==filename)
	{//�����ļ���Ϊ��
		BOOL isOpen = FALSE;//TRUE:��,FALSE:����  
		CString defExt = L"jpg";//Ĭ�ϴ򿪵��ļ�·��  
		CString defFilename = L"image.jpg";//Ĭ�ϴ򿪵��ļ���  
		CString filter = L"ͼƬ (*.jpg; *.png; *.bmp)|*.jpg;*.png;*.bmp||"; //�ļ����ǵ�����  
		CFileDialog openFileDlg(isOpen, defExt, defFilename, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);  
		INT_PTR result = openFileDlg.DoModal();  
		if(result == IDOK) 
		{  
			CString filePath = openFileDlg.GetPathName();   
			strcpy(filename,(char*)filePath.GetBuffer(0));
		}
		else
		{
			return NULL;
		}
	}
	HELOAMIMAGE hEloamImg = EloamVideo_CreateImage(pDevInfo->video, 0, pDevInfo->viewWnd);
	if (!hEloamImg)
	{
		return NULL;
	}
	if (iClearBK||iDeskew)
	{//���ȥ�߻��ƫ
		if (!EloamImage_Rectify(hEloamImg,0))
		{
			EloamImage_Release(hEloamImg);
			hEloamImg=NULL;
			return NULL;
		}
	}
	//ͼƬ��ɫ
	BOOL isSuccess=FALSE;
	if (1==colorType||2==colorType)
	{//�ڰ�
		isSuccess=EloamImage_AdaptiveThreshold(hEloamImg,30);
		//isSuccess=EloamImage_ToGray(hEloamImg);
	}
	else
	{//��ɫ
		isSuccess=EloamImage_ToColor(hEloamImg);
	}
	if (!isSuccess)
	{
		EloamImage_Release(hEloamImg);
		hEloamImg=NULL;
		return NULL;
	}
	//��ת
	if (0!=iRotateType!=iRotateType&&2!=iRotateType&&3!=iRotateType)
	{
		iRotateType=0;
	}
	if (!EloamImage_Rotate(hEloamImg,360-iRotateType*90,RGB(192,192,192),1))
	{
		EloamImage_Release(hEloamImg);
		hEloamImg=NULL;
		return NULL;
	}
	//ͼƬ����
	jpgCompress=jpgCompress>=100?99:jpgCompress;
	jpgCompress=jpgCompress<0?0:jpgCompress;
	int imgQuality=0x0800>>(int(jpgCompress/20));
	CComBSTR bstrFilename = filename;
	if (!EloamImage_Save(hEloamImg, bstrFilename, imgQuality))
	{
		return NULL;
	}
	return filename;
}

extern "C" CAMERADEV_API int _cdecl ggcaSetDynamicClear( int iDeviceNum,BOOL dynamicClear )
{
	int iRet=-1;
	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (!pDevInfo)
	{
		return iRet;
	}
	if (!pDevInfo->video)
	{
		return iRet;
	}
	BOOL isSuccess=FALSE;
	if (dynamicClear)
	{
		isSuccess=EloamVideo_EnableDeskew(pDevInfo->video,0);
	}
	else
	{
		isSuccess=EloamVideo_DisableDeskew(pDevInfo->video);
	}
	if (isSuccess)
	{
		iRet=0;
	}
	return iRet;
}

VOID ELOAMAPI DevChangeCallback( LONG type, LONG idx, LONG dbt, LPVOID userData )
{
	if (1 != type)	
	{//ֻ������Ƶ�豸
		return;
	}
	if(1 == dbt)
	{//�豸����
		g_deviceCnt++;
	}
	else if(2 == dbt)
	{//�豸��ʧ
		std::vector<int> userIndexs;
		for (int i=0;i<g_cameras.size();i++)
		{
			userIndexs.push_back(g_cameras[i].userIndex);
		}
		for (int i=0;i<userIndexs.size();i++)
		{
			ggcaRelease(userIndexs[i]);
		}
	}
}

void GetSubtype( long subtype,std::vector<int>& subtypeIndex )
{
	if (subtype & 0x01)
	{//YUY2
		subtypeIndex.push_back(0x01);
	}
	if (subtype & 0x02)
	{//MJPG
		subtypeIndex.push_back(0x02);
	}
	if (subtype & 0x04)
	{//UYVY
		subtypeIndex.push_back(0x04);
	}
	if (subtype & 0x08)
	{//RGB24
		subtypeIndex.push_back(0x08);
	}
	if (subtype & 0x10)
	{//H264
		subtypeIndex.push_back(0x10);
	}
}

DevInfo * GetDevInfo( int iDeviceNum,int &index )
{
	index=-1;
	DevInfo *pDevInfo=NULL;
	for (int i=0;i<g_cameras.size();i++)
	{
		DevInfo devInfo=g_cameras[i];
		if (devInfo.userIndex==iDeviceNum)
		{
			pDevInfo=&g_cameras[i];
			index=i;
			break;
		}
	}
	return pDevInfo;
}

int GetDeveloperDevId( int userIndex )
{
	int deveIndex=-1;//�����߶��������ͷ��� 0=������ͷ��1=������ͷ1��2=������ͷ2��Ĭ��-1,��ʾ��Ӧ��ϵ������
	switch (userIndex)
	{//��ǰ�û���������ͷ����뿪���߶�����ŵĶ�Ӧ��ϵΪ:0-0��1-1��2-2������в�ͬ�Ķ�Ӧ��ϵ���޸�case�Ĳ������ɡ�
	case 0:
		deveIndex=0;
		break;
	case 1:
		deveIndex=1;
		break;
	case 2:
		deveIndex=2;
		break;
	}
	return deveIndex;
}