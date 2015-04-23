// MapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChinaMobile.h"
#include "MapDlg.h"
#include "afxdialogex.h"
#include "Aria.h"
#include <cmath>

// MapDlg 对话框

IMPLEMENT_DYNAMIC(MapDlg, CDialogEx)

MapDlg::MapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(MapDlg::IDD, pParent)
{

}

MapDlg::~MapDlg()
{
}

void MapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_newMap, m_newMapDisplay);
}


BEGIN_MESSAGE_MAP(MapDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &MapDlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


//--- loading a new MAP ---//
void MapDlg::OnBnClickedOk()
{
	SetTimer(2, 1000, NULL); 

	CRect rectPicture;  
	m_newMapDisplay.GetClientRect(&rectPicture);   
	newMap(m_newMapDisplay.GetDC(), rectPicture);   // display new MAP
	//newMapBot(m_newMapDisplay.GetDC(), rectPicture);
}


void MapDlg::OnTimer(UINT_PTR nIDEvent)
{
	CRect rectPicture;
	switch(nIDEvent)
	{
	case 2: m_newMapDisplay.GetClientRect(&rectPicture); 
			newMapBot(m_newMapDisplay.GetDC(), rectPicture);
			break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

