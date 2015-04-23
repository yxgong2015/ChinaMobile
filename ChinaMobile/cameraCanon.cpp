// cameraCanon.cpp : 实现文件
//

#include "stdafx.h"
#include "ChinaMobile.h"
#include "cameraCanon.h"
#include "afxdialogex.h"


// cameraCanon 对话框

IMPLEMENT_DYNAMIC(cameraCanon, CDialogEx)

cameraCanon::cameraCanon(CWnd* pParent /*=NULL*/)
	: CDialogEx(cameraCanon::IDD, pParent)
{

}

cameraCanon::~cameraCanon()
{
}

void cameraCanon::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cameraCanon, CDialogEx)
END_MESSAGE_MAP()


// cameraCanon 消息处理程序
