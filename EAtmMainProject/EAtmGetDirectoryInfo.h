#ifndef __EAtmGetDirectoryInfo_h__
#define __EAtmGetDirectoryInfo_h__

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif


#define  EAtm_FILE_PATH_LEN  256


#define MAXSIZE 1000//这是多叉树最多可以的分支数

typedef struct _TREENODE
{
    char  cData[EAtm_FILE_PATH_LEN];
    int cnChild;
    struct _TREENODE *parent;
    struct _TREENODE *child[MAXSIZE];
} TREENODE, *PTREENODE;


//文件夹下的信息
typedef struct ST_DIRECTOR_INFO_s
{
    char **cppFile;          //文件夹中的文件名
    int  iFileCount;         //文件夹中的文件名个数

    char **cppDirectory;     //文件夹中的文件夹名
    int iDirectotyCount;     //文件夹中的文件夹个数
}stDirectoryInfo_t,*stDirectoryInfo_p_t;

//判断是否是文件夹
BOOL EAtmCheckIsDirectory(char cCheckDir[EAtm_FILE_PATH_LEN]);

//获取当前文件夹下所有文件夹和文件（不包括子文件夹中的文件夹和文件）
int EAtmGetAllInfoInRoot(char *cpFilePath,stDirectoryInfo_t *stDirectortInfo);

//获取当前文件夹下所有文件夹（包括子文件夹中的文件夹）
int EAtmGetAllInfo(char *cpFilePath,stDirectoryInfo_t *stDirectoryInfo);

//获取文件夹下所有指定格式的文件
int EAtmGetFileOfSpecifyFormat(char *cpFilePath,char *cpExt,char ***cpppFilePath,int *npFileCount);

//获取文件夹下所有指定格式的文件（不包括子文件夹中的文件）
int EAtmGetFileOfSpecifyFormatX(char *cpFilePath,char *cpExt,char ***cpppFilePath,int *npFileCount);

int CommonReallocCharPointerToPointer(char ***cpppStr,int nOriginCount,int nCount,int nStrLen);
int CommonSplitPath(char *cpFilePath,char cPath[_MAX_PATH],char cName[_MAX_FNAME],char cExt[_MAX_EXT]);

#ifdef __cplusplus
}
#endif
#endif  //__EAtmGetDirectoryInfo_h__