
// OMokDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "OMok.h"
#include "OMokDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COMokDlg 대화 상자



COMokDlg::COMokDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OMOK_DIALOG, pParent)
	,m_strResult(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_winBlack = 0;
	m_winWhite = 0;
	m_draw = 0;
}

COMokDlg::~COMokDlg()
{
	DeleteMemDC(m_PanDC);
	DeleteMemDC(m_BackDC);
	DeleteMemDC(m_ImgDC);
}

void COMokDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_RESULT, m_strResult);
	DDX_Control(pDX, IDC_CHECK, m_Check);
}

BEGIN_MESSAGE_MAP(COMokDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_RESTART,
		&COMokDlg::OnBnClickedButtonRestart)
//	ON_BN_CLICKED(IDC_CHECK, &COMokDlg::OnBnClickedCheck)
//ON_BN_CLICKED(IDC_CHECK, &COMokDlg::OnBnClickedCheck)
ON_BN_CLICKED(IDC_CHECK, &COMokDlg::OnBnClickedCheck)
ON_BN_CLICKED(IDC_BUTTON_RETURN, &COMokDlg::OnBnClickedButtonReturn)
END_MESSAGE_MAP()


// COMokDlg 메시지 처리기

BOOL COMokDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetResult();
	ProgramInit();
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void COMokDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void COMokDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	dc.BitBlt(m_BoardRt.left, m_BoardRt.top, m_BoardRt.Width(), m_BoardRt.Height(), m_PanDC, 0, 0, SRCCOPY);
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR COMokDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CDC* COMokDlg::DeleteMemDC(CDC* pDC)
{
	CBitmap* pBit = pDC->GetCurrentBitmap();
	pBit->DeleteObject();
	pDC->DeleteDC();

	delete pBit;
	delete pDC;
	return pDC;
}

CDC* COMokDlg::MakeMemDC(CRect rt)
{
	CDC* pDC = new CDC;//cdc 만들기
	CBitmap* pBit = new CBitmap;//bitmap 만들기
	CWindowDC sdc(NULL);//window dc 만들기
	pDC->CreateCompatibleDC(&sdc);//cdc로 window dc지정
	pBit->CreateCompatibleBitmap(&sdc, rt.Width(), rt.Height());//rect의 크기만큼으로 윈도우 dc에 저장
	pDC->SelectObject(pBit);//비트맵 지정
	return pDC;//cdc만들어서 반환
}

void COMokDlg::Bitmap_Load(CDC* pDC, CRect rt, UINT nID)
{
	CDC mDC;
	CBitmap mBit;
	BITMAP bm;

	mBit.LoadBitmapW(nID);
	mBit.GetObject(sizeof(bm), &bm);

	mDC.CreateCompatibleDC(pDC);
	mDC.SelectObject(&mBit);

	pDC->SetStretchBltMode(HALFTONE);
	pDC->StretchBlt(rt.left, rt.top, rt.Width(), rt.Height(), &mDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	mDC.DeleteDC();
}

void COMokDlg::ProgramInit()
{
	m_DolNo = STONE_BLACK;//돌의 색
	m_CellRt = CRect(0, 0, CELL_SIZE, CELL_SIZE);
	m_BoardRt = m_BackRt = m_PanRt = CRect(0, 0, m_CellRt.Width() * LINE_CNT, m_CellRt.Width() * LINE_CNT);
	m_BoardRt.OffsetRect(BDStartPT);//(10,10)으로 만큼 옮긴다.
	m_ImgRt = CRect(0, 0, m_CellRt.Width() *3 , m_CellRt.Height());

	CWindowDC sdc(NULL);

	m_PanDC = MakeMemDC(m_PanRt);//make memdc 함수로 rt를 cdc에 저장
	m_BackDC = MakeMemDC(m_BackRt);
	m_ImgDC = MakeMemDC(m_ImgRt);

	Bitmap_Load(m_BackDC, m_BackRt, IDB_BACKIMG);
	Bitmap_Load(m_ImgDC, m_ImgRt, IDB_STONES2);
	
	m_SusunFlag = FALSE;
	m_Susun = 0;

	for (int no = 0; no < 361; no++) {
		m_BdCell[no].Initialize(no, m_CellRt, m_PanDC, m_BackDC, m_ImgDC);
		m_BdCell[no].DrawLine();
	}
	for (int no = 0; no < 361; no++) {
		numberCell[no].Initialize(no, m_CellRt, m_PanDC, m_BackDC, m_ImgDC);
		numberCell[no].DrawLine();
	}
	m_PanDC->BitBlt(m_PanRt.left, m_PanRt.top, m_PanRt.Width(), m_PanRt.Height(), m_BackDC, m_PanRt.left, m_PanRt.top, SRCCOPY);
	a = 0;
	m_Check.SetCheck(0);
	for (int i = 0; i < 362; i++)
	{
		number[i] = NULL;
	}
}

COMokCell* COMokDlg::GetCell(CPoint xy)
{
	xy -= BDStartPT;

	for (int no = 0; no < 361; no++)
	{
		COMokCell* pCell = &m_BdCell[no];
		if (pCell->isPTinRect(xy) != NULL) return pCell;
	}

	return nullptr;
}

void COMokDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	COMokCell* pCell = GetCell(point);

	if ((10 < point.x && point.x < 40 * 40 * 19) && (10 < point.y && point.y < 40 * 40 * 19))
	{
		int ret;
		if (pCell)
		{
			ret = pCell->DrawStone(m_DolNo, m_Susun + 1, m_SusunFlag);
			if (ret == false)
			{
				MessageBox(_T("이미 착점이 있습니다"), _T("알림"), MB_OK);
			}
			else
			{
				m_Stack.push(pCell);
				m_Susun++;
				Invalidate(FALSE);
				if (isGameFinish(pCell) == WIN) {
					int result = MessageBox(_T("게임을 다시 시작하시겠습니다?"), _T("알림"), MB_OKCANCEL);
					if (result == IDOK) {
						DeleteMemDC(m_PanDC);
						DeleteMemDC(m_BackDC);
						DeleteMemDC(m_ImgDC);
						ProgramInit();
						Invalidate(FALSE);
					}
				}
				m_DolNo = !m_DolNo;
			}
		}
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}



void COMokDlg::OnBnClickedCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.'
	
	m_SusunFlag = !m_SusunFlag;

	int max = LINE_CNT * LINE_CNT;

	for (int no = 0; no < max; no++)
	{
		COMokCell* pCell = &m_BdCell[no];
		pCell->Refresh(m_SusunFlag);
	}
	Invalidate(FALSE);

}



int COMokDlg::isGameFinish(COMokCell* pCell)
{
	if (pCell->m_DolNo == STONE_MASK) return -1;
	if (FiveCheck(pCell) == 5) {
		CString str;
		if (pCell->m_DolNo == STONE_BLACK) {
			str = _T("흑돌이 이겼습니다!");
			m_winBlack++;
		}
		else {
			str = _T("백돌이 이겼습니다!");
			m_winWhite++;
		}
		SetResult();
		MessageBox(str, _T("알림"), MB_OK);
		return WIN;
	}
	return 0;
}

int COMokDlg::FiveCheck(COMokCell* pCell)
{
	int checkWin[8] = { 0, };
	COMokCell* pStartCell, * pAroundCell;
	for (int dir = 0; dir < 8; dir++)
	{
		pStartCell = pCell;
		while ((pAroundCell = pStartCell->GetAroundCell(dir)) != NULL && pAroundCell->m_DolNo == pCell->m_DolNo)
		{
			checkWin[dir]++;
			pStartCell = pAroundCell;
		}
		if (checkWin[dir] == 4) return 5;
	}
	if (checkWin[UP] + checkWin[DOWN] == 4)return 5;
	if (checkWin[LEFT] + checkWin[RIGHT] == 4)return 5;
	if (checkWin[LTOP] + checkWin[RBOTTOM] == 4)return 5;
	if (checkWin[RTOP] + checkWin[LBOTTOM] == 4)return 5;

	return 0;
}


void COMokDlg::OnBnClickedButtonRestart()
{
	m_draw++;
	ProgramInit();
	Invalidate(FALSE);
}

void COMokDlg::SetResult()
{
	UpdateData(TRUE);
	m_strResult.Format(L"흑승 : %d회\n백승 : %d회\n무승부 : %d회", m_winBlack, m_winWhite, m_draw);
	UpdateData(FALSE);
}

void COMokDlg::OnBnClickedButtonReturn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_Stack.size() == 0)
	{
		MessageBox(L"처음입니다", MB_OK);
		return;
	}

	COMokCell* ReturnCell;
	ReturnCell = m_Stack.top();
	m_Stack.pop();
	m_Susun--;
	ReturnCell->RemoveDol();
	Invalidate(FALSE);
	m_DolNo = !m_DolNo;
}