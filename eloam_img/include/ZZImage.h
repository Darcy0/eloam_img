#ifndef __ZZIMAGE_H__
#define __ZZIMAGE_H__

#include "ZZUtility.h"

/* image format */
#define ZZ_IMG_FORMAT_BMP	1L
#define ZZ_IMG_FORMAT_JPEG	2L
#define ZZ_IMG_FORMAT_PNG	3L
#define ZZ_IMG_FORMAT_TIFF	4L
#define ZZ_IMG_FORMAT_GIF	5L

/* barcode type */
#define ZZ_BAR_TYPE_NONE		0L
#define ZZ_BAR_TYPE_PARTIAL		1L
#define ZZ_BAR_TYPE_EAN2		2L
#define ZZ_BAR_TYPE_EAN5		5L
#define ZZ_BAR_TYPE_EAN8		8L
#define ZZ_BAR_TYPE_UPCE		9L
#define ZZ_BAR_TYPE_ISBN10		10L
#define ZZ_BAR_TYPE_UPCA		12L
#define ZZ_BAR_TYPE_EAN13		13L
#define ZZ_BAR_TYPE_ISBN13		14L
#define ZZ_BAR_TYPE_COMPOSITE	15L
#define ZZ_BAR_TYPE_I25			25L
#define ZZ_BAR_TYPE_DATABAR		34L
#define ZZ_BAR_TYPE_DATABAR_EXP	35L
#define ZZ_BAR_TYPE_CODABAR		38L
#define ZZ_BAR_TYPE_CODE39		39L
#define ZZ_BAR_TYPE_PDF417		57L
#define ZZ_BAR_TYPE_QRCODE		64L
#define ZZ_BAR_TYPE_CODE93		93L
#define ZZ_BAR_TYPE_CODE128		128L

/* auth face flag */
#define ZZ_SDK_AUTH_FACE	0x00000002L
#define ZZ_SDK_AUTH_LIVE	0x00000004L
/* face status */
#define ZZ_FAC_STATUS_UNKNOWN	0L
#define ZZ_FAC_STATUS_FAKE		1L
#define ZZ_FAC_STATUS_LIVE		2L

/* recognition type */
#define ZZ_RCG_TYPE_NONE		0L
#define ZZ_RCG_TYPE_IDCARD		1L

/* image region pos */
typedef struct tagZZIMAGEREGIONPOS
{
	POINT pt1;
	POINT pt2;
	POINT pt3;
	POINT pt4;
} ZZIMAGEREGIONPOS, *LPZZIMAGEREGIONPOS;

/* image region info */
typedef struct tagZZIMAGEREGIONINFO
{
	ULONG nWidth;
	ULONG nHeight;
	DOUBLE dAngle;
	POINT ptOrigin;
	POINT ptCenter;
} ZZIMAGEREGIONINFO, *LPZZIMAGEREGIONINFO;

/* face region pos */
typedef struct tagZZFACEREGIONPOS
{
	RECT rcPos;
	LONG nYaw;
	LONG nPitch;
	LONG nRoll;
} ZZFACEREGIONPOS, *LPZZFACEREGIONPOS;

DECLARE_HANDLE(HIMAGEREADER);
DECLARE_HANDLE(HIMAGEWRITER);
DECLARE_HANDLE(HBARCODEMGR);
DECLARE_HANDLE(HBARCODEINFO);
DECLARE_HANDLE(HOCRMGR);
DECLARE_HANDLE(HOCRINFO);
DECLARE_HANDLE(HFACEMGR);
DECLARE_HANDLE(HFPMGR);
DECLARE_HANDLE(HSTITCHERMGR);
DECLARE_HANDLE(HRECOGNITIONMGR);
DECLARE_HANDLE(HRECOGNITIONINFO);

ZZSTDAPI(ZZResult) ZZLoadImage(__in CONST WCHAR *pszFileName, __in ULONG nType, __out HIMAGE *phImage);
ZZSTDAPI(ZZResult) ZZLoadImage2(__in CONST WCHAR *pszFileName, __out HIMAGE2 *phImage);
ZZSTDAPI(ZZResult) ZZLoadImageFromStream(__in HSTREAM hStream, __in ULONG nType, __out HIMAGE *phImage);
ZZSTDAPI(ZZResult) ZZLoadImage2FromStream(__in HSTREAM hStream, __out HIMAGE2 *phImage);
ZZSTDAPI(ZZResult) ZZLoadImage2FromResource(__in HINSTANCE hInst, __in UINT nResID, __in LPCWSTR pszType, __out HIMAGE2 *phImage);
ZZSTDAPI(ZZResult) ZZOpenImageReader(__in CONST WCHAR *pszFileName, __out HIMAGEREADER *phImageReader);
ZZSTDAPI(ZZResult) ZZOpenImageReaderWithStream(__in HSTREAM hStream, __out HIMAGEREADER *phImageReader);
ZZSTDAPI(ZZResult) ZZCloseImageReader(__in HIMAGEREADER hImageReader);
ZZSTDAPI(ZZResult) ZZGetImageReaderPageCount(__in HIMAGEREADER hImageReader, __out ULONG *pnCount);
ZZSTDAPI(ZZResult) ZZLoadImageFromImageReader(__in HIMAGEREADER hImageReader, __in ULONG nIndex, __in ULONG nType, __out HIMAGE *phImage);

ZZSTDAPI(ZZResult) ZZSaveImage(__in HIMAGE hImage, __in CONST WCHAR *pszFileName, __in ULONG nQuality);
ZZSTDAPI(ZZResult) ZZSaveImageToStream(__in HIMAGE hImage, __in HSTREAM hStream, __in ULONG nFormat, __in ULONG nQuality);
ZZSTDAPI(ZZResult) ZZCreateImageWriter(__in CONST WCHAR *pszFileName, __in HIMAGE hImage, __in ULONG nQuality, __out HIMAGEWRITER *phImageWriter);
ZZSTDAPI(ZZResult) ZZCreateImageWriterWithStream(__in HSTREAM hStream, __in ULONG nFormat, __in HIMAGE hImage, __in ULONG nQuality, __out HIMAGEWRITER *phImageWriter);
ZZSTDAPI(ZZResult) ZZDestroyImageWriter(__in HIMAGEWRITER hImageWriter);
ZZSTDAPI(ZZResult) ZZSaveImageToImageWriter(__in HIMAGEWRITER hImageWriter, __in HIMAGE hImage, __in ULONG nQuality);

ZZSTDAPI(ZZResult) ZZCopyImage(__in HIMAGE hImage, __in HIMAGE hDestImage);
ZZSTDAPI(ZZResult) ZZDrawImageDisplayer(__in HIMAGE hImage, __in CONST RECT *pROI);
ZZSTDAPI(ZZResult) ZZDrawImageString(__in HIMAGE hImage, __in LONG nLeft, __in LONG nTop, __in CONST WCHAR *pszString, __in HFONT hFont, __in COLORREF nColor, __in BYTE nOpacity);
ZZSTDAPI(ZZResult) ZZBlendImage(__in HIMAGE hImage, __in HIMAGE hDestImage, __in BYTE nOpacity);
ZZSTDAPI(ZZResult) ZZResizeImage(__in HIMAGE hImage, __in HIMAGE hDestImage);
ZZSTDAPI(ZZResult) ZZReverseImage(__in HIMAGE hImage, __in HIMAGE hDestImage);
ZZSTDAPI(ZZResult) ZZRotateImageMirror(__in HIMAGE hImage, __in HIMAGE hDestImage);
ZZSTDAPI(ZZResult) ZZRotateImageFlip(__in HIMAGE hImage, __in HIMAGE hDestImage);
ZZSTDAPI(ZZResult) ZZRotateImageLeft(__in HIMAGE hImage, __in HIMAGE hDestImage);
ZZSTDAPI(ZZResult) ZZRotateImageRight(__in HIMAGE hImage, __in HIMAGE hDestImage);
ZZSTDAPI(ZZResult) ZZRotateImageLeftMirror(__in HIMAGE hImage, __in HIMAGE hDestImage);
ZZSTDAPI(ZZResult) ZZRotateImageRightMirror(__in HIMAGE hImage, __in HIMAGE hDestImage);
ZZSTDAPI(ZZResult) ZZRotateImage180(__in HIMAGE hImage, __in HIMAGE hDestImage);
ZZSTDAPI(ZZResult) ZZGrayScaleImage(__in HIMAGE hImage, __in HIMAGE hDestImage);
ZZSTDAPI(ZZResult) ZZBinarizationImage(__in HIMAGE hImage, __in HIMAGE hDestImage);
ZZSTDAPI(ZZResult) ZZClearImageBkgnd(__in HIMAGE hImage, __in HIMAGE hDestImage);
ZZSTDAPI(ZZResult) ZZDenoisingImage(__in HIMAGE hImage, __in HIMAGE hDestImage);
ZZSTDAPI(ZZResult) ZZGetImageRegionInfo(__in CONST ZZIMAGEREGIONPOS *pPos, __out ZZIMAGEREGIONINFO *pInfo);
ZZSTDAPI(ZZResult) ZZSplitImageRegion(__in HIMAGE hImage, __in HIMAGE hDestImage, __in CONST ZZIMAGEREGIONPOS *pPos);
ZZSTDAPI(ZZResult) ZZGetImageDiversity(__in HIMAGE hImage, __in HIMAGE hImage2, __out DOUBLE *pResult);
ZZSTDAPI(ZZResult) ZZGetImageObjectRegionPos(__in HIMAGE hImage, __out ZZIMAGEREGIONPOS *pPos);
ZZSTDAPI(ZZResult) ZZGetImageMultiObjectRegionPos(__in HIMAGE hImage, __out_ecount(ZZ_MAX_COUNT) ZZIMAGEREGIONPOS *pPos, __out ULONG *pnCount);

ZZSTDAPI(ZZResult) ZZCreateBarcodeMgr(__in CONST WCHAR *pszSN, __out HBARCODEMGR *phBarcodeMgr);
ZZSTDAPI(ZZResult) ZZDestroyBarcodeMgr(__in HBARCODEMGR hBarcodeMgr);
ZZSTDAPI(ZZResult) ZZCreateBarcodeInfo(__in HBARCODEMGR hBarcodeMgr, __in HIMAGE hImage, __out HBARCODEINFO *phBarcodeInfo);
ZZSTDAPI(ZZResult) ZZDestroyBarcodeInfo(__in HBARCODEINFO hBarcodeInfo);
ZZSTDAPI(ZZResult) ZZGetBarcodeCount(__in HBARCODEINFO hBarcodeInfo, __out ULONG *pnCount);
ZZSTDAPI(ZZResult) ZZGetBarcodeType(__in HBARCODEINFO hBarcodeInfo, __in ULONG nIndex, __out ULONG *pnType);
ZZSTDAPI(ZZResult) ZZGetBarcodeText(__in HBARCODEINFO hBarcodeInfo, __in ULONG nIndex, __out CONST WCHAR **ppszText);
ZZSTDAPI(ZZResult) ZZGetBarcodeRegionPos(__in HBARCODEINFO hBarcodeInfo, __in ULONG nIndex, __out CONST ZZIMAGEREGIONPOS **ppPos);

ZZSTDAPI(ZZResult) ZZCreateOcrMgr(__in CONST WCHAR *pszSN, __out HOCRMGR *phOcrMgr);
ZZSTDAPI(ZZResult) ZZDestroyOcrMgr(__in HOCRMGR hOcrMgr);
ZZSTDAPI(ZZResult) ZZCreateOcrInfo(__in HOCRMGR hOcrMgr, __in HIMAGE hImage, __out HOCRINFO *phOcrInfo);
ZZSTDAPI(ZZResult) ZZDestroyOcrInfo(__in HOCRINFO hOcrInfo);
ZZSTDAPI(ZZResult) ZZGetOcrPlainText(__in HOCRINFO hOcrInfo, __out CONST WCHAR **ppszText);
ZZSTDAPI(ZZResult) ZZGetOcrCount(__in HOCRINFO hOcrInfo, __out ULONG *pnCount);
ZZSTDAPI(ZZResult) ZZGetOcrText(__in HOCRINFO hOcrInfo, __in ULONG nIndex, __out CONST WCHAR **ppszText);
ZZSTDAPI(ZZResult) ZZGetOcrRegionPos(__in HOCRINFO hOcrInfo, __in ULONG nIndex, __out CONST ZZIMAGEREGIONPOS **ppPos);

ZZSTDAPI(ZZResult) ZZCreateFaceMgr(__in CONST WCHAR *pszSN, __out HFACEMGR *phFaceMgr);
ZZSTDAPI(ZZResult) ZZDestroyFaceMgr(__in HFACEMGR hFaceMgr);
ZZSTDAPI(ZZResult) ZZDetectFace(__in HFACEMGR hFaceMgr, __in HIMAGE hImage, __in HIMAGE hAnciImage, __out ZZFACEREGIONPOS *pPos,
	__out HBUFFER *phFaceFeature, __out ULONG *pnStatus);
ZZSTDAPI(ZZResult) ZZDetectMultiFace(__in HFACEMGR hFaceMgr, __in HIMAGE hImage, __in HIMAGE hAnciImage, __out_ecount(ZZ_MAX_COUNT) ZZFACEREGIONPOS *pPos,
	__out_ecount(ZZ_MAX_COUNT) HBUFFER *phFaceFeature, __out_ecount(ZZ_MAX_COUNT) ULONG *pnStatus, __out ULONG *pnCount);
ZZSTDAPI(ZZResult) ZZGetFaceFeatureSimilarity(__in HFACEMGR hFaceMgr, __in HBUFFER hFaceFeature1, __in HBUFFER hFaceFeature2, __out DOUBLE *pResult);

ZZSTDAPI(ZZResult) ZZCreateFpMgr(__in CONST WCHAR *pszSN, __out HFPMGR *phFpMgr);
ZZSTDAPI(ZZResult) ZZDestroyFpMgr(__in HFPMGR hFpMgr);
ZZSTDAPI(ZZResult) ZZDetectFp(__in HFPMGR hFpMgr, __in HIMAGE hImage, __out ULONG *pnQuality, __out HBUFFER *phFpFeature);
ZZSTDAPI(ZZResult) ZZCompareFpFeature(__in HFPMGR hFpMgr, __in HBUFFER hFpFeature1, __in HBUFFER hFpFeature2, __in DOUBLE dThreshold, __out BOOL *pbPass);
ZZSTDAPI(ZZResult) ZZGetFpFeatureSimilarity(__in HFPMGR hFpMgr, __in HBUFFER hFpFeature1, __in HBUFFER hFpFeature2, __out DOUBLE *pResult);

ZZSTDAPI(ZZResult) ZZCreateStitcherMgr(__in CONST WCHAR *pszSN, __out HSTITCHERMGR *phStitcherMgr);
ZZSTDAPI(ZZResult) ZZDestroyStitcherMgr(__in HSTITCHERMGR hStitcherMgr);
ZZSTDAPI(ZZResult) ZZCreateImageFromStitcher(__in HSTITCHERMGR hStitcherMgr, __in CONST HIMAGE *phInImage, __in ULONG nCount, __out HIMAGE *phOutImage);

ZZSTDAPI(ZZResult) ZZCreateRecognitionMgr(__in ULONG nType, __in CONST WCHAR *pszSN, __out HRECOGNITIONMGR *phRecoMgr);
ZZSTDAPI(ZZResult) ZZDestroyRecognitionMgr(__in HRECOGNITIONMGR hRecoMgr);
ZZSTDAPI(ZZResult) ZZCreateRecognitionInfo(__in HRECOGNITIONMGR hRecoMgr, __in HIMAGE hImage, __out HRECOGNITIONINFO *phRecoInfo);
ZZSTDAPI(ZZResult) ZZDestroyRecognitionInfo(__in HRECOGNITIONINFO hRecoInfo);
ZZSTDAPI(ZZResult) ZZGetRecognitionCount(__in HRECOGNITIONINFO hRecoInfo, __out ULONG *pnCount);
ZZSTDAPI(ZZResult) ZZGetRecognitionTag(__in HRECOGNITIONINFO hRecoInfo, __in ULONG nIndex, __out CONST WCHAR **ppszTag);
ZZSTDAPI(ZZResult) ZZGetRecognitionText(__in HRECOGNITIONINFO hRecoInfo, __in ULONG nIndex, __out CONST WCHAR **ppszText);

#endif /* __ZZIMAGE_H__ */