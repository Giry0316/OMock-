#pragma once

#define		CELL_SIZE	40
#define		LINE_CNT	19
#define		BDStartPT	CPoint(10,10)
#define		NO_DOL		-1
#define		STONE_BLACK	0
#define		STONE_WHITE	1
#define		STONE_MASK	2


#define		LEFT		0
#define		RIGHT		1
#define		UP			2
#define		DOWN		3
#define		LTOP		4
#define		RTOP		5
#define		LBOTTOM		6
#define		RBOTTOM		7

#define		WIN			1

#define		BLACK	RGB(0,0,0)
#define		WHITE	RGB(255,255,255)

class COMokCell
{
public:
	int m_No;
	CPoint m_XY;
	CRect m_Rt;

	CDC* m_pDrawDC;
	CDC* m_pImgDC;
	CDC* m_pBackDC;
	CRect m_ImgRt[3];

	int m_DolNo;
	int m_GroupNo;
	int m_Susun;

public:
	COMokCell();
	~COMokCell();
	void Initialize(int no, CRect rt, CDC* pDC, CDC* bDC, CDC* iDC);
	void DrawLine();
	bool DrawStone(int stoneNo, int number, int susunFlag);
	bool isPTinRect(CPoint xy);
	COMokCell* GetAroundCell(int dir);
	void PutStone(int susunFlag);
	void Refresh(int susunFlag);
	void RemoveDol();
};

