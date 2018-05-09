#include "eloam_img.h"
#include <afxdlgs.h>
//#include <commdlg.h>

#pragma comment(lib,"lib/eloamDll.lib")

extern "C" CAMERADEV_API int _cdecl ggcaInit( int iDeviceNum )
{
	int iRet=-1;
	if (!g_isInit)
	{//初始化设备
		g_deviceCnt=0;
		EloamGlobal_InitDevs(DevChangeCallback,NULL);
		g_isInit=true;
	}
	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (pDevInfo)
	{//该序号设备已初始化
		iRet=0;
	}
	else
	{
		int deveIndex=GetDeveloperDevId(iDeviceNum);
		int computerIndex=-1;
		for(int i =0;i<g_deviceCnt;i++ )
		{	
			int devType=EloamGlobal_GetEloamType(1,i);
			if(devType-1 == deveIndex)//设备类型=设备开发者定义序号+1
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
			devInfo.displayName = EloamDevice_GetDisplayName(hDev);//获取设备显示名称
			devInfo.friendlyName = EloamDevice_GetFriendlyName(hDev);//获取友好名称
			devInfo.devType=EloamGlobal_GetEloamType(1,deveIndex);//获取设备类型
			devInfo.developerIndex=deveIndex;//开发者定义的设备索引
			devInfo.userIndex=iDeviceNum;//用户定义的设备索引
			LONG subtype = EloamDevice_GetSubtype(hDev);//获取子模式
			GetSubtype(subtype,devInfo.subtypes);
			if (!devInfo.subtypes.empty())
			{
				int subtypeIndex=devInfo.subtypes[0];
				int resCount = EloamDevice_GetResolutionCountEx(hDev, subtypeIndex);//获取分辨率个数
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
	{//释放视频对象
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
	{//1=8bit图像 3=24bit图像
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
		EloamView_SetText(pDevInfo->viewWnd, L"等待打开视频中...", RGB(255, 255, 255));
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
	{//释放视频对象
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
		EloamView_SetText(pDevInfo->viewWnd, L"打开视频中，请等待...", RGB(255, 255, 255));
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
		EloamVideo_Release(pDevInfo->video);//释放视频对象
		pDevInfo->video = NULL;
		EloamView_SetText(pDevInfo->viewWnd, L"无信号输入...", 0);
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
	{//保存文件名为空
		BOOL isOpen = FALSE;//TRUE:打开,FALSE:保存  
		CString defExt = L"jpg";//默认打开的文件路径  
		CString defFilename = L"image.jpg";//默认打开的文件名  
		CString filter = L"图片 (*.jpg; *.png; *.bmp)|*.jpg;*.png;*.bmp||"; //文件过虑的类型  
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
	{//如果去边或纠偏
		if (!EloamImage_Rectify(hEloamImg,0))
		{
			EloamImage_Release(hEloamImg);
			hEloamImg=NULL;
			return NULL;
		}
	}
	//图片颜色
	BOOL isSuccess=FALSE;
	if (1==colorType||2==colorType)
	{//黑白
		isSuccess=EloamImage_AdaptiveThreshold(hEloamImg,30);
		//isSuccess=EloamImage_ToGray(hEloamImg);
	}
	else
	{//彩色
		isSuccess=EloamImage_ToColor(hEloamImg);
	}
	if (!isSuccess)
	{
		EloamImage_Release(hEloamImg);
		hEloamImg=NULL;
		return NULL;
	}
	//旋转
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
	//图片保存
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
	{//只处理视频设备
		return;
	}
	if(1 == dbt)
	{//设备到达
		g_deviceCnt++;
	}
	else if(2 == dbt)
	{//设备丢失
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
	int deveIndex=-1;//开发者定义的摄像头序号 0=主摄像头，1=副摄像头1，2=副摄像头2。默认-1,表示对应关系不存在
	switch (userIndex)
	{//当前用户定义摄像头序号与开发者定义序号的对应关系为:0-0、1-1、2-2。如果有不同的对应关系，修改case的参数即可。
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