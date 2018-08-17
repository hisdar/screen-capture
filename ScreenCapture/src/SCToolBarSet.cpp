// SCToolBarSet.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "SCToolBarSet.h"
#include "afxdialogex.h"


// CSCToolBarSet 对话框

IMPLEMENT_DYNAMIC(CSCToolBarSet, CDialogEx)

CSCToolBarSet::CSCToolBarSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSCToolBarSet::IDD, pParent)
{

}

CSCToolBarSet::~CSCToolBarSet()
{
}

void CSCToolBarSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR, m_mainColorStatic);
	DDX_Control(pDX, IDC_STATIC_SIZE, m_sizeStatic);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR2, m_color2Static);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR3, m_color3Static);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR4, m_color4Static);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR5, m_color5Static);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR6, m_color6Static);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR7, m_color7Static);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR8, m_color8Static);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR9, m_color9Static);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR10, m_color10Static);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR11, m_color11Static);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR12, m_color12Static);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR13, m_color13Static);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR14, m_color14Static);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR15, m_color15Static);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR16, m_color16Static);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR17, m_color17Static);
	DDX_Control(pDX, IDC_EDIT_SIZE, m_fontSizeEdit);
}


BEGIN_MESSAGE_MAP(CSCToolBarSet, CDialogEx)
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR, &CSCToolBarSet::OnStnClickedStaticMainColor)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR2, &CSCToolBarSet::OnStnClickedStaticMainColor2)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR3, &CSCToolBarSet::OnStnClickedStaticMainColor3)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR4, &CSCToolBarSet::OnStnClickedStaticMainColor4)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR5, &CSCToolBarSet::OnStnClickedStaticMainColor5)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR6, &CSCToolBarSet::OnStnClickedStaticMainColor6)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR7, &CSCToolBarSet::OnStnClickedStaticMainColor7)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR8, &CSCToolBarSet::OnStnClickedStaticMainColor8)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR9, &CSCToolBarSet::OnStnClickedStaticMainColor9)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR10, &CSCToolBarSet::OnStnClickedStaticMainColor10)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR11, &CSCToolBarSet::OnStnClickedStaticMainColor11)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR12, &CSCToolBarSet::OnStnClickedStaticMainColor12)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR13, &CSCToolBarSet::OnStnClickedStaticMainColor13)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR14, &CSCToolBarSet::OnStnClickedStaticMainColor14)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR15, &CSCToolBarSet::OnStnClickedStaticMainColor15)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR16, &CSCToolBarSet::OnStnClickedStaticMainColor16)
	ON_STN_CLICKED(IDC_STATIC_MAIN_COLOR17, &CSCToolBarSet::OnStnClickedStaticMainColor17)
	ON_EN_CHANGE(IDC_EDIT_SIZE, &CSCToolBarSet::OnEnChangeEditSize)
END_MESSAGE_MAP()


// CSCToolBarSet 消息处理程序


void CSCToolBarSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rect;
	GetWindowRect(&rect);

	dc.MoveTo(0, 0);
	dc.LineTo(rect.Width(), 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CSCToolBarSet::OnStnClickedStaticMainColor2()
{
	// m_color2Static.SetIsMouseIn(TRUE);
	m_mainColorStatic.SetBgColor(m_color2Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor3()
{
	m_mainColorStatic.SetBgColor(m_color3Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor4()
{
	m_mainColorStatic.SetBgColor(m_color4Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor5()
{
	m_mainColorStatic.SetBgColor(m_color5Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor6()
{
	m_mainColorStatic.SetBgColor(m_color6Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor7()
{
	m_mainColorStatic.SetBgColor(m_color7Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor8()
{
	m_mainColorStatic.SetBgColor(m_color8Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor9()
{
	m_mainColorStatic.SetBgColor(m_color9Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor10()
{
	m_mainColorStatic.SetBgColor(m_color10Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor11()
{
	m_mainColorStatic.SetBgColor(m_color11Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor12()
{
	m_mainColorStatic.SetBgColor(m_color12Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor13()
{
	m_mainColorStatic.SetBgColor(m_color13Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor14()
{
	m_mainColorStatic.SetBgColor(m_color14Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor15()
{
	m_mainColorStatic.SetBgColor(m_color15Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor16()
{
	m_mainColorStatic.SetBgColor(m_color16Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


void CSCToolBarSet::OnStnClickedStaticMainColor17()
{
	m_mainColorStatic.SetBgColor(m_color17Static.GetBgColor());
	GetParent()->PostMessage(SCREEN_CAPTURE_COLOR, 0, 0);
}


BOOL CSCToolBarSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_color2Static.SetBgColor(RGB(  0,   0,   0));
	m_color3Static.SetBgColor(RGB(255, 255, 255));
	m_color4Static.SetBgColor(RGB(128, 128, 128));
	m_color5Static.SetBgColor(RGB(192, 192, 192));
	m_color6Static.SetBgColor(RGB(128,   0,   0));
	m_color7Static.SetBgColor(RGB(251,  56,  56));
	m_color8Static.SetBgColor(RGB(247, 136,  58));
	m_color9Static.SetBgColor(RGB(255, 255,   0));
	m_color10Static.SetBgColor(RGB( 48, 132,  48));
	m_color11Static.SetBgColor(RGB(153, 204,   0));
	m_color12Static.SetBgColor(RGB( 56,  90, 211));
	m_color13Static.SetBgColor(RGB( 56, 148, 228));
	m_color14Static.SetBgColor(RGB(128,   0, 128));
	m_color15Static.SetBgColor(RGB(243,  27, 243));
	m_color16Static.SetBgColor(RGB(  0, 153, 153));
	m_color17Static.SetBgColor(RGB(22, 220, 220));
	
	m_mainColorStatic.SetBgColor(m_color2Static.GetBgColor());

	CString defaultSize;
	defaultSize.Format(_T("%d"), DEFAULT_SIZE);
	m_fontSizeEdit.SetWindowTextW(defaultSize);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSCToolBarSet::OnEnChangeEditSize()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	GetParent()->PostMessage(SCREEN_CAPTURE_SIZE, 0, 0);
}

COLORREF CSCToolBarSet::GetSelectedColor() {
	return m_mainColorStatic.GetBgColor();
}

int CSCToolBarSet::GetFontSize() {
	CString fontSize;
	m_fontSizeEdit.GetWindowTextW(fontSize);

	return _ttoi(fontSize);
}

void CSCToolBarSet::SetFontSize(int size) {
	CString fontSize;
	fontSize.Format(_T("%d"), size);
	m_fontSizeEdit.SetWindowTextW(fontSize);
}


BOOL CSCToolBarSet::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN) {

		// 屏蔽回车和 ESC 
		if (pMsg->wParam==VK_ESCAPE || pMsg->wParam==VK_RETURN) {     
			return TRUE;
		}
	}

	// 屏蔽ALT+F4
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4 ) {

		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
