#include <string>
#include "stdafx.h"
#include "client_MFC_new.h"
#include "client_MFC_newDlg.h"
#include "afxdialogex.h"

#include "CameraDlg.h"
#include "SonarDlg.h"
#include "SystemsDlg.h"
#include "MapDlg.h"
#include <time.h>
#include "Aria.h"
#include "ArNetworking.h"
#include "robot.h"

#include <vector>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <iostream.h>
#include <fstream.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#define TM 220
//#define sPro 200
#define dPro 0.6       // For MAP proportion <<  >>
#define rPro 0.6   
#define mdPro 0.6
#define faX 0.17
#define faY 0.7
#endif

//using namespace std;
using std::vector;
//vector< double > vec[1][4];     // vector< int > vcc(20)
int co=0,csPro=200,flag=0,readingNo=0,srX=0,srY=0,f1=0,px,py;
int staticMkey=0,currentMkey=0;
float ccx[1024],ccy[1024],ssx[1024],ssy[1024],zzx[10240],zzy[10240],lenX1,lenY1;//ccxNew[500][4],ccyNew[500][4];
float X1,X2,Y1,Y2,mX1,mX2,mY1,mY2,dX,dY,picX,picY,mX,mY,lenX,lenY,delX,delY,matchX1,matchY1,matchX2,matchY2;
float mlenX,mlenY,mlenX1,mlenY1,mdelX,mdelY,moveex=0,moveey=0,rotatee=1,zoome=0;
bool safedrive=0;
int sonarX=0,sonarY=0,readingNum=0;
int windowMaxX=452,windowMinX=6,windowMaxY=340,windowMinY=18;
double tempX=0,tempY=0,tempA=0,tempB=0,tempV=0,byX,byY;
CString Warning;
CString Global_IP;


class CAboutDlg : public CDialogEx
{
public:
CAboutDlg();

// 对话框数据
enum { IDD = IDD_ABOUTBOX };

protected:
virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Cclient_MFC_newDlg 对话框
Cclient_MFC_newDlg::Cclient_MFC_newDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(Cclient_MFC_newDlg::IDD, pParent)
{
m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
m_Xposition = 0.0;
m_Yposition = 0.0;
m_Angle = 0.0;
m_Voltage = 0.0;
m_Volocity = 0.0;
m_picDraw = _T("");
//memset(m_nzValues, 0, sizeof(int) * POINT_COUNT);
m_byX = 0;
m_byY = 0;
m_mouse = _T("");
}

void Cclient_MFC_newDlg::DoDataExchange(CDataExchange* pDX)
{
CDialogEx::DoDataExchange(pDX);
DDX_Control(pDX, IDC_CHECK1, m_SafeDrive);
DDX_Text(pDX, IDC_EDIT2, m_Xposition);
DDX_Text(pDX, IDC_EDIT3, m_Yposition);
DDX_Text(pDX, IDC_EDIT4, m_Angle);
DDX_Text(pDX, IDC_EDIT5, m_Voltage);
DDX_Text(pDX, IDC_EDIT6, m_Volocity);
DDX_Control(pDX, IDC_MAP_DRAW, m_mapDraw);
DDX_Text(pDX, IDC_EDIT7, m_byX);
DDX_Text(pDX, IDC_EDIT8, m_byY);
DDX_Control(pDX, IDC_IPADDRESS1, m_IP);
DDX_Control(pDX, IDC_EDIT1, m_log);
}

BEGIN_MESSAGE_MAP(Cclient_MFC_newDlg, CDialogEx)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BUTTON1, &Cclient_MFC_newDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON2, &Cclient_MFC_newDlg::OnBnClickedButton2)
ON_BN_CLICKED(IDC_BUTTON7, &Cclient_MFC_newDlg::OnBnClickedButton7)
ON_BN_CLICKED(IDC_BUTTON9, &Cclient_MFC_newDlg::OnBnClickedButton9)
ON_BN_CLICKED(IDC_BUTTON10, &Cclient_MFC_newDlg::OnBnClickedButton10)
ON_BN_CLICKED(IDC_BUTTON8, &Cclient_MFC_newDlg::OnBnClickedButton8)
ON_BN_CLICKED(IDC_BUTTON3, &Cclient_MFC_newDlg::OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON4, &Cclient_MFC_newDlg::OnBnClickedButton4)
ON_BN_CLICKED(IDC_BUTTON5, &Cclient_MFC_newDlg::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON6, &Cclient_MFC_newDlg::OnBnClickedButton6)
ON_WM_TIMER()
ON_STN_CLICKED(IDC_MAP_DRAW, &Cclient_MFC_newDlg::OnStnClickedMapDraw)
ON_BN_CLICKED(IDC_WAVE_DRAW, &Cclient_MFC_newDlg::OnBnClickedWaveDraw)
ON_BN_CLICKED(IDC_BUTTON12, &Cclient_MFC_newDlg::OnBnClickedButton12)
ON_BN_CLICKED(IDC_BUTTON13, &Cclient_MFC_newDlg::OnBnClickedButton13)
ON_BN_CLICKED(IDC_BUTTON14, &Cclient_MFC_newDlg::OnBnClickedButton14)
ON_BN_CLICKED(IDC_BUTTON15, &Cclient_MFC_newDlg::OnBnClickedButton15)
ON_BN_CLICKED(IDC_BUTTON16, &Cclient_MFC_newDlg::OnBnClickedButton16)
ON_BN_CLICKED(IDC_BUTTON17, &Cclient_MFC_newDlg::OnBnClickedButton17)
ON_BN_CLICKED(IDC_BUTTON18, &Cclient_MFC_newDlg::OnBnClickedButton18)
ON_BN_CLICKED(IDC_BUTTON25, &Cclient_MFC_newDlg::OnBnClickedButton25)
ON_BN_CLICKED(IDC_BUTTON26, &Cclient_MFC_newDlg::OnBnClickedButton26)
ON_BN_CLICKED(IDC_BUTTON19, &Cclient_MFC_newDlg::OnBnClickedButton19)
ON_BN_CLICKED(IDC_BUTTON21, &Cclient_MFC_newDlg::OnBnClickedButton21)
ON_BN_CLICKED(IDC_BUTTON24, &Cclient_MFC_newDlg::OnBnClickedButton24)
ON_BN_CLICKED(IDC_BUTTON22, &Cclient_MFC_newDlg::OnBnClickedButton22)
ON_BN_CLICKED(IDC_BUTTON20, &Cclient_MFC_newDlg::OnBnClickedButton20)
ON_BN_CLICKED(IDC_BUTTON23, &Cclient_MFC_newDlg::OnBnClickedButton23)
ON_BN_CLICKED(IDC_BUTTON27, &Cclient_MFC_newDlg::OnBnClickedButton27)
ON_BN_CLICKED(IDC_BUTTON28, &Cclient_MFC_newDlg::OnBnClickedButton28)
ON_BN_CLICKED(IDC_BUTTON29, &Cclient_MFC_newDlg::OnBnClickedButton29)
END_MESSAGE_MAP()


// Cclient_MFC_newDlg 消息处理程序
BOOL Cclient_MFC_newDlg::OnInitDialog()
{
CDialogEx::OnInitDialog();

// 将“关于...”菜单项添加到系统菜单中。
// IDM_ABOUTBOX 必须在系统命令范围内。
ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
ASSERT(IDM_ABOUTBOX < 0xF000);

CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

//  设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
//  执行此操作
SetIcon(m_hIcon, TRUE);	 // 设置大图标
SetIcon(m_hIcon, FALSE); // 设置小图标

// TODO: 在此添加额外的初始化代码
SetTimer(1, TM, NULL);  // set TIMER 
m_IP.SetAddress(192,168,244,128);
return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cclient_MFC_newDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

//  如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void Cclient_MFC_newDlg::OnPaint()
{
	if (IsIconic())
	{
	CPaintDC dc(this); // 用于绘制的设备上下文
	SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

	// 使图标在工作区矩形中居中
	int cxIcon = GetSystemMetrics(SM_CXICON);
	int cyIcon = GetSystemMetrics(SM_CYICON);
	CRect rect;
	GetClientRect(&rect);
	int x = (rect.Width() - cxIcon + 1) / 2;
	int y = (rect.Height() - cyIcon + 1) / 2;
	// 绘制图标
	dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
	CDialogEx::OnPaint();
	}
}


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cclient_MFC_newDlg::OnQueryDragIcon()
{
return static_cast<HCURSOR>(m_hIcon);
}


void Cclient_MFC_newDlg::OnBnClickedButton1()//client连接server
{
CString strx, m_temp; 
m_IP.GetWindowText(strx); 

AfxBeginThread(CaptureThread, this);	// start capture thread
AfxBeginThread(CaptureThread_1, this);  // start a new thread

UpdateData(true);
m_temp += strx;
Global_IP = strx;
m_temp += "\r\n";
m_log.SetWindowText(m_temp);
m_log.LineScroll(m_log.GetLineCount());
UpdateData(false);

SetTimer(1,TM,0);
}

//-------------------- Drawing robot & dots --------------------//
/*void Cclient_MFC_newDlg::DrawRobot(CDC *rDC, CRect &rectPicture)
{
int rX=0,rY=0;
CPen newPen;       // 用于创建新画笔   
CPen *pOldPen;     // 用于存放旧画笔    
// 创建实心画笔，粗度为1，颜色为绿色   
newPen.CreatePen(PS_SOLID, 5, RGB(255,0,0));   
// 选择新画笔，并将旧画笔的指针保存到pOldPen   
pOldPen = rDC->SelectObject(&newPen);  

rX= (rectPicture.Width()*0.17 + (tempX+lenX1)*delX)*dPro;
rY= (rectPicture.Height()*0.7 + (tempY+lenY1)*delY)*dPro;

	if(rX<420 && rX<420 && rX>20 && rX>20 && rY<340 && rY<340)
	{
	rDC->MoveTo(rX, rY);
	rDC->LineTo(rX, rY); 
	}

// 恢复旧画笔    
rDC->SelectObject(pOldPen);   
// 删除新画笔   
newPen.DeleteObject(); 
}*/
//-----------------//


//------------------ laser current data -------------------- //
void Cclient_MFC_newDlg::DrawWave(CDC *pDC, CRect &rectPicture)   
{   
float currentMX=0,currentMY=0;
CPen newPen;   // 用于创建新画笔   
CPen *pOldPen; // 用于存放旧画笔   
//CBrush newBrush;   // 用于创建新画刷   
//CBrush *pOldBrush; // 用于存放旧画刷  
//--- draw current MAP ---//
ArMap armap;              
ArLineSegment seg;
CString abc, cde;
abc = "error for loading";
//char* oldMapName = "laserData.map";
//armap.readFile(oldMapName);
/*   
newBrush.CreateSolidBrush(RGB(255,255,255)); // 创建黑色新画刷  
pOldBrush = pDC->SelectObject(&newBrush);    // 选择新画刷，并将旧画刷的指针保存到pOldBrush  
pDC->Rectangle(rectPicture);  // 以黑色画刷为绘图控件填充黑色，形成黑色背景   
pDC->SelectObject(pOldBrush); // 恢复旧画刷   
newBrush.DeleteObject();      // 删除新画刷  */
newPen.CreatePen(PS_SOLID, 2, RGB(0,249,0)); // 创建实心画笔，粗度为2，颜色为绿色    
pOldPen = pDC->SelectObject(&newPen); // 选择新画笔，并将旧画笔的指针保存到pOldPen 
			
	/*for(vector<ArLineSegment>::iterator it = armap.getLines()->begin();it != armap.getLines()->end();it++)
	{
	currentMX=(16000+moveex*currentMkey); // laser's largest data (scan range) will not   
	currentMY=(19000+moveey*currentMkey); // exceed 20000 for X or Y coordinates
	X1= (currentMX+(*it).getX1())/(100+zoome*currentMkey);
	Y1= (currentMY+(*it).getY1())/(100+zoome*currentMkey);
	X2= (currentMX+(*it).getX2())/(100+zoome*currentMkey);
	Y2= (currentMY+(*it).getY2())/(100+zoome*currentMkey);
	
		if(X1<windowMaxX && X2<windowMaxX && X1>windowMinX && X2>windowMinX && Y1<windowMaxY && Y2<windowMaxY && Y1>windowMinY && Y2>windowMinY)
		{
		pDC->MoveTo(X1, Y1);
		pDC->LineTo(X2, Y2); 
		}
	}*/

	for(int t=0;t<readingNo-1;t++)
	{
	currentMX=(16000+moveex*currentMkey); // laser's largest data (scan range) will not   
	currentMY=(19000+moveey*currentMkey); // exceed 20000 for X or Y coordinates
	X1= (currentMX+ccx[t])/(100+zoome*currentMkey);
	Y1= (currentMY+ccy[t])/(100+zoome*currentMkey);
	X2= (currentMX+ccx[t+1])/(100+zoome*currentMkey);
	Y2= (currentMY+ccy[t+1])/(100+zoome*currentMkey);
	
		if(X1<windowMaxX && X2<windowMaxX && X1>windowMinX && X2>windowMinX && Y1<windowMaxY && Y2<windowMaxY && Y1>windowMinY && Y2>windowMinY)
		{
		pDC->MoveTo(X1, Y1);
		pDC->LineTo(X2, Y2); 
		}
	}  

pDC->SelectObject(pOldPen); // 恢复旧画笔  
newPen.DeleteObject(); // 删除新画笔

//--- draw LX robot on the MAP ---//
int rX=0,rY=0;
CPen newPen_bot;    // 用于创建新画笔   
CPen *pOldPen_bot;  // 用于存放旧画笔    
newPen_bot.CreatePen(PS_SOLID, 5, RGB(255,0,0)); // 创建实心画笔，粗度为5，颜色为red   
pOldPen_bot = pDC->SelectObject(&newPen_bot);    // 选择新画笔，并将旧画笔的指针保存到pOldPen 

rX= (currentMX+tempX)/(100+zoome*currentMkey);
rY= (currentMY+tempY)/(100+zoome*currentMkey);

	if(rX<windowMaxX && rX>windowMinX && rY<windowMaxY && rY>windowMinY)
	{
	pDC->MoveTo(rX, rY);
	pDC->LineTo(rX, rY); 
	}  
pDC->SelectObject(pOldPen_bot);   
newPen_bot.DeleteObject(); 
} 
//---------------------//


//-------------- draw the MAP which comes from sever --------------//
void Cclient_MFC_newDlg::rawDataTrans(CDC *rData, CRect &rectPicture)
{
float staticMX=0,staticMY=0;
CPen newPen;       
CPen *pOldPen;    
CBrush newBrush;   
CBrush *pOldBrush;   
ArMap armap;             
ArLineSegment seg;
char* oldMapName = "AAA.map";

armap.readFile(oldMapName);
newBrush.CreateSolidBrush(RGB(255,255,255));   
pOldBrush = rData->SelectObject(&newBrush);   
rData->Rectangle(rectPicture);   
rData->SelectObject(pOldBrush);   
newBrush.DeleteObject();  
newPen.CreatePen(PS_SOLID, 1, RGB(0,0,0));     
pOldPen = rData->SelectObject(&newPen); 

	for(vector<ArLineSegment>::iterator itRD = armap.getLines()->begin();itRD != armap.getLines()->end();itRD++)
	{ 
	//staticMX=fabs(armap.getLineMaxPose().getX())+moveex*staticMkey;
	//staticMY=fabs(armap.getLineMaxPose().getY())+moveey*staticMkey;

	staticMX=16000+moveex*staticMkey;
	staticMY=19000+moveey*staticMkey;

	matchX1=(((*itRD).getX1()+staticMX)/(100+zoome*staticMkey));//*cos(rotatee);
	matchY1=(((*itRD).getY1()+staticMY)/(100+zoome*staticMkey));//*sin(rotatee);
	matchX2=(((*itRD).getX2()+staticMX)/(100+zoome*staticMkey));//*cos(rotatee);
	matchY2=(((*itRD).getY2()+staticMY)/(100+zoome*staticMkey));//*sin(rotatee);

		if(matchX1<windowMaxX && matchX2<windowMaxX && matchX1>windowMinX && matchX2>windowMinX && matchY1<windowMaxY && matchY2<windowMaxY && matchY1>windowMinY && matchY2>windowMinY)
		{
		rData->MoveTo(matchX1, matchY1);
		rData->LineTo(matchX2, matchY2); 
		}
	}
rData->SelectObject(pOldPen);   
newPen.DeleteObject(); 
}
//-----------------//


//---------------- Static MAP ----------------//
void CMapDlg::sMap(CDC *mDC, CRect &rectPicture)
{
int a1=0,b1=1;
CPen newPen;    // 用于创建新画笔   
CPen *pOldPen;  // 用于存放旧画笔   
CBrush newBrush;   // 用于创建新画刷   
CBrush *pOldBrush; // 用于存放旧画刷    
//--- 地图坐标获取 ---//
ArMap armap;              
ArLineSegment seg;
CString abc, cde;
abc = "error for loading static MAP";
char* oldMapName = "AAA.map";  //"columbia_ori.map";
armap.readFile(oldMapName);

newBrush.CreateSolidBrush(RGB(255,255,255));  // 选择新画刷，并将旧画刷的指针保存到pOldBrush    
pOldBrush = mDC->SelectObject(&newBrush);     // 以黑色画刷为绘图控件填充黑色，形成黑色背景   
mDC->Rectangle(rectPicture);  // 恢复旧画刷     
mDC->SelectObject(pOldBrush); // 删除新画刷   
newBrush.DeleteObject();  

newPen.CreatePen(PS_SOLID, 1.2, RGB(0,0,0));  // 创建实心画笔，粗度为1.2，颜色为黑色     
pOldPen = mDC->SelectObject(&newPen);  // 选择新画笔，并将旧画笔的指针保存到pOldPen  
			
	for(vector<ArLineSegment>::iterator itM = armap.getLines()->begin();itM != armap.getLines()->end();itM++)
	{
	zzx[a1]=(*itM).getX1();
	zzy[a1]=(*itM).getY1();      
	zzx[b1]=(*itM).getX2();       
	zzy[b1]=(*itM).getY2();   
		
	mlenX = armap.getLineMaxPose().getX() - armap.getLineMinPose().getX();
	mlenY = armap.getLineMaxPose().getY() - armap.getLineMinPose().getY();

	mlenX1 = (fabs(armap.getLineMaxPose().getX())+fabs(armap.getLineMinPose().getX()))/2;
	mlenY1 = (fabs(armap.getLineMaxPose().getY())+fabs(armap.getLineMinPose().getY()))/2;

	mdelX = ((float)rectPicture.Width() / mlenX);
	mdelY = ((float)rectPicture.Height() / mlenY);

	mX1= (rectPicture.Width()*0.1 + ((*itM).getX1()+mlenX1)*mdelX)*mdPro;
	mY1= (rectPicture.Height()*0.3 + ((*itM).getY1()+mlenY1)*mdelY)*mdPro;
	mX2= (rectPicture.Width()*0.1 + ((*itM).getX2()+mlenX1)*mdelX)*mdPro;
	mY2= (rectPicture.Height()*0.3 + ((*itM).getY2()+mlenY1)*mdelY)*mdPro;

	mDC->MoveTo(mX1, mY1);
	mDC->LineTo(mX2, mY2);

	a1=a1+2;
	b1=b1+2;
	f1=f1+1;
	}
    
mDC->SelectObject(pOldPen); // 恢复旧画笔  
newPen.DeleteObject();  // 删除新画笔   
}
//-----------------//


//-------------- Static MAP with BOT --------------//
void CMapDlg::smMap(CDC *smDC, CRect &rectPicture)
{
CPen newPen;    // 用于创建新画笔   
CPen *pOldPen;  // 用于存放旧画笔    
newPen.CreatePen(PS_SOLID, 7, RGB(255,0,0));   
pOldPen = smDC->SelectObject(&newPen);  

srX= (rectPicture.Width()*0.1 + (tempX+mlenX1)*mdelX)*mdPro;
srY= (rectPicture.Height()*0.3 + (tempY+mlenY1)*mdelY)*mdPro;
	   
smDC->MoveTo(srX, srY);
smDC->LineTo(srX, srY); 
   
smDC->SelectObject(pOldPen);    
newPen.DeleteObject(); 
}
//-----------------//


//---------- Mouse coordinate ----------//
void Cclient_MFC_newDlg::OnLButtonDblClk()
{
CRect rectP;
CPoint pointP;

//GetDlgItem(IDC_MAP_DRAW)->GetWindowRect(&rectP);
//ScreenToClient(&pointP);

GetDlgItem(IDC_MAP_DRAW)->GetClientRect(&rectP);
GetCursorPos(&pointP);
ScreenToClient(&pointP);

px=pointP.x;
py=pointP.y;
}
//--------//


//--- Callback functions for requests ---//
/*void Handler(ArNetPacket* packet)
{
char robotType[50]; char robotSubtype[50];
int width; int lengthFront; int lengthRear;
packet->bufToStr(robotType, sizeof(robotType));        // Robot type
packet->bufToStr(robotSubtype, sizeof(robotSubtype));  // Short name
width = packet->bufToByte2();        // Width
lengthFront = packet->bufToByte2();  // Front length
lengthRear = packet->bufToByte2();   // Rear length
fflush(stdout);
}*/
//----------//


//------- Sensor lists -------//
/*void sData (ArNetPacket* packet)
{
char nameLen1[50],nameLen2[50],nameLen3[50],nameLen4[50],nameLen5[50],nameLen6[50];
int sList;
sList = packet->bufToByte2();
packet->bufToStr(nameLen1, sizeof(nameLen1)); // forbidden
packet->bufToStr(nameLen2, sizeof(nameLen2)); // bumpers
packet->bufToStr(nameLen3, sizeof(nameLen3)); // irs
packet->bufToStr(nameLen4, sizeof(nameLen4)); // multiRobot
packet->bufToStr(nameLen5, sizeof(nameLen5)); // sim_S3Series_1
packet->bufToStr(nameLen6, sizeof(nameLen6)); // sonar
fflush(stdout);
}*/
//----------//


//---------- robotInfo ----------//
void robotInfo(ArNetPacket *packet)
{
/* Extract the data from the update packet. Its format is status and
* mode (null-terminated strings), then 6 doubles for battery voltage, 
* x position, y position and orientation (theta) (from odometry), current
* translational velocity, and current rotational velocity. Translation is
* always milimeters, rotation in degrees.
*/
double myX;
double myY;
double myTh;
double myVel;
double myRotVel;
double myVoltage;
char myStatus[256];
char myMode[32];

memset(myStatus, 0, sizeof(myStatus));
memset(myMode, 0, sizeof(myMode));
packet->bufToStr(myStatus, sizeof(myStatus));
packet->bufToStr(myMode, sizeof(myMode));
myVoltage = ( (double) packet->bufToByte2() )/10.0;
myX = (double) packet->bufToByte4();
myY = (double) packet->bufToByte4();
myTh = (double) packet->bufToByte2();
myVel = (double) packet->bufToByte2();
myRotVel = (double) packet->bufToByte2();

fflush(stdout);

tempX=myX;
tempY=myY;
tempA=myTh;
tempB=myVoltage;
tempV=myVel;
}
//-------//


//----- Realtime laser data -----//
void sonarData(ArNetPacket* packet)
{
using namespace std;
int cc=0;
char sName[50];
ofstream write;
ArMap armap;

readingNo = packet->bufToByte2();       // Number of reading
packet->bufToStr(sName, sizeof(sName)); // Sensor name

	for(int i=0;i<readingNo*2;i++)
	{ 
	sonarX = packet->bufToByte4();  // X coordinate
	sonarY = packet->bufToByte4();  // Y coordinate
	ccx[i] = sonarX;
	ccy[i] = sonarY;

	//ssx[cc] = sonarX;
	//ssy[cc] = sonarY;
	cc = cc + 1;
	}

/* sort the data in order to get max & min number */
/*sort(ssx,ssx+readingNo);
sort(ssy,ssy+readingNo);

//--- dots for current MAP ---//		
write.open("laserData.map");
write<<"2D-Map"<<endl;

write<<"LineMinPos: "<<ssx[0]<<" "<<ssy[0]<<endl;  // need to modify later
write<<"LineMaxPos: "<<ssx[readingNo-1]<<" "<<ssy[readingNo-1]<<endl;  // need to modify later

write<<"NumLines: "<<readingNo<<endl;
write<<"LinesAreSorted: "<<"true"<<endl;
write<<"LINES"<<endl;

	for(int q=0;q<readingNo;q++)  
	{
		if(ccx[q]!=0 && ccy[q]!=0 && ccx[q+1]!=0 && ccy[q+1]!=0)
		{
		write<<ccx[q]<<" "<<ccy[q]<<" "<<ccx[q+1]<<" "<<ccy[q+1]; // add laser sensor dots
		write<<endl;

		//ccxNew[q][0]=ccx[q];
		//ccxNew[q][1]=ccy[q];
		//ccxNew[q][2]=ccx[q+1];
		//ccxNew[q][3]=ccy[q+1];
		}
	}
write<<"DATA"<<endl;

write.close();*/
fflush(stdout);
}
//----------//


//--- CB for goals infomation ---//
void goalInfo(ArNetPacket* packet)
{
char g1[20],g2[20];
packet->bufToStr(g1, sizeof(g1));
packet->bufToStr(g2, sizeof(g2));

fflush(stdout);   
}
//----------//


//------- Get static MAP ------//
void mapInfo(ArNetPacket* packet)
{
using namespace std;
ofstream write;
char mapD[200], *mp, *rt;
mp=mapD;
write.open("AAA.map",ios::app);

memset(mapD,0,sizeof(mapD));
packet->bufToStr(mapD, sizeof(mapD));
rt=mapD;

write<<mp;
write<<endl;
write.close(); 
fflush(stdout);
}
//----------//


/*void getConfig(ArNetPacket* packet)
{
char c1[128],c2[128],c3[128];
ArClientBase client;
ArClientHandlerConfig clientConfig(&client,Aria::getConfig());

clientConfig.lock();
//clientConfig.getConfig();

	while(1)
	{
	//clientConfig.handleGetConfigBySections(packet);
	packet->bufToStr(c1,sizeof(c1));
	packet->bufToStr(c2,sizeof(c2));
	packet->bufToStr(c3,sizeof(c3));

	fflush(stdout);
	}
clientConfig.unlock();
}*/


//--------------------- New thread -------------------//
UINT Cclient_MFC_newDlg::CaptureThread_1(LPVOID Param_1)
{
Cclient_MFC_newDlg *dlg_1;
dlg_1=(Cclient_MFC_newDlg*)Param_1; // Get pointer to parent dialog
dlg_1->IsThreadRunning=true;

using namespace std;
ofstream write;

	while(1)
	{
    //--- record laser data ---//
	/*write.open("ccxy.txt");
	for(int w=0;w<500;w++)
	{
	write<<ccx[w]<<" "<<ccy[w];    // realtime data for Current Laser
	write<<endl;
	}
	write.close();
	ArUtil::sleep(50);

	//--- record MAP data ---//	
	write.open("zzxy.txt");
	for(int v=0;v<10240;v++)
	{
	write<<zzx[v]<<" "<<zzy[v];    // realtime data for a Static MAP
	write<<endl;
	}
	write.close();*/
	
	/*POINT pt; 
	if(GetCursorPos(&pt)) // mouse pointer
	{
	px=pt.x;
	py=pt.y;
	}*/
	    /* flag for handle image displayed on picture control zone */
		switch(flag)
		{
		case 20:zoome=zoome+20;flag=0;break;
		case 21:zoome=zoome-20;flag=0;break;

		case 22:moveey=moveey-2000;flag=0;break;
		case 23:moveex=moveex-2000;flag=0;break;
		case 25:moveex=moveex+2000;flag=0;break;
		case 26:moveey=moveey+2000;flag=0;break;

		case 24:rotatee=340;break;
		case 28:rotatee=20;break;
		}
		ArUtil::sleep(200);
	}
return flag;
}
//-------//


//---------- main thread of the program -----------//
UINT Cclient_MFC_newDlg::CaptureThread(LPVOID lParam)
{
Cclient_MFC_newDlg *dlg; // Capture Thread 
dlg=(Cclient_MFC_newDlg*)lParam; // Get pointer to parent dialog
dlg->IsThreadRunning=true;

Aria::init();
int argc=5; char *argv[6]; //argc是字符串个数，argv是字符串内容
argv[0]="./client_MFC_new.exe";
argv[1]="-host";                                                           
argv[2]=(LPSTR)(LPCTSTR)Global_IP; //Sim-192.168.244.128/ LX-213.111     
argv[3]="-port";                                                          
argv[4]="7272";
argv[5]=0;

/* Create our client object. This is the object which connects with a remote
* server over the network, and which manages all of our communication with it
* once connected by sending data "requests".  Requests may be sent once, or
* may be repeated at any frequency. Requests and replies to requsets contain 
* payload "packets", into which various data types may be packed (when making a 
* request), and from which they may also be extracted (when handling a reply). 
* See the InputHandler and OutputHandler classes above for
* examples of making requests and reading/writing the data in packets.
*/
ArClientBase client;

/* Aria components use this to get options off the command line: */
ArArgumentParser parser(&argc, argv);

/* This will be used to connect our client to the server, including
* various bits of handshaking (e.g. sending a password, retrieving a list
* of data requests and commands...)
* It will get the hostname from the -host command line argument: */
ArClientSimpleConnector clientConnector(&parser);

parser.loadDefaultArguments();

/* Check for -help, and unhandled arguments: */
if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
{
Aria::logOptions();
exit(0);
}
  
/* Connect our client object to the remote server: */	
if (!clientConnector.connectClient(&client))
{
if (client.wasRejected())
Warning="Server rejected connection, exiting ";
else
Warning = "Could not connect to server, exiting ";
exit(1);
}
client.runAsync();

ArKeyHandler keyHandler;
Aria::setKeyHandler(&keyHandler);
ArGlobalFunctor escapeCB(&escape);
keyHandler.addKeyHandler(ArKeyHandler::ESCAPE, &escapeCB);

InputHandler inputHandler(&client);
inputHandler.safeDrive();
OutputHandler outputHandler(&client);

/* Associate each request with related callback function, here uses the ArGlobalFunctor1 */
ArNetPacket pkt, requestPacket;
/*ArGlobalFunctor1<ArNetPacket *> HandlerCB(&Handler);
client.addHandler("physicalInfo", &HandlerCB);  

ArGlobalFunctor1<ArNetPacket *> sDataCB(&sData);
client.addHandler("getSensorList", &sDataCB); */ 

ArGlobalFunctor1<ArNetPacket *> robotInfoCB(&robotInfo);
client.addHandler("update", &robotInfoCB);

ArGlobalFunctor1<ArNetPacket *> sonarDataCB(&sonarData);
client.addHandler("getSensorCurrent",&sonarDataCB);

ArGlobalFunctor1<ArNetPacket *> goalInfoCB(&goalInfo);
client.addHandler("getMapName",&goalInfoCB);

ArGlobalFunctor1<ArNetPacket *> mapInfoCB(&mapInfo);
client.addHandler("getMap", &mapInfoCB);

/*ArGlobalFunctor1<ArNetPacket *> getConfigCB(&getConfig);
client.addHandler("getConfigBySections", &getConfigCB);*/

//--- sending the requests as sequence---// 
//client.requestOnce("physicalInfo");

//client.requestOnce("getSensorList");

client.request("update",TM-2); 

requestPacket.strToBuf("sim_S3Series_1"); // sim_S3Series_1 for mobileSim use only
client.request("getSensorCurrent",TM-2,&requestPacket); 

	/*if(flag==16)
	{
	client.requestOnce("getMapName");
	client.requestOnce("getMap");
	}*/

//client.runAsync();

/*pk.byte4ToBuf(0);
pk.byte4ToBuf(0);
pk.byte4ToBuf(0);  //gotoPose
client.request("gotoGoal",10000,&pkt);*/

//--- check whether the request exists ---/
/*if(client.dataExists("setGoals")) 
{
client.requestOnce("setGoals");
}*/
//top:
	while(client.getRunningWithLock())
	{ 
		switch(flag)
		{		
		case 1:if(WM_LBUTTONDOWN) inputHandler.up();ArUtil::sleep(220);break;
		case 2:if(WM_LBUTTONDOWN) inputHandler.down();ArUtil::sleep(160);break;
		case 3:inputHandler.left();ArUtil::sleep(50);break;
		case 4:inputHandler.right();ArUtil::sleep(50);break;
		case 5:client.requestOnce("stop");flag=0;break;  //inputHandler.doStop();flag=0;break;
		
		case 11:client.requestOnce("localizeToPose");flag=0;break;
		case 12:pkt.strToBuf("Workshop");client.requestOnce("gotoGoal",&pkt);ArUtil::sleep((TM-2)*10);break;
		case 14:client.requestOnce("wander");ArUtil::sleep(2600);break;
		case 13:client.requestStop("gotoGoal");flag=0;break; 
		}

	if(safedrive){inputHandler.unsafeDrive();} //安全模式
	else{inputHandler.safeDrive();}
		
	inputHandler.sendInput();
	/* make sure both point to same robot */
	
	if(inputHandler.getClient()!=outputHandler.getClient()){outputHandler.setClient( inputHandler.getClient() );}
	ArUtil::sleep(100);

	/*tempX=outputHandler.myX;
	tempY=outputHandler.myY;
	tempA=outputHandler.myTh;
	tempB=outputHandler.myVoltage;
	tempV=outputHandler.myVel;*/

		/*if(flag==12)
		{
		pkt.strToBuf("Workshop");
		client.requestOnce("gotoGoal",&pkt);
		client.run();
		}*/
		/*else if(flag==14)
		{
		client.requestOnce("wander");
		ArUtil::sleep(1600);
		}*/
	}	
//goto top;
}
//------//


//------------------ Handle the cameral stream --------------------//
void Cclient_MFC_newDlg::OnBnClickedButton7()//开启摄像头非模态对话框
{
//非模态对话框标准开启格式
/*
CCameraDlg *dlg = new CCameraDlg;
dlg->Create(IDD_DIALOG1);
dlg->ShowWindow(SW_SHOW);
*/
WinExec("ddddemo1.exe", SW_SHOW);
}
//----------------------------//


//------------- Disconnect from server --------------//
void Cclient_MFC_newDlg::OnBnClickedButton8()//断开连接
{
Aria::shutdown();
}
//------------//


void Cclient_MFC_newDlg::OnBnClickedButton9()//声纳数据读取
{
CSonarDlg *dlg2 = new CSonarDlg;
dlg2->Create(IDD_DIALOG2);
dlg2->ShowWindow(SW_SHOW);
}

void Cclient_MFC_newDlg::OnBnClickedButton10()
{
CSystemsDlg *dlg3 = new CSystemsDlg;
dlg3->Create(IDD_DIALOG3); 
dlg3->ShowWindow(SW_SHOW);
}


//------------------- Data refresh by using timer -------------------//
void Cclient_MFC_newDlg::OnTimer(UINT_PTR nIDEvent)//定时器消息处理函数
{
UpdateData(true);

m_Xposition = tempX;
m_Yposition = tempY;
m_Angle = tempA;
m_Voltage = tempB;
m_Volocity = tempV;
m_byX = px;  // coordinates for mouse point
m_byY = py;
UpdateData(false);

if(m_SafeDrive.GetCheck()){safedrive=true;}
else{safedrive=false;}

//--- display the MAP on "picture control" zone ---//
CRect rectPicture;   

/* 客户区坐标以窗口的左上角为原点，这区别于以屏幕左上角为原点的屏幕坐标 */ 
m_mapDraw.GetClientRect(&rectPicture);   
  
/* 绘制波形图 */ 
DrawWave(m_mapDraw.GetDC(), rectPicture);   // drawing current MAP dots
rawDataTrans(m_mapDraw.GetDC(), rectPicture); // drawing static MAP 
//DrawRobot(m_mapDraw.GetDC(), rectPicture);    // drawing the robot on MAP
//smMap(m_staticMapDraw.GetDC(), rectPicture);  // drawing the static MAP	

if(flag==18){OnLButtonDblClk();}
CDialogEx::OnTimer(nIDEvent);
}
//------------------------//


//-------------- for buttons ---------------//
void Cclient_MFC_newDlg::OnStnClickedMapDraw()
{
	
}

void Cclient_MFC_newDlg::OnBnClickedWaveDraw()
{
	
}
    
void Cclient_MFC_newDlg::OnBnClickedButton2()//机器人前进
{
flag=1;
}

void Cclient_MFC_newDlg::OnBnClickedButton4()//机器人向后
{
flag=2;
}

void Cclient_MFC_newDlg::OnBnClickedButton3()//机器人向左
{
flag=3;
}

void Cclient_MFC_newDlg::OnBnClickedButton5()//机器人向右
{
flag=4;
}

void Cclient_MFC_newDlg::OnBnClickedButton6()//机器人停止
{
flag=5;
}
void Cclient_MFC_newDlg::OnBnClickedButton12()
{
flag=14;    // navigation
}
														
void Cclient_MFC_newDlg::OnBnClickedButton13()
{
flag=13;    // control
}

void Cclient_MFC_newDlg::OnBnClickedButton14()
{
flag=9;
CMapDlg *dlg3 = new CMapDlg;
dlg3->Create(IDD_DIALOG4); 
dlg3->ShowWindow(SW_SHOW);
}

void Cclient_MFC_newDlg::OnBnClickedButton15()
{
flag=11;    // home position request
}

void Cclient_MFC_newDlg::OnBnClickedButton16() // wander
{
flag=12;    // goto pose
}

void Cclient_MFC_newDlg::OnBnClickedButton17()
{
flag=16; //load MAP
}

void Cclient_MFC_newDlg::OnBnClickedButton18()
{
flag=18;  // localization
}

void Cclient_MFC_newDlg::OnBnClickedButton25()
{
flag=20;
}

void Cclient_MFC_newDlg::OnBnClickedButton26()
{
flag=21;
}

void Cclient_MFC_newDlg::OnBnClickedButton19()
{
flag=22;
}

void Cclient_MFC_newDlg::OnBnClickedButton21()
{
flag=23;
}

void Cclient_MFC_newDlg::OnBnClickedButton24()
{
flag=24;
}

void Cclient_MFC_newDlg::OnBnClickedButton22()
{
flag=25;
}

void Cclient_MFC_newDlg::OnBnClickedButton20()
{
flag=26;
}

void Cclient_MFC_newDlg::OnBnClickedButton23()
{
flag=27;
moveex=0,moveey=0,rotatee=1,zoome=0;
}

void Cclient_MFC_newDlg::OnBnClickedButton27()
{
flag=28;
}

void Cclient_MFC_newDlg::OnBnClickedButton28()
{
staticMkey=1;
//currentMkey=0;
}

void Cclient_MFC_newDlg::OnBnClickedButton29()
{
//staticMkey=0;
currentMkey=1;
}
//---------//