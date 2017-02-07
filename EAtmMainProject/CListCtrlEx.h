#pragma once
#ifndef __CLISTCTRLEX__
#define __CLISTCTRLEX__

#include <afxext.h>
#include <afxwin.h> 
#include <afxcmn.h>
#include <string>
#include <vector>

#include "CChildPWnd.h"
using namespace  std;
//自定义消息;
#define WM_VALIDATE        WM_USER + 0x7FFD
#define WM_SET_ITEMS    WM_USER + 0x7FFC

//三种编辑方式;
#define CLISTCTRL_COMMON   0  //常规，不支持编辑;
#define CLISTCTRL_EDIT     1  //使用编辑框;
#define CLISTCTRL_COMBOX   2  //使用组合框;


#define COLOR_INVALID    0xffffffff
#define RC_ITEM_NONE        0x0000 // Void, indicates invalid items only
#define    RC_ITEM_ALL            0x0001 // All items regardless of states
#define    RC_ITEM_SELECTED    0x0002 // Selected items
#define    RC_ITEM_UNSELECTED    0x0004 // Unselected items
#define    RC_ITEM_CHECKED        0x0008 // Checked items
#define    RC_ITEM_UNCHECKED    0x0010 // Unchecked items
#define    RC_ITEM_FOCUSED        0x0020 // Focused item
#define    RC_ITEM_UNFOCUSED    0x0040 // Unfocused items
//组合框;
class CComboxExtend : public CComboBox
{
    DECLARE_DYNAMIC(CComboxExtend)
public:
    CComboxExtend();
    virtual ~CComboxExtend();
    //组合框添加下拉选项;
    int  AddItemStr(vector <CString> ListVector);
    //显示组合框;
    int  ShowOption(CString  stritem,CRect RcCtrl,int niRowitem,int niColumnitem);
    //完成编辑;
    int  FunishEdit(BOOL bEdit);

    //处理这部件是因为在UG使用分页夹创建的对话框，在非MFC下PreTranslateMessage不生效
    //按下ESC 由于截获不到该消息，WM_CLOSE直接发送到UG主窗口，使UG关闭所以采用键盘钩子来处理这一部分;
    void Unloadhook();
    BOOL installhook();
    static LRESULT KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam);
protected:
    CString  m_OrginString;
    int    m_iRowIndex;
    int    m_iColumnIndex;
    HHOOK  m_Hook;
protected:
    
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnCbnSelendok();
};


class CEditExtend : public CEdit
{
    DECLARE_DYNAMIC(CEditExtend)

public:
    CEditExtend();
    virtual ~CEditExtend();
    //显示编辑框;
    int   ShowEdit(CString strItem,CRect  rcCtrl,int niRowitem,int niColumnitem);
protected:
    DECLARE_MESSAGE_MAP()

    int    m_iRowIndex;
    int    m_iColumnIndex;
public:
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

typedef struct EditInfo_s 
{
    int    nEditType;
    vector <string>  option;
}EditInfo;

typedef void (*FunlbuttonDown)(); //列表单击;
typedef void (*FunlbuttonDdwon)();//列表双击;
typedef void (*FunFinishEdit)();  //完成编辑;

class CListCtrlEx : public CListCtrl
{
    DECLARE_DYNAMIC(CListCtrlEx)
public:
    CListCtrlEx();
    virtual ~CListCtrlEx();

    int  RegisterParent(HWND hParent);
    //查找要生成控件的父对象;
    int  SearchParentWindow(char *lpszTargeTitle,char *lpszTargetClass);
    //生成父窗口，主要是为了处理自绘消息;
    int  CreateParentWnd();
    //按指定的样式和ID生成控件;
    int  CreateControlEx(int nColtrolID,DWORD dwStyle,DWORD dwExStyle);
    //注册用于组合框编辑下拉选项;
    int  RegisterOptionList(vector <CString> OptionVector);
    int  RegisterOptionList(vector <string> OptionVector);
    //注册编辑方式和UG界面响应的回调;
    int  RegisterEditType(vector <EditInfo> EditVector,
                          FunlbuttonDown funlbdown=NULL,
                          FunlbuttonDdwon fundlbdown=NULL,
                          FunFinishEdit   funEditFunish=NULL);

    //取表头文字,目前只能从结构中获取;
    CString getColHeaderText(int nCol);

    //取当前列表的选中项;
    void GetSelectedItem(int *nItem,int *nSubItem)
    {*nItem=m_nItem;*nSubItem=m_nSubItem;}

    //设置行高，效果不好，还要在DRAMITEM中处理自绘消息太麻烦，所以行高直接采用添加
    //空白图片来处理，设置图片的Y像素更简单;
    void  SetRowHeight(int nHeight);
    //为UG提供消息响应;
    FunlbuttonDown  m_funlbdown;  //左键单击;
    FunlbuttonDdwon m_funldbdown; //左键双击;
    FunFinishEdit   m_EditFinish; //编辑完成;

    virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
    //************************************************************************
public:
    
protected:
    //**********************创建控件使用************************************;
    HWND  FindSubWindow(HWND hSearch,char *lpszTargeTitle,char *lpszTargetClass);
    HWND  m_hParent;   //父窗口句柄;
    int   m_ControlID;
    //************************************************************************
    int   EditSubItem(int nItem,int nSubItem);  //编辑框编辑;
    int   ComBoxSubItem(int nItem,int nSubItem);//组合框编辑;

    int   m_nSel;
    int   m_nItem;   //主项;
    int   m_nSubItem;//子项;
    CChildPWnd     m_pWndParent;//父窗口对象;

    CString        m_StrCurSelect;//当前选中子项的文本;
    CEditExtend    m_Edit_ctlItem;//关联的编辑框对象;
    
    CFont m_FontItem;
    int   m_nRowHeight;
    COLORREF m_colRow1;
    COLORREF m_colRow2;
    //组合框相关的信息;
    bool  m_bRegOption;
    bool  m_bRegEdit;

    CComboxExtend  m_ComBox_ctlItem; //关联的组合框对象;
    vector <CString>  m_OptionVector;//下拉列表的数据;
    vector <EditInfo> m_EdityType;   //各列对应的编辑方式;
protected:
//    virtual void PreSubclassWindow();
protected:
    DECLARE_MESSAGE_MAP()
public:
    void    MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);

    afx_msg void OnCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLvnBeginScroll(NMHDR *pNMHDR, LRESULT *pResult);

    void SetItemTextColor(int nItem = -1, int nSubItem = -1, COLORREF color = COLOR_INVALID, BOOL bRedraw = TRUE);
    int GetItemCount(DWORD dwStates = RC_ITEM_ALL);
    BOOL ExamItemStates(int nItem, DWORD dwStates) const;
    BOOL _IsValidIndex(int nIndex) const;
    void _AllocItemMemory(int nItem);
    int GetColumnCount() const; // Get the column count.
    DWORD GetItemStates(int nItem) const;







    //////////////////////////////////////////////////////////////////////////
//protected:
    CDWordArray            m_anDragIndexes;
    int                    m_nDropIndex;
    CImageList*            m_pDragImage;
    int                    m_nPrevDropIndex;
    UINT                m_uPrevDropState;
    DWORD                m_dwStyle;
	int                  isItemValid;    //指示被拖动的行是否有效（即不属于前两行）

    enum EScrollDirection
    {
        scrollNull,
        scrollUp,
        scrollDown
    };
    EScrollDirection    m_ScrollDirection;
    UINT                m_uScrollTimer;

public:
    bool IsDragging() const { return m_pDragImage != NULL; }


//protected:
    void DropItem();
    void RestorePrevDropItemState();
    void UpdateSelection(int nDropIndex);
    void SetScrollTimer(EScrollDirection ScrollDirection);
    void KillScrollTimer();
    CImageList* CreateDragImageEx(LPPOINT lpPoint);

    // Generated message map functions
protected:
    //{{AFX_MSG(CDragDropListCtrl)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
#endif
