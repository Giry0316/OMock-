
// OMokDlg.h: 헤더 파일
//

#pragma once
#include "OMokCell.h"

#include <stack>
#include <queue>
using namespace std;

// COMokDlg 대화 상자
class COMokDlg : public CDialogEx
{
public:
	CRect m_BoardRt, m_PanRt, m_BackRt, m_ImgRt, m_CellRt;
	CDC* m_PanDC, * m_BackDC, * m_ImgDC;

	COMokCell m_BdCell[361];

	int m_DolNo;
	stack<COMokCell*>m_Stack;
// 생성입니다.
public:
	COMokDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~COMokDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OMOK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CDC* DeleteMemDC(CDC* pDC);
	void ProgramInit();
	CDC* MakeMemDC(CRect rt);
	void Bitmap_Load(CDC* pDC, CRect rt, UINT nID);
	COMokCell* GetCell(CPoint xy);
	int isGameFinish(COMokCell* pCell);
	int FiveCheck(COMokCell* pCell);
	CString m_strResult;
	void SetResult();
	int m_winBlack;
	int m_winWhite;
	int m_draw;

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonRestart();;
};
