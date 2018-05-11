#include "eloam_img.h"
#include "SAStatusLog.h"
#include <afxdlgs.h>
//声明日志全局变量
CSAStatusLog g_logOut(_T("eloam_dll"));

extern "C" CAMERADEV_API int _cdecl ggcaInit( int iDeviceNum )
{
	int iRet=-1;

	//加载动态库模块、函数
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("开始加载动态库模块..."),__LINE__);
	int loadDllStatus=loadDllModule();
	if (loadDllStatus>=0)
	{//动态库模块加载成功
		if (!g_isLoadDllFun)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("开始加载动态库函数..."),__LINE__);
			if (LoadDllFunction()<0)
			{
				g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("开始加载动态库函数加载失败."),__LINE__);
				return iRet;
			}
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("动态库函数加载成功."),__LINE__);
			g_isLoadDllFun=true;
		}
	}
	else
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("动态库模块加载失败."),__LINE__);
		return iRet;
	}

	if (!g_isInit)
	{//初始化设备
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("开始设备初始化..."),__LINE__);
		g_deviceCnt=0;
		g_pInitDevs(DevChangeCallback,NULL);
		if(g_deviceCnt<=0)
		{
			g_pDeinitDevs();
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("设备初始化失败."),__LINE__);
			g_logOut.StatusOut(Info,_T("(%s) g_deviceCnt=%d  line:%d\r\n"),__FUNCTION__,g_deviceCnt,__LINE__);
			g_logOut.StatusOut(Info,_T("(%s) g_deviceCntEx=%d  line:%d\r\n"),__FUNCTION__,g_pGetDevCount(1),__LINE__);
			return iRet;		
		}		
		g_isInit=true;
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("设备初始化完成."),__LINE__);
	}
	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (!pDevInfo)
	{//该序号设备未初始化
		g_logOut.StatusOut(Info,_T("(%s) 摄像头未初始化:iDeviceNum=%d  开始初始化...  line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		int deveIndex=GetDeveloperDevId(iDeviceNum);
		int computerIndex=-1;
		for(int i =0;i<g_deviceCnt;i++ )
		{	
			int devType=g_pGetEloamType(1,i);
			if(devType-1 == deveIndex)//设备类型=设备开发者定义序号+1
			{
				computerIndex = i;
			}	
		}
		if (computerIndex<0)
		{
			g_logOut.StatusOut(Info,_T("(%s) 未根据用户定义摄像头ID找到开发者定义摄像头ID。摄像头初始化失败:iDeviceNum=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
			return iRet;
		}
		g_logOut.StatusOut(Info,_T("(%s) 用户输入摄像头ID=iDeviceNum=%d->开发者定义摄像头ID=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,deveIndex,__LINE__);

		HELOAMDEVICE hDev = g_pCreateDevice(1,computerIndex);
		if (NULL==hDev)
		{
			g_logOut.StatusOut(Info,_T("(%s) 设备创建失败。摄像头初始化失败:iDeviceNum=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
			return iRet;
		}
		else
		{
			DevInfo devInfo;
			devInfo.hDev=hDev;
			devInfo.displayName = g_pGetDisplayName(hDev);//获取设备显示名称
			devInfo.friendlyName = g_pGetFriendlyName(hDev);//获取友好名称
			devInfo.devType=g_pGetEloamType(1,deveIndex);//获取设备类型
			devInfo.developerIndex=deveIndex;//开发者定义的设备索引
			devInfo.userIndex=iDeviceNum;//用户定义的设备索引
			LONG subtype = g_pGetSubtype(hDev);//获取子模式
			GetSubtype(subtype,devInfo.subtypes);
			if (!devInfo.subtypes.empty())
			{
				int subtypeIndex=devInfo.subtypes[0];
				int resCount = g_pGetResolutionCountEx(hDev, subtypeIndex);//获取分辨率个数			
				for (int i = 0; i < resCount; ++i)
				{
					EloamSize eloamSize;

					eloamSize.width = g_pGetResolutionWidthEx(hDev, subtypeIndex, i);
					eloamSize.height = g_pGetResolutionHeightEx(hDev, subtypeIndex, i);
					devInfo.sizes.push_back(eloamSize);
				}
			}
			g_cameras.push_back(devInfo);
			iRet=0;
			g_logOut.StatusOut(Info,_T("(%s) 摄像头已初始化:iDeviceNum=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		}	
	}
	else
	{//该序号设备已初始化
		g_logOut.StatusOut(Info,_T("(%s) 摄像头已初始化:iDeviceNum=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		iRet=0;
	}
	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaRelease( int iDeviceNum )
{
	int iRet=-1;
	if (ReleaseDevResource(iDeviceNum)<0)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("设备释放失败."),__LINE__);
		return iRet;
	}
	if (g_cameras.empty())
	{
		g_pDeinitDevs();
		g_isInit=false;
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("设备列表为空,设备已进行反初始化."),__LINE__);
	}
	iRet=0;
	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaSetResolution( int iDeviceNum, int iWidth, int iHeight, int iBPP )
{
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
	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaSetPrvWnd( int iDeviceNum,HWND hControl )
{
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
		g_pViewRelease(pDevInfo->viewWnd);
		pDevInfo->viewWnd=NULL;
	}
	HELOAMVIEW hView = g_pCreateView(hControl,NULL,0);
	if (!hView)
	{
		g_logOut.StatusOut(Info,_T("(%s) 图像显示窗口对象创建失败.  line:%d\r\n"),__FUNCTION__,__LINE__);
		return iRet;
	}
	pDevInfo->viewWnd=hView;
	g_pViewSetText(pDevInfo->viewWnd, L"等待打开视频中...", RGB(255, 255, 255));

	iRet=0;
	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaStart( int iDeviceNum,int iType )
{
	int iRet=-1;
	if (0!=iType)
	{
		g_logOut.StatusOut(Info,_T("(%s) iType!=0.  line:%d\r\n"),__FUNCTION__,__LINE__);
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
		g_pVideoRelease(pDevInfo->video);
		pDevInfo->video=NULL;
	}
	int subType=pDevInfo->subtypes[0];
	if (pDevInfo->subtypes.size()>1)
	{//优先MJPG出图
		subType=pDevInfo->subtypes[1];
	}
	HELOAMVIDEO video = g_pCreateVideo(pDevInfo->hDev, pDevInfo->resIndex, subType, NULL, NULL, NULL, NULL,0,2);	
	if (!video)
	{
		g_logOut.StatusOut(Info,_T("(%s) 视频对象创建失败. End. line:%d\r\n"),__FUNCTION__,__LINE__);
		return iRet;
	}
	pDevInfo->video=video;

	if (g_pViewSelectVideo(pDevInfo->viewWnd, pDevInfo->video, AttachCallback, pDevInfo))
	{
		g_pViewSetText(pDevInfo->viewWnd, L"打开视频中，请等待...", RGB(255, 255, 255));
		iRet=0;
	} 
	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaStop( int iDeviceNum,int iType )
{
	int iRet=-1;
	if (0!=iType)
	{
		g_logOut.StatusOut(Info,_T("(%s) iType!=0.  line:%d\r\n"),__FUNCTION__,__LINE__);
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
		g_pVideoRelease(pDevInfo->video);//释放视频对象
		pDevInfo->video = NULL;
		g_logOut.StatusOut(Info,_T("(%s) 已释放视频对象 line:%d\r\n"),__FUNCTION__,__LINE__);
		g_pViewSetText(pDevInfo->viewWnd, L"无信号输入...", 0);
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
		g_logOut.StatusOut(Info,_T("(%s) 没有找到对应设备。iDeviceNum=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		return NULL;
	}
	if (NULL==pDevInfo->video||NULL==pDevInfo->viewWnd)
	{
		g_logOut.StatusOut(Info,_T("(%s) 视频对象或视频显示窗口对象不存在  line:%d\r\n"),__FUNCTION__,__LINE__);
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
	CComBSTR bstrFilename = filename;

	HELOAMIMAGE hEloamImg = g_pCreateImage(pDevInfo->video, 0, pDevInfo->viewWnd);
	if (!hEloamImg)
	{
		g_logOut.StatusOut(Info,_T("(%s) 图像对象创建失败.  line:%d\r\n"),__FUNCTION__,__LINE__);
		return NULL;
	}

	if (iClearBK||iDeskew)
	{//如果去边或纠偏
		if (!g_pImageDeskew(hEloamImg,0))
		{
			g_pImageRelease(hEloamImg);
			hEloamImg=NULL;
			g_logOut.StatusOut(Info,_T("(%s) 纠偏裁边失败.  line:%d\r\n"),__FUNCTION__,__LINE__);
			return NULL;
		}
	}
	//图片颜色
	BOOL isSuccess=FALSE;
	if (1==colorType||2==colorType)
	{//黑白
		isSuccess=g_pAdaptiveThreshold(hEloamImg,30);
		//isSuccess=EloamImage_ToGray(hEloamImg);
	}
	else
	{//彩色
		isSuccess=g_pImageToColor(hEloamImg);
	}
	if (!isSuccess)
	{
		g_pImageRelease(hEloamImg);
		hEloamImg=NULL;
		g_logOut.StatusOut(Info,_T("(%s) 生成指定颜色图片失败.  line:%d\r\n"),__FUNCTION__,__LINE__);
		return NULL;
	}
	//旋转
	if (0!=iRotateType!=iRotateType&&2!=iRotateType&&3!=iRotateType)
	{
		iRotateType=0;
	}
	if (!g_pImageRotate(hEloamImg,360-iRotateType*90,RGB(192,192,192),1))
	{
		g_pImageRelease(hEloamImg);
		hEloamImg=NULL;
		g_logOut.StatusOut(Info,_T("(%s) 图片旋转失败。 line:%d\r\n"),__FUNCTION__,__LINE__);
		return NULL;
	}
	//图片保存
	jpgCompress=jpgCompress>=100?99:jpgCompress;
	jpgCompress=jpgCompress<0?0:jpgCompress;
	int imgQuality=0x0800>>(int(jpgCompress/20));
	if (!g_pImageSave(hEloamImg, bstrFilename, imgQuality))
	{
		g_logOut.StatusOut(Info,_T("(%s) 图片保存失败。 line:%d\r\n"),__FUNCTION__,__LINE__);
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
		isSuccess=g_pVideoEnableDeskew(pDevInfo->video,0);
	}
	else
	{
		isSuccess=g_pVideoDisableDeskew(pDevInfo->video);
	}
	if (isSuccess)
	{
		iRet=0;
	}
	else
	{
		g_logOut.StatusOut(Info,_T("(%s) 纠偏裁边失败。 line:%d\r\n"),__FUNCTION__,__LINE__);
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
		g_pVideoEnableGray(video);
	}
	if (TRUE==pDevInfo->isDeskew)
	{
		g_pVideoEnableDeskew(video,0);
	}
}

int loadDllModule()
{//-1:动态库加载失败 0=eloamDll.dll加载成功 1=eloamDll.dll已加载
	SetCurrentEnvPath();//设置环境变量
	int iRet=-1;
	if (g_eloamDll)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("eloamDll.dll已加载."),__LINE__);
		return iRet=1;
	}
	else
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("开始加载动态库eloamDll.dll ..."),__LINE__);
		g_eloamDll=LoadLibrary(_T("eloamDll.dll"));
		if (g_eloamDll)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("eloamDll.dll加载成功."),__LINE__);
			iRet=0;		
		}
		else
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("eloamDll.dll加载失败."),__LINE__);
		}
	}
	return iRet;
}

int LoadDllFunction()
{
	//!eloamDll.dll模块函数加载
	//设备初始化函数指针
	g_pInitDevs=(pEloamGlobal_InitDevs)GetFunctionPointer(g_eloamDll,_T("EloamGlobal_InitDevs"));
	if (NULL==g_pInitDevs)
	{
		return -1;
	}
	//设备反初始化函数指针
	g_pDeinitDevs=(pEloamGlobal_DeinitDevs)GetFunctionPointer(g_eloamDll,_T("EloamGlobal_DeinitDevs"));
	if (NULL==g_pDeinitDevs)
	{
		return -1;
	}
	//获取设备个数
	g_pGetDevCount=(pEloamGlobal_GetDevCount)GetFunctionPointer(g_eloamDll,_T("EloamGlobal_GetDevCount"));
	if (NULL==g_pGetDevCount)
	{
		return -1;
	}
	//获取设备类型函数指针
	g_pGetEloamType=(pEloamGlobal_GetEloamType)GetFunctionPointer(g_eloamDll,_T("EloamGlobal_GetEloamType"));
	if (NULL==g_pGetEloamType)
	{
		return -1;
	}
	//创建设备函数指针
	g_pCreateDevice=(pEloamGlobal_CreateDevice)GetFunctionPointer(g_eloamDll,_T("EloamGlobal_CreateDevice"));
	if (NULL==g_pCreateDevice)
	{
		return -1;
	}
	//释放设备函数指针
	g_pDeviceRelease=(pEloamDevice_Release)GetFunctionPointer(g_eloamDll,_T("EloamDevice_Release"));
	if (NULL==g_pDeviceRelease)
	{
		return -1;
	}
	//获取显示名称函数指针
	g_pGetDisplayName=(pEloamDevice_GetDisplayName)GetFunctionPointer(g_eloamDll,_T("EloamDevice_GetDisplayName"));
	if (NULL==g_pGetDisplayName)
	{
		return -1;
	}
	//获取友好名称函数指针
	g_pGetFriendlyName=(pEloamDevice_GetFriendlyName)GetFunctionPointer(g_eloamDll,_T("EloamDevice_GetFriendlyName"));
	if (NULL==g_pGetFriendlyName)
	{
		return -1;
	}
	//释放文字函数指针
	g_pDestroyString=(pEloamGlobal_DestroyString)GetFunctionPointer(g_eloamDll,_T("EloamGlobal_DestroyString"));
	if (NULL==g_pDestroyString)
	{
		return -1;
	}
	//获取出图格式函数指针
	g_pGetSubtype=(pEloamDevice_GetSubtype)GetFunctionPointer(g_eloamDll,_T("EloamDevice_GetSubtype"));
	if (NULL==g_pGetSubtype)
	{
		return -1;
	}
	//获取分辨率个数函数指针
	g_pGetResolutionCountEx=(pEloamDevice_GetResolutionCountEx)GetFunctionPointer(g_eloamDll,_T("EloamDevice_GetResolutionCountEx"));
	if (NULL==g_pGetResolutionCountEx)
	{
		return -1;
	}
	//获取分辨率宽度函数指针
	g_pGetResolutionWidthEx=(pEloamDevice_GetResolutionWidthEx)GetFunctionPointer(g_eloamDll,_T("EloamDevice_GetResolutionWidthEx"));
	if (NULL==g_pGetResolutionWidthEx)
	{
		return -1;
	}
	//获取分辨率高度函数指针
	g_pGetResolutionHeightEx=(pEloamDevice_GetResolutionHeightEx)GetFunctionPointer(g_eloamDll,_T("EloamDevice_GetResolutionHeightEx"));
	if (NULL==g_pGetResolutionHeightEx)
	{
		return -1;
	}
	//创建视频窗口对象函数指针
	g_pCreateView=(pEloamGlobal_CreateView)GetFunctionPointer(g_eloamDll,_T("EloamGlobal_CreateView"));
	if (NULL==g_pCreateView)
	{
		return -1;
	}
	//释放视频窗口对象函数指针
	g_pViewRelease=(pEloamView_Release)GetFunctionPointer(g_eloamDll,_T("EloamView_Release"));
	if (NULL==g_pViewRelease)
	{
		return -1;
	}
	//视频窗口设置显示文字函数指针
	g_pViewSetText=(pEloamView_SetText)GetFunctionPointer(g_eloamDll,_T("EloamView_SetText"));
	if (NULL==g_pViewSetText)
	{
		return -1;
	}
	//创建视频函数指针
	g_pCreateVideo=(pEloamDevice_CreateVideo)GetFunctionPointer(g_eloamDll,_T("EloamDevice_CreateVideo"));
	if (NULL==g_pCreateVideo)
	{
		return -1;
	}
	//视频对象释放函数指针
	g_pVideoRelease=(pEloamVideo_Release)GetFunctionPointer(g_eloamDll,_T("EloamVideo_Release"));
	if (NULL==g_pVideoRelease)
	{
		return -1;
	}
	//将视频显示于窗口对象函数指针
	g_pViewSelectVideo=(pEloamView_SelectVideo)GetFunctionPointer(g_eloamDll,_T("EloamView_SelectVideo"));
	if (NULL==g_pViewSelectVideo)
	{
		return -1;
	}
	//创建图像函数指针
	g_pCreateImage=(pEloamVideo_CreateImage)GetFunctionPointer(g_eloamDll,_T("EloamVideo_CreateImage"));
	if (NULL==g_pCreateImage)
	{
		return -1;
	}
	//从文件获取图片函数指针
	g_CreateImageFromFile=(pEloamGlobal_CreateImageFromFile)GetFunctionPointer(g_eloamDll,_T("EloamGlobal_CreateImageFromFile"));
	if (NULL==g_CreateImageFromFile)
	{
		return -1;
	}
	//释放图像函数指针
	g_pImageRelease=(pEloamImage_Release)GetFunctionPointer(g_eloamDll,_T("EloamImage_Release"));
	if (NULL==g_pImageRelease)
	{
		return -1;
	}
	//纠偏去边函数指针
	g_pImageDeskew=(pEloamImage_Deskew)GetFunctionPointer(g_eloamDll,_T("EloamImage_Deskew"));
	if (NULL==g_pImageDeskew)
	{
		return -1;
	}
	//自适应二值化函数指针
	g_pAdaptiveThreshold=(pEloamImage_AdaptiveThreshold)GetFunctionPointer(g_eloamDll,_T("EloamImage_AdaptiveThreshold"));
	if (NULL==g_pAdaptiveThreshold)
	{
		return -1;
	}
	//图片彩色话函数指针
	g_pImageToColor=(pEloamImage_ToColor)GetFunctionPointer(g_eloamDll,_T("EloamImage_ToColor"));
	if (NULL==g_pImageToColor)
	{
		return -1;
	}
	//图片旋转函数指针
	g_pImageRotate=(pEloamImage_Rotate)GetFunctionPointer(g_eloamDll,_T("EloamImage_Rotate"));
	if (NULL==g_pImageRotate)
	{
		return -1;
	}
	//图片保存函数指针
	g_pImageSave=(pEloamImage_Save)GetFunctionPointer(g_eloamDll,_T("EloamImage_Save"));
	if (NULL==g_pImageSave)
	{
		return -1;
	}
	//视频纠偏裁边函数指针
	g_pVideoEnableDeskew=(pEloamVideo_EnableDeskew)GetFunctionPointer(g_eloamDll,_T("EloamVideo_EnableDeskew"));
	if (NULL==g_pVideoEnableDeskew)
	{
		return -1;
	}
	//取消视频纠偏裁边函数指针
	g_pVideoDisableDeskew=(pEloamVideo_DisableDeskew)GetFunctionPointer(g_eloamDll,_T("EloamVideo_DisableDeskew"));
	if (NULL==g_pVideoDisableDeskew)
	{
		return -1;
	}
	//视频灰度化函数指针
	g_pVideoEnableGray=(pEloamVideo_EnableGray)GetFunctionPointer(g_eloamDll,_T("EloamVideo_EnableGray"));
	if (NULL==g_pVideoEnableGray)
	{
		return -1;
	}
	return 0;
}

int SetCurrentEnvPath()
{
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("开始写入环境变量..."),__LINE__);
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("开始加载当前模块eloam_img.dll句柄..."),__LINE__);
	HANDLE hDllhandle=GetModuleHandle("eloam_img.dll");//获取当前模块句柄
	if (hDllhandle==NULL)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("当前模块句柄获取失败."),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("当前模块句柄加载成功."),__LINE__);
	TCHAR tchModule[MAX_PATH];
	if (!GetModuleFileName((HMODULE)hDllhandle, tchModule, MAX_PATH))
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("当前模块路径获取失败."),__LINE__);
		return -1;
	}
	CString strDLLPath = tchModule;
	strDLLPath = strDLLPath.Left(strDLLPath.ReverseFind(_T('\\')));
	g_logOut.StatusOut(Info,_T("(%s) %s:%s  line:%d\r\n"),__FUNCTION__,_T("当前模块路径获取成功"),strDLLPath.GetBuffer(0),__LINE__);
	// 设置环境变量
	DWORD len = GetEnvironmentVariable("path", NULL, 0);
	CString strEnvironmentOld;
	char *p = strEnvironmentOld.GetBuffer(len);
	if (!GetEnvironmentVariable("path", p, len))
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("环境变量获取失败."),__LINE__);
		return -1;
	}
	if (strEnvironmentOld.Find(strDLLPath)>0)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s:%s  line:%d\r\n"),__FUNCTION__,_T("当前模块环境变量已存在"),strDLLPath.GetBuffer(0),__LINE__);
		return 1;
	}
	strEnvironmentOld.ReleaseBuffer();
	CString strEnvironmentNew = strEnvironmentOld;
	if (';' != strEnvironmentNew[strEnvironmentNew.GetLength() - 1])
	{
		strEnvironmentNew += ";";
	}
	strEnvironmentNew += strDLLPath;
	if(!SetEnvironmentVariable("path", strEnvironmentNew))
	{
		g_logOut.StatusOut(Info,_T("(%s) 写入环境变量:%s失败 line:%d\r\n"),__FUNCTION__,(char*)strDLLPath.GetBuffer(0),__LINE__);
	}
	g_logOut.StatusOut(Info,_T("(%s) 写入环境变量:%s成功 line:%d\r\n"),__FUNCTION__,(char*)strDLLPath.GetBuffer(0),__LINE__);
	return 0;
}

void *GetFunctionPointer( HINSTANCE module, const char* funName )
{
	if (NULL==module) 
	{
		g_logOut.StatusOut(Error,_T("(%s) %s line:%d\r\n"),__FUNCTION__,_T("动态库未加载"),__LINE__);
		return NULL;
	}
	if (NULL==funName)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("传入函数名为空."),__LINE__);
		return NULL;
	}
	//获取模块名称
	TCHAR szModuleName[MAX_PATH] = {0};
	GetModuleFileName ( module, szModuleName, sizeof ( szModuleName ) - 1 );
	CString strDLLName = szModuleName;
	strDLLName = strDLLName.Right(strDLLName.GetLength()-strDLLName.ReverseFind(_T('\\'))-1);

	//获取模块方法
	void* pFun=(void *)GetProcAddress(module,funName);
	if (pFun)
	{//方法获取成功
		g_logOut.StatusOut(Info,_T("(%s) 函数:%s加载成功(%s)  line:%d\r\n"),__FUNCTION__,funName,strDLLName,__LINE__);
	} 
	else
	{//方法获取失败
		g_logOut.StatusOut(Info,_T("(%s) 函数:%s加载失败(%s)  line:%d\r\n"),__FUNCTION__,funName,strDLLName,__LINE__);
	}
	return pFun;
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
	int iRet=-1;

	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(userIndex,vecIndex);
	if (NULL==pDevInfo)
	{
		g_logOut.StatusOut(Info,_T("(%s) 没有找到对应设备。iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,userIndex,__LINE__);
		return iRet;
	}
	g_pDestroyString(pDevInfo->displayName);
	pDevInfo->displayName=NULL;
	g_pDestroyString(pDevInfo->friendlyName);
	pDevInfo->friendlyName=NULL;
	pDevInfo->subtypes.clear();
	pDevInfo->sizes.clear();
	if (NULL!=pDevInfo->video)
	{//释放视频对象
		g_pVideoRelease(pDevInfo->video);
		pDevInfo->video=NULL;
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("视频对象已释放."),__LINE__);
	}
	if (NULL!=pDevInfo->viewWnd)
	{
		g_pViewRelease(pDevInfo->viewWnd);
		pDevInfo->viewWnd=NULL;
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("视频窗口对象已释放."),__LINE__);
	}
	g_pDeviceRelease(pDevInfo->hDev);
	g_cameras.erase(g_cameras.begin()+vecIndex);	
	iRet=0;

	return iRet;
}