#ifndef __ZZUTILITY_H__
#define __ZZUTILITY_H__

#include <WTypes.h>
#include <Windows.h>
#include <ObjIdl.h>

/* return value */
typedef ULONG ZZResult;
/* error code */
#define ZZ_ERR_SUCCESS				0L
#define ZZ_ERR_FAILED				1L
#define ZZ_ERR_NOT_IMPLEMENTED		2L
#define ZZ_ERR_INVALID_PARAMETER	3L
#define ZZ_ERR_INVALID_HANDLE		4L
#define ZZ_ERR_INVALID_DATA			5L
#define ZZ_ERR_INVALID_THREAD		6L
#define ZZ_ERR_MEMORY_NOT_ENOUGH	7L
#define ZZ_ERR_BUFFER_NOT_ENOUGH	8L
#define ZZ_ERR_RESOURCES_NOT_OPEN	9L
#define ZZ_ERR_RESOURCES_NOT_CLOSED	10L
#define ZZ_ERR_ELEMENT_NOT_FOUND	11L
#define ZZ_ERR_FILE_NOT_FOUND		12L
#define ZZ_ERR_DEVICE_IO			13L
#define ZZ_ERR_BUSY					14L
#define ZZ_ERR_ACCESS_DENIED		15L
#define ZZ_ERR_IMAGE_PROCESS		16L
#define ZZ_ERR_DISABLED				17L
#define ZZ_ERR_LOAD_LIBRARY			18L
#define ZZ_ERR_AUTHORIZATION		19L
#define ZZ_ERR_CANCELLED			20L

#define ZZAPI __stdcall
#define ZZAPIV __cdecl
#define ZZSTDAPI(type) EXTERN_C type ZZAPI
#define ZZSTDAPIV(type) EXTERN_C type ZZAPIV

#define ZZ_MAX_LEN		512L
#define ZZ_MAX_COUNT	64L

/* auth base flag */
#define ZZ_SDK_AUTH_BASE    0x00000001L
/* sdk msgname */
#define ZZ_SDK_MSGNAME_FATAL	L"Fatal"
#define ZZ_SDK_MSGNAME_ERROR	L"Error"
#define ZZ_SDK_MSGNAME_WARNING	L"Warning"
#define ZZ_SDK_MSGNAME_DESC		L"Description"

/* image type */
#define ZZ_IMG_TYPE_NULL	0L
#define ZZ_IMG_TYPE_COLOR	1L
#define ZZ_IMG_TYPE_GRAY	2L
/* image origin */
#define ZZ_IMG_ORIGIN_TOP	1L
#define ZZ_IMG_ORIGIN_DOWN	2L

/* sdk config */
typedef struct tagZZSDKCFGINFO
{
	ULONG nTrialDays;
	ULONG nAuthFlags;
} ZZSDKCFGINFO, *LPZZSDKCFGINFO;

DECLARE_HANDLE(HBUFFER);
DECLARE_HANDLE(HSTREAM);
DECLARE_HANDLE(HIMAGE);
DECLARE_HANDLE(HIMAGE2);
DECLARE_HANDLE(HVOICE);
DECLARE_HANDLE(HSHOW);
DECLARE_HANDLE(HPRINTER);
DECLARE_HANDLE(HSPEAKER);

ZZSTDAPI(ZZResult) ZZInitSdk(__in CONST WCHAR *pszSN);
ZZSTDAPI(ZZResult) ZZDeinitSdk(VOID);
ZZSTDAPI(ZZResult) ZZGetSdkCfgInfo(__out ZZSDKCFGINFO *pInfo);
ZZSTDAPIV(ZZResult) ZZWriteSdkMsg(__in CONST WCHAR *pszName, __in CONST WCHAR *pszFormat, ...);

ZZSTDAPI(ZZResult) ZZGetGuidString(__out_ecount(ZZ_MAX_LEN) WCHAR *pszString, __out ULONG *pnLen);
ZZSTDAPI(ZZResult) ZZGetTempName(__out_ecount(ZZ_MAX_LEN) WCHAR *pszName, __out ULONG *pnLen);
ZZSTDAPI(ZZResult) ZZGetDisplayerSize(__out ULONG *pnWidth, __out ULONG *pnHeight);
ZZSTDAPI(ZZResult) ZZCreateDirectory(__in CONST WCHAR *pszPath);
ZZSTDAPI(ZZResult) ZZGetStringLocation(__in LONG nLeft, __in LONG nTop, __in CONST WCHAR *pszString, __in HFONT hFont,
	__out LONG *pnX, __out LONG *pnY, __out ULONG *pnWidth, __out ULONG *pnHeight);

ZZSTDAPI(ZZResult) ZZBase64Encode(__in CONST BYTE *pData, __in ULONG nDataSize, __out BYTE *pBase64, __out ULONG *pnBase64Size);
ZZSTDAPI(ZZResult) ZZBase64Decode(__in CONST BYTE *pBase64, __in ULONG nBase64Size, __out BYTE *pData, __out ULONG *pnDataSize);
ZZSTDAPI(ZZResult) ZZDesEncrypt(__inout BYTE *pData, __in ULONG nSize, __in_ecount(8) CONST BYTE *pKey);
ZZSTDAPI(ZZResult) ZZDesDecrypt(__inout BYTE *pData, __in ULONG nSize, __in_ecount(8) CONST BYTE *pKey);
ZZSTDAPI(ZZResult) ZZGenerateMd5(__in CONST BYTE *pData, __in ULONG nSize, __out_ecount(16) BYTE *pMd5);

ZZSTDAPI(ZZResult) ZZCreateBuffer(__in ULONG nSize, __out HBUFFER *phBuffer);
ZZSTDAPI(ZZResult) ZZCreateBufferWithRawData(__in VOID *pBits, __in ULONG nSize, __out HBUFFER *phBuffer);
ZZSTDAPI(ZZResult) ZZCloneBuffer(__in HBUFFER hSrcBuffer, __out HBUFFER *phBuffer);
ZZSTDAPI(ZZResult) ZZDestroyBuffer(__in HBUFFER hBuffer);
ZZSTDAPI(ZZResult) ZZGetBufferBits(__in HBUFFER hBuffer, __out VOID **ppBits);
ZZSTDAPI(ZZResult) ZZGetBufferSize(__in HBUFFER hBuffer, __out ULONG *pnSize);

ZZSTDAPI(ZZResult) ZZCreateStream(__out HSTREAM *phStream);
ZZSTDAPI(ZZResult) ZZCloneStream(__in HSTREAM hSrcStream, __out HSTREAM *phStream);
ZZSTDAPI(ZZResult) ZZDestroyStream(__in HSTREAM hStream);
ZZSTDAPI(ZZResult) ZZGetIStreamOfStream(__in HSTREAM hStream, __out IStream **ppStream);
ZZSTDAPI(ZZResult) ZZSetStreamSize(__in HSTREAM hStream, __in ULONGLONG nSize);
ZZSTDAPI(ZZResult) ZZGetStreamSize(__in HSTREAM hStream, __out ULONGLONG *pnSize);
ZZSTDAPI(ZZResult) ZZSetStreamPointer(__in HSTREAM hStream, __in ULONGLONG nPointer);
ZZSTDAPI(ZZResult) ZZGetStreamPointer(__in HSTREAM hStream, __out ULONGLONG *pnPointer);
ZZSTDAPI(ZZResult) ZZWriteStream(__in HSTREAM hStream, __in CONST VOID *pBits, __in ULONG nSize, __out ULONG *pnWriteSize);
ZZSTDAPI(ZZResult) ZZReadStream(__in HSTREAM hStream, __in VOID *pBits, __in ULONG nSize, __out ULONG *pnReadSize);

ZZSTDAPI(ZZResult) ZZCreateImage(__in ULONG nWidth, __in ULONG nHeight, __in ULONG nType, __out HIMAGE *phImage);
ZZSTDAPI(ZZResult) ZZCreateImageWithRawData(__in VOID *pBits, __in ULONG nWidth, __in ULONG nHeight, __in ULONG nWidthStep, __in ULONG nType, __out HIMAGE *phImage);
ZZSTDAPI(ZZResult) ZZCreateImageFromRawData(__in CONST VOID *pBits, __in ULONG nWidth, __in ULONG nHeight, __in ULONG nWidthStep, __in ULONG nType, __in ULONG nOrigin, __out HIMAGE *phImage);
ZZSTDAPI(ZZResult) ZZCloneImage(__in HIMAGE hSrcImage, __out HIMAGE *phImage);
ZZSTDAPI(ZZResult) ZZCloneImageEx(__in HIMAGE hSrcImage, __out HIMAGE *phImage);
ZZSTDAPI(ZZResult) ZZDestroyImage(__in HIMAGE hImage);
ZZSTDAPI(ZZResult) ZZGetHBITMAPOfImage(__in HIMAGE hImage, __out HBITMAP *phBitmap);
ZZSTDAPI(ZZResult) ZZDetachHBITMAPOfImage(__in HIMAGE hImage, __out HBITMAP *phBitmap);
ZZSTDAPI(ZZResult) ZZGetImageBits(__in HIMAGE hImage, __out VOID **ppBits);
ZZSTDAPI(ZZResult) ZZGetImageWidth(__in HIMAGE hImage, __out ULONG *pnWidth);
ZZSTDAPI(ZZResult) ZZGetImageHeight(__in HIMAGE hImage, __out ULONG *pnHeight);
ZZSTDAPI(ZZResult) ZZGetImageWidthStep(__in HIMAGE hImage, __out ULONG *pnWidthStep);
ZZSTDAPI(ZZResult) ZZGetImageType(__in HIMAGE hImage, __out ULONG *pnType);
ZZSTDAPI(ZZResult) ZZSetImageXDPI(__in HIMAGE hImage, __in ULONG nXDPI);
ZZSTDAPI(ZZResult) ZZSetImageYDPI(__in HIMAGE hImage, __in ULONG nYDPI);
ZZSTDAPI(ZZResult) ZZGetImageXDPI(__in HIMAGE hImage, __out ULONG *pnXDPI);
ZZSTDAPI(ZZResult) ZZGetImageYDPI(__in HIMAGE hImage, __out ULONG *pnYDPI);
ZZSTDAPI(ZZResult) ZZSetImageROI(__in HIMAGE hImage, __in CONST RECT *pROI);
ZZSTDAPI(ZZResult) ZZResetImageROI(__in HIMAGE hImage);
ZZSTDAPI(ZZResult) ZZGetImageROI(__in HIMAGE hImage, __out RECT *pROI);

ZZSTDAPI(ZZResult) ZZCreateImage2(__in ULONG nWidth, __in ULONG nHeight, __out HIMAGE2 *phImage);
ZZSTDAPI(ZZResult) ZZCreateImage2WithRawData(__in VOID *pBits, __in ULONG nWidth, __in ULONG nHeight, __in ULONG nWidthStep, __out HIMAGE2 *phImage);
ZZSTDAPI(ZZResult) ZZCreateImage2FromRawData(__in CONST VOID *pBits, __in ULONG nWidth, __in ULONG nHeight, __in ULONG nWidthStep, __in ULONG nOrigin, __out HIMAGE2 *phImage);
ZZSTDAPI(ZZResult) ZZCloneImage2(__in HIMAGE2 hSrcImage, __out HIMAGE2 *phImage);
ZZSTDAPI(ZZResult) ZZCloneImage2Ex(__in HIMAGE2 hSrcImage, __out HIMAGE2 *phImage);
ZZSTDAPI(ZZResult) ZZDestroyImage2(__in HIMAGE2 hImage);
ZZSTDAPI(ZZResult) ZZGetHBITMAPOfImage2(__in HIMAGE2 hImage, __out HBITMAP *phBitmap);
ZZSTDAPI(ZZResult) ZZDetachHBITMAPOfImage2(__in HIMAGE2 hImage, __out HBITMAP *phBitmap);
ZZSTDAPI(ZZResult) ZZGetImage2Bits(__in HIMAGE2 hImage, __out VOID **ppBits);
ZZSTDAPI(ZZResult) ZZGetImage2Width(__in HIMAGE2 hImage, __out ULONG *pnWidth);
ZZSTDAPI(ZZResult) ZZGetImage2Height(__in HIMAGE2 hImage, __out ULONG *pnHeight);
ZZSTDAPI(ZZResult) ZZGetImage2WidthStep(__in HIMAGE2 hImage, __out ULONG *pnWidthStep);
ZZSTDAPI(ZZResult) ZZSetImage2XDPI(__in HIMAGE2 hImage, __in ULONG nXDPI);
ZZSTDAPI(ZZResult) ZZSetImage2YDPI(__in HIMAGE2 hImage, __in ULONG nYDPI);
ZZSTDAPI(ZZResult) ZZGetImage2XDPI(__in HIMAGE2 hImage, __out ULONG *pnXDPI);
ZZSTDAPI(ZZResult) ZZGetImage2YDPI(__in HIMAGE2 hImage, __out ULONG *pnYDPI);
ZZSTDAPI(ZZResult) ZZSetImage2ROI(__in HIMAGE2 hImage, __in CONST RECT *pROI);
ZZSTDAPI(ZZResult) ZZResetImage2ROI(__in HIMAGE2 hImage);
ZZSTDAPI(ZZResult) ZZGetImage2ROI(__in HIMAGE2 hImage, __out RECT *pROI);

ZZSTDAPI(ZZResult) ZZCreateVoice(__in ULONG nSamples, __in ULONG nChannels, __in ULONG nBitsPerSample, __out HVOICE *phVoice);
ZZSTDAPI(ZZResult) ZZCreateVoiceWithRawData(__in VOID *pBits, __in ULONG nSamples, __in ULONG nChannels, __in ULONG nBitsPerSample, __out HVOICE *phVoice);
ZZSTDAPI(ZZResult) ZZCloneVoice(__in HVOICE hSrcVoice, __out HVOICE *phVoice);
ZZSTDAPI(ZZResult) ZZDestroyVoice(__in HVOICE hVoice);
ZZSTDAPI(ZZResult) ZZGetVoiceBits(__in HVOICE hVoice, __out VOID **ppBits);
ZZSTDAPI(ZZResult) ZZGetVoiceSamples(__in HVOICE hVoice, __out ULONG *pnSamples);
ZZSTDAPI(ZZResult) ZZGetVoiceChannels(__in HVOICE hVoice, __out ULONG *pnChannels);
ZZSTDAPI(ZZResult) ZZGetVoiceBitsPerSample(__in HVOICE hVoice, __out ULONG *pnBitsPerSample);

ZZSTDAPI(ZZResult) ZZCreateShow(__in HWND hParent, __out HSHOW *phShow);
ZZSTDAPI(ZZResult) ZZDestroyShow(__in HSHOW hShow);
ZZSTDAPI(ZZResult) ZZSetShowVisible(__in HSHOW hShow, __in BOOL bVisible);
ZZSTDAPI(ZZResult) ZZSetShowPosition(__in HSHOW hShow, __in CONST RECT *pPos);
ZZSTDAPI(ZZResult) ZZSetShowBkColor(__in HSHOW hShow, __in COLORREF nColor);
ZZSTDAPI(ZZResult) ZZSetShowBkImage(__in HSHOW hShow, __in HIMAGE hImage);
ZZSTDAPI(ZZResult) ZZWriteImageToShow(__in HSHOW hShow, __in HIMAGE hImage);
ZZSTDAPI(ZZResult) ZZClearShowImage(__in HSHOW hShow);
ZZSTDAPI(ZZResult) ZZGetImageOfShow(__in HSHOW hShow, __out ULONG *pnWidth, __out ULONG *pnHeight, __out ULONG *pnType);
ZZSTDAPI(ZZResult) ZZCreateImageFromShow(__in HSHOW hShow, __out HIMAGE *phImage);

ZZSTDAPI(ZZResult) ZZOpenPrinter(__in HWND hOwner, __out HPRINTER *phPrinter);
ZZSTDAPI(ZZResult) ZZClosePrinter(__in HPRINTER hPrinter);
ZZSTDAPI(ZZResult) ZZWriteImageToPrinter(__in HPRINTER hPrinter, __in HIMAGE hImage);

ZZSTDAPI(ZZResult) ZZCreateSpeaker(__in ULONG nSampleRate, __in ULONG nChannels, __in ULONG nBitsPerSample, __out HSPEAKER *phSpeaker);
ZZSTDAPI(ZZResult) ZZDestroySpeaker(__in HSPEAKER hSpeaker);
ZZSTDAPI(ZZResult) ZZWriteVoiceToSpeaker(__in HSPEAKER hSpeaker, __in HVOICE hVoice);
ZZSTDAPI(ZZResult) ZZClearSpeakerVoice(__in HSPEAKER hSpeaker);

#endif /* __ZZUTILITY_H__ */