#include "eloam_img.h"
#include "SAStatusLog.h"
#include <afxdlgs.h>
//������־ȫ�ֱ���
CSAStatusLog g_logOut(_T("eloam_dll"));

extern "C" CAMERADEV_API int _cdecl ggcaInit( int iDeviceNum )
{
	int iRet=-1;

	//���ض�̬��ģ�顢����
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("��ʼ���ض�̬��ģ��..."),__LINE__);
	int loadModuleStatus=loadDllModule();
	if (loadModuleStatus<0)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("��̬��ģ�����ʧ��."),__LINE__);
		return iRet;
	}
	else if (loadModuleStatus==0)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("��̬��ģ����سɹ�."),__LINE__);
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("��ʼ���ض�̬�⺯��..."),__LINE__);
		if (LoadDllFunction()<0)
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("��ʼ���ض�̬�⺯������ʧ��."),__LINE__);
			return iRet;
		}
		else
		{
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("��̬�⺯�����سɹ�."),__LINE__);
		}
	}
	else
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("��̬��ģ���Ѽ���."),__LINE__);
	}

	if (!g_isInit)
	{//��ʼ���豸
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("��ʼ�豸��ʼ��..."),__LINE__);
		g_deviceCnt=0;
		g_pInitDevs(DevChangeCallback,NULL);
		if(g_deviceCnt<=0)
		{
			g_pDeinitDevs();
			g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("�豸��ʼ��ʧ��."),__LINE__);
			g_logOut.StatusOut(Info,_T("(%s) g_deviceCnt=%d  line:%d\r\n"),__FUNCTION__,g_deviceCnt,__LINE__);
			g_logOut.StatusOut(Info,_T("(%s) g_deviceCntEx=%d  line:%d\r\n"),__FUNCTION__,g_pGetDevCount(1),__LINE__);
			return iRet;		
		}		
		g_isInit=true;
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("�豸��ʼ�����."),__LINE__);
	}
	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(iDeviceNum,vecIndex);
	if (!pDevInfo)
	{//������豸δ��ʼ��
		g_logOut.StatusOut(Info,_T("(%s) ����ͷδ��ʼ��:iDeviceNum=%d  ��ʼ��ʼ��...  line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		int deveIndex=GetDeveloperDevId(iDeviceNum);
		int computerIndex=-1;
		for(int i =0;i<g_deviceCnt;i++ )
		{	
			int devType=g_pGetEloamType(1,i);
			if(devType-1 == deveIndex)//�豸����=�豸�����߶������+1
			{
				computerIndex = i;
			}	
		}
		if (computerIndex<0)
		{
			g_logOut.StatusOut(Info,_T("(%s) δ�����û���������ͷID�ҵ������߶�������ͷID������ͷ��ʼ��ʧ��:iDeviceNum=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
			return iRet;
		}
		g_logOut.StatusOut(Info,_T("(%s) �û���������ͷID=iDeviceNum=%d->�����߶�������ͷID=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,deveIndex,__LINE__);

		HELOAMDEVICE hDev = g_pCreateDevice(1,computerIndex);
		if (NULL==hDev)
		{
			g_logOut.StatusOut(Info,_T("(%s) �豸����ʧ�ܡ�����ͷ��ʼ��ʧ��:iDeviceNum=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
			return iRet;
		}
		else
		{
			DevInfo devInfo;
			devInfo.hDev=hDev;
			devInfo.displayName = g_pGetDisplayName(hDev);//��ȡ�豸��ʾ����
			devInfo.friendlyName = g_pGetFriendlyName(hDev);//��ȡ�Ѻ�����
			devInfo.devType=g_pGetEloamType(1,deveIndex);//��ȡ�豸����
			devInfo.developerIndex=deveIndex;//�����߶�����豸����
			devInfo.userIndex=iDeviceNum;//�û�������豸����
			LONG subtype = g_pGetSubtype(hDev);//��ȡ��ģʽ
			GetSubtype(subtype,devInfo.subtypes);
			if (!devInfo.subtypes.empty())
			{
				int subtypeIndex=devInfo.subtypes[0];
				int resCount = g_pGetResolutionCountEx(hDev, subtypeIndex);//��ȡ�ֱ��ʸ���			
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
			g_logOut.StatusOut(Info,_T("(%s) ����ͷ�ѳ�ʼ��:iDeviceNum=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		}	
	}
	else
	{//������豸�ѳ�ʼ��
		g_logOut.StatusOut(Info,_T("(%s) ����ͷ�ѳ�ʼ��:iDeviceNum=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		iRet=0;
	}
	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaRelease( int iDeviceNum )
{
	int iRet=-1;
	if (ReleaseDevResource(iDeviceNum)<0)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("�豸�ͷ�ʧ��."),__LINE__);
		return iRet;
	}
	if (g_cameras.empty())
	{
		g_pDeinitDevs();
		g_isInit=false;
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("�豸�б�Ϊ��,�豸�ѽ��з���ʼ��."),__LINE__);
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
	return iRet;
}

extern "C" CAMERADEV_API int _cdecl ggcaSetPrvWnd( int iDeviceNum,HWND hControl )
{
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
		g_pViewRelease(pDevInfo->viewWnd);
		pDevInfo->viewWnd=NULL;
	}
	HELOAMVIEW hView = g_pCreateView(hControl,NULL,0);
	if (!hView)
	{
		g_logOut.StatusOut(Info,_T("(%s) ͼ����ʾ���ڶ��󴴽�ʧ��.  line:%d\r\n"),__FUNCTION__,__LINE__);
		return iRet;
	}
	pDevInfo->viewWnd=hView;
	g_pViewSetText(pDevInfo->viewWnd, L"�ȴ�����Ƶ��...", RGB(255, 255, 255));

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
		g_pVideoRelease(pDevInfo->video);
		pDevInfo->video=NULL;
	}
	int subType=pDevInfo->subtypes[0];
	if (pDevInfo->subtypes.size()>1)
	{//����MJPG��ͼ
		subType=pDevInfo->subtypes[1];
	}
	HELOAMVIDEO video = g_pCreateVideo(pDevInfo->hDev, pDevInfo->resIndex, subType, NULL, NULL, NULL, NULL,0,2);	
	if (!video)
	{
		g_logOut.StatusOut(Info,_T("(%s) ��Ƶ���󴴽�ʧ��. End. line:%d\r\n"),__FUNCTION__,__LINE__);
		return iRet;
	}
	pDevInfo->video=video;

	if (g_pViewSelectVideo(pDevInfo->viewWnd, pDevInfo->video, AttachCallback, pDevInfo))
	{
		g_pViewSetText(pDevInfo->viewWnd, L"����Ƶ�У���ȴ�...", RGB(255, 255, 255));
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
		g_logOut.StatusOut(Info,_T("(%s) û���ҵ���Ӧ�豸��iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		return iRet;
	}
	if (NULL!=pDevInfo->video&&NULL!=pDevInfo->viewWnd)
	{
		g_pVideoRelease(pDevInfo->video);//�ͷ���Ƶ����
		pDevInfo->video = NULL;
		g_logOut.StatusOut(Info,_T("(%s) ���ͷ���Ƶ���� line:%d\r\n"),__FUNCTION__,__LINE__);
		g_pViewSetText(pDevInfo->viewWnd, L"���ź�����...", 0);
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
		g_logOut.StatusOut(Info,_T("(%s) û���ҵ���Ӧ�豸��iDeviceNum=%d  line:%d\r\n"),__FUNCTION__,iDeviceNum,__LINE__);
		return NULL;
	}
	if (NULL==pDevInfo->video||NULL==pDevInfo->viewWnd)
	{
		g_logOut.StatusOut(Info,_T("(%s) ��Ƶ�������Ƶ��ʾ���ڶ��󲻴���  line:%d\r\n"),__FUNCTION__,__LINE__);
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

	HELOAMIMAGE hEloamImg = g_pCreateImage(pDevInfo->video, 0, pDevInfo->viewWnd);
	if (!hEloamImg)
	{
		g_logOut.StatusOut(Info,_T("(%s) ͼ����󴴽�ʧ��.  line:%d\r\n"),__FUNCTION__,__LINE__);
		return NULL;
	}

	if (iClearBK||iDeskew)
	{//���ȥ�߻��ƫ
		if (!g_pImageDeskew(hEloamImg,0))
		{
			g_pImageRelease(hEloamImg);
			hEloamImg=NULL;
			g_logOut.StatusOut(Info,_T("(%s) ��ƫ�ñ�ʧ��.  line:%d\r\n"),__FUNCTION__,__LINE__);
			return NULL;
		}
	}
	//ͼƬ��ɫ
	BOOL isSuccess=FALSE;
	if (1==colorType||2==colorType)
	{//�ڰ�
		isSuccess=g_pAdaptiveThreshold(hEloamImg,30);
		//isSuccess=EloamImage_ToGray(hEloamImg);
	}
	else
	{//��ɫ
		isSuccess=g_pImageToColor(hEloamImg);
	}
	if (!isSuccess)
	{
		g_pImageRelease(hEloamImg);
		hEloamImg=NULL;
		g_logOut.StatusOut(Info,_T("(%s) ����ָ����ɫͼƬʧ��.  line:%d\r\n"),__FUNCTION__,__LINE__);
		return NULL;
	}
	//��ת
	if (0!=iRotateType!=iRotateType&&2!=iRotateType&&3!=iRotateType)
	{
		iRotateType=0;
	}
	if (!g_pImageRotate(hEloamImg,360-iRotateType*90,RGB(192,192,192),1))
	{
		g_pImageRelease(hEloamImg);
		hEloamImg=NULL;
		g_logOut.StatusOut(Info,_T("(%s) ͼƬ��תʧ�ܡ� line:%d\r\n"),__FUNCTION__,__LINE__);
		return NULL;
	}
	//ͼƬ����
	jpgCompress=jpgCompress>=100?99:jpgCompress;
	jpgCompress=jpgCompress<0?0:jpgCompress;
	int imgQuality=0x0800>>(int(jpgCompress/20));
	CComBSTR bstrFilename = filename;
	if (!g_pImageSave(hEloamImg, bstrFilename, imgQuality))
	{
		g_logOut.StatusOut(Info,_T("(%s) ͼƬ����ʧ�ܡ� line:%d\r\n"),__FUNCTION__,__LINE__);
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
		g_logOut.StatusOut(Info,_T("(%s) ��ƫ�ñ�ʧ�ܡ� line:%d\r\n"),__FUNCTION__,__LINE__);
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
		g_pVideoEnableGray(video);
	}
	if (TRUE==pDevInfo->isDeskew)
	{
		g_pVideoEnableDeskew(video,0);
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
	int iRet=-1;

	int vecIndex=0;
	DevInfo *pDevInfo=GetDevInfo(userIndex,vecIndex);
	if (NULL==pDevInfo)
	{
		g_logOut.StatusOut(Info,_T("(%s) û���ҵ���Ӧ�豸��iDeviceNum=%d  End. line:%d\r\n"),__FUNCTION__,userIndex,__LINE__);
		return iRet;
	}
	g_pDestroyString(pDevInfo->displayName);
	pDevInfo->displayName=NULL;
	g_pDestroyString(pDevInfo->friendlyName);
	pDevInfo->friendlyName=NULL;
	pDevInfo->subtypes.clear();
	pDevInfo->sizes.clear();
	if (NULL!=pDevInfo->video)
	{//�ͷ���Ƶ����
		g_pVideoRelease(pDevInfo->video);
		pDevInfo->video=NULL;
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("��Ƶ�������ͷ�."),__LINE__);
	}
	if (NULL!=pDevInfo->viewWnd)
	{
		g_pViewRelease(pDevInfo->viewWnd);
		pDevInfo->viewWnd=NULL;
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("��Ƶ���ڶ������ͷ�."),__LINE__);
	}
	g_pDeviceRelease(pDevInfo->hDev);
	g_cameras.erase(g_cameras.begin()+vecIndex);	
	iRet=0;

	return iRet;
}

void *GetFunctionPointer( const char* funName )
{
	if (NULL==funName)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s End. line:%d\r\n"),__FUNCTION__,_T("���뺯����Ϊ��."),__LINE__);
		return NULL;
	}
	if (NULL==g_eloamDll)
	{
		g_logOut.StatusOut(Error,_T("(%s) %s line:%d\r\n"),__FUNCTION__,_T("eloamDll.dll��̬�����δ����."),__LINE__);
		return NULL;
	}
	void* pFun=(void *)GetProcAddress(g_eloamDll,funName);
	return pFun;
}

BOOL SetPathEnvVar(const char* var)
{
//	DWORD dwRet, dwErr;
//	LPTSTR pszOldVal; 
//	BOOL fExist; 
//
//	//LPCTSTR VARNAME = L"Path";
//
//	//pszOldVal = (LPTSTR) malloc(BUFSIZE*sizeof(TCHAR));
//	wchar_t pszOldVal[MAX_PATH];
//// 	if(NULL == pszOldVal)
//// 	{
//// 		//printf("Out of memory\n");
//// 		return FALSE;
//// 	}
//
//	dwRet = GetEnvironmentVariable(L"Path", pszOldVal, MAX_PATH);
//
//	if(0 == dwRet)
//	{
//		dwErr = GetLastError();
//		if( ERROR_ENVVAR_NOT_FOUND == dwErr )
//		{
//			//printf("Environment variable does not exist.\n");
//			fExist=FALSE;
//		}
//	}
//	else if(BUFSIZE < dwRet)
//	{
//		pszOldVal = (LPTSTR) realloc(pszOldVal, dwRet*sizeof(TCHAR));   
//		if(NULL == pszOldVal)
//		{
//			//printf("Out of memory\n");
//			return FALSE;
//		}
//		dwRet = GetEnvironmentVariable(VARNAME, pszOldVal, dwRet);
//		if(!dwRet)
//		{
//			printf("GetEnvironmentVariable failed (%d)\n", GetLastError());
//			return FALSE;
//		}
//		else fExist=TRUE;
//	}
//	else fExist=TRUE;
//
//	// Set a value for the child process to inherit. 
//	if (! SetEnvironmentVariable(VARNAME, strTempPath)) 
//	{
//		//printf("SetEnvironmentVariable failed (%d)\n", GetLastError()); 
//		return FALSE;
//	}
	return TRUE;
}

int LoadDllFunction()
{
	//�豸��ʼ������ָ��
	g_pInitDevs=(pEloamGlobal_InitDevs)GetFunctionPointer(_T("EloamGlobal_InitDevs"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamGlobal_InitDevs����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamGlobal_InitDevs���سɹ�"),__LINE__);
	//�豸����ʼ������ָ��
	g_pDeinitDevs=(pEloamGlobal_DeinitDevs)GetFunctionPointer(_T("EloamGlobal_DeinitDevs"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamGlobal_DeinitDevs����ʧ��"),__LINE__);
		return -1;
	}
	//��ȡ�豸����
	g_pGetDevCount=(pEloamGlobal_GetDevCount)GetFunctionPointer(_T("EloamGlobal_GetDevCount"));
	if (NULL==g_pGetDevCount)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamGlobal_GetDevCount����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamGlobal_GetDevCount���سɹ�"),__LINE__);
	//��ȡ�豸���ͺ���ָ��
	g_pGetEloamType=(pEloamGlobal_GetEloamType)GetFunctionPointer(_T("EloamGlobal_GetEloamType"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamGlobal_GetEloamType����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamGlobal_GetEloamType���سɹ�"),__LINE__);
	//�����豸����ָ��
	g_pCreateDevice=(pEloamGlobal_CreateDevice)GetFunctionPointer(_T("EloamGlobal_CreateDevice"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamGlobal_CreateDevice����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamGlobal_CreateDevice���سɹ�"),__LINE__);
	//�ͷ��豸����ָ��
	g_pDeviceRelease=(pEloamDevice_Release)GetFunctionPointer(_T("EloamDevice_Release"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_Release����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_Release���سɹ�"),__LINE__);
	//��ȡ��ʾ���ƺ���ָ��
	g_pGetDisplayName=(pEloamDevice_GetDisplayName)GetFunctionPointer(_T("EloamDevice_GetDisplayName"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_GetDisplayName����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_GetDisplayName���سɹ�"),__LINE__);
	//��ȡ�Ѻ����ƺ���ָ��
	g_pGetFriendlyName=(pEloamDevice_GetFriendlyName)GetFunctionPointer(_T("EloamDevice_GetFriendlyName"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_GetFriendlyName����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_GetFriendlyName���سɹ�"),__LINE__);
	//�ͷ����ֺ���ָ��
	g_pDestroyString=(pEloamGlobal_DestroyString)GetFunctionPointer(_T("EloamGlobal_DestroyString"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamGlobal_DestroyString����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamGlobal_DestroyString���سɹ�"),__LINE__);
	//��ȡ��ͼ��ʽ����ָ��
	g_pGetSubtype=(pEloamDevice_GetSubtype)GetFunctionPointer(_T("EloamDevice_GetSubtype"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_GetSubtype����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_GetSubtype���سɹ�"),__LINE__);
	//��ȡ�ֱ��ʸ�������ָ��
	g_pGetResolutionCountEx=(pEloamDevice_GetResolutionCountEx)GetFunctionPointer(_T("EloamDevice_GetResolutionCountEx"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_GetResolutionCountEx����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_GetResolutionCountEx���سɹ�"),__LINE__);
	//��ȡ�ֱ��ʿ�Ⱥ���ָ��
	g_pGetResolutionWidthEx=(pEloamDevice_GetResolutionWidthEx)GetFunctionPointer(_T("EloamDevice_GetResolutionWidthEx"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_GetResolutionWidthEx����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_GetResolutionWidthEx���سɹ�"),__LINE__);
	//��ȡ�ֱ��ʸ߶Ⱥ���ָ��
	g_pGetResolutionHeightEx=(pEloamDevice_GetResolutionHeightEx)GetFunctionPointer(_T("EloamDevice_GetResolutionHeightEx"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_GetResolutionHeightEx����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_GetResolutionHeightEx���سɹ�"),__LINE__);
	//������Ƶ���ڶ�����ָ��
	g_pCreateView=(pEloamGlobal_CreateView)GetFunctionPointer(_T("EloamGlobal_CreateView"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamGlobal_CreateView����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamGlobal_CreateView���سɹ�"),__LINE__);
	//�ͷ���Ƶ���ڶ�����ָ��
	g_pViewRelease=(pEloamView_Release)GetFunctionPointer(_T("EloamView_Release"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamView_Release����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamView_Release���سɹ�"),__LINE__);
	//��Ƶ����������ʾ���ֺ���ָ��
	g_pViewSetText=(pEloamView_SetText)GetFunctionPointer(_T("EloamView_SetText"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamView_SetText����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamView_SetText���سɹ�"),__LINE__);
	//������Ƶ����ָ��
	g_pCreateVideo=(pEloamDevice_CreateVideo)GetFunctionPointer(_T("EloamDevice_CreateVideo"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_CreateVideo����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamDevice_CreateVideo���سɹ�"),__LINE__);
	//��Ƶ�����ͷź���ָ��
	g_pVideoRelease=(pEloamVideo_Release)GetFunctionPointer(_T("EloamVideo_Release"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamVideo_Release����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamVideo_Release���سɹ�"),__LINE__);
	//����Ƶ��ʾ�ڴ��ڶ�����ָ��
	g_pViewSelectVideo=(pEloamView_SelectVideo)GetFunctionPointer(_T("EloamView_SelectVideo"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamView_SelectVideo����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamView_SelectVideo���سɹ�"),__LINE__);
	//����ͼ����ָ��
	g_pCreateImage=(pEloamVideo_CreateImage)GetFunctionPointer(_T("EloamVideo_CreateImage"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamVideo_CreateImage����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamVideo_CreateImage���سɹ�"),__LINE__);
	//�ͷ�ͼ����ָ��
	g_pImageRelease=(pEloamImage_Release)GetFunctionPointer(_T("EloamImage_Release"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamImage_Release����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamImage_Release���سɹ�"),__LINE__);
	//��ƫȥ�ߺ���ָ��
	g_pImageDeskew=(pEloamImage_Deskew)GetFunctionPointer(_T("EloamImage_Deskew"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamImage_Deskew����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamImage_Deskew���سɹ�"),__LINE__);
	//����Ӧ��ֵ������ָ��
	g_pAdaptiveThreshold=(pEloamImage_AdaptiveThreshold)GetFunctionPointer(_T("EloamImage_AdaptiveThreshold"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamImage_AdaptiveThreshold����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamImage_AdaptiveThreshold���سɹ�"),__LINE__);
	//ͼƬ��ɫ������ָ��
	g_pImageToColor=(pEloamImage_ToColor)GetFunctionPointer(_T("EloamImage_ToColor"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamImage_ToColor����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamImage_ToColor���سɹ�"),__LINE__);
	//ͼƬ��ת����ָ��
	g_pImageRotate=(pEloamImage_Rotate)GetFunctionPointer(_T("EloamImage_Rotate"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamImage_Rotate����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamImage_Rotate���سɹ�"),__LINE__);
	//ͼƬ���溯��ָ��
	g_pImageSave=(pEloamImage_Save)GetFunctionPointer(_T("EloamImage_Save"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamImage_Save����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamImage_Save���سɹ�"),__LINE__);
	//��Ƶ��ƫ�ñߺ���ָ��
	g_pVideoEnableDeskew=(pEloamVideo_EnableDeskew)GetFunctionPointer(_T("EloamVideo_EnableDeskew"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamVideo_EnableDeskew����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamVideo_EnableDeskew���سɹ�"),__LINE__);
	//ȡ����Ƶ��ƫ�ñߺ���ָ��
	g_pVideoDisableDeskew=(pEloamVideo_DisableDeskew)GetFunctionPointer(_T("EloamVideo_DisableDeskew"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamVideo_DisableDeskew����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamVideo_DisableDeskew���سɹ�"),__LINE__);
	//��Ƶ�ҶȻ�����ָ��
	g_pVideoEnableGray=(pEloamVideo_EnableGray)GetFunctionPointer(_T("EloamVideo_EnableGray"));
	if (NULL==g_pInitDevs)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamVideo_EnableGray����ʧ��"),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  line:%d\r\n"),__FUNCTION__,_T("EloamVideo_EnableGray���سɹ�"),__LINE__);
	return 0;
}

int loadDllModule()
{
	if (g_eloamDll)
	{//����ֵ1��ʾ�Ѽ���
		return 1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  End. line:%d\r\n"),__FUNCTION__,_T("��ʼд�뻷������..."),__LINE__);
	g_logOut.StatusOut(Info,_T("(%s) %s  End. line:%d\r\n"),__FUNCTION__,_T("��ʼ���ص�ǰģ��eloam_img.dll���..."),__LINE__);
	HANDLE hDllhandle=GetModuleHandle("eloam_img.dll");//��ȡ��ǰģ����
	if (hDllhandle==NULL)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  End. line:%d\r\n"),__FUNCTION__,_T("��ȡ��ǰģ����ʧ��."),__LINE__);
	}
	else
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  End. line:%d\r\n"),__FUNCTION__,_T("���ص�ǰģ��eloam_img.dll����ɹ�."),__LINE__);
		TCHAR tchModule[MAX_PATH];
		GetModuleFileName((HMODULE)hDllhandle, tchModule, MAX_PATH);
		CString strDLLPath = tchModule;
		strDLLPath = strDLLPath.Left(strDLLPath.ReverseFind(_T('\\')));

		// ���û�������
		DWORD len = GetEnvironmentVariable("path", NULL, 0);
		CString strEnvironmentOld;
		char *p = strEnvironmentOld.GetBuffer(len);
		GetEnvironmentVariable("path", p, len);
		strEnvironmentOld.ReleaseBuffer();
		CString strEnvironmentNew = strEnvironmentOld;
		if (';' != strEnvironmentNew[strEnvironmentNew.GetLength() - 1])
		{
			strEnvironmentNew += ";";
		}
		strEnvironmentNew += strDLLPath;
		if(SetEnvironmentVariable("path", strEnvironmentNew))
		{
			g_logOut.StatusOut(Info,_T("(%s) д�뻷������:%s�ɹ� line:%d\r\n"),__FUNCTION__,(char*)strDLLPath.GetBuffer(0),__LINE__);
		}
		else
		{
			g_logOut.StatusOut(Info,_T("(%s) д�뻷������:%sʧ�� line:%d\r\n"),__FUNCTION__,(char*)strDLLPath.GetBuffer(0),__LINE__);
		}
	}
	//���ض�̬��
	g_logOut.StatusOut(Info,_T("(%s) %s  End. line:%d\r\n"),__FUNCTION__,_T("��ʼ���ض�̬��eloamDll.dll ..."),__LINE__);
	g_eloamDll=LoadLibrary(_T("eloamDll.dll"));
	if (!g_eloamDll)
	{
		g_logOut.StatusOut(Info,_T("(%s) %s  End. line:%d\r\n"),__FUNCTION__,_T("eloamDll.dll����ʧ��."),__LINE__);
		return -1;
	}
	g_logOut.StatusOut(Info,_T("(%s) %s  End. line:%d\r\n"),__FUNCTION__,_T("eloamDll.dll���سɹ�."),__LINE__);
	return 0;
}