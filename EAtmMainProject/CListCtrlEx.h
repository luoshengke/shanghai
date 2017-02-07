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
//�Զ�����Ϣ;
#define WM_VALIDATE        WM_USER + 0x7FFD
#define WM_SET_ITEMS    WM_USER + 0x7FFC

//���ֱ༭��ʽ;
#define CLISTCTRL_COMMON   0  //���棬��֧�ֱ༭;
#define CLISTCTRL_EDIT     1  //ʹ�ñ༭��;
#define CLISTCTRL_COMBOX   2  //ʹ����Ͽ�;


#define COLOR_INVALID    0xffffffff
#define RC_ITEM_NONE        0x0000 // Void, indicates invalid items only
#define    RC_ITEM_ALL            0x0001 // All items regardless of states
#define    RC_ITEM_SELECTED    0x0002 // Selected items
#define    RC_ITEM_UNSELECTED    0x0004 // Unselected items
#define    RC_ITEM_CHECKED        0x0008 // Checked items
#define    RC_ITEM_UNCHECKED    0x0010 // Unchecked items
#define    RC_ITEM_FOCUSED        0x0020 // Focused item
#define    RC_ITEM_UNFOCUSED    0x0040 // Unfocused items
//��Ͽ�;
class CComboxExtend : public CComboBox
{
    DECLARE_DYNAMIC(CComboxExtend)
public:
    CComboxExtend();
    virtual ~CComboxExtend();
    //��Ͽ��������ѡ��;
    int  AddItemStr(vector <CString> ListVector);
    //��ʾ��Ͽ�;
    int  ShowOption(CString  stritem,CRect RcCtrl,int niRowitem,int niColumnitem);
    //��ɱ༭;
    int  FunishEdit(BOOL bEdit);

    //�����ⲿ������Ϊ��UGʹ�÷�ҳ�д����ĶԻ����ڷ�MFC��PreTranslateMessage����Ч
    //����ESC ���ڽػ񲻵�����Ϣ��WM_CLOSEֱ�ӷ��͵�UG�����ڣ�ʹUG�ر����Բ��ü��̹�����������һ����;
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
    //��ʾ�༭��;
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

typedef void (*FunlbuttonDown)(); //�б���;
typedef void (*FunlbuttonDdwon)();//�б�˫��;
typedef void (*FunFinishEdit)();  //��ɱ༭;

class CListCtrlEx : public CListCtrl
{
    DECLARE_DYNAMIC(CListCtrlEx)
public:
    CListCtrlEx();
    virtual ~CListCtrlEx();

    int  RegisterParent(HWND hParent);
    //����Ҫ���ɿؼ��ĸ�����;
    int  SearchParentWindow(char *lpszTargeTitle,char *lpszTargetClass);
    //���ɸ����ڣ���Ҫ��Ϊ�˴����Ի���Ϣ;
    int  CreateParentWnd();
    //��ָ������ʽ��ID���ɿؼ�;
    int  CreateControlEx(int nColtrolID,DWORD dwStyle,DWORD dwExStyle);
    //ע��������Ͽ�༭����ѡ��;
    int  RegisterOptionList(vector <CString> OptionVector);
    int  RegisterOptionList(vector <string> OptionVector);
    //ע��༭��ʽ��UG������Ӧ�Ļص�;
    int  RegisterEditType(vector <EditInfo> EditVector,
                          FunlbuttonDown funlbdown=NULL,
                          FunlbuttonDdwon fundlbdown=NULL,
                          FunFinishEdit   funEditFunish=NULL);

    //ȡ��ͷ����,Ŀǰֻ�ܴӽṹ�л�ȡ;
    CString getColHeaderText(int nCol);

    //ȡ��ǰ�б��ѡ����;
    void GetSelectedItem(int *nItem,int *nSubItem)
    {*nItem=m_nItem;*nSubItem=m_nSubItem;}

    //�����иߣ�Ч�����ã���Ҫ��DRAMITEM�д����Ի���Ϣ̫�鷳�������и�ֱ�Ӳ������
    //�հ�ͼƬ����������ͼƬ��Y���ظ���;
    void  SetRowHeight(int nHeight);
    //ΪUG�ṩ��Ϣ��Ӧ;
    FunlbuttonDown  m_funlbdown;  //�������;
    FunlbuttonDdwon m_funldbdown; //���˫��;
    FunFinishEdit   m_EditFinish; //�༭���;

    virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
    //************************************************************************
public:
    
protected:
    //**********************�����ؼ�ʹ��************************************;
    HWND  FindSubWindow(HWND hSearch,char *lpszTargeTitle,char *lpszTargetClass);
    HWND  m_hParent;   //�����ھ��;
    int   m_ControlID;
    //************************************************************************
    int   EditSubItem(int nItem,int nSubItem);  //�༭��༭;
    int   ComBoxSubItem(int nItem,int nSubItem);//��Ͽ�༭;

    int   m_nSel;
    int   m_nItem;   //����;
    int   m_nSubItem;//����;
    CChildPWnd     m_pWndParent;//�����ڶ���;

    CString        m_StrCurSelect;//��ǰѡ��������ı�;
    CEditExtend    m_Edit_ctlItem;//�����ı༭�����;
    
    CFont m_FontItem;
    int   m_nRowHeight;
    COLORREF m_colRow1;
    COLORREF m_colRow2;
    //��Ͽ���ص���Ϣ;
    bool  m_bRegOption;
    bool  m_bRegEdit;

    CComboxExtend  m_ComBox_ctlItem; //��������Ͽ����;
    vector <CString>  m_OptionVector;//�����б������;
    vector <EditInfo> m_EdityType;   //���ж�Ӧ�ı༭��ʽ;
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
	int                  isItemValid;    //ָʾ���϶������Ƿ���Ч����������ǰ���У�

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
