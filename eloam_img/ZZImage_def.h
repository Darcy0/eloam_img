#ifndef __ZZUTILITY_DEF_H__
#define __ZZUTILITY_DEF_H__

//��ƫ�ñ߳�ʼ��
typedef ZZResult (ZZAPI *pZZInitSdk)(CONST WCHAR *pszSN);
//����ͼƬ
typedef ZZResult (ZZAPI *pZZLoadImage)(__in CONST WCHAR *pszFileName, __in ULONG nType, __out HIMAGE *phImage);
//��ȡ������������
typedef ZZResult (ZZAPI *pZZGetImageObjectRegionPos)(__in HIMAGE hImage, __out ZZIMAGEREGIONPOS *pPos);
//������������Ϣ��ȡ������Ϣ
typedef ZZResult (ZZAPI *pZZGetImageRegionInfo)(__in CONST ZZIMAGEREGIONPOS *pPos, __out ZZIMAGEREGIONINFO *pInfo);
//������ƫ�ñ�ͼ��
typedef ZZResult (ZZAPI *pZZCreateImage)(__in ULONG nWidth, __in ULONG nHeight, __in ULONG nType, __out HIMAGE *phImage);
//��������
typedef ZZResult (ZZAPI *pZZSplitImageRegion)(__in HIMAGE hImage, __in HIMAGE hDestImage, __in CONST ZZIMAGEREGIONPOS *pPos);
//����ͼƬ
typedef ZZResult (ZZAPI *pZZSaveImage)(__in HIMAGE hImage, __in CONST WCHAR *pszFileName, __in ULONG nQuality);
//�ͷ�ͼƬ
typedef ZZResult (ZZAPI *pZZDestroyImage)(__in HIMAGE hImage);
//��ƫ�ñ߷���ʼ��
typedef ZZResult (ZZAPI *pZZDeinitSdk)(VOID);
#endif /* __ZZUTILITY_H__ */