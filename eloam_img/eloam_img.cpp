#include "eloam_img.h"
#include "SAStatusLog.h"
#include <afxdlgs.h>

//������־ȫ�ֱ���
CSAStatusLog g_logOut(_T("eloam_dll"));

extern "C" CAMERADEV_API int _cdecl ggcaInit( int iDeviceNum )
{
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("Start..."),__LINE__);
	int iRet=-1;
	if (!g_eloamDll)
	{//���ض�̬��
		g_eloamDll=LoadLibrary(_T("eloamDll.dll"));
		if (!g_eloamDll)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  End. line:%d\r\n"),__FUNCTION__,_T("eloamDll.dll����ʧ��."),__LINE__);
			return iRet;
		}
	}
	if (!g_isInit)
	{//��ʼ���豸
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("��ʼ�豸��ʼ��..."),__LINE__);
		g_deviceCnt=0;
		pEloamGlobal_InitDevs pInitDevs=(pEloamGlobal_InitDevs)GetFunctionPointer(_T("EloamGlobal_InitDevs"));
		if (NULL==pInitDevs)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return iRet;
		}
		pInitDevs(DevChangeCallback,NULL);
		if(g_deviceCnt<=0)
		{
			pEloamGlobal_DeinitDevs pDeinitDevs=(pEloamGlobal_DeinitDevs)GetFunctionPointer(_T("EloamGlobal_DeinitDevs"));
			if (NULL==pDeinitDevs)
			{
				g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
				return iRet;
			}
			pDeinitDevs();
			g_logOut.StatusOut(Info,_T("(%s) %s  End. line:%d\r\n"),__FUNCTION__,_T("�豸��ʼ��ʧ��."),__LINE__);
			return iRet;		
		}		
		g_isInit=true;
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("�豸��ʼ�����."),__LINE__);
	}
	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (pDevInfo)
	{//������豸�ѳ�ʼ��
		g_logOut.StatusOut(Info,_T("(%s) ����ͷ�ѳ�ʼ��:iDeviceNum=%d line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		iRet=0;
	}
	else
	{
		g_logOut.StatusOut(Info,_T("(%s) ����ͷδ��ʼ��:iDeviceNum=%d  ��ʼ��ʼ��... line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		int deveIndex=GetDeveloperDevId(iDeviceNum);
		int computerIndex=-1;
		for(int i =0;i<g_deviceCnt;i++ )
		{	
			pEloamGlobal_GetEloamType pGetEloamType=(pEloamGlobal_GetEloamType)GetFunctionPointer(_T("EloamGlobal_GetEloamType"));
			if (NULL==pGetEloamType)
			{
				g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
				return iRet;
			}
			int devType=pGetEloamType(1,i);
			if(devType-1 == deveIndex)//�豸����=�豸�����߶������+1
			{
				computerIndex = i;
			}	
		}
		if (computerIndex<0)
		{
			g_logOut.StatusOut(Info,_T("(%s) δ�����û���������ͷID�ҵ������߶�������ͷID������ͷ��ʼ��ʧ��:iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
			return iRet;
		}
		g_logOut.StatusOut(Info,_T("(%s) �û���������ͷID=iDeviceNum=%d->�����߶�������ͷID=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,deveIndex,__LINE__);
		
		pEloamGlobal_CreateDevice pCreateDevice=(pEloamGlobal_CreateDevice)GetFunctionPointer(_T("EloamGlobal_CreateDevice"));
		if (NULL==pCreateDevice)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return iRet;
		}
		HELOAMDEVICE hDev = pCreateDevice(1,computerIndex);
		if (NULL==hDev)
		{
			g_logOut.StatusOut(Info,_T("(%s) �豸����ʧ�ܡ�����ͷ��ʼ��ʧ��:iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
			return iRet;
		}
		else
		{
			DevInfo devInfo;
			devInfo.hDev=hDev;
			pEloamDevice_GetDisplayName pGetDisplayName=(pEloamDevice_GetDisplayName)GetFunctionPointer(_T("EloamDevice_GetDisplayName"));
			if (NULL==pGetDisplayName)
			{
				g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
				return iRet;
			}
			devInfo.displayName = pGetDisplayName(hDev);//��ȡ�豸��ʾ����
			pEloamDevice_GetFriendlyName pGetFriendlyName=(pEloamDevice_GetFriendlyName)GetFunctionPointer(_T("EloamDevice_GetFriendlyName"));
			if (NULL==pGetFriendlyName)
			{
				g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
				return iRet;
			}
			devInfo.friendlyName = pGetFriendlyName(hDev);//��ȡ�Ѻ�����
			pEloamGlobal_GetEloamType pGetEloamType=(pEloamGlobal_GetEloamType)GetFunctionPointer(_T("EloamGlobal_GetEloamType"));
			if (NULL==pGetEloamType)
			{
				g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
				return iRet;
			}
			devInfo.devType=pGetEloamType(1,deveIndex);//��ȡ�豸����
			devInfo.developerIndex=deveIndex;//�����߶�����豸����
			devInfo.userIndex=iDeviceNum;//�û�������豸����
			pEloamDevice_GetSubtype pGetSubtype=(pEloamDevice_GetSubtype)GetFunctionPointer(_T("EloamDevice_GetSubtype"));
			if (NULL==pGetSubtype)
			{
				g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
				return iRet;
			}
			LONG subtype = pGetSubtype(hDev);//��ȡ��ģʽ
			GetSubtype(subtype,devInfo.subtypes);
			if (!devInfo.subtypes.empty())
			{
				int subtypeIndex=devInfo.subtypes[0];
				pEloamDevice_GetResolutionCountEx pGetResolutionCountEx=(pEloamDevice_GetResolutionCountEx)GetFunctionPointer(_T("EloamDevice_GetResolutionCountEx"));
				if (NULL==pGetResolutionCountEx)
				{
					g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
					return iRet;
				}
				int resCount = pGetResolutionCountEx(hDev, subtypeIndex);//��ȡ�ֱ��ʸ���			
				pEloamDevice_GetResolutionWidthEx pGetResolutionWidthEx=(pEloamDevice_GetResolutionWidthEx)GetFunctionPointer(_T("EloamDevice_GetResolutionWidthEx"));
				if (NULL==pGetResolutionWidthEx)
				{
					g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
					return iRet;
				}
				pEloamDevice_GetResolutionHeightEx pGetResolutionHeightEx=(pEloamDevice_GetResolutionHeightEx)GetFunctionPointer(_T("EloamDevice_GetResolutionHeightEx"));
				if (NULL==pGetResolutionHeightEx)
				{
					g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
					return iRet;
				}
				for (int i = 0; i < resCount; ++i)
				{
					EloamSize eloamSize;

					eloamSize.width = pGetResolutionWidthEx(hDev, subtypeIndex, i);
					eloamSize.height = pGetResolutionHeightEx(hDev, subtypeIndex, i);
					devInfo.sizes.push_back(eloamSize);
				}
			}
			g_cameras.push_back(devInfo);
			iRet=0;
			g_logOut.StatusOut(Info,_T("(%s) ����ͷ�ѳ�ʼ��:iDeviceNum=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
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
		pEloamGlobal_DeinitDevs pDeinitDevs=(pEloamGlobal_DeinitDevs)GetFunctionPointer(_T("EloamGlobal_DeinitDevs"));
		if (NULL==pDeinitDevs)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return iRet;
		}
		pDeinitDevs();
		g_isInit=false;
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("�豸�б�Ϊ��,�豸�ѽ��з���ʼ��."),__LINE__);
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
		g_logOut.StatusOut(Info,_T("(%s) û���ҵ���Ӧ�豸��iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
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
			g_logOut.StatusOut(Info,_T("(%s) width=%d,height=%d���豸֧�ֱַ������ҵ����ֱ�������Ϊ:%d  line:%d\r\n"),__FUNCTION__,iWidth,iHeight,i,__LINE__);
			break;
		}
	}
	if (!isFindRes)
	{
		g_logOut.StatusOut(Info,_T("(%s) width=%d,height=%d���豸֧�ֱַ�����δ�ҵ����ֱ���������ΪĬ��ֵ:0 line:%d\r\n"),__FUNCTION__,iWidth,iHeight,__LINE__);
		pDevInfo->resIndex=0;	
	}
	if (1!=iBPP&&3!=iBPP)
	{//1=8bitͼ�� 3=24bitͼ��
		g_logOut.StatusOut(Info,_T("(%s) iBPP=%d������1��3������ΪĬ��ͼƬͨ����3. line:%d\r\n"),__FUNCTION__,iBPP,__LINE__);
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
		g_logOut.StatusOut(Info,_T("(%s) û���ҵ���Ӧ�豸��iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		return -1;
	}

	if (pDevInfo->viewWnd)
	{
		pEloamView_Release pViewRelease=(pEloamView_Release)GetFunctionPointer(_T("EloamView_Release"));
		if (NULL==pViewRelease)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return iRet;
		}
		pViewRelease(pDevInfo->viewWnd);
		pDevInfo->viewWnd=NULL;
	}
	pEloamGlobal_CreateView pCreateView=(pEloamGlobal_CreateView)GetFunctionPointer(_T("EloamGlobal_CreateView"));
	if (NULL==pCreateView)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
		return iRet;
	}
	HELOAMVIEW hView = pCreateView(hControl,NULL,0);
	if (!hView)
	{
		g_logOut.StatusOut(Info,_T("(%s) ͼ����ʾ���ڶ��󴴽�ʧ��. End. line:%d\r\n"),__FUNCTION__,__LINE__);
		return iRet;
	}
	pDevInfo->viewWnd=hView;
	pEloamView_SetText pViewSetText=(pEloamView_SetText)GetFunctionPointer(_T("EloamView_SetText"));
	if (NULL==pViewSetText)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
		return iRet;
	}
	pViewSetText(pDevInfo->viewWnd, L"�ȴ�����Ƶ��...", RGB(255, 255, 255));
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
		g_logOut.StatusOut(Info,_T("(%s) û���ҵ���Ӧ�豸��iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		return iRet;
	}

	if (!(pDevInfo->hDev)||!(pDevInfo->viewWnd)||(pDevInfo->resIndex<0)||(pDevInfo->subtypes.empty()))
	{
		g_logOut.StatusOut(Info,_T("(%s) �豸����Ϊ�ջ���Ƶ��ʾ���ڶ���Ϊ�ջ�ֱ�������С��0��֧�ֳ�ͼ��ʽ����Ϊ0�� End. line:%d\r\n"),__FUNCTION__,__LINE__);
		return iRet;
	}	
	if (NULL!=pDevInfo->video)
	{//�ͷ���Ƶ����
		pEloamVideo_Release pVideoRelease=(pEloamVideo_Release)GetFunctionPointer(_T("EloamVideo_Release"));
		if (NULL==pVideoRelease)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return iRet;
		}
		pVideoRelease(pDevInfo->video);
		pDevInfo->video=NULL;
	}
	pEloamDevice_CreateVideo pCreateVideo=(pEloamDevice_CreateVideo)GetFunctionPointer(_T("EloamDevice_CreateVideo"));
	if (NULL==pCreateVideo)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
		return iRet;
	}
	HELOAMVIDEO video = pCreateVideo(pDevInfo->hDev, pDevInfo->resIndex, pDevInfo->subtypes[0], NULL, NULL, NULL, NULL,0,2);	
	if (!video)
	{
		g_logOut.StatusOut(Info,_T("(%s) ��Ƶ���󴴽�ʧ��. End. line:%d\r\n"),__FUNCTION__,__LINE__);
		return iRet;
	}

	pDevInfo->video=video;
	pEloamView_SelectVideo pViewSelectVideo=(pEloamView_SelectVideo)GetFunctionPointer(_T("EloamView_SelectVideo"));
	if (NULL==pViewSelectVideo)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
		return iRet;
	}
	if (pViewSelectVideo(pDevInfo->viewWnd, pDevInfo->video, AttachCallback, pDevInfo))
	{
		pEloamView_SetText pViewSetText=(pEloamView_SetText)GetFunctionPointer(_T("EloamView_SetText"));
		if (NULL==pViewSetText)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return iRet;
		}
		pViewSetText(pDevInfo->viewWnd, L"����Ƶ�У���ȴ�...", RGB(255, 255, 255));
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
		g_logOut.StatusOut(Info,_T("(%s) û���ҵ���Ӧ�豸��iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		return iRet;
	}
	if (NULL!=pDevInfo->video&&NULL!=pDevInfo->viewWnd)
	{
		pEloamVideo_Release pVideoRelease=(pEloamVideo_Release)GetFunctionPointer(_T("EloamVideo_Release"));
		if (NULL==pVideoRelease)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return iRet;
		}
		pVideoRelease(pDevInfo->video);//�ͷ���Ƶ����
		pDevInfo->video = NULL;
		g_logOut.StatusOut(Info,_T("(%s) ���ͷ���Ƶ���� line:%d\r\n"),__FUNCTION__,__LINE__);
		pEloamView_SetText pViewSetText=(pEloamView_SetText)GetFunctionPointer(_T("EloamView_SetText"));
		if (NULL==pViewSetText)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return iRet;
		}
		pViewSetText(pDevInfo->viewWnd, L"���ź�����...", 0);
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
		g_logOut.StatusOut(Info,_T("(%s) û���ҵ���Ӧ�豸��iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		return NULL;
	}
	if (NULL==pDevInfo->video||NULL==pDevInfo->viewWnd)
	{
		g_logOut.StatusOut(Info,_T("(%s) ��Ƶ�������Ƶ��ʾ���ڶ��󲻴��� End. line:%d\r\n"),__FUNCTION__,__LINE__);
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
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return NULL;
		}
	}
	pEloamVideo_CreateImage pCreateImage=(pEloamVideo_CreateImage)GetFunctionPointer(_T("EloamVideo_CreateImage"));
	if (NULL==pCreateImage)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
		return NULL;
	}
	HELOAMIMAGE hEloamImg = pCreateImage(pDevInfo->video, 0, pDevInfo->viewWnd);
	if (!hEloamImg)
	{
		g_logOut.StatusOut(Info,_T("(%s) ͼ����󴴽�ʧ��. End. line:%d\r\n"),__FUNCTION__,__LINE__);
		return NULL;
	}
	pEloamImage_Release pImageRelease=(pEloamImage_Release)GetFunctionPointer(_T("EloamImage_Release"));
	if (NULL==pImageRelease)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
		return NULL;
	}
	if (iClearBK||iDeskew)
	{//���ȥ�߻��ƫ
		pEloamImage_Deskew pImageDeskew=(pEloamImage_Deskew)GetFunctionPointer(_T("EloamImage_Deskew"));
		if (NULL==pImageDeskew)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return NULL;
		}
		if (!pImageDeskew(hEloamImg,0))
		{
			//g_logOut.StatusOut(Info,_T("(%s) XXXXXXXXXXXXXXXXXXXXXXXXX line:%d\r\n"),__FUNCTION__,__LINE__);
			pImageRelease(hEloamImg);
			hEloamImg=NULL;
			g_logOut.StatusOut(Info,_T("(%s) ��ƫ�ñ�ʧ��. End. line:%d\r\n"),__FUNCTION__,__LINE__);
			return NULL;
		}
	}
	//ͼƬ��ɫ
	BOOL isSuccess=FALSE;
	if (1==colorType||2==colorType)
	{//�ڰ�
		pEloamImage_AdaptiveThreshold pAdaptiveThreshold=(pEloamImage_AdaptiveThreshold)GetFunctionPointer(_T("EloamImage_AdaptiveThreshold"));
		if (NULL==pAdaptiveThreshold)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return NULL;
		}
		isSuccess=pAdaptiveThreshold(hEloamImg,30);
		//isSuccess=EloamImage_ToGray(hEloamImg);
	}
	else
	{//��ɫ
		pEloamImage_ToColor pToColor=(pEloamImage_ToColor)GetFunctionPointer(_T("EloamImage_ToColor"));
		if (NULL==pToColor)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return NULL;
		}
		isSuccess=pToColor(hEloamImg);
	}
	if (!isSuccess)
	{
		pImageRelease(hEloamImg);
		hEloamImg=NULL;
		g_logOut.StatusOut(Info,_T("(%s) ����ָ����ɫͼƬʧ��. End. line:%d\r\n"),__FUNCTION__,__LINE__);
		return NULL;
	}
	//��ת
	if (0!=iRotateType!=iRotateType&&2!=iRotateType&&3!=iRotateType)
	{
		iRotateType=0;
	}
	pEloamImage_Rotate pImageRotate=(pEloamImage_Rotate)GetFunctionPointer(_T("EloamImage_Rotate"));
	if (NULL==pImageRotate)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
		return NULL;
	}
	if (!pImageRotate(hEloamImg,360-iRotateType*90,RGB(192,192,192),1))
	{
		pImageRelease(hEloamImg);
		hEloamImg=NULL;
		g_logOut.StatusOut(Info,_T("(%s) ͼƬ��תʧ�ܡ� End. line:%d\r\n"),__FUNCTION__,__LINE__);
		return NULL;
	}
	//ͼƬ����
	jpgCompress=jpgCompress>=100?99:jpgCompress;
	jpgCompress=jpgCompress<0?0:jpgCompress;
	int imgQuality=0x0800>>(int(jpgCompress/20));
	CComBSTR bstrFilename = filename;
	pEloamImage_Save pImageSave=(pEloamImage_Save)GetFunctionPointer(_T("EloamImage_Save"));
	if (NULL==pImageRotate)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
		return NULL;
	}
	if (!pImageSave(hEloamImg, bstrFilename, imgQuality))
	{
		g_logOut.StatusOut(Info,_T("(%s) ͼƬ����ʧ�ܡ� End. line:%d\r\n"),__FUNCTION__,__LINE__);
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
		g_logOut.StatusOut(Info,_T("(%s) û���ҵ���Ӧ�豸��iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		return iRet;
	}
	pDevInfo->isDeskew=dynamicClear;
	if (!pDevInfo->video)
	{	
		g_logOut.StatusOut(Info,_T("(%s) ��Ƶ���󲻴��ڡ� End. line:%d\r\n"),__FUNCTION__,__LINE__);
		return iRet;
	}
	BOOL isSuccess=FALSE;
	if (dynamicClear)
	{
		pEloamVideo_EnableDeskew pVideoEnableDeskew=(pEloamVideo_EnableDeskew)GetFunctionPointer(_T("EloamVideo_EnableDeskew"));
		if (NULL==pVideoEnableDeskew)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return NULL;
		}
		isSuccess=pVideoEnableDeskew(pDevInfo->video,0);
	}
	else
	{
		pEloamVideo_DisableDeskew pVideoDisableDeskew=(pEloamVideo_DisableDeskew)GetFunctionPointer(_T("EloamVideo_DisableDeskew"));
		if (NULL==pVideoDisableDeskew)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return NULL;
		}
		isSuccess=pVideoDisableDeskew(pDevInfo->video);
	}
	if (isSuccess)
	{
		iRet=0;
	}
	else
	{
		g_logOut.StatusOut(Info,_T("(%s) ��ƫ�ñ�ʧ�ܡ� line:%d\r\n"),__FUNCTION__,__LINE__);
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);

	return iRet;
}

VOID ELOAMAPI DevChangeCallback( LONG type, LONG idx, LONG dbt, LPVOID userData )
{
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("Start..."),__LINE__);

	if (1 != type)	
	{//ֻ������Ƶ�豸
		return;
	}
	if(1 == dbt)
	{//�豸����
		g_deviceCnt++;
		g_logOut.StatusOut(Info,_T("(%s) �豸����:g_deviceCnt=%d�� line:%d\r\n"),__FUNCTION__,g_deviceCnt,__LINE__);
	}
	else if(2 == dbt)
	{//�豸��ʧ
		g_logOut.StatusOut(Info,_T("(%s) �豸��ʧ. line:%d\r\n"),__FUNCTION__,__LINE__);
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
	{//ֻ�ڵ�һ֡ͼ�񵽴�ʱ����
		return;
	}
	DevInfo *pDevInfo=(DevInfo *)userData;
	if (!pDevInfo)
	{
		return;
	}
	if (1==pDevInfo->channels)
	{
		pEloamVideo_EnableGray pVideoEnableGray=(pEloamVideo_EnableGray)GetFunctionPointer(_T("EloamVideo_EnableGray"));
		if (NULL==pVideoEnableGray)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return;
		}
		pVideoEnableGray(video);
	}
	if (TRUE==pDevInfo->isDeskew)
	{
		pEloamVideo_EnableDeskew pVideoEnableDeskew=(pEloamVideo_EnableDeskew)GetFunctionPointer(_T("EloamVideo_EnableDeskew"));
		if (NULL==pVideoEnableDeskew)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return;
		}
		pVideoEnableDeskew(video,0);
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

int ReleaseDevResource( int userIndex )
{
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("Start..."),__LINE__);
	int iRet=-1;

	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(userIndex,vecIndex);
	if (NULL==pDevInfo)
	{
		g_logOut.StatusOut(Info,_T("(%s) û���ҵ���Ӧ�豸��iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,userIndex,__LINE__);
		return iRet;
	}
	pEloamGlobal_DestroyString pDestroyString=(pEloamGlobal_DestroyString)GetFunctionPointer(_T("EloamGlobal_DestroyString"));
	if (NULL==pDestroyString)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
		return iRet;
	}
	pDestroyString(pDevInfo->displayName);
	pDevInfo->displayName=NULL;
	pDestroyString(pDevInfo->friendlyName);
	pDevInfo->friendlyName=NULL;
	pDevInfo->subtypes.clear();
	pDevInfo->sizes.clear();
	if (NULL!=pDevInfo->video)
	{//�ͷ���Ƶ����
		pEloamVideo_Release pVideoRelease=(pEloamVideo_Release)GetFunctionPointer(_T("EloamVideo_Release"));
		if (NULL==pDestroyString)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return iRet;
		}
		pVideoRelease(pDevInfo->video);
		pDevInfo->video=NULL;
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("��Ƶ�������ͷ�."),__LINE__);
	}
	if (NULL!=pDevInfo->viewWnd)
	{
		pEloamView_Release pViewRelease=(pEloamView_Release)GetFunctionPointer(_T("EloamView_Release"));
		if (NULL==pDestroyString)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
			return iRet;
		}
		pViewRelease(pDevInfo->viewWnd);
		pDevInfo->viewWnd=NULL;
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("��Ƶ���ڶ������ͷ�."),__LINE__);
	}
	pEloamDevice_Release pDeviceRelease=(pEloamDevice_Release)GetFunctionPointer(_T("EloamDevice_Release"));
	if (NULL==pDeviceRelease)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
		return iRet;
	}
	pDeviceRelease(pDevInfo->hDev);
	g_cameras.erase(g_cameras.begin()+vecIndex);
	
	iRet=0;
	return iRet;

	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
}

void *GetFunctionPointer( const char* funName )
{
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("Start..."),__LINE__);
	if (NULL==funName)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s End. line:%d\r\n"),__FUNCTION__,_T("���뺯����Ϊ��."),__LINE__);
		return NULL;
	}
	if (NULL==g_eloamDll)
	{
		g_logOut.StatusOut(Error,_T("(%s) %s line:%d\r\n"),__FUNCTION__,_T("eloamDll.dll��̬�����δ����. End."),__LINE__);
		return NULL;
	}
	void* pFun=(void *)GetProcAddress(g_eloamDll,funName);
	if (NULL==pFun)
	{//����ָ���ȡʧ��
		g_logOut.StatusOut(Error,_T("(%s) ����%s����ʧ�� line:%d\r\n"),__FUNCTION__,funName,__LINE__);
	}
	else
	{//����ָ���ȡ�ɹ�
		g_logOut.StatusOut(Info,_T("(%s) ����%s���سɹ� line:%d\r\n"),__FUNCTION__,funName,__LINE__);
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("End."),__LINE__);
	return pFun;
}