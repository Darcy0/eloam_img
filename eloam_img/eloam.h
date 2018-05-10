#ifndef __ELOAM_H__
#define __ELOAM_H__

#include <Windows.h>
#include <WTypes.h>

typedef void *HELOAMIMAGEDATA;
typedef void *HELOAMIMAGE;
typedef void *HELOAMIMAGELIST;
typedef void *HELOAMFTP;
typedef void *HELOAMHTTP;
typedef void *HELOAMDEVICE;
typedef void *HELOAMVIDEO;
typedef void *HELOAMVIEW;
typedef void *HELOAMMEMORY;
typedef void *HELOAMTHUMBNAIL;
typedef void *HELOAMRECT;
typedef void *HELOAMREGION;
typedef void *HELOAMREGIONLIST;
typedef void *HELOAMFONT;
typedef void *HELOAMTEMPL;
typedef void *HELOAMVIDEOCAP;

#define THUMBNAIL_MENU_NULL				0x01
#define THUMBNAIL_MENU_BIND				0x02
#define THUMBNAIL_MENU_FOLDER			0x03
#define THUMBNAIL_MENU_FILE				0X04

#define THUMBNAIL_ITEM_REFRESH			0x00000001	
#define THUMBNAIL_ITEM_OPENFOLDERPOS	0x00000002
#define THUMBNAIL_ITEM_DELETE			0x00000004
#define THUMBNAIL_ITEM_PROPERTY			0x00000008
#define THUMBNAIL_ITEM_EDIT				0x00000010
#define THUMBNAIL_ITEM_BIND				0x00000020
#define THUMBNAIL_ITEM_UNBIND			0x00000040
#define THUMBNAIL_ITEM_OPENFILEPOS		0x00000080
#define THUMBNAIL_ITEM_BLEND			0x00000100
#define THUMBNAIL_ITEM_CREATEPDF		0x00000200
#define THUMBNAIL_ITEM_OCR				0x00000400
#define THUMBNAIL_ITEM_PRINT			0x00000800

#define EXTERN_C extern "C"
#define ELOAMAPI __stdcall

// callback
typedef VOID (ELOAMAPI *ELOAM_DEVCHANGECALLBACK)(LONG type, LONG idx, LONG dbt, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_IDCARDCHANGECALLBACK)(LONG dbt, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_IDCARDCALLBACK)(LONG ret, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_OCRCALLBACK)(LONG flag, LONG value, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_TEMPLOCRCALLBACK)(LONG ret, LPVOID userData); 
typedef VOID (ELOAMAPI *ELOAM_ARRIVALCALLBACK)(HELOAMVIDEO video, LONG id, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_TOUCHCALLBACK)(HELOAMVIDEO video, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_MOVEDETECCALLBACK)(HELOAMVIDEO video, LONG id, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_VIEWCALLBACK)(HELOAMVIEW view, LONG flag, LONG value, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_ATTACHCALLBAK)(HELOAMVIDEO video, LONG videoId, HELOAMVIEW view, LONG viewId, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_DESKEWCALLBACK)(HELOAMVIDEO video, HELOAMVIEW view, HELOAMREGIONLIST list, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_HTTPCALLBACK)(HELOAMHTTP http, LONG type, LONG flag, LONG value, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_FTPCALLBACK)(HELOAMFTP ftp, LONG type, LONG flag, LONG value, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_BIOKEYCHANGECALLBACK)(LONG dbt, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_BIOKEYCALLBACK)(LONG type, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_READERCHANGECALLBACK)(LONG dbt, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_READERCALLBACK)(LONG type, LONG subtype, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_MAGNETIICCARDALLBACK)(LONG RET, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_SHENZHENTONGCARDALLBACK)(LONG RET, LPVOID userData);

// global
typedef BOOL (ELOAMAPI *pEloamGlobal_DestroyString)(BSTR str);
typedef HELOAMIMAGE (ELOAMAPI *pEloamGlobal_CreateImage)(LONG width, LONG height, LONG channels);
typedef HELOAMIMAGE (ELOAMAPI *pEloamGlobal_CreateImageFromFile)(BSTR fileName, LONG flag);
typedef HELOAMIMAGE (ELOAMAPI *pEloamGlobal_CreateImageFromBase64)(BSTR base64, LONG flag);
typedef HELOAMIMAGELIST (ELOAMAPI *pEloamGlobal_CreateImageList)(void);
typedef HELOAMIMAGELIST (ELOAMAPI *pEloamGlobal_CreateImageListFromFile)(BSTR fileName, LONG flag);
typedef HELOAMFTP (ELOAMAPI *pEloamGlobal_CreateFtp)(BSTR ftpPath);
typedef HELOAMHTTP (ELOAMAPI *pEloamGlobal_CreateHttp)(BSTR httpPath);
typedef HELOAMRECT (ELOAMAPI *pEloamGlobal_CreateRect)(LONG x, LONG y, LONG width, LONG height);
typedef HELOAMREGION (ELOAMAPI *pEloamGlobal_CreateRegion)(LONG x1, LONG y1, LONG x2, LONG y2, LONG x3, LONG y3, LONG x4, LONG y4);
typedef HELOAMREGIONLIST (ELOAMAPI *pEloamGlobal_CreateRegionList)(void);
typedef HELOAMFONT (ELOAMAPI *pEloamGlobal_CreateTypeface)(LONG height, LONG width, LONG escap, LONG orien, LONG thickness, LONG italic, LONG underline, LONG strike, BSTR font);
typedef HELOAMMEMORY (ELOAMAPI *pEloamGlobal_CreateMemory)(void);
typedef HELOAMMEMORY (ELOAMAPI *pEloamGlobal_CreateMemoryFromFile)(BSTR fileName);
typedef HELOAMVIEW (ELOAMAPI *pEloamGlobal_CreateView)(HWND hWnd, HELOAMRECT rect, LONG flag);
typedef HELOAMTHUMBNAIL (ELOAMAPI *pEloamGlobal_CreateThumbnail)(HWND hWnd, HELOAMRECT rect, LONG flag);
typedef BOOL (ELOAMAPI *pEloamGlobal_PlayCaptureVoice)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_DelFile)(BSTR fileName);
typedef BSTR (ELOAMAPI *pEloamGlobal_GetTempName)(BSTR ext);
typedef BOOL (ELOAMAPI *pEloamGlobal_CreateDir)(BSTR dirPath);
typedef BOOL (ELOAMAPI *pEloamGlobal_RemoveDir)(BSTR dirPath);
typedef BOOL (ELOAMAPI *pEloamGlobal_InitDevs)(ELOAM_DEVCHANGECALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamGlobal_DeinitDevs)(void);
typedef LONG (ELOAMAPI *pEloamGlobal_GetDevCount)(LONG type);
typedef BSTR (ELOAMAPI *pEloamGlobal_GetDisplayName)(LONG type, LONG idx);
typedef BSTR (ELOAMAPI *pEloamGlobal_GetFriendlyName)(LONG type, LONG idx);
typedef LONG (ELOAMAPI *pEloamGlobal_GetEloamType)(LONG type, LONG idx);
typedef HELOAMDEVICE (ELOAMAPI *pEloamGlobal_CreateDevice)(LONG type, LONG idx);
typedef BOOL (ELOAMAPI *pEloamGlobal_InitIdCard)(ELOAM_IDCARDCHANGECALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamGlobal_DeinitIdCard)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_DiscernIdCard)(ELOAM_IDCARDCALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamGlobal_ReadIdCard)(void);
typedef HELOAMIMAGE (ELOAMAPI *pEloamGlobal_GetIdCardImage)(LONG flag);
typedef BSTR (ELOAMAPI *pEloamGlobal_GetIdCardData)(LONG flag);
typedef HELOAMMEMORY (ELOAMAPI *pEloamGlobal_GetIdCardFingerprint)();
typedef BOOL (ELOAMAPI *pEloamGlobal_StopIdCardDiscern)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_InitBarcode)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_DeinitBarcode)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_DiscernBarcode)(HELOAMIMAGE img);
typedef LONG (ELOAMAPI *pEloamGlobal_GetBarcodeCount)(void);
typedef LONG (ELOAMAPI *pEloamGlobal_GetBarcodeType)(LONG idx);
typedef BSTR (ELOAMAPI *pEloamGlobal_GetBarcodeData)(LONG idx);
typedef BOOL (ELOAMAPI *pEloamGlobal_InitFaceDetect)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_DeinitFaceDetect)(void);
typedef HELOAMRECT (ELOAMAPI *pEloamGlobal_GetFaceRect)(HELOAMIMAGE img);
typedef LONG (ELOAMAPI *pEloamGlobal_DiscernFaceDetect)(HELOAMIMAGE img1, HELOAMIMAGE img2);
typedef LONG (ELOAMAPI *pEloamGlobal_DetectFaceLive)(HELOAMIMAGE color_image,HELOAMIMAGE black_image,int nThreshold,DWORD  dwReserved);
typedef BOOL (ELOAMAPI *pEloamGlobal_EnableFaceRectCrop)(HELOAMVIDEO video, LONG flag);
typedef BOOL (ELOAMAPI *pEloamGlobal_DisableFaceRectCrop)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamGlobal_InitOcr)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_DeinitOcr)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_SetOcrLanguage)(LONG lang);
typedef BOOL (ELOAMAPI *pEloamGlobal_DiscernOcr)(LONG flag, HELOAMIMAGE img, ELOAM_OCRCALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamGlobal_DiscernOcrList)(LONG flag, HELOAMIMAGELIST list, ELOAM_OCRCALLBACK fun, LPVOID userData);
typedef LONG (ELOAMAPI *pEloamGlobal_GetOcrPageCount)(void);
typedef LONG (ELOAMAPI *pEloamGlobal_GetOcrBlockCount)(LONG page);
typedef LONG (ELOAMAPI *pEloamGlobal_GetOcrBarcodeType)(LONG page, LONG blk);
typedef BSTR (ELOAMAPI *pEloamGlobal_GetOcrPlainText)(LONG page);
typedef BSTR (ELOAMAPI *pEloamGlobal_GetOcrData)(LONG page, LONG blk);
typedef BOOL (ELOAMAPI *pEloamGlobal_SaveOcr)(BSTR fileName, LONG flag);
typedef BOOL (ELOAMAPI *pEloamGlobal_StopOcrDiscern)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_WaitOcrDiscern)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_InitBiokey)(ELOAM_BIOKEYCHANGECALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamGlobal_DeinitBiokey)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_GetBiokeyTemplate)(ELOAM_BIOKEYCALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamGlobal_StopGetBiokeyTemplate)(void);
typedef HELOAMMEMORY (ELOAMAPI *pEloamGlobal_GetBiokeyTemplateData)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_GetBiokeyFeature)(ELOAM_BIOKEYCALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamGlobal_StopGetBiokeyFeature)(void);
typedef HELOAMMEMORY (ELOAMAPI *pEloamGlobal_GetBiokeyFeatureData)(void);
typedef HELOAMIMAGE (ELOAMAPI *pEloamGlobal_GetBiokeyImage)(void);
typedef LONG (ELOAMAPI *pEloamGlobal_BiokeyVerify)(HELOAMMEMORY mem1, HELOAMMEMORY mem2);
typedef LONG (ELOAMAPI *pEloamGlobal_BiokeyVerifyFromString)(BSTR str1, BSTR str2);
typedef HELOAMTEMPL (ELOAMAPI *pEloamGlobal_CreateTempl)(BSTR templName);
typedef HELOAMTEMPL (ELOAMAPI *pEloamGlobal_CreateTemplFromFile)(BSTR fileName, LONG flag);
typedef HELOAMTEMPL (ELOAMAPI *pEloamGlobal_CreateTemplFromData)(BSTR data, LONG flag);
typedef BOOL (ELOAMAPI *pEloamGlobal_InitTemplOcr)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_DeinitTemplOcr)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_SetTemplOcrLanguage)(LONG lang);
typedef BOOL (ELOAMAPI *pEloamGlobal_DiscernTempl)(HELOAMIMAGE image, HELOAMTEMPL templ, ELOAM_TEMPLOCRCALLBACK fun, LPVOID userData);
typedef HELOAMTEMPL (ELOAMAPI *pEloamGlobal_GetTemplResult)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_StopTemplDiscern)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_WaitTemplDiscern)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_QuickOcr)(BSTR fileName, BSTR resultPath, LONG flag);
typedef BOOL (ELOAMAPI *pEloamGlobal_InitReader)(ELOAM_READERCHANGECALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamGlobal_DeinitReader)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_ReaderStart)(ELOAM_READERCALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamGlobal_ReaderStop)(void);
typedef BSTR (ELOAMAPI *pEloamGlobal_ReaderGetCpuId)(void);

typedef BOOL (ELOAMAPI *pEloamGlobal_CpuGetBankCardTrack)(BSTR *info);
typedef LONG (ELOAMAPI *pEloamGlobal_CpuGetRecordNumber)();
typedef BOOL (ELOAMAPI *pEloamGlobal_CpuGetankCardRecord)(LONG index, BSTR *data);

typedef BSTR (ELOAMAPI *pEloamGlobal_ReaderGetCpuCreditCardNumber)(void);
typedef BSTR (ELOAMAPI *pEloamGlobal_ReaderGetMemoryId)(void);
typedef BSTR (ELOAMAPI *pEloamGlobal_ReaderGetQuickPassData)(LONG type);
typedef BSTR (ELOAMAPI *pEloamGlobal_ReaderGetM1Id)(void);
typedef BSTR (ELOAMAPI *pEloamGlobal_ReaderGetSocialCardData)(LONG flag);
typedef BOOL (ELOAMAPI *pEloamGlobal_InitMagneticCard)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_DeinitMagneticCard)(void);
typedef BOOL (ELOAMAPI *pEloamGlobal_MagneticCardReaderStart)(ELOAM_MAGNETIICCARDALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamGlobal_MagneticCardReaderStop)(void);
typedef BSTR (ELOAMAPI *pEloamGlobal_MagneticCardGetData)(LONG type);
typedef BSTR (ELOAMAPI *pEloamGlobal_MagneticCardGetNumber)();

typedef BOOL (*pEloamGlobal_InitShenZhenTong)(void);
typedef BOOL (*pEloamGlobal_DeinitShenZhenTong)(void);
typedef BOOL (*pEloamGlobal_StartShenZhenTongCard)(ELOAM_SHENZHENTONGCARDALLBACK fun, void *param);
typedef BOOL (*pEloamGlobal_StopShenZhenTongCard)(void);
typedef BOOL (*pEloamGlobal_GetShenZhenTongNumber)(BSTR *number,BSTR *amount);
typedef LONG (*pEloamGlobal_GetShenZhenTongCardRecordNumber)();
typedef BOOL (*pEloamGlobal_GetShenZhenTongCardRecord)(LONG index, BSTR *data);


typedef BOOL (ELOAMAPI *pEloamGlobal_IsUserAdmin)();
typedef LONG (ELOAMAPI *pEloamGlobal_GetPrinterCount)();
typedef BSTR (ELOAMAPI *pEloamGlobal_GetPrinterName)(LONG idx);

typedef BOOL (*pEloamGlobal_VideoCapInit)();
typedef HELOAMVIDEOCAP (*pEloamGlobal_CreatVideoCap)();
typedef BOOL (*pEloamGlobal_DestroyVideoCap)(HELOAMVIDEOCAP hVideoCap);
typedef BOOL (*pEloamGlobal_VideoCapPreCap)(
	HELOAMVIDEOCAP hVideoCap,
	BSTR outputFileName,
	int micIndex,	
	int frameRate,				
	int compressMode,	
	int videoWidth, 
	int videoHeight,	
	BOOL bCapVideo);
typedef BOOL (*pEloamGlobal_VideoCapStart)(
	HELOAMVIDEOCAP hVideoCap
	);
typedef BOOL (*pEloamGlobal_VideoCapStop)(HELOAMVIDEOCAP hVideoCap);
typedef BOOL (*pEloamGlobal_VideoCapPause)(HELOAMVIDEOCAP hVideoCap);
typedef BOOL (*pEloamGlobal_VideoCapSetWatermark)(
	HELOAMVIDEOCAP hVideoCap,
	LONG bWatermark,	
	LONG bAddTime ,	
	LONG mode,
	LONG pos,	
	LONG alpha,	
	BSTR imgPath,	
	BSTR pStrText,	
	LONG Color,	
	BSTR faceName,
	LONG lHeight,	
	LONG lWeight,
	LONG lItalic);
typedef BOOL (*pEloamGlobal_VideoCapAddVideoSrc)(HELOAMVIDEOCAP hVideoCap, HELOAMVIDEO video);
typedef BOOL (*pEloamGlobal_VideoCapAddVideoSrcEx)(HELOAMVIDEOCAP hVideoCap, HELOAMVIDEO video, LONG dstX, LONG dstY, LONG dstWidth, LONG dstHeight, LONG bProportion, LONG alpha);
typedef BOOL (*pEloamGlobal_VideoCapRemoveAllVideoSrc)(HELOAMVIDEOCAP hVideoCap);
typedef LONG (*pEloamGlobal_VideoCapGetAudioDevNum)();
typedef BSTR (*pEloamGlobal_VideoCapGetAudioDevName)(LONG devIndex);
typedef LONG (*pEloamGlobal_VideoCapGetState)(HELOAMVIDEOCAP hVideoCap);
typedef HELOAMMEMORY (*pEloamGlobal_CreateMemoryFromString)(BSTR data);
typedef BOOL (ELOAMAPI *pEloamGlobal_SetSoftDog)(BOOL open);
typedef BSTR (ELOAMAPI *pEloamGlobal_GetKeyFromSoftDog)(LONG len);
typedef HELOAMMEMORY (ELOAMAPI *pEloamGlobal_GetIdCardFingerprintEx)(LONG type);
typedef LONG (ELOAMAPI *pEloamGlobal_BiokeyImageMatch)(HELOAMMEMORY mem1, HELOAMMEMORY mem2);

// image
typedef LONG (ELOAMAPI *pEloamImage_AddRef)(HELOAMIMAGE img);
typedef LONG (ELOAMAPI *pEloamImage_Release)(HELOAMIMAGE img);
typedef HELOAMIMAGEDATA (ELOAMAPI *pEloamImage_GetImageData)(HELOAMIMAGE img);
typedef HELOAMMEMORY (ELOAMAPI *pEloamImage_CreateMemory)(HELOAMIMAGE img, LONG fmt, LONG flag);
typedef BOOL (ELOAMAPI *pEloamImage_Copy)(HELOAMIMAGE imgDest, HELOAMIMAGE imgSrc);
typedef BOOL (ELOAMAPI *pEloamImage_Save)(HELOAMIMAGE img, BSTR fileName, LONG flag);
typedef BOOL (ELOAMAPI *pEloamImage_SaveToPDF)(HELOAMIMAGE img, LONG fmt, BSTR fileName, LONG flag);
typedef BOOL (ELOAMAPI *pEloamImage_SetDiscernRect)(HELOAMIMAGE img, HELOAMRECT rect);
typedef BOOL (ELOAMAPI *pEloamImage_Print)(HELOAMIMAGE img, FLOAT x, FLOAT y, FLOAT width, FLOAT height, BSTR printer);
typedef BOOL (ELOAMAPI *pEloamImage_PrintByDPI)(HELOAMIMAGE img, FLOAT x, FLOAT y, BSTR printer);
typedef BOOL (ELOAMAPI *pEloamImage_AdaptivePrint)(HELOAMIMAGE img, FLOAT width, FLOAT height, BSTR printer);
typedef BOOL (ELOAMAPI *pEloamImage_AdaptivePrintByDPI)(HELOAMIMAGE img, BSTR printer);
typedef LONG (ELOAMAPI *pEloamImage_GetWidth)(HELOAMIMAGE img);
typedef LONG (ELOAMAPI *pEloamImage_GetHeight)(HELOAMIMAGE img);
typedef LONG (ELOAMAPI *pEloamImage_GetChannels)(HELOAMIMAGE img);
typedef LONG (ELOAMAPI *pEloamImage_GetWidthStep)(HELOAMIMAGE img);
typedef LONG (ELOAMAPI *pEloamImage_GetXDPI)(HELOAMIMAGE img);
typedef LONG (ELOAMAPI *pEloamImage_GetYDPI)(HELOAMIMAGE img);
typedef BOOL (ELOAMAPI *pEloamImage_SetXDPI)(HELOAMIMAGE img, LONG value);
typedef BOOL (ELOAMAPI *pEloamImage_SetYDPI)(HELOAMIMAGE img, LONG value);
typedef BSTR (ELOAMAPI *pEloamImage_GetBase64)(HELOAMIMAGE img, LONG fmt, LONG flag);
typedef BSTR (ELOAMAPI *pEloamImage_GetMD5)(HELOAMIMAGE img, LONG fmt, LONG flag);
typedef BOOL (ELOAMAPI *pEloamImage_DrawText)(HELOAMIMAGE img, HELOAMFONT font, LONG x, LONG y, BSTR text, COLORREF clr, LONG weight);
typedef BOOL (ELOAMAPI *pEloamImage_DrawTextEx)(HELOAMIMAGE img, LONG pos, BSTR text, COLORREF clr, LONG weight);
typedef BOOL (ELOAMAPI *pEloamImage_Rotate)(HELOAMIMAGE img, FLOAT angle, COLORREF clr, LONG flag);
typedef BOOL (ELOAMAPI *pEloamImage_Crop)(HELOAMIMAGE img, HELOAMRECT rect);
typedef BOOL (ELOAMAPI *pEloamImage_Resize)(HELOAMIMAGE img, LONG width, LONG height, LONG flag);
typedef BOOL (ELOAMAPI *pEloamImage_Blend)(HELOAMIMAGE imgDest, HELOAMRECT rectDest, HELOAMIMAGE imgSrc, HELOAMRECT rectSrc, LONG weight, LONG flag);
typedef BOOL (ELOAMAPI *pEloamImage_ToColor)(HELOAMIMAGE img);
typedef BOOL (ELOAMAPI *pEloamImage_ToGray)(HELOAMIMAGE img);
typedef BOOL (ELOAMAPI *pEloamImage_Threshold)(HELOAMIMAGE img, LONG threshold);
typedef BOOL (ELOAMAPI *pEloamImage_AdaptiveThreshold)(HELOAMIMAGE img, LONG flag);
typedef BOOL (ELOAMAPI *pEloamImage_Deskew)(HELOAMIMAGE img, LONG flag);
typedef BOOL (ELOAMAPI *pEloamImage_Reverse)(HELOAMIMAGE img);
typedef BOOL (ELOAMAPI *pEloamImage_Smooth)(HELOAMIMAGE img, LONG flag);
typedef BOOL (ELOAMAPI *pEloamImage_Rectify)(HELOAMIMAGE img, LONG flag);
typedef BOOL (ELOAMAPI *pEloamImage_DelImageBackgrand)(HELOAMIMAGE img, HELOAMIMAGE dest);
typedef BOOL (ELOAMAPI *pEloamImage_DelImageBackColor)(HELOAMIMAGE img, HELOAMIMAGE dest);
typedef BOOL (ELOAMAPI *pEloamImage_Denoising)(HELOAMIMAGE img, HELOAMIMAGE dest);
typedef BOOL (ELOAMAPI *pEloamImage_Balance)(HELOAMIMAGE img, HELOAMIMAGE dest);
typedef BOOL (ELOAMAPI *pEloamImage_IdCardProcess)(HELOAMIMAGE img);
typedef BOOL (ELOAMAPI *pEloamImage_FormProcess)(HELOAMIMAGE img);
typedef BOOL (ELOAMAPI *pEloamImage_Whiten)(HELOAMIMAGE img, LONG flag, LONG threshold, FLOAT autoThresholdRatio, LONG aroundNum, LONG lowestBrightness);
typedef BOOL (ELOAMAPI *pEloamImage_EraseBlackEdge)(HELOAMIMAGE img, LONG flag);
// memory
typedef LONG (ELOAMAPI *pEloamMemory_AddRef)(HELOAMMEMORY mem);
typedef LONG (ELOAMAPI *pEloamMemory_Release)(HELOAMMEMORY mem);
typedef BSTR (ELOAMAPI *pEloamMemory_GetBase64)(HELOAMMEMORY mem);
typedef HELOAMIMAGE (ELOAMAPI *pEloamMemory_CreateImage)(HELOAMMEMORY mem, LONG flag);
typedef BOOL (ELOAMAPI *pEloamMemory_Save)(HELOAMMEMORY mem, BSTR fileName);
typedef BSTR (ELOAMAPI *pEloamMemory_GetString)(HELOAMMEMORY mem);

//MD5
typedef BSTR (ELOAMAPI *pEloamMemory_GetMD5)(HELOAMMEMORY mem);

// image list
typedef LONG (ELOAMAPI *pEloamImageList_AddRef)(HELOAMIMAGELIST list);
typedef LONG (ELOAMAPI *pEloamImageList_Release)(HELOAMIMAGELIST list);
typedef BOOL (ELOAMAPI *pEloamImageList_Add)(HELOAMIMAGELIST list, HELOAMIMAGE img);
typedef BOOL (ELOAMAPI *pEloamImageList_Insert)(HELOAMIMAGELIST list, HELOAMIMAGE img, LONG pos);
typedef BOOL (ELOAMAPI *pEloamImageList_Remove)(HELOAMIMAGELIST list, LONG idx);
typedef BOOL (ELOAMAPI *pEloamImageList_Clear)(HELOAMIMAGELIST list);
typedef LONG (ELOAMAPI *pEloamImageList_GetCount)(HELOAMIMAGELIST list);
typedef HELOAMIMAGE (ELOAMAPI *pEloamImageList_GetImage)(HELOAMIMAGELIST list, LONG idx);
typedef BOOL (ELOAMAPI *pEloamImageList_Save)(HELOAMIMAGELIST list, BSTR fileName, LONG flag);
typedef BOOL (ELOAMAPI *pEloamImageList_SaveToPDF)(HELOAMIMAGELIST list, LONG fmt, BSTR fileName, LONG flag);

// http
typedef LONG (ELOAMAPI *pEloamHttp_AddRef)(HELOAMHTTP http);
typedef LONG (ELOAMAPI *pEloamHttp_Release)(HELOAMHTTP http);
typedef BOOL (ELOAMAPI *pEloamHttp_Upload)(HELOAMHTTP http, LONG flag, BSTR localPath, BSTR headers, BSTR predata, BSTR taildata, ELOAM_HTTPCALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamHttp_UploadMemory)(HELOAMHTTP http, LONG flag, HELOAMMEMORY mem, BSTR headers, BSTR predata, BSTR taildata, ELOAM_HTTPCALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamHttp_UploadImageFile)(HELOAMHTTP http, BSTR fileName, BSTR remoteName, ELOAM_HTTPCALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamHttp_UploadImageMemory)(HELOAMHTTP http, HELOAMMEMORY mem, BSTR remoteName, ELOAM_HTTPCALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamHttp_UploadImage)(HELOAMHTTP http, HELOAMIMAGE img, int fmt, int flag, BSTR remoteName, ELOAM_HTTPCALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamHttp_StopUpload)(HELOAMHTTP http);
typedef BOOL (ELOAMAPI *pEloamHttp_WaitUpload)(HELOAMHTTP http);
typedef BSTR (ELOAMAPI *pEloamHttp_GetServerInfo)(HELOAMHTTP http);

// ftp
typedef LONG (ELOAMAPI *pEloamFtp_AddRef)(HELOAMFTP ftp);
typedef LONG (ELOAMAPI *pEloamFtp_Release)(HELOAMFTP ftp);
typedef BOOL (ELOAMAPI *pEloamFtp_Upload)(HELOAMFTP ftp, LONG flag, BSTR localPath, BSTR remotePath, ELOAM_FTPCALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamFtp_StopUpload)(HELOAMFTP ftp);
typedef BOOL (ELOAMAPI *pEloamFtp_WaitUpload)(HELOAMFTP ftp);
typedef BOOL (ELOAMAPI *pEloamFtp_CreateDir)(HELOAMFTP ftp, BSTR dirPath);
typedef BOOL (ELOAMAPI *pEloamFtp_RemoveDir)(HELOAMFTP ftp, BSTR dirPath);

// device
typedef LONG (ELOAMAPI *pEloamDevice_AddRef)(HELOAMDEVICE dev);
typedef LONG (ELOAMAPI *pEloamDevice_Release)(HELOAMDEVICE dev);
typedef LONG (ELOAMAPI *pEloamDevice_GetType)(HELOAMDEVICE dev);
typedef LONG (ELOAMAPI *pEloamDevice_GetIndex)(HELOAMDEVICE dev);
typedef LONG (ELOAMAPI *pEloamDevice_GetState)(HELOAMDEVICE dev);
typedef BSTR (ELOAMAPI *pEloamDevice_GetFriendlyName)(HELOAMDEVICE dev);
typedef BSTR (ELOAMAPI *pEloamDevice_GetDisplayName)(HELOAMDEVICE dev);
typedef LONG (ELOAMAPI *pEloamDevice_GetEloamType)(HELOAMDEVICE dev);
typedef LONG (ELOAMAPI *pEloamDevice_GetScanSize)(HELOAMDEVICE dev);
typedef LONG (ELOAMAPI *pEloamDevice_GetSubtype)(HELOAMDEVICE dev);
typedef LONG (ELOAMAPI *pEloamDevice_GetResolutionCount)(HELOAMDEVICE dev);
typedef LONG (ELOAMAPI *pEloamDevice_GetResolutionWidth)(HELOAMDEVICE dev, LONG idx);
typedef LONG (ELOAMAPI *pEloamDevice_GetResolutionHeight)(HELOAMDEVICE dev, LONG idx);
typedef LONG (ELOAMAPI *pEloamDevice_GetResolutionCountEx)(HELOAMDEVICE dev, LONG subType);
typedef LONG (ELOAMAPI *pEloamDevice_GetResolutionWidthEx)(HELOAMDEVICE dev, LONG subType, LONG idx);
typedef LONG (ELOAMAPI *pEloamDevice_GetResolutionHeightEx)(HELOAMDEVICE dev, LONG subType,LONG idx);
typedef BOOL (ELOAMAPI *pEloamDevice_PausePreview)(HELOAMDEVICE dev);
typedef BOOL (ELOAMAPI *pEloamDevice_ResumePreview)(HELOAMDEVICE dev);
typedef LONG (ELOAMAPI *pEloamDevice_GetVideoProcAmp)(HELOAMDEVICE dev, LONG prop, LONG value);
typedef BOOL (ELOAMAPI *pEloamDevice_SetVideoProcAmp)(HELOAMDEVICE dev, LONG prop, LONG value, BOOL isAuto);
typedef LONG (ELOAMAPI *pEloamDevice_GetCameraControl)(HELOAMDEVICE dev, LONG prop, LONG value);
typedef BOOL (ELOAMAPI *pEloamDevice_SetCameraControl)(HELOAMDEVICE dev, LONG prop, LONG value, BOOL isAuto);
typedef BOOL (ELOAMAPI *pEloamDevice_ShowProperty)(HELOAMDEVICE dev, HWND hWnd);
typedef BOOL (ELOAMAPI *pEloamDevice_ShowPropertyEx)(HELOAMDEVICE dev, HELOAMVIEW view);
typedef BSTR (ELOAMAPI *pEloamDevice_GetSonixSerialNumber)(HELOAMDEVICE dev);
typedef HELOAMVIDEO (ELOAMAPI *pEloamDevice_CreateVideo)(HELOAMDEVICE dev, LONG resolution, LONG subtype, ELOAM_ARRIVALCALLBACK funArrival, LPVOID userArrival,
	ELOAM_TOUCHCALLBACK funTouch, LPVOID userTouch,int resolutionStillCap,int subtypeStillCap);

// video
typedef LONG (ELOAMAPI *pEloamVideo_AddRef)(HELOAMVIDEO video);
typedef LONG (ELOAMAPI *pEloamVideo_Release)(HELOAMVIDEO video);
typedef HELOAMDEVICE (ELOAMAPI *pEloamVideo_GetDevice)(HELOAMVIDEO video);
typedef LONG (ELOAMAPI *pEloamVideo_GetResolution)(HELOAMVIDEO video);
typedef LONG (ELOAMAPI *pEloamVideo_GetSubtype)(HELOAMVIDEO video);
typedef LONG (ELOAMAPI *pEloamVideo_GetWidth)(HELOAMVIDEO video);
typedef LONG (ELOAMAPI *pEloamVideo_GetHeight)(HELOAMVIDEO video);
typedef HELOAMIMAGE (ELOAMAPI *pEloamVideo_CreateImage)(HELOAMVIDEO video, LONG scanSize, HELOAMVIEW view);
typedef HELOAMIMAGELIST (ELOAMAPI *pEloamVideo_CreateImageList)(HELOAMVIDEO video, LONG scanSize, HELOAMVIEW view);
typedef BOOL (ELOAMAPI *pEloamVideo_RotateLeft)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_RotateRight)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_Rotate180)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_Flip)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_Mirror)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_FlipAndMirror)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_EnableGray)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_DisableGray)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_EnableAdaptiveThreshold)(HELOAMVIDEO video, LONG flag);
typedef BOOL (ELOAMAPI *pEloamVideo_DisableAdaptiveThreshold)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_EnableThreshold)(HELOAMVIDEO video, LONG threshold);
typedef BOOL (ELOAMAPI *pEloamVideo_DisableThreshold)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_EnableDelBkColor)(HELOAMVIDEO video, LONG flag);
typedef BOOL (ELOAMAPI *pEloamVideo_DisableDelBkColor)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_EnableAddText)(HELOAMVIDEO video, HELOAMFONT font, LONG x, LONG y, BSTR text, COLORREF clr, LONG weight);
typedef BOOL (ELOAMAPI *pEloamVideo_EnableAddTextEx)(HELOAMVIDEO video, LONG pos, BSTR text, COLORREF clr, LONG weight);
typedef BOOL (ELOAMAPI *pEloamVideo_DisableAddText)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_EnableDeskew)(HELOAMVIDEO video, LONG flag);
typedef BOOL (ELOAMAPI *pEloamVideo_EnableDeskewEx)(HELOAMVIDEO video, LONG flag);
typedef BOOL (ELOAMAPI *pEloamVideo_EnableDeskew2)(HELOAMVIDEO video, LONG flag, ELOAM_DESKEWCALLBACK fun, LPVOID userData);	// 回调版本
typedef BOOL (ELOAMAPI *pEloamVideo_EnableDeskew2Ex)(HELOAMVIDEO video, LONG flag, ELOAM_DESKEWCALLBACK fun, LPVOID userData); // 回调版本
typedef BOOL (ELOAMAPI *pEloamVideo_DisableDeskew)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_EnableReverse)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_DisableReverse)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_EnableSmooth)(HELOAMVIDEO video, LONG flag);
typedef BOOL (ELOAMAPI *pEloamVideo_DisableSmooth)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_EnableMoveDetec)(HELOAMVIDEO video, LONG flag, ELOAM_MOVEDETECCALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamVideo_DisableMoveDetec)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_EnableMoveDetecEx)(HELOAMVIDEO video, LONG flag, ELOAM_MOVEDETECCALLBACK fun, LPVOID userData,ELOAM_MOVEDETECCALLBACK fun2, LPVOID userData2);
typedef BOOL (ELOAMAPI *pEloamVideo_DisableMoveDetecEx)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_EnableDate)(HELOAMVIDEO video, HELOAMFONT font, LONG x, LONG y, COLORREF clr, LONG weight);
typedef BOOL (ELOAMAPI *pEloamVideo_EnableDateEx)(HELOAMVIDEO video, LONG pos, COLORREF clr, LONG weight);
typedef BOOL (ELOAMAPI *pEloamVideo_DisableDate)(HELOAMVIDEO video);
typedef BOOL (ELOAMAPI *pEloamVideo_SetCropState)(HELOAMVIDEO video, LONG state);

typedef BOOL (ELOAMAPI *pEloamVideo_StartRecord)(HELOAMVIDEO video, BSTR fileName, LONG flag);
typedef BOOL (ELOAMAPI *pEloamVideo_StopRecord)(HELOAMVIDEO video);

typedef BOOL (*pEloamVideo_FindRect)(HELOAMVIDEO video,HELOAMRECT rect);
typedef BOOL (*pEloamVideo_SetDisplayRect)(HELOAMVIDEO video,HELOAMRECT rect,BOOL enable);

// view
typedef LONG (ELOAMAPI *pEloamView_AddRef)(HELOAMVIEW view);
typedef LONG (ELOAMAPI *pEloamView_Release)(HELOAMVIEW view);
typedef BOOL (ELOAMAPI *pEloamView_SetCallback)(HELOAMVIEW view, ELOAM_VIEWCALLBACK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamView_SelectImage)(HELOAMVIEW view, HELOAMIMAGE image);
typedef BOOL (ELOAMAPI *pEloamView_SelectVideo)(HELOAMVIEW view, HELOAMVIDEO video, ELOAM_ATTACHCALLBAK fun, LPVOID userData);
typedef BOOL (ELOAMAPI *pEloamView_SelectNull)(HELOAMVIEW view);
typedef BOOL (ELOAMAPI *pEloamView_SetZoomIn)(HELOAMVIEW view);
typedef BOOL (ELOAMAPI *pEloamView_SetZoomOut)(HELOAMVIEW view);
typedef BOOL (ELOAMAPI *pEloamView_SetCarpete)(HELOAMVIEW view);
typedef BOOL (ELOAMAPI *pEloamView_SetOriginal)(HELOAMVIEW view);
typedef BOOL (ELOAMAPI *pEloamView_SetAdapt)(HELOAMVIEW view);
typedef VOID (ELOAMAPI *pEloamView_EnableFullScreen)(HELOAMVIEW view, BOOL bFull);
typedef BOOL (ELOAMAPI *pEloamView_SetFullScreen)(HELOAMVIEW view, BOOL bFull);
typedef LONG (ELOAMAPI *pEloamView_GetState)(HELOAMVIEW view);
typedef BOOL (ELOAMAPI *pEloamView_SetState)(HELOAMVIEW view, LONG state);
typedef BOOL (ELOAMAPI *pEloamView_SetBkColor)(HELOAMVIEW view, COLORREF clr);
typedef BOOL (ELOAMAPI *pEloamView_SetText)(HELOAMVIEW view, BSTR text, COLORREF clr);
typedef HELOAMRECT (ELOAMAPI *pEloamView_GetSelectedRect)(HELOAMVIEW view);
typedef HELOAMRECT (ELOAMAPI *pEloamView_GetImageRect)(HELOAMVIEW view);
typedef BOOL (ELOAMAPI *pEloamView_SetSelectedRect)(HELOAMVIEW view, HELOAMRECT rect);
typedef BOOL (ELOAMAPI *pEloamView_PlayCaptureEffect)(HELOAMVIEW view);
typedef BOOL (ELOAMAPI *pEloamView_SetScale)(HELOAMVIEW view, LONG scale);
typedef BOOL (ELOAMAPI *pEloamView_SetRatio)(HELOAMVIEW view, LONG ratio);
typedef BOOL (ELOAMAPI *pEloamView_SetMessage)(HELOAMVIEW view, BSTR msg);
typedef BOOL (ELOAMAPI *pEloamView_SetRectangleFormat)(HELOAMVIEW view, int flag, int lineSize, COLORREF lineColor,
	int lineStyle, int pointSize, COLORREF pointColor);

typedef BOOL (ELOAMAPI *pEloamView_DrawCustomRect)(HELOAMVIEW view, int flag,HELOAMRECT rect,int cWidth,COLORREF color);

// rect
typedef LONG (ELOAMAPI *pEloamRect_AddRef)(HELOAMRECT rect);
typedef LONG (ELOAMAPI *pEloamRect_Release)(HELOAMRECT rect);
typedef LONG (ELOAMAPI *pEloamRect_GetLeft)(HELOAMRECT rect);
typedef BOOL (ELOAMAPI *pEloamRect_SetLeft)(HELOAMRECT rect, LONG left);
typedef LONG (ELOAMAPI *pEloamRect_GetTop)(HELOAMRECT rect);
typedef BOOL (ELOAMAPI *pEloamRect_SetTop)(HELOAMRECT rect, LONG top);
typedef LONG (ELOAMAPI *pEloamRect_GetWidth)(HELOAMRECT rect);
typedef BOOL (ELOAMAPI *pEloamRect_SetWidth)(HELOAMRECT rect, LONG width);
typedef LONG (ELOAMAPI *pEloamRect_GetHeight)(HELOAMRECT rect);
typedef BOOL (ELOAMAPI *pEloamRect_SetHeight)(HELOAMRECT rect, LONG height);
typedef BOOL (ELOAMAPI *pEloamRect_Copy)(HELOAMRECT rectDest, HELOAMRECT rectSrc);

// region
typedef LONG (ELOAMAPI *pEloamRegion_AddRef)(HELOAMREGION region);
typedef LONG (ELOAMAPI *pEloamRegion_Release)(HELOAMREGION region);
typedef LONG (ELOAMAPI *pEloamRegion_GetX1)(HELOAMREGION region);
typedef LONG (ELOAMAPI *pEloamRegion_GetY1)(HELOAMREGION region);
typedef LONG (ELOAMAPI *pEloamRegion_GetX2)(HELOAMREGION region);
typedef LONG (ELOAMAPI *pEloamRegion_GetY2)(HELOAMREGION region);
typedef LONG (ELOAMAPI *pEloamRegion_GetX3)(HELOAMREGION region);
typedef LONG (ELOAMAPI *pEloamRegion_GetY3)(HELOAMREGION region);
typedef LONG (ELOAMAPI *pEloamRegion_GetX4)(HELOAMREGION region);
typedef LONG (ELOAMAPI *pEloamRegion_GetY4)(HELOAMREGION region);
typedef LONG (ELOAMAPI *pEloamRegion_GetWidth)(HELOAMREGION region);
typedef LONG (ELOAMAPI *pEloamRegion_GetHeight)(HELOAMREGION region);

// region list
typedef LONG (ELOAMAPI *pEloamRegionList_AddRef)(HELOAMREGIONLIST list);
typedef LONG (ELOAMAPI *pEloamRegionList_Release)(HELOAMREGIONLIST list);
typedef BOOL (ELOAMAPI *pEloamRegionList_Add)(HELOAMREGIONLIST list, HELOAMREGION region);
typedef BOOL (ELOAMAPI *pEloamRegionList_Insert)(HELOAMREGIONLIST list, HELOAMREGION region, LONG pos);
typedef BOOL (ELOAMAPI *pEloamRegionList_Remove)(HELOAMREGIONLIST list, LONG idx);
typedef BOOL (ELOAMAPI *pEloamRegionList_Clear)(HELOAMREGIONLIST list);
typedef LONG (ELOAMAPI *pEloamRegionList_GetCount)(HELOAMREGIONLIST list);
typedef HELOAMREGION (ELOAMAPI *pEloamRegionList_GetRegion)(HELOAMREGIONLIST list, LONG idx);

// thumbnail
typedef LONG (ELOAMAPI *pEloamThumbnail_AddRef)(HELOAMTHUMBNAIL thumb);
typedef LONG (ELOAMAPI *pEloamThumbnail_Release)(HELOAMTHUMBNAIL thumb);
typedef BOOL (ELOAMAPI *pEloamThumbnail_Add)(HELOAMTHUMBNAIL thumb, BSTR imagePath);
typedef BOOL (ELOAMAPI *pEloamThumbnail_Insert)(HELOAMTHUMBNAIL thumb, BSTR imagePath, LONG pos);
typedef BOOL (ELOAMAPI *pEloamThumbnail_Remove)(HELOAMTHUMBNAIL thumb, LONG idx, BOOL bDel);
typedef BOOL (ELOAMAPI *pEloamThumbnail_Clear)(HELOAMTHUMBNAIL thumb, BOOL bDel);
typedef LONG (ELOAMAPI *pEloamThumbnail_GetCount)(HELOAMTHUMBNAIL thumb);
typedef BSTR (ELOAMAPI *pEloamThumbnail_GetFileName)(HELOAMTHUMBNAIL thumb, LONG idx);
typedef LONG (ELOAMAPI *pEloamThumbnail_GetSelected)(HELOAMTHUMBNAIL thumb);
typedef BOOL (ELOAMAPI *pEloamThumbnail_SetLanguage)(HELOAMTHUMBNAIL thumb, LONG langId);
typedef BOOL (ELOAMAPI *pEloamThumbnail_SetMenuItem)(HELOAMTHUMBNAIL thumb, LONG menuId, LONG flag);
typedef BOOL (ELOAMAPI *pEloamThumbnail_GetCheck)(HELOAMTHUMBNAIL thumb, LONG idx);
typedef BOOL (ELOAMAPI *pEloamThumbnail_SetCheck)(HELOAMTHUMBNAIL thumb, LONG idx, BOOL bCheck);
typedef BOOL (ELOAMAPI *pEloamThumbnail_HttpUploadCheckImage)(HWND hWnd, HELOAMTHUMBNAIL thumb, BSTR serverAddress, LONG flag);
typedef BSTR (ELOAMAPI *pEloamThumbnail_GetHttpServerInfo)();
// font
typedef LONG (ELOAMAPI *pEloamFont_AddRef)(HELOAMFONT font);
typedef LONG (ELOAMAPI *pEloamFont_Release)(HELOAMFONT font);

// templ
typedef LONG (ELOAMAPI *pEloamTempl_AddRef)(HELOAMTEMPL templ);
typedef LONG (ELOAMAPI *pEloamTempl_Release)(HELOAMTEMPL templ);
typedef BSTR (ELOAMAPI *pEloamTempl_GetName)(HELOAMTEMPL templ);
typedef BSTR (ELOAMAPI *pEloamTempl_GetId)(HELOAMTEMPL templ);
typedef BOOL (ELOAMAPI *pEloamTempl_AppendField)(HELOAMTEMPL templ, BSTR fieldname, LONG type, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom);
typedef LONG (ELOAMAPI *pEloamTempl_GetFieldCount)(HELOAMTEMPL templ);
typedef BOOL (ELOAMAPI *pEloamTempl_ClearField)(HELOAMTEMPL templ);
typedef BSTR (ELOAMAPI *pEloamTempl_GetFieldName)(HELOAMTEMPL templ, LONG idx);
typedef LONG (ELOAMAPI *pEloamTempl_GetFieldType)(HELOAMTEMPL templ, LONG idx);
typedef FLOAT (ELOAMAPI *pEloamTempl_GetFieldRectLeft)(HELOAMTEMPL templ, LONG idx);
typedef FLOAT (ELOAMAPI *pEloamTempl_GetFieldRectTop)(HELOAMTEMPL templ, LONG idx);
typedef FLOAT (ELOAMAPI *pEloamTempl_GetFieldRectRight)(HELOAMTEMPL templ, LONG idx);
typedef FLOAT (ELOAMAPI *pEloamTempl_GetFieldRectBottom)(HELOAMTEMPL templ, LONG idx);
typedef BSTR (ELOAMAPI *pEloamTempl_GetFieldResult)(HELOAMTEMPL templ, LONG idx);
typedef BOOL (ELOAMAPI *pEloamTempl_SetFieldResult)(HELOAMTEMPL templ, LONG idx, BSTR result);
typedef BSTR (ELOAMAPI *pEloamTempl_GetData)(HELOAMTEMPL templ, LONG flag);
typedef BOOL (ELOAMAPI *pEloamTempl_Save)(HELOAMTEMPL templ, BSTR filename, LONG flag);
#endif