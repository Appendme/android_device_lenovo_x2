
#ifndef __EXPORT_h
#define __EXPORT_h

#ifdef __cplusplus
fd
extern "C" {
#endif

#pragma once

#ifdef _WIN32

#ifdef CDMAAPI_EXPORT
#define CDMA_API extern "C" _declspec(dllexport)
#else
#define CDMA_API extern "C" _declspec(dllimport)
#endif

#else

#ifdef CDMAAPI_EXPORT
ff
#define CDMA_API extern "C" 
#else
#define CDMA_API extern
#endif 

#endif

typedef enum
{
    CAVE_SUCESS = 0, //�ɹ�
    CAVE_NOUIM =1,	 //û��UIM
    CAVE_RUNFAIL=2, //����ʧ��
    CAVE_UNKNOW = 3  //δ֪����
}ECaveResult;

//UIM����Ȩ�㷨����
#define CT_AUTH_CAVE		1	//cave�㷨
#define CT_AUTH_MD5			1<<1//md5�㷨
#define CT_AUTH_AKACAVE		1<<2

//------------------------------------------------------------------------
//��������ȡUIM����IMSI��
//������szIMSI:���ص�IMSI��
//���أ��ɹ�����true,ʧ�ܷ���false
//------------------------------------------------------------------------
CDMA_API int ReadIMSI(char *** szIMSI);//ReadIMSI(BYTE szIMSI[32]);


//------------------------------------------------------------------------
//������UIM����Ȩ�㷨��ѯ
//��������
//���أ�0��ʾʲô�㷨����֧�ֻ������
//		1��ʾ֧��cave�㷨��
//		���������UIM����Ȩ�㷨����
//------------------------------------------------------------------------
CDMA_API unsigned int EnumAuthAlgs();


//------------------------------------------------------------------------
//������ִ��CAVE��Ȩ
//������pstrrandu ��ο�ͳһ��֤ATָ���˵��
//		pstrathoru
//���أ������ECaveResult����
//------------------------------------------------------------------------
CDMA_API ECaveResult RequestCave(const char *pstrrandu, char*** pstrathoru);

//------------------------------------------------------------------------
//������ִ��CAVE��Ȩ
//������pcStrChapid: ��ο�ͳһ��֤ATָ���˵��
//		pcStrChapchallenge
//		pszResponse
//���أ�1�ɹ�������ֵʧ��
//------------------------------------------------------------------------
CDMA_API int MakeMD5(const char *pcStrChapid, const char *pcStrChapchallenge, char ***pszResponse);


//------------------------------------------------------------------------
//���������ݿ���֧�ֵ�ģʽ��ѯ
//��������
//���أ�-1��ȡʧ��
//		0:cdmaģʽ
//		1:hdrģʽ
//		2:���ģʽ
//------------------------------------------------------------------------
CDMA_API int ListCardType();

//------------------------------------------------------------------------
//��������ȡ���ݿ�/ģ���ESN��UIMID
//������pszMeid: ��ο�ͳһ��֤ATָ���˵��
//		pszUimid
//���أ�0�ɹ�������ֵʧ��
//------------------------------------------------------------------------
CDMA_API int GetUimid(char ***pszMeid, char ***pszUimid);


//------------------------------------------------------------------------
//������Generate key/VPM��ѯ
//������pcStrRandu: ��ο�ͳһ��֤ATָ���˵��
//		pcStrAuthu
//		pszSmekey
//		pszVpm
//���أ�1�ɹ�������ֵʧ��
//------------------------------------------------------------------------
CDMA_API int GetGeneratekey(char ***pszSmekey, char ***pszVpm);


//------------------------------------------------------------------------
//������SSD����ȷ��
//���أ�0�ɹ�������ֵʧ��
//------------------------------------------------------------------------
CDMA_API int SSDConfirm(const char *pcStrAuthbs);


//------------------------------------------------------------------------
//������ SSD����
//���أ�0�ɹ�������ֵʧ��
//------------------------------------------------------------------------
CDMA_API int SSDUpdate(const char * pcStrRandssd, char ***pszRandbs);

#ifdef __cplusplus
}
#endif

#endif /* _EXPORT_H */
