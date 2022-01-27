#include "pch.h"
#include "OMokCell.h"


COMokCell::COMokCell()
{

}

COMokCell::~COMokCell()
{
}

void COMokCell::Initialize(int no, CRect rt, CDC* pDC, CDC* bDC, CDC* iDC)
{
	m_DolNo = NO_DOL;
	m_No = no;
	m_XY.x = no % LINE_CNT;
	m_XY.y = no / LINE_CNT;

	rt.OffsetRect(CELL_SIZE * m_XY.x, CELL_SIZE * m_XY.y);
	m_Rt = rt;

	m_pDrawDC = pDC;
	m_pBackDC = bDC;
	m_pImgDC = iDC;

	rt.MoveToXY(0, 0);
	for (int x = 0; x < 3; x++) {
		m_ImgRt[x] = rt;
		rt.OffsetRect(rt.Width(), 0);
	}
}


void COMokCell::DrawLine()
{
	CPoint cp = m_Rt.CenterPoint();
	int maxLine = LINE_CNT - 1;
	//x
	if (m_XY.x == 0)
	{
		m_pBackDC->MoveTo(cp);
		m_pBackDC->LineTo(m_Rt.right, cp.y);
	}
	else if (m_XY.x == maxLine)
	{
		m_pBackDC->MoveTo(cp);
		m_pBackDC->LineTo(m_Rt.left, cp.y);
	}
	else
	{
		m_pBackDC->MoveTo(m_Rt.left, cp.y);
		m_pBackDC->LineTo(m_Rt.right, cp.y);
	}

	//y
	if (m_XY.y == 0)
	{
		m_pBackDC->MoveTo(cp);
		m_pBackDC->LineTo(cp.x, m_Rt.bottom);
	}
	else if (m_XY.y == maxLine)
	{
		m_pBackDC->MoveTo(cp);
		m_pBackDC->LineTo(cp.x, m_Rt.top);
	}
	else
	{
		m_pBackDC->MoveTo(cp.x, m_Rt.top);
		m_pBackDC->LineTo(cp.x, m_Rt.bottom);
	}

	CRect DotRt(0, 0, 10, 10);
	DotRt.OffsetRect(m_Rt.CenterPoint() - DotRt.CenterPoint());

	if ((m_XY.x == 3 || m_XY.x == 9 || m_XY.x == 15) && (m_XY.y == 3 || m_XY.y == 9 || m_XY.y == 15))
	{
		m_pBackDC->SelectStockObject(BLACK_BRUSH);
		m_pBackDC->Ellipse(DotRt);
	}
}

bool COMokCell::DrawStone(int stoneNo, int number, int susunFlag)
{
	if (m_DolNo != NO_DOL)
	{
		return false;
	}

	m_DolNo = stoneNo;
	m_Susun = number;

	PutStone(susunFlag);

	return true;
}

bool COMokCell::isPTinRect(CPoint xy)
{
	return m_Rt.PtInRect(xy);
}

COMokCell* COMokCell::GetAroundCell(int dir)
{
	int maxLine = LINE_CNT - 1;

	switch (dir)
	{
	case LEFT:
		if (this->m_XY.x == 0) return NULL;
		return (this - 1);

	case RIGHT:
		if (this->m_XY.x == maxLine) return NULL;
		return(this + 1);
	case UP:
		if (this->m_XY.y == 0) return NULL;
		return (this - LINE_CNT);
	case DOWN:
		if (this->m_XY.y == maxLine) return NULL;
		return (this + LINE_CNT);
	case LTOP:
		if (this->m_XY.x == 0 || this->m_XY.y == 0) return NULL;
		return(this - LINE_CNT - 1);
	case RTOP:
		if (this->m_XY.x == maxLine || this->m_XY.y == 0) return NULL;
		return (this - LINE_CNT + 1);

	case LBOTTOM:
		if (this->m_XY.x == 0 || this->m_XY.y == maxLine) return NULL;
		return (this + LINE_CNT - 1);
	case RBOTTOM:
		if (this->m_XY.x == maxLine || this->m_XY.y == maxLine) return NULL;
		return (this + LINE_CNT + 1);
	}

	return NULL;
}

void COMokCell::PutStone(int susunFlag)
{
	m_pDrawDC->BitBlt(m_Rt.left, m_Rt.top,
		m_Rt.Width(), m_Rt.Height(),
		m_pImgDC,
		m_ImgRt[STONE_MASK].left,
		m_ImgRt[STONE_MASK].top, SRCAND);

	m_pDrawDC->BitBlt(m_Rt.left, m_Rt.top,
		m_Rt.Width(), m_Rt.Height(),
		m_pImgDC,
		m_ImgRt[m_DolNo].left,
		m_ImgRt[m_DolNo].top, SRCINVERT);

	if (susunFlag)
	{
		CString str;
		str.Format(_T("%d"), m_Susun);

		m_pDrawDC->SetBkMode(TRANSPARENT);

		if (m_DolNo == STONE_BLACK)
			m_pDrawDC->SetTextColor(WHITE);

		if (m_DolNo == STONE_WHITE)
			m_pDrawDC->SetTextColor(BLACK);

		m_pDrawDC->DrawText(
			str, m_Rt,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
}

void COMokCell::Refresh(int susunFlag)
{
	if (m_DolNo != NO_DOL)
	{
		PutStone(susunFlag);
	}
}

void COMokCell::RemoveDol()
{
	m_DolNo = NO_DOL;
	m_pDrawDC->BitBlt(m_Rt.left, m_Rt.top,
		m_Rt.Width(), m_Rt.Height(),
		m_pBackDC,
		m_Rt.left, m_Rt.top, SRCCOPY);
}