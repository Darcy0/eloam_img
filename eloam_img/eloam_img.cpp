#include "eloam_img.h"
#include "SAStatusLog.h"
#include <afxdlgs.h>

#pragma comment(lib,"lib/eloamDll.lib")

//声明日志全局变量
CSAStatusLog g_logOut(_T("eloam_dll"));

extern "C" CAMERADEV_API int _cdecl ggcaInit( int iDeviceNum )
{
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("Start..."),__LINE__);

	int iRet=-1;
	if (!g_isInit)
	{//初始化设备
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("开始设备初始化..."),__LINE__);
		g_deviceCnt=0;
		EloamGlobal_InitDevs(DevChangeCallback,NULL);
		if(g_deviceCnt<=0)
		{
			EloamGlobal_DeinitDevs();
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("设备初始化失败."),__LINE__);
			return iRet;		
		}		
		g_isInit=true;
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("设备初始化完成."),__LINE__);
	}
	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (pDevInfo)
	{//该序号设备已初始化
		g_logOut.StatusOut(Info,_T("(%s) 摄像头已初始化:iDeviceNum=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		iRet=0;
	}
	else
	{
		g_logOut.StatusOut(Info,_T("(%s) 摄像头未初始化:iDeviceNum=%d  开始初始化... line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
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
			g_logOut.StatusOut(Info,_T("(%s) 未根据用户定义摄像头ID找到开发者定义摄像头ID。摄像头初始化失败:iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
			return iRet;
		}
		g_logOut.StatusOut(Info,_T("(%s) 用户输入摄像头ID=iDeviceNum=%d->开发者定义摄像头ID=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,deveIndex,__LINE__);
		HELOAMDEVICE hDev = EloamGlobal_CreateDevice(1,computerIndex);
		if (NULL==hDev)
		{
			g_logOut.StatusOut(Info,_T("(%s) 设备创建失败。摄像头初始化失败:iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
			return iRet;
		}
		else
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
			g_logOut.StatusOut(Info,_T("(%s) 摄像头已初始化:iDeviceNum=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		}
	}

	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
	
	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaRelease( int iDeviceNum )
{
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("Start..."),__LINE__);

	int iRet=-1;
	if (ReleaseDevResource(iDeviceNum)<0)
	{
		return iRet;
	}
	if (g_cameras.empty())
	{
		EloamGlobal_DeinitDevs();
		g_isInit=false;
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("设备列表为空,设备已进行反初始化."),__LINE__);
	}
	iRet=0;
	
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);

	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaSetResolution( int iDeviceNum, int iWidth, int iHeight, int iBPP )
{
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("Start..."),__LINE__);

	int iRet=-1;

	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (NULL==pDevInfo)
	{
		g_logOut.StatusOut(Info,_T("(%s) 没有找到对应设备。iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
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
			g_logOut.StatusOut(Info,_T("(%s) width=%d,height=%d在设备支持分辨率中找到。分辨率索引为:%d  line:%d\r\n"),__FUNCTION__,iWidth,iHeight,i,__LINE__);
			break;
		}
	}
	if (!isFindRes)
	{
		g_logOut.StatusOut(Info,_T("(%s) width=%d,height=%d在设备支持分辨率中未找到。分辨率索引设为默认值:0 line:%d\r\n"),__FUNCTION__,iWidth,iHeight,__LINE__);
		pDevInfo->resIndex=0;	
	}
	if (1!=iBPP&&3!=iBPP)
	{//1=8bit图像 3=24bit图像
		g_logOut.StatusOut(Info,_T("(%s) iBPP=%d不等于1或3。设置为默认图片通道数3. line:%d\r\n"),__FUNCTION__,iBPP,__LINE__);
		iBPP=3;
	}
	pDevInfo->channels=iBPP;
	iRet=0;

	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);

	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaSetPrvWnd( int iDeviceNum,HWND hControl )
{
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("Start..."),__LINE__);

	int iRet=-1;

	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (NULL==pDevInfo)
	{
		g_logOut.StatusOut(Info,_T("(%s) 没有找到对应设备。iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		return -1;
	}

	if (pDevInfo->viewWnd)
	{
		EloamView_Release(pDevInfo->viewWnd);
		pDevInfo->viewWnd=NULL;
	}
	HELOAMVIEW hView = EloamGlobal_CreateView(hControl,NULL,0);
	if (!hView)
	{
		g_logOut.StatusOut(Info,_T("(%s) 图像显示窗口对象创建失败. End. line:%d\r\n"),__FUNCTION__,__LINE__);
		return iRet;
	}
	pDevInfo->viewWnd=hView;
	EloamView_SetText(pDevInfo->viewWnd, L"等待打开视频中...", RGB(255, 255, 255));
	iRet=0;

	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);

	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaStart( int iDeviceNum,int iType )
{
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("Start..."),__LINE__);

	int iRet=-1;
	if (0!=iType)
	{
		g_logOut.StatusOut(Info,_T("(%s) iType!=0. End. line:%d\r\n"),__FUNCTION__,__LINE__);
		return iRet;
	}
	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (NULL==pDevInfo)
	{
		g_logOut.StatusOut(Info,_T("(%s) 没有找到对应设备。iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		return iRet;
	}

	if (!(pDevInfo->hDev)||!(pDevInfo->viewWnd)||(pDevInfo->resIndex<0)||(pDevInfo->subtypes.empty()))
	{
		g_logOut.StatusOut(Info,_T("(%s) 设备对象为空或视频显示窗口对象为空或分辨率索引小于0或支持出图格式个数为0。 End. line:%d\r\n"),__FUNCTION__,__LINE__);
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
		g_logOut.StatusOut(Info,_T("(%s) 视频对象创建失败. End. line:%d\r\n"),__FUNCTION__,__LINE__);
		return iRet;
	}

	pDevInfo->video=video;
	if (EloamView_SelectVideo(pDevInfo->viewWnd, pDevInfo->video, AttachCallback, pDevInfo))
	{
		EloamView_SetText(pDevInfo->viewWnd, L"打开视频中，请等待...", RGB(255, 255, 255));
		iRet=0;
	} 

	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);

	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaStop( int iDeviceNum,int iType )
{
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("Start..."),__LINE__);

	int iRet=-1;
	if (0!=iType)
	{
		return iRet;
	}
	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (!pDevInfo)
	{
		g_logOut.StatusOut(Info,_T("(%s) 没有找到对应设备。iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		return iRet;
	}
	if (NULL!=pDevInfo->video&&NULL!=pDevInfo->viewWnd)
	{
		EloamVideo_Release(pDevInfo->video);//释放视频对象
		pDevInfo->video = NULL;
		g_logOut.StatusOut(Info,_T("(%s) 已释放视频对象 line:%d\r\n"),__FUNCTION__,__LINE__);
		EloamView_SetText(pDevInfo->viewWnd, L"无信号输入...", 0);
		iRet=0;
	}

	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);

	return iRet;
}

extern "C" CAMERADEV_API char* _cdecl ggcaGetImage( int iDeviceNum , char *filename , int colorType , BOOL iClearBK/*=0*/, BOOL iDeskew/*=0*/, int jpgCompress/*=50*/, int iRotateType/*=0*/ )
{
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("Start..."),__LINE__);

	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (!pDevInfo)
	{
		g_logOut.StatusOut(Info,_T("(%s) 没有找到对应设备。iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		return NULL;
	}
	if (NULL==pDevInfo->video||NULL==pDevInfo->viewWnd)
	{
		g_logOut.StatusOut(Info,_T("(%s) 视频对象或视频显示窗口对象不存在 End. line:%d\r\n"),__FUNCTION__,__LINE__);
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
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return NULL;
		}
	}
	HELOAMIMAGE hEloamImg = EloamVideo_CreateImage(pDevInfo->video, 0, pDevInfo->viewWnd);
	if (!hEloamImg)
	{
		g_logOut.StatusOut(Info,_T("(%s) 图像对象创建失败. End. line:%d\r\n"),__FUNCTION__,__LINE__);
		return NULL;
	}
	if (iClearBK||iDeskew)
	{//如果去边或纠偏
		if (!EloamImage_Deskew(hEloamImg,0))
		{
			//g_logOut.StatusOut(Info,_T("(%s) XXXXXXXXXXXXXXXXXXXXXXXXX line:%d\r\n"),__FUNCTION__,__LINE__);
			EloamImage_Release(hEloamImg);
			hEloamImg=NULL;
			g_logOut.StatusOut(Info,_T("(%s) 纠偏裁边失败. End. line:%d\r\n"),__FUNCTION__,__LINE__);
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
		g_logOut.StatusOut(Info,_T("(%s) 生成指定颜色图片失败. End. line:%d\r\n"),__FUNCTION__,__LINE__);
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
		g_logOut.StatusOut(Info,_T("(%s) 图片旋转失败。 End. line:%d\r\n"),__FUNCTION__,__LINE__);
		return NULL;
	}
	//图片保存
	jpgCompress=jpgCompress>=100?99:jpgCompress;
	jpgCompress=jpgCompress<0?0:jpgCompress;
	int imgQuality=0x0800>>(int(jpgCompress/20));
	CComBSTR bstrFilename = filename;
	if (!EloamImage_Save(hEloamImg, bstrFilename, imgQuality))
	{
		g_logOut.StatusOut(Info,_T("(%s) 图片保存失败。 End. line:%d\r\n"),__FUNCTION__,__LINE__);
		return NULL;
	}

	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);

	return filename;
}

extern "C" CAMERADEV_API int _cdecl ggcaSetDynamicClear( int iDeviceNum,BOOL dynamicClear )
{
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("Start..."),__LINE__);

	int iRet=-1;
	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (!pDevInfo)
	{
		g_logOut.StatusOut(Info,_T("(%s) 没有找到对应设备。iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		return iRet;
	}
	pDevInfo->isDeskew=dynamicClear;
	if (!pDevInfo->video)
	{	
		g_logOut.StatusOut(Info,_T("(%s) 视频对象不存在。 End. line:%d\r\n"),__FUNCTION__,__LINE__);
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
	else
	{
		g_logOut.StatusOut(Info,_T("(%s) 纠偏裁边失败。 line:%d\r\n"),__FUNCTION__,__LINE__);
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);

	return iRet;
}

VOID ELOAMAPI DevChangeCallback( LONG type, LONG idx, LONG dbt, LPVOID userData )
{
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("Start..."),__LINE__);

	if (1 != type)	
	{//只处理视频设备
		return;
	}
	if(1 == dbt)
	{//设备到达
		g_deviceCnt++;
		g_logOut.StatusOut(Info,_T("(%s) 设备到达:g_deviceCnt=%d。 line:%d\r\n"),__FUNCTION__,g_deviceCnt,__LINE__);
	}
	else if(2 == dbt)
	{//设备丢失
		g_logOut.StatusOut(Info,_T("(%s) 设备丢失. line:%d\r\n"),__FUNCTION__,__LINE__);
		std::vector<int> userIndexs;
		for (int i=0;i<g_cameras.size();i++)
		{
			userIndexs.push_back(g_cameras[i].userIndex);
		}
		for (int i=0;i<userIndexs.size();i++)
		{
			ReleaseDevResource(userIndexs[i]);
		}
		g_deviceCnt=0;
	}

	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
}

VOID ELOAMAPI AttachCallback( HELOAMVIDEO video, LONG videoId, HELOAMVIEW view, LONG viewId, LPVOID userData )
{
	if (1!=videoId)
	{//只在第一帧图像到达时处理
		return;
	}
	DevInfo *pDevInfo=(DevInfo *)userData;
	if (!pDevInfo)
	{
		return;
	}
	if (1==pDevInfo->channels)
	{
		EloamVideo_EnableGray(video);
	}
	if (TRUE==pDevInfo->isDeskew)
	{
		EloamVideo_EnableDeskew(video,0);
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

int ReleaseDevResource( int userIndex )
{
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("Start..."),__LINE__);
	int iRet=-1;

	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(userIndex,vecIndex);
	if (NULL==pDevInfo)
	{
		g_logOut.StatusOut(Info,_T("(%s) 没有找到对应设备。iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,userIndex,__LINE__);
		return iRet;
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
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("视频对象已释放."),__LINE__);
	}
	if (NULL!=pDevInfo->viewWnd)
	{
		EloamView_Release(pDevInfo->viewWnd);
		pDevInfo->viewWnd=NULL;
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("视频窗口对象已释放."),__LINE__);
	}
	EloamDevice_Release(pDevInfo->hDev);
	g_cameras.erase(g_cameras.begin()+vecIndex);
	
	iRet=0;
	return iRet;

	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
}