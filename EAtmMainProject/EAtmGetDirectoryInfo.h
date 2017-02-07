#ifndef __EAtmGetDirectoryInfo_h__
#define __EAtmGetDirectoryInfo_h__

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif


#define  EAtm_FILE_PATH_LEN  256


#define MAXSIZE 1000//���Ƕ���������Եķ�֧��

typedef struct _TREENODE
{
    char  cData[EAtm_FILE_PATH_LEN];
    int cnChild;
    struct _TREENODE *parent;
    struct _TREENODE *child[MAXSIZE];
} TREENODE, *PTREENODE;


//�ļ����µ���Ϣ
typedef struct ST_DIRECTOR_INFO_s
{
    char **cppFile;          //�ļ����е��ļ���
    int  iFileCount;         //�ļ����е��ļ�������

    char **cppDirectory;     //�ļ����е��ļ�����
    int iDirectotyCount;     //�ļ����е��ļ��и���
}stDirectoryInfo_t,*stDirectoryInfo_p_t;

//�ж��Ƿ����ļ���
BOOL EAtmCheckIsDirectory(char cCheckDir[EAtm_FILE_PATH_LEN]);

//��ȡ��ǰ�ļ����������ļ��к��ļ������������ļ����е��ļ��к��ļ���
int EAtmGetAllInfoInRoot(char *cpFilePath,stDirectoryInfo_t *stDirectortInfo);

//��ȡ��ǰ�ļ����������ļ��У��������ļ����е��ļ��У�
int EAtmGetAllInfo(char *cpFilePath,stDirectoryInfo_t *stDirectoryInfo);

//��ȡ�ļ���������ָ����ʽ���ļ�
int EAtmGetFileOfSpecifyFormat(char *cpFilePath,char *cpExt,char ***cpppFilePath,int *npFileCount);

//��ȡ�ļ���������ָ����ʽ���ļ������������ļ����е��ļ���
int EAtmGetFileOfSpecifyFormatX(char *cpFilePath,char *cpExt,char ***cpppFilePath,int *npFileCount);

int CommonReallocCharPointerToPointer(char ***cpppStr,int nOriginCount,int nCount,int nStrLen);
int CommonSplitPath(char *cpFilePath,char cPath[_MAX_PATH],char cName[_MAX_FNAME],char cExt[_MAX_EXT]);

#ifdef __cplusplus
}
#endif
#endif  //__EAtmGetDirectoryInfo_h__