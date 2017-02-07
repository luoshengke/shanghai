// MyListCtrl.cpp : 实现文件
//
#include "stdafx.h"
#include "CListCtrlEx.h"
#include <afxext.h>
#include <afxwin.h> 
#include <afxcmn.h>
#include <afxtempl.h>

#include <vector>
#include <stdio.h> 


#define RECT_ENLARGE  0
#define RECT_ENSMALL  1

static const int SCROLL_TIMER_ID        = 1;


int RectZoom(CRect *Rect,double dsize,int nType)
{
    if (nType==RECT_ENLARGE)//放大;
    {
        Rect->top-=dsize;
        Rect->bottom+=dsize;
        Rect->left-=dsize;
        Rect->right+=dsize;
    }
    if (nType==RECT_ENSMALL)//缩小;
    {
        Rect->top+=dsize;
        Rect->bottom-=dsize;
        Rect->left+=dsize;
        Rect->right-=dsize;
    }

    return 0;
}

//******************************************组合框****************************************//
IMPLEMENT_DYNAMIC(CComboxExtend, CComboBox)

BEGIN_MESSAGE_MAP(CComboxExtend, CComboBox)
    ON_WM_KILLFOCUS()
    ON_WM_CHAR()
    ON_CONTROL_REFLECT(CBN_SELENDOK, &CComboxExtend::OnCbnSelendok)
END_MESSAGE_MAP()

CComboxExtend::CComboxExtend()
{
    m_Hook=NULL;

}

CComboxExtend::~CComboxExtend()
{

}

int  CComboxExtend::FunishEdit(BOOL bEdit)
{
    Unloadhook();
    ShowWindow(SW_HIDE);

    if (bEdit)//如果可以编辑;
    {
        //取当前编辑框的值;
        CString strEdit;
        GetWindowText(strEdit);

        //向列表控件发送反射消息;
        LV_DISPINFO dispinfo;
        dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
        dispinfo.hdr.idFrom = GetDlgCtrlID();
        dispinfo.hdr.code = LVN_ENDLABELEDIT;

        dispinfo.item.mask = LVIF_TEXT;
        dispinfo.item.iItem = m_iRowIndex;
        dispinfo.item.iSubItem = m_iColumnIndex;
        dispinfo.item.pszText =  LPTSTR((LPCTSTR)strEdit);
        dispinfo.item.cchTextMax = strEdit.GetLength();

        GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);
    }

    return 0;
}
void CComboxExtend::OnKillFocus(CWnd* pNewWnd)
{
    CComboBox::OnKillFocus(pNewWnd);
    FunishEdit(TRUE);
}

void CComboxExtend::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CComboBox::OnChar(nChar, nRepCnt, nFlags);
}


int CComboxExtend::AddItemStr(vector <CString> ListVector)
{
    int  i=0;
    ResetContent();

    if (ListVector.size()>0)
    {
        for (i=0;i<ListVector.size();i++)
        {
            SetItemHeight(i,16);
            AddString(ListVector[i]);
        }
    }
    
    return 0;
}

void CComboxExtend::OnCbnSelendok()
{
    FunishEdit(TRUE);//完成选择结束编辑;
}

int CComboxExtend::ShowOption(CString  stritem,CRect RcCtrl,int niRowitem,int niColumnitem)
{
    m_iRowIndex=niRowitem;
    m_iColumnIndex=niColumnitem;
    m_OrginString=stritem;

    SetFocus();

    CRect  CRcomBox;
    GetWindowRect(CRcomBox);
    CRcomBox.top=RcCtrl.top;
    CRcomBox.bottom=RcCtrl.bottom+500;//这里多添加一点，否则下拉条会显示不正常;
    CRcomBox.left=RcCtrl.left;
    CRcomBox.right=RcCtrl.right;

    MoveWindow(CRcomBox);// 移动到子项所在区域;
    ShowWindow(SW_SHOW);
    
    //installhook();
    return 0;
}

LRESULT CComboxExtend::KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)
{ 
    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
        case WM_KEYDOWN: 
        case WM_SYSKEYDOWN:
        case WM_KEYUP:    
        case WM_SYSKEYUP: 
            {
                PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) lParam;
                
                if (p->vkCode==VK_ESCAPE)
                {
                    //FunishEdit(FALSE);
                    return TRUE;
                }
            }
            break;
        }
    }
    return  CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL CComboxExtend::installhook()
{
    HINSTANCE hins=AfxGetInstanceHandle();
    m_Hook = ::SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)KeyboardProc,hins,0); 
    return TRUE;
}

void CComboxExtend::Unloadhook()
{
    if (m_Hook)
    {
        ::UnhookWindowsHookEx(m_Hook);
    }
}

//******************************************编辑框***************************************//
IMPLEMENT_DYNAMIC(CEditExtend, CEdit)
CEditExtend::CEditExtend()
{
    

}

CEditExtend::~CEditExtend()
{

}

BEGIN_MESSAGE_MAP(CEditExtend, CEdit)
    ON_WM_KILLFOCUS()
    ON_WM_CHAR()
    ON_WM_SYSKEYDOWN()
    ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


int  CEditExtend::ShowEdit(CString strItem,CRect  rcCtrl,int niRowitem,int niColumnitem)
{
    m_iRowIndex=niRowitem;
    m_iColumnIndex=niColumnitem;
    RectZoom(&rcCtrl,0.5,RECT_ENSMALL);

    MoveWindow(&rcCtrl);
    ShowWindow(SW_SHOW);
    SetWindowText(strItem);
    SetFocus();
    SetSel(-1); 

    return 0;
}

void CEditExtend::OnKillFocus(CWnd* pNewWnd)
{
    CEdit::OnKillFocus(pNewWnd);
    //取当前编辑框的值;
    CString strEdit;
    GetWindowText(strEdit);

    //向列表控件发送反射消息;
    LV_DISPINFO dispinfo;
    dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
    dispinfo.hdr.idFrom = GetDlgCtrlID();
    dispinfo.hdr.code = LVN_ENDLABELEDIT;

    dispinfo.item.mask = LVIF_TEXT;
    dispinfo.item.iItem = m_iRowIndex;
    dispinfo.item.iSubItem = m_iColumnIndex;
    dispinfo.item.pszText =  LPTSTR((LPCTSTR)strEdit);
    dispinfo.item.cchTextMax = strEdit.GetLength();

    GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);
    ShowWindow(SW_HIDE);
}

void CEditExtend::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (13==nChar)//如果按下了回车;
    {
        CWnd *pNewWnd=(CWnd*)m_hWnd;
        OnKillFocus(pNewWnd);
    }

    CEdit::OnChar(nChar,nRepCnt,nFlags);
}

BOOL CEditExtend::PreTranslateMessage(MSG* pMsg)
{
    //这里的消息UG下暂时响应不了，在MFC下正常;
    if (pMsg->message == WM_KEYDOWN)
    {
        switch(pMsg->wParam)
        {
        case VK_RETURN:
            return TRUE;
        case VK_ESCAPE:
            return TRUE;
        default:
            break;
        }
    }

    return CEdit::PreTranslateMessage(pMsg);
}

//********************************************列表控件*****************************//
IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)

CListCtrlEx::CListCtrlEx()
{
    m_colRow1 = RGB(255,255,255);
    m_colRow2 = RGB(247,247,247);
    m_bRegOption=FALSE;
    m_bRegEdit=FALSE;
    m_OptionVector.clear();
    m_EdityType.clear();

    m_funlbdown=NULL;//单击事件;
    m_funldbdown=NULL;//双击事件;
    m_EditFinish=NULL;//编辑完成;

    m_nDropIndex=-1;
    m_pDragImage=NULL;
    m_nPrevDropIndex=-1;
    m_uPrevDropState=NULL;
    m_uScrollTimer=0;
    m_ScrollDirection=scrollNull;
    m_dwStyle=NULL;

}

CListCtrlEx::~CListCtrlEx()
{
    m_EdityType.clear();
    m_OptionVector.clear();

    delete m_pDragImage;
    m_pDragImage = NULL;

    KillScrollTimer();
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &CListCtrlEx::OnLvnEndlabeledit)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlEx::OnCustomdraw)
    ON_WM_MEASUREITEM()
    ON_WM_MEASUREITEM_REFLECT()
    //ON_WM_ERASEBKGND()//UG下效果不佳;
    ON_NOTIFY_REFLECT(LVN_BEGINSCROLL, &CListCtrlEx::OnLvnBeginScroll)



    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBeginDrag)
    ON_WM_TIMER()

END_MESSAGE_MAP()

//取表头文字,目前只能从结构中获取;
CString CListCtrlEx::getColHeaderText(int nCol)
{
    LVCOLUMN   lvColumn;   
    char  strChar[256];
    lvColumn.pszText=strChar;   
    lvColumn.cchTextMax=256 ;
    lvColumn.mask = LVCF_TEXT;
    GetColumn(nCol,&lvColumn);
    CString strHeader=lvColumn.pszText;

    return strHeader;
}

// CListCtrlEx 消息处理程序;
void CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
    //取当前选中项;
    LVHITTESTINFO lvhti;
    lvhti.pt = point;
    m_nItem =SubItemHitTest(&lvhti);
    if (m_nItem == -1&&m_bRegEdit==FALSE) 
        return;
    m_nSubItem = lvhti.iSubItem;


    CListCtrl::OnLButtonDown(nFlags, point);
	if (m_nItem == 0 || m_nItem == 1)
		isItemValid = 0;
	else
		isItemValid = 1;
    //响应单击外部事件;
    if (m_funlbdown)
        m_funlbdown();
}


void CListCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    CRect rcCtrl;
    LVHITTESTINFO lvhti;
    lvhti.pt = point;
    m_nItem =SubItemHitTest(&lvhti);

    if (m_nItem == -1||m_bRegEdit==FALSE) 
        return;

    int nEditType=CLISTCTRL_EDIT;

    m_nSubItem = lvhti.iSubItem;
    if (m_nSubItem<m_EdityType.size())//取编辑方式;
        nEditType=m_EdityType[m_nSubItem].nEditType;

    //当前的选中项;
    m_StrCurSelect = GetItemText(m_nItem, m_nSubItem);
    if (CLISTCTRL_COMMON==nEditType)//不进行处理;
    {
    }
    else if (CLISTCTRL_EDIT==nEditType)//编辑框进行编辑;
    {
        EditSubItem(m_nItem,m_nSubItem);
    }
    else if (CLISTCTRL_COMBOX==nEditType)//通过组合框编辑;
    {
        RegisterOptionList(m_EdityType[m_nSubItem].option);
        ComBoxSubItem(m_nItem,m_nSubItem);
    }
    CListCtrl::OnLButtonDblClk(nFlags, point);

    //响应单双外部事件;
    if (m_funldbdown)
        m_funldbdown();
}

int CListCtrlEx::CreateParentWnd()
{
    if (m_hParent)
    {
        CWnd *pWndParent=NULL;
        pWndParent=FromHandle(m_hParent);

        m_pWndParent.Create(NULL,NULL,WS_VISIBLE|WS_CHILDWINDOW,CRect(0,0,100,100),pWndParent,1000,NULL);

        m_pWndParent.ShowWindow(SW_SHOW);
    }

    return 0;
}

int CListCtrlEx::CreateControlEx(int nColtrolID,DWORD dwStyle,DWORD dwExStyle)
{
    if (m_hParent)
    {
        CString CSclass="SysListView32";
        DWORD   dBaseStyle=WS_CHILDWINDOW|WS_VISIBLE|WS_TABSTOP;

        m_ControlID=nColtrolID;
        dBaseStyle|=dwStyle;
        //这种方式直接在UG的界面上生成控件，该方式无法处理自绘消息例如设置颜色等，控件重绘正常;
        /*CWnd *pWndParent=NULL;
        pWndParent=FromHandle(m_hParent);
        Create(dBaseStyle,CRect(0,0,100,100),pWndParent,nColtrolID);*/
        
        //生成父窗口，这样可以处理LIST控件的自绘;
        CreateParentWnd();
        if (m_pWndParent.m_hWnd!=NULL)//父窗口生成成功;
        {
            Create(dBaseStyle,CRect(0,0,100,100),&m_pWndParent,nColtrolID);
            //WS_EX_NOPARENTNOTIFY这个样式不能添加，原因是目前的处理方式未能截获ESC的消息;
            //按下ESC默认向UG主窗口发送了WM_CLOSE;
            //有些文章有说这个样式和控件自绘的消息处理有关，去掉这个属性目前没有发现异常;
            DWORD   dExStyle=WS_EX_LEFT|WS_EX_LTRREADING;
            //设置扩展样式;
            dExStyle = GetExtendedStyle();
            dExStyle|= dwExStyle;
            //设置扩展风格;
            SetExtendedStyle(dExStyle); 
            //窗口顶置;
            SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE);
        }
    }

    return 0;
}

//注册编辑方式和UG界面响应的回调;
int CListCtrlEx::RegisterEditType(vector <EditInfo> EditVector,
                                  FunlbuttonDown funlbdown,
                                  FunlbuttonDdwon fundlbdown,
                                  FunFinishEdit   funEditFunish)
{
    if (funlbdown)
        m_funlbdown=funlbdown;//单击事件;
    if (fundlbdown)
        m_funldbdown=fundlbdown;//双击事件;
    if (funEditFunish)
        m_EditFinish=funEditFunish;//编辑完成;

    m_EdityType=EditVector;    
    m_bRegEdit=TRUE;
    return 0;
}

int CListCtrlEx::RegisterOptionList(vector <string> OptionVector)
{
    int  nSize=OptionVector.size();

    vector <CString> Option;
    for (int i=0;i<nSize;i++)
    {
        CString  strItem;
        strItem=OptionVector[i].c_str();
        Option.push_back(strItem);
    }
    RegisterOptionList(Option);
    return 0;
}

//注册用于组合框编辑下拉选项;
int CListCtrlEx::RegisterOptionList(vector <CString> OptionVector)
{
    if (OptionVector.size()>0)
    {
        m_OptionVector.clear();
        m_OptionVector=OptionVector;
        m_bRegOption=TRUE;
    }
    else
        m_bRegOption=FALSE;
    
    return 0;
}

int CListCtrlEx::RegisterParent(HWND hParent)
{
    m_hParent=hParent;

    return 0;
}

int CListCtrlEx::SearchParentWindow(char *lpszTargeTitle,char *lpszTargetClass)
{
    HWND  hUgWindow=NULL;
    HWND  hWndNext=NULL;
    HWND  hParent=NULL;
    hUgWindow=::GetActiveWindow( );//当前的活动窗口;


    if (hUgWindow)
    {
        hWndNext = ::GetWindow( hUgWindow, GW_CHILD);//当前窗口的子窗口;
        if (hWndNext)
        {
            while (hWndNext != NULL)//在同级窗口中查找;
            {
                //当前窗口的类名和标题;
                char  lpszTitleName[132]="",lpszClassName[132]="";
                ::GetWindowText( hWndNext, lpszTitleName, 132 );
                ::GetClassName( hWndNext, lpszClassName, 132 );
                //当前窗口就是要查找的窗口,这里一般都不会是;
                if (stricmp(lpszTargeTitle,lpszTitleName)==0&&
                    stricmp(lpszTargetClass,lpszClassName)==0)
                {
                    hWndNext = ::GetNextWindow(hWndNext, GW_HWNDNEXT);
                    if (hWndNext!=NULL)
                    {
                        RegisterParent(hWndNext);
                        return 0;
                    }
                }
                else
                {
                    //递归查找到子窗口;
                    hParent=FindSubWindow(hWndNext,lpszTargeTitle,lpszTargetClass);
                    if (hParent)
                    {
                        m_hParent=::GetNextWindow(hParent, GW_HWNDNEXT);
                        if (m_hParent)
                        {
                            RegisterParent(m_hParent);
                            return 0;
                        }
                    }
                }
                hWndNext =::GetNextWindow(hWndNext, GW_HWNDNEXT);
            }
        }
    }
    return -1;
}

HWND CListCtrlEx::FindSubWindow(HWND hSearch,char *lpszTargeTitle,char *lpszTargetClass)
{
    HWND  hTargeWnd=NULL;
    hTargeWnd = ::GetWindow( hSearch, GW_CHILD); 
    if (hTargeWnd)//当前窗口的子窗口不为空;
    {
        //取当前窗口的类名和标题;
        char  lpszTitleName[132]="",lpszClassName[132]="";
        while(hTargeWnd)
        {
            ::GetWindowText( hTargeWnd, lpszTitleName, 132 );
            ::GetClassName( hTargeWnd, lpszClassName, 132 );

            //当前窗口就是要查找的窗口;
            if (stricmp(lpszTargeTitle,lpszTitleName)==0&&
                stricmp(lpszTargetClass,lpszClassName)==0)
            {
                return hTargeWnd;
            }
            //递归查找;
            hSearch=FindSubWindow(hTargeWnd,lpszTargeTitle,lpszTargetClass);
            if (hSearch)
                return hSearch;
            //未找到，找当前同级的;
            hTargeWnd = ::GetNextWindow(hTargeWnd, GW_HWNDNEXT);
        }
    }

    return hTargeWnd;
}

int  CListCtrlEx::EditSubItem(int nItem,int nSubItem)
{
    CRect rcCtrl;

    GetSubItemRect(nItem,nSubItem,LVIR_LABEL,rcCtrl);
    if (m_Edit_ctlItem.m_hWnd == NULL)
    {
        m_Edit_ctlItem.Create(ES_AUTOHSCROLL|WS_CHILD|ES_LEFT|ES_WANTRETURN|WS_BORDER,CRect(0,0,0,0),this,99);
        m_Edit_ctlItem.ShowWindow(SW_HIDE);

        m_FontItem.CreateStockObject(DEFAULT_GUI_FONT);
        m_Edit_ctlItem.SetFont(&m_FontItem);
        m_FontItem.DeleteObject();
    }
    CString strItem=GetItemText(nItem,nSubItem);
    m_Edit_ctlItem.ShowEdit(strItem,rcCtrl,nItem,nSubItem);

    return 0;
}

int CListCtrlEx::ComBoxSubItem(int nItem,int nSubItem)
{
    int   irc=0;
    CRect rcCtrl;
    //当前CTRL单元格尺寸;
    GetSubItemRect(nItem,nSubItem,LVIR_LABEL,rcCtrl);
    if (m_ComBox_ctlItem.m_hWnd==NULL)
    {
        DWORD dboxStyle=WS_BORDER | WS_CHILD | WS_VISIBLE|
            CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL;
        
        m_ComBox_ctlItem.Create(dboxStyle,CRect(100,100,500,500),this,100);
        m_ComBox_ctlItem.ShowWindow(SW_HIDE);// 隐藏;
        //字体;
        CFont  tpFont;
        tpFont.CreateStockObject(DEFAULT_GUI_FONT);
        m_ComBox_ctlItem.SetFont(&tpFont);
        tpFont.DeleteObject();
    }
    //组合框添加项目;
    m_ComBox_ctlItem.AddItemStr(m_OptionVector);
    m_ComBox_ctlItem.ShowOption(m_StrCurSelect,rcCtrl,nItem,nSubItem);
    //设置组合框选项为当前CTRL项;
    irc=m_ComBox_ctlItem.SelectString(-1,m_StrCurSelect);

    return 0;
}

void CListCtrlEx::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
    LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
    //更新显示;
    SetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText);

    GetParent()->SendMessage(WM_VALIDATE, GetDlgCtrlID(), (LPARAM)pDispInfo); 
    
    //让UG界面同时响应变更;
    if (m_EditFinish)
        m_EditFinish();

    *pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CListCtrlEx::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
    if (message==WM_PAINT)
    {
        //CRect  CR;
        //CWnd   *pWParent=NULL; 
        //pWParent=GetParent();        //取父窗口对象;
        //pWParent->GetWindowRect(&CR);//获取屏幕坐标系下的坐标;
        //CR.top+=1;
        //CR.bottom-=2;
        //CR.left+=1;
        //CR.right-=2;
        //pWParent->ScreenToClient(CR);//坐标转换到客户区;
        //MoveWindow(CR);              //设置当前控件;
        //Invalidate(FALSE);
    }
    //其它消息;
    return CListCtrl::WindowProc(message,wParam,lParam);
}

void CListCtrlEx::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CListCtrlEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    if (m_nRowHeight>0)
    {
        lpMeasureItemStruct->itemHeight = m_nRowHeight;
    }
}

void CListCtrlEx::SetRowHeight(int nHeight)
{
    m_nRowHeight = nHeight;
    CRect rcWin;
    GetWindowRect(&rcWin);
    WINDOWPOS wp;
    wp.hwnd = m_hWnd;
    wp.cx = rcWin.Width();
    wp.cy = rcWin.Height();
    wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
    SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}

//让LIST的行和UG一样隔行变色;
void CListCtrlEx::OnCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
    int iRow = lplvcd->nmcd.dwItemSpec;

    switch(lplvcd->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT :
        {
            *pResult = CDRF_NOTIFYITEMDRAW;
            return;
        }
        // Modify item text and or background
    case CDDS_ITEMPREPAINT:
        {
            lplvcd->clrText = RGB(0,0,0);
            // If you want the sub items the same as the item,
            // set *pResult to CDRF_NEWFONT
            *pResult = CDRF_NOTIFYSUBITEMDRAW;
            return;
        }
        // Modify sub item text and/or background
    case CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM:
        {

            if(iRow %2){
                lplvcd->clrTextBk = m_colRow2;
            }
            else{
                lplvcd->clrTextBk = m_colRow1;
            }
            *pResult = CDRF_DODEFAULT;
            return;
        }
    }

    *pResult = 0;
}


BOOL CListCtrlEx::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    CRect rect;
    CListCtrlEx::GetClientRect(rect);

    POINT mypoint;  

    COLORREF m_colRow1;
    COLORREF m_colRow2;
    m_colRow1 = RGB(240,247,249);
    m_colRow2 = RGB(229,232,239);
    CBrush brush0(m_colRow1);
    CBrush brush1(m_colRow2);

    int chunk_height=GetCountPerPage();
    pDC->FillRect(&rect,&brush1);

    for (int i=0;i<=chunk_height;i++)
    {
        GetItemPosition(i,&mypoint);
        rect.top=mypoint.y ;
        GetItemPosition(i+1,&mypoint);
        rect.bottom =mypoint.y;
        pDC->FillRect(&rect,i %2 ? &brush1 : &brush0);
    }
    brush0.DeleteObject();
    brush1.DeleteObject();

    return FALSE;
    //return CListCtrl::OnEraseBkgnd(pDC);
}


void CListCtrlEx::OnLvnBeginScroll(NMHDR *pNMHDR, LRESULT *pResult)
{
    // 此功能要求 Internet Explorer 5.5 或更高版本。
    // 符号 _WIN32_IE 必须是 >= 0x0560。
    LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);

    //由于控件是动态生成的，当前LIST控件的窗口进行滚动后，创建的控件直接移动不好处理。
    //这里直接隐藏;
    if (m_Edit_ctlItem.m_hWnd!=NULL)
    {
        if (m_Edit_ctlItem.IsWindowVisible())
            m_Edit_ctlItem.ShowWindow(SW_HIDE);
    }
    if (m_ComBox_ctlItem.m_hWnd!=NULL)
    {
        if (m_ComBox_ctlItem.IsWindowVisible())
            m_ComBox_ctlItem.ShowWindow(SW_HIDE);
    }

    *pResult = 0;
}
#include <afxtempl.h>
struct ROWINFO
{
    DWORD dwData;
    DWORD dwStates;
    CArray<int, int> aImages;
    CStringArray aTexts;
};

int CListCtrlEx::GetItemCount(DWORD dwStates) 
{
    const int ITEMS = CListCtrl::GetItemCount();
    if (dwStates == RC_ITEM_ALL)
        return ITEMS;

    int nCount = 0;
    for (int i = 0; i <ITEMS; i++)
    {
        if (ExamItemStates(i, dwStates))
            nCount++;
    }
    return nCount;
}
BOOL CListCtrlEx::ExamItemStates(int nItem, DWORD dwStates) const
{
    if (dwStates == RC_ITEM_NONE)
        return FALSE;

    if (dwStates & RC_ITEM_ALL)
        return TRUE;

    return (GetItemStates(nItem) & dwStates) == dwStates;
}
DWORD CListCtrlEx::GetItemStates(int nItem) const
{
    DWORD dwStates = 0;

    if (CListCtrl::GetItemState(nItem, LVIS_SELECTED))
        dwStates |= RC_ITEM_SELECTED;
    else
        dwStates |= RC_ITEM_UNSELECTED;

    if (CListCtrl::GetCheck(nItem) > 0)
        dwStates |= RC_ITEM_CHECKED;
    else
        dwStates |= RC_ITEM_UNCHECKED;

    if (CListCtrl::GetItemState(nItem, LVIS_FOCUSED))
        dwStates |= RC_ITEM_FOCUSED;
    else
        dwStates |= RC_ITEM_UNFOCUSED;

    return dwStates;
}
int CListCtrlEx::GetColumnCount() const
{
    return GetHeaderCtrl()->GetItemCount();
}

BOOL CListCtrlEx::_IsValidIndex(int nIndex) const
{
    return nIndex >= 0 && nIndex < CListCtrl::GetItemCount();
}






void CListCtrlEx::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if (pNMListView)
    {
        m_nPrevDropIndex    = -1;
        m_uPrevDropState    = NULL;

        // Items being dragged - can be one or more.
        m_anDragIndexes.RemoveAll();
        POSITION pos = GetFirstSelectedItemPosition();
        while (pos)
        {
            m_anDragIndexes.Add(GetNextSelectedItem(pos));
        }

        DWORD dwStyle = GetStyle();
        if ((dwStyle & LVS_SINGLESEL) == LVS_SINGLESEL)
        {
            // List control is single select; we need it to be multi-select so
            // we can show both the drag item and potential drag target as selected.
            m_dwStyle = dwStyle;
            ModifyStyle(LVS_SINGLESEL, NULL);
        }

        if (m_anDragIndexes.GetSize() > 0)
        {
            // Create a drag image from the centre point of the item image.
            // Clean up any existing drag image first.
            delete m_pDragImage;
            CPoint ptDragItem;
            m_pDragImage = CreateDragImageEx(&ptDragItem);
            if (m_pDragImage)
            {
                m_pDragImage->BeginDrag(0, ptDragItem);
                m_pDragImage->DragEnter(CWnd::GetDesktopWindow(), pNMListView->ptAction);

                // Capture all mouse messages in case the user drags outside the control.
                SetCapture();
            }
        }
    }

    *pResult = 0;
}

// Based on code by Frank Kobs.
CImageList* CListCtrlEx::CreateDragImageEx(LPPOINT lpPoint)
{
    CRect rectSingle;    
    CRect rectComplete(0, 0, 0, 0);
    int    nIndex    = -1;
    BOOL bFirst    = TRUE;

    // Determine the size of the drag image.
    POSITION pos = GetFirstSelectedItemPosition();
    while (pos)
    {
        nIndex = GetNextSelectedItem(pos);
        GetItemRect(nIndex, rectSingle, LVIR_BOUNDS);
        if (bFirst)
        {
            // Initialize the CompleteRect
            GetItemRect(nIndex, rectComplete, LVIR_BOUNDS);
            bFirst = FALSE;
        }
        rectComplete.UnionRect(rectComplete, rectSingle);
    }

    // Create bitmap in memory DC
    CClientDC dcClient(this);    
    CDC dcMem;    
    CBitmap Bitmap;

    if (!dcMem.CreateCompatibleDC(&dcClient))
    {
        return NULL;
    }

    if (!Bitmap.CreateCompatibleBitmap(&dcClient, rectComplete.Width(), rectComplete.Height()))
    {
        return NULL;
    }

    CBitmap* pOldMemDCBitmap = dcMem.SelectObject(&Bitmap);
    // Here green is used as mask color.
    dcMem.FillSolidRect(0, 0, rectComplete.Width(), rectComplete.Height(), RGB(0, 255, 0)); 

    // Paint each DragImage in the DC.
    CImageList* pSingleImageList = NULL;
    CPoint pt;

    pos = GetFirstSelectedItemPosition();
    while (pos)
    {
        nIndex = GetNextSelectedItem(pos);
        GetItemRect(nIndex, rectSingle, LVIR_BOUNDS);

        pSingleImageList = CreateDragImage(nIndex, &pt);
        if (pSingleImageList)
        {
            // Make sure width takes into account not using LVS_EX_FULLROWSELECT style.
            IMAGEINFO ImageInfo;
            pSingleImageList->GetImageInfo(0, &ImageInfo);
            rectSingle.right = rectSingle.left + (ImageInfo.rcImage.right - ImageInfo.rcImage.left);

            pSingleImageList->DrawIndirect(
                &dcMem, 
                0, 
                CPoint(rectSingle.left - rectComplete.left, 
                rectSingle.top - rectComplete.top),
                rectSingle.Size(), 
                CPoint(0,0));

            delete pSingleImageList;
        }
    }

    dcMem.SelectObject(pOldMemDCBitmap);

    // Create the imagelist    with the merged drag images.
    CImageList* pCompleteImageList = new CImageList;

    pCompleteImageList->Create(rectComplete.Width(), rectComplete.Height(), ILC_COLOR | ILC_MASK, 0, 1);
    // Here green is used as mask color.
    pCompleteImageList->Add(&Bitmap, RGB(0, 255, 0)); 

    Bitmap.DeleteObject();

    // As an optional service:
    // Find the offset of the current mouse cursor to the imagelist
    // this we can use in BeginDrag().
    if (lpPoint)
    {
        CPoint ptCursor;
        GetCursorPos(&ptCursor);
        ScreenToClient(&ptCursor);
        lpPoint->x = ptCursor.x - rectComplete.left;
        lpPoint->y = ptCursor.y - rectComplete.top;
    }

    return pCompleteImageList;
}

void CListCtrlEx::OnMouseMove(UINT nFlags, CPoint point) 
{
    if (isItemValid == 0)
    {
		CListCtrl::OnMouseMove(nFlags, point);
		return;
    }
	if (m_pDragImage)
    {
        // Must be dragging, as there is a drag image.

        // Move the drag image.
        CPoint ptDragImage(point);
        ClientToScreen(&ptDragImage);
        m_pDragImage->DragMove(ptDragImage);

        // Leave dragging so we can update potential drop target selection.
        m_pDragImage->DragLeave(CWnd::GetDesktopWindow());

        // Force x coordinate to always be in list control - only interested in y coordinate.
        // In effect the list control has captured all horizontal mouse movement.
        static const int nXOffset = 8;
        CRect rect;
        GetWindowRect(rect);
        CWnd* pDropWnd = CWnd::WindowFromPoint(CPoint(rect.left + nXOffset, ptDragImage.y));

        // Get the window under the drop point.
        if (pDropWnd == this)
        {
            // Still in list control so select item under mouse as potential drop target.
            point.x = nXOffset;    // Ensures x coordinate is always valid.
            UpdateSelection(HitTest(point));
        }

        CRect rectClient;
        GetClientRect(rectClient);
        CPoint ptClientDragImage(ptDragImage);
        ScreenToClient(&ptClientDragImage);

        // Client rect includes header height, so ignore it, i.e.,
        // moving the mouse over the header (and higher) will result in a scroll up.
        CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
        if (pHeader)
        {
            CRect rectHeader;
            pHeader->GetClientRect(rectHeader);
            rectClient.top += rectHeader.Height();
        }

        if (ptClientDragImage.y < rectClient.top)
        {
            // Mouse is above the list control - scroll up.
            SetScrollTimer(scrollUp);
        }
        else if (ptClientDragImage.y > rectClient.bottom)
        {
            // Mouse is below the list control - scroll down.
            SetScrollTimer(scrollDown);
        }
        else
        {
            KillScrollTimer();
        }

        // Resume dragging.
        m_pDragImage->DragEnter(CWnd::GetDesktopWindow(), ptDragImage);
    }
    else
    {
        KillScrollTimer();
    }

    CListCtrl::OnMouseMove(nFlags, point);
}

void CListCtrlEx::UpdateSelection(int nDropIndex)
{
    if (nDropIndex > -1 && nDropIndex < GetItemCount())
    {
        // Drop index is valid and has changed since last mouse movement.

        RestorePrevDropItemState();

        // Save information about current potential drop target for restoring next time round.
        m_nPrevDropIndex = nDropIndex;
        m_uPrevDropState = GetItemState(nDropIndex, LVIS_SELECTED);

        // Select current potential drop target.
        SetItemState(nDropIndex, LVIS_SELECTED, LVIS_SELECTED);
        m_nDropIndex = nDropIndex;        // Used by DropItem().

        UpdateWindow();
    }
}

void CListCtrlEx::RestorePrevDropItemState()
{
    if (m_nPrevDropIndex > -1 && m_nPrevDropIndex < GetItemCount())
    {
        // Restore state of previous potential drop target.
        SetItemState(m_nPrevDropIndex, m_uPrevDropState, LVIS_SELECTED);
    }
}

void CListCtrlEx::SetScrollTimer(EScrollDirection ScrollDirection)
{
    if (m_uScrollTimer == 0)
    {
        m_uScrollTimer = SetTimer(SCROLL_TIMER_ID, 100, NULL);
    }
    m_ScrollDirection = ScrollDirection;
}

void CListCtrlEx::KillScrollTimer()
{
    if (m_uScrollTimer != 0)
    {
        KillTimer(SCROLL_TIMER_ID);
        m_uScrollTimer        = 0;
        m_ScrollDirection    = scrollNull;
    }
}

void CListCtrlEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if (m_pDragImage)
    {
        KillScrollTimer();

        // Release the mouse capture and end the dragging.
        ::ReleaseCapture();
        m_pDragImage->DragLeave(CWnd::GetDesktopWindow());
        m_pDragImage->EndDrag();

        delete m_pDragImage;
        m_pDragImage = NULL;

        // Drop the item on the list.
		if (isItemValid == 1)
		{
			DropItem();
		}
    }

    CListCtrl::OnLButtonUp(nFlags, point);
}

void CListCtrlEx::DropItem()
{
    RestorePrevDropItemState();
    // Drop after currently selected item.
    /*m_nDropIndex++;*/

    if (m_nDropIndex < 2 || m_nDropIndex > GetItemCount() - 1)
    {
        // Fail safe - invalid drop index, so drop at second of list.
        m_nDropIndex = 2;
    }

    int nColumns = 1;
    CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
    if (pHeader)
    {
        nColumns = pHeader->GetItemCount();
    }

    // Move all dragged items to their new positions.
    for (int nDragItem = 0; nDragItem < m_anDragIndexes.GetSize(); nDragItem++)
    {
        int nDragIndex = m_anDragIndexes[nDragItem];

		if (nDragIndex > -1 && nDragIndex < GetItemCount())
		{
            // Get information about this drag item.
            char szText[256];
            LV_ITEM lvItem;
            ZeroMemory(&lvItem, sizeof(LV_ITEM));
            lvItem.iItem        = nDragIndex;
            lvItem.mask            = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
            lvItem.stateMask    = LVIS_DROPHILITED | LVIS_FOCUSED | LVIS_SELECTED | LVIS_STATEIMAGEMASK;
            lvItem.pszText        = szText;
            lvItem.cchTextMax    = sizeof(szText) - 1;
            GetItem(&lvItem);
            BOOL bChecked = GetCheck(nDragIndex);

            // Before moving drag item, make sure it is deselected in its original location,
            // otherwise GetSelectedCount() will return 1 too many.
            SetItemState(nDragIndex, static_cast<UINT>(~LVIS_SELECTED), LVIS_SELECTED);

            // Insert the dragged item at drop index.
            lvItem.iItem        = m_nDropIndex;
            InsertItem(&lvItem);
            if (bChecked)
            {
                SetCheck(m_nDropIndex);
            }

            // Index of dragged item will change if item has been dropped above itself.
            if (nDragIndex > m_nDropIndex)
            {
                nDragIndex++;
            }

            // Fill in all the columns for the dragged item.
            lvItem.mask        = LVIF_TEXT;
            lvItem.iItem    = m_nDropIndex;

            for (int nColumn = 1; nColumn < nColumns; nColumn++)
            {
                _tcscpy(lvItem.pszText, (LPCTSTR)(GetItemText(nDragIndex, nColumn)));
                lvItem.iSubItem = nColumn;
                SetItem(&lvItem);
            }

            // Delete the original item.
            DeleteItem(nDragIndex);

            // Need to adjust indexes of remaining drag items.
            for (int nNewDragItem = nDragItem; nNewDragItem < m_anDragIndexes.GetSize(); nNewDragItem++)
            {
                int nNewDragIndex = m_anDragIndexes[nNewDragItem];

                if (nDragIndex < nNewDragIndex && nNewDragIndex < m_nDropIndex)
                {
                    // Item has been removed from above this item, and inserted after it,
                    // so this item moves up the list.
                    m_anDragIndexes[nNewDragItem] = max(nNewDragIndex - 1, 0);
                }
                else if (nDragIndex > nNewDragIndex && nNewDragIndex > m_nDropIndex)
                {
                    // Item has been removed from below this item, and inserted before it,
                    // so this item moves down the list.
                    m_anDragIndexes[nNewDragItem] = nNewDragIndex + 1;
                }
            }
            if (nDragIndex > m_nDropIndex)
            {
                // Item has been added before the drop target, so drop target moves down the list.
                m_nDropIndex++;
            }
        }
    }

    if (m_dwStyle != NULL)
    {
        // Style was modified, so return it back to original style.
        ModifyStyle(NULL, m_dwStyle);
        m_dwStyle = NULL;
    }

    //重新填充序号
    for (int i=0;i<GetItemCount();i++)
    {
        char cIndex[133]="";

        sprintf_s(cIndex,133,"%d",i+1);
        SetItemText(i,0,cIndex);
    }
}

void CListCtrlEx::OnTimer(UINT_PTR nIDEvent) 
{
    if (nIDEvent == SCROLL_TIMER_ID && m_pDragImage)
    {
        WPARAM wParam    = NULL;
        int nDropIndex    = -1;
        if (m_ScrollDirection == scrollUp)
        {
            wParam        = MAKEWPARAM(SB_LINEUP, 0);
            nDropIndex    = m_nDropIndex - 1;
        }
        else if (m_ScrollDirection == scrollDown)
        {
            wParam        = MAKEWPARAM(SB_LINEDOWN, 0);
            nDropIndex    = m_nDropIndex + 1;
        }
        m_pDragImage->DragShowNolock(FALSE);
        SendMessage(WM_VSCROLL, wParam, NULL);
        UpdateSelection(nDropIndex);
        m_pDragImage->DragShowNolock(TRUE);
    }
    else
    {
        CListCtrl::OnTimer(nIDEvent);
    }
}