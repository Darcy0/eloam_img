#ifndef __ZZUTILITY_DEF_H__
#define __ZZUTILITY_DEF_H__

//纠偏裁边初始化
typedef ZZResult (ZZAPI *pZZInitSdk)(CONST WCHAR *pszSN);
//下载图片
typedef ZZResult (ZZAPI *pZZLoadImage)(__in CONST WCHAR *pszFileName, __in ULONG nType, __out HIMAGE *phImage);
//获取物体区域坐标
typedef ZZResult (ZZAPI *pZZGetImageObjectRegionPos)(__in HIMAGE hImage, __out ZZIMAGEREGIONPOS *pPos);
//从区域坐标信息提取区域信息
typedef ZZResult (ZZAPI *pZZGetImageRegionInfo)(__in CONST ZZIMAGEREGIONPOS *pPos, __out ZZIMAGEREGIONINFO *pInfo);
//创建纠偏裁边图像
typedef ZZResult (ZZAPI *pZZCreateImage)(__in ULONG nWidth, __in ULONG nHeight, __in ULONG nType, __out HIMAGE *phImage);
//分离物体
typedef ZZResult (ZZAPI *pZZSplitImageRegion)(__in HIMAGE hImage, __in HIMAGE hDestImage, __in CONST ZZIMAGEREGIONPOS *pPos);
//保存图片
typedef ZZResult (ZZAPI *pZZSaveImage)(__in HIMAGE hImage, __in CONST WCHAR *pszFileName, __in ULONG nQuality);
//释放图片
typedef ZZResult (ZZAPI *pZZDestroyImage)(__in HIMAGE hImage);
//纠偏裁边反初始化
typedef ZZResult (ZZAPI *pZZDeinitSdk)(VOID);
#endif /* __ZZUTILITY_H__ */