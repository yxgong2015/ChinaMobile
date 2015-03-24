/* ------------------------------------------------------- *
 * Last modification date 2015/3/24 17:49  by: Xiaoxi Gong *
 * ------------------------------------------------------- *
*/
/*#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>*/

#include "stdafx.h"
#include "ChinaMobile.h"
#include "ChinaMobileDlg.h"
#include "afxdialogex.h"

#include <windows.h>
#include <iostream.h>
#include <fstream.h>
#include <vector>
#include <math.h>

#include "Aria.h"
#include "ArNetworking.h"
#include "robot.h"
#include "MapDlg.h"
#include "taskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define TM 50
#endif

int flag=0,tFlag=0,laserReadingNo=0,laserReadingX[512],laserReadingY[512],pathReadingNo=0,pathReadingX[256],pathReadingY[256];
double tempX=0,tempY=0,tempA=0,tempB=0,tempV=0,mousePoseX=0,mousePoseY=0,localScore=0;
float zoomMap=0,shiftX=0,shiftY=0,robot_Th=0, windowMinX=3,windowMinY=4, windowMaxX=481,windowMaxY=371;
bool safedrive=0;
char ls_handler;

CString Global_IP,Warning,Goal_name,Goal_list,local_Status,map_name,task_Goal;
std::vector<ArLineSegment> sMap;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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


// CChinaMobileDlg 对话框




CChinaMobileDlg::CChinaMobileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChinaMobileDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Velocity = 0.0;
	m_Xpose = 0.0;
	m_Ypose = 0.0;
	m_Th = 0.0;
	m_Voltage = 0.0;
	m_px = 0.0;
	m_py = 0.0;
	m_score = 0.0;
	m_status = _T("");
	m_mapName = _T("");
}

void CChinaMobileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IP);
	DDX_Control(pDX, IDC_EDIT1, m_log);
	DDX_Control(pDX, IDC_CHECK1, m_safeDrive);
	DDX_Text(pDX, IDC_EDIT2, m_Velocity);
	DDX_Text(pDX, IDC_EDIT3, m_Xpose);
	DDX_Text(pDX, IDC_EDIT4, m_Ypose);
	DDX_Text(pDX, IDC_EDIT5, m_Th);
	DDX_Text(pDX, IDC_EDIT6, m_Voltage);
	DDX_Control(pDX, IDC_serverMap, m_serverMap);
	DDX_Text(pDX, IDC_EDIT7, m_px);
	DDX_Text(pDX, IDC_EDIT8, m_py);
	DDX_Control(pDX, IDC_EDIT9, m_goal);
	DDX_Text(pDX, IDC_EDIT10, m_score);
	DDX_Text(pDX, IDC_EDIT11, m_status);
	DDX_Text(pDX, IDC_EDIT12, m_mapName);
}

	BEGIN_MESSAGE_MAP(CChinaMobileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CChinaMobileDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CChinaMobileDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CChinaMobileDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CChinaMobileDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CChinaMobileDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CChinaMobileDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CChinaMobileDlg::OnBnClickedButton7)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON8, &CChinaMobileDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON11, &CChinaMobileDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON9, &CChinaMobileDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON12, &CChinaMobileDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CChinaMobileDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CChinaMobileDlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CChinaMobileDlg::OnBnClickedButton15)
	ON_BN_CLICKED(IDOK, &CChinaMobileDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CChinaMobileDlg::OnBnClickedCancel)
	//ON_BN_CLICKED(IDC_BUTTON16, &CChinaMobileDlg::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON17, &CChinaMobileDlg::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON18, &CChinaMobileDlg::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON19, &CChinaMobileDlg::OnBnClickedButton19)
	ON_BN_CLICKED(IDC_BUTTON20, &CChinaMobileDlg::OnBnClickedButton20)
	ON_BN_CLICKED(IDC_BUTTON21, &CChinaMobileDlg::OnBnClickedButton21)
	ON_BN_CLICKED(IDC_BUTTON22, &CChinaMobileDlg::OnBnClickedButton22)
	ON_BN_CLICKED(IDC_BUTTON10, &CChinaMobileDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON23, &CChinaMobileDlg::OnBnClickedButton23)
	ON_BN_CLICKED(IDC_BUTTON25, &CChinaMobileDlg::OnBnClickedButton25)
	ON_BN_CLICKED(IDC_BUTTON16, &CChinaMobileDlg::OnBnClickedButton16)
	END_MESSAGE_MAP()


// CChinaMobileDlg 消息处理程序

BOOL CChinaMobileDlg::OnInitDialog()
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetTimer(1, TM*4, NULL);  // set TIMER 
	m_IP.SetAddress(192,168,244,128);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChinaMobileDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChinaMobileDlg::OnPaint()
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
HCURSOR CChinaMobileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//------------------ Draw MAP & laser points -------------------//
void CChinaMobileDlg::drawServerMap(CDC *pDC, CRect &rectPicture)
{
	/* define all objects following */
	using std::vector;
	float serverMapX1=0,serverMapX2=0,serverMapY1=0,serverMapY2=0, patX1=0,patY1=0,currentLaserX1=0,currentLaserY1=0;
	double headX1=0,headY1=0,headX2=0,headY2=0;
	//float currentLaserX2=0,currentLaserY2=0,originX=0,originY=0,patX2=0,patY2=0;
	
	/* ------------------------- *
	 * draw static MAP on screen *
	 * ------------------------- *
	*/
	CPen newPen, newPen_bot, newPen_lsr, newPen_path;      // 用于创建新画笔  
	CPen *pOldPen, *pOldPen_bot, *pOldPen_lsr, *pOldPen_path;  // 用于存放旧画笔
	CBrush newBrush;   
	CBrush *pOldBrush;  

	ArMap armap;      
	char* oldMapName = "2D_MAP.map";
	armap.readFile(oldMapName);
	newBrush.CreateSolidBrush(RGB(255,255,255));   
	pOldBrush = pDC->SelectObject(&newBrush);   
	pDC->Rectangle(rectPicture);   
	pDC->SelectObject(pOldBrush);   
	newBrush.DeleteObject();

	newPen.CreatePen(PS_SOLID, 1.3, RGB(0,0,0));     
	pOldPen = pDC->SelectObject(&newPen); 

		for(vector<ArLineSegment>::iterator it = armap.getLines()->begin();it != armap.getLines()->end();it++)
		{ 

		serverMapX1 = ((*it).getX1()+(19000)+shiftX)/(100+zoomMap);
		serverMapY1 = ((*it).getY1()+(19000)+shiftY)/(100+zoomMap);
		serverMapX2 =((*it).getX2()+(19000)+shiftX)/(100+zoomMap);
		serverMapY2 = ((*it).getY2()+(19000)+shiftY)/(100+zoomMap);

			if(serverMapX1<windowMaxX && serverMapX2<windowMaxX && serverMapX1>windowMinX && serverMapX2>windowMinX 
				&& serverMapY1<windowMaxY && serverMapY2<windowMaxY && serverMapY1>windowMinY && serverMapY2>windowMinY)
			{
			pDC->MoveTo(serverMapX1, serverMapY1);
			pDC->LineTo(serverMapX2, serverMapY2); 
			}
			(*it).~ArLineSegment();
		}
	sMap.clear();
	pDC->SelectObject(pOldPen);   
	newPen.DeleteObject(); 
	//DeleteObject(newPen.m_hObject); 


	/* ---------------------------------- *
	 * draw current robot position on MAP *
	 * ---------------------------------- *
	*/
	newPen_bot.CreatePen(PS_SOLID, 5.5, RGB(255,0,0)); // 创建实心画笔，粗度为5.5，颜色为red   
	pOldPen_bot = pDC->SelectObject(&newPen_bot);    // 选择新画笔，并将旧画笔的指针保存到pOldPen 

	//--- robot towards the direction based on tempA value ---//
	headX1 = (tempX+(19000)+shiftX)/(100+zoomMap);
	headY1 = (tempY+(19000)+shiftY)/(100+zoomMap);
	headX2 = ((tempX+250*cos(tempA/180*3.1416))+(19000)+shiftX)/(100+zoomMap); 
	headY2 = ((tempY+250*sin(tempA/180*3.1416))+(19000)+shiftY)/(100+zoomMap);

		if(headX1<windowMaxX && headX1>windowMinX && headX2<windowMaxX && headX1>windowMinX &&
			headY1<windowMaxY && headY1>windowMinY && headY2<windowMaxY && headY2>windowMinY)
		{
		pDC->MoveTo(headX1,headY1);
		pDC->LineTo(headX2,headY2);
		}

	pDC->SelectObject(pOldPen_bot);   
	newPen_bot.DeleteObject(); 


	/* ------------------------------ *
	 * draw current laser data on MAP *
	 * ------------------------------ *
	*/
	newPen_lsr.CreatePen(PS_DASH, 3, RGB(0,249,0)); // 创建实心画笔，粗度为2，颜色为绿色    
	pOldPen_lsr = pDC->SelectObject(&newPen_lsr); // 选择新画笔，并将旧画笔的指针保存到pOldPen 
	//originX = (19000+shiftX)/(100+zoomMap);
	//originY = (19000+shiftY)/(100+zoomMap);

		for(int t=0;t<laserReadingNo;t++)
		{
		currentLaserX1 = (laserReadingX[t]+(19000)+shiftX)/(100+zoomMap);
		currentLaserY1 = (laserReadingY[t]+(19000)+shiftY)/(100+zoomMap);
		//currentLaserX2 = (laserReadingX[t+1]+(19000)+shiftX)/(100+zoomMap);
		//currentLaserY2 = (laserReadingY[t+1]+(19000)+shiftY)/(100+zoomMap);
	
			//if(currentLaserX1<windowMaxX && currentLaserX2<windowMaxX && currentLaserX1>windowMinX && currentLaserX2>windowMinX && 
				//currentLaserY1<windowMaxY && currentLaserY2<windowMaxY && currentLaserY1>windowMinY && currentLaserY2>windowMinY)
			if(currentLaserX1<windowMaxX && currentLaserX1>windowMinX && 
				currentLaserY1<windowMaxY && currentLaserY1>windowMinY)
			{
			    //--- avoid (0, 0, 0.0) point be plotted on the screen ---//
				//if(currentLaserX1!=originX && currentLaserX2!=originX && 
					//currentLaserY1!=originY && currentLaserY2!=originY)
				//if(currentLaserX1!=originX && currentLaserY1!=originY)
				//{
				pDC->MoveTo(currentLaserX1, currentLaserY1);
				pDC->LineTo(currentLaserX1, currentLaserY1); 
				//}
			}
		}  

	pDC->SelectObject(pOldPen_lsr); // 恢复旧画笔  
	newPen_lsr.DeleteObject(); // 删除新画笔


	/* ------------------------- *
	 * path planning to the goal *
	 * ------------------------- *
	*/     
	newPen_path.CreatePen(PS_DOT, 3, RGB(0,0,229));    
	pOldPen_path = pDC->SelectObject(&newPen_path);   

	for(int pt=0;pt<pathReadingNo;pt++)
	{
	patX1 = (pathReadingX[pt]+(19000)+shiftX)/(100+zoomMap);
	patY1 = (pathReadingY[pt]+(19000)+shiftY)/(100+zoomMap);
	//patX2 = (pathReadingX[pt+1]+(19000)+shiftX)/(100+zoomMap);
	//patY2 = (pathReadingY[pt+1]+(19000)+shiftY)/(100+zoomMap);

		//if(patX1<windowMaxX && patX1>windowMinX && patY1<windowMaxY && patY1>windowMinY &&
			//patX2<windowMaxX && patX2>windowMinX && patY2<windowMaxY && patY2>windowMinY)

		if(patX1<windowMaxX && patY1<windowMaxY && patX1>windowMinX && patY1>windowMinY)
		{
			//if(patX1!=originX && patX2!=originX && 
				//patY1!=originY && patY2!=originY)
			//{
			pDC->MoveTo(patX1, patY1);
			pDC->LineTo(patX1, patY1); 
			//pDC->SetPixel(patX1, patY1, RGB(0,0,229));
			//pDC->SetPixel(patX2, patY2, RGB(0,0,229));
			//}
		}
	}
	pDC->SelectObject(pOldPen_path);   
	newPen_path.DeleteObject(); 

	ReleaseDC(pDC);
}
//--------------//


//-------------------- Reserved --------------------//
/*void CChinaMobileDlg::drawLaserCurrent(CDC *pDC, CRect &rectPicture)
{
	
}*/
//--------------//


//--------- get MAP name ---------//
void getMapName(ArNetPacket* packet)
{
	char mapNam[64];
	memset(mapNam,0,sizeof(mapNam));
	packet->bufToStr(mapNam,sizeof(mapNam));
	fflush(stdout);
	map_name=mapNam;
}
//---------------//


//---------- robotInfo ----------//
void robotInfo(ArNetPacket* packet)
{
	/* Extract the data from the update packet. Its format is status and
	* mode (null-terminated strings), then 6 doubles for battery voltage, 
	* x position, y position and orientation (theta) (from odometry), current
	* translational velocity, and current rotational velocity. Translation is
	* always milimeters, rotation in degrees.
	*/
	double myX_1;
	double myY_1;
	double myTh_1;
	double myVel_1;
	double myRotVel_1;
	double myVoltage_1;
	char myStatus_1[256];
	char myMode_1[32];

	memset(myStatus_1, 0, sizeof(myStatus_1));
	memset(myMode_1, 0, sizeof(myMode_1));
	packet->bufToStr(myStatus_1, sizeof(myStatus_1));
	packet->bufToStr(myMode_1, sizeof(myMode_1));
	myVoltage_1 = ( (double) packet->bufToByte2() )/10.0;
	myX_1 = (double) packet->bufToByte4();
	myY_1 = (double) packet->bufToByte4();
	myTh_1 = (double) packet->bufToByte2();
	myVel_1 = (double) packet->bufToByte2();
	myRotVel_1 = (double) packet->bufToByte2();

	fflush(stdout);

	tempX=myX_1;
	tempY=myY_1;
	tempA=myTh_1;
	tempB=myVoltage_1;
	tempV=myVel_1;
}
//-------------//


//---- gets localization state ----//
void getLocState(ArNetPacket* packet)
{
	double state=0, score=0;
	state = (double) packet->bufToUByte();
	score = (double) packet->bufToUByte2();
	fflush(stdout);

	localScore=score/10;
}
//-------------------//


//---- gets localization status ----//
void pathPlannerStatus(ArNetPacket* packet)
{
	char localStatus[64];
	memset(localStatus,0,sizeof(localStatus));
	packet->bufToStr(localStatus, sizeof(localStatus));
	fflush(stdout);
	local_Status=localStatus;
	ls_handler=localStatus[0];
}
//----------------------//


//----- Get MAP from server ------//
void getMap(ArNetPacket* packet)
{
	using namespace std;
	char mapData[256], *mapString;
	ofstream write;

	mapString=mapData;
	memset(mapData,0,sizeof(mapData));
	packet->bufToStr(mapData, sizeof(mapData));
	write.open("2D_MAP.map",ios::app);
	write<<mapString;
	write<<endl;
	write.close(); 
	fflush(stdout);
}
//--------------//


//------ Get real time laser -------//
void laserCurrent(ArNetPacket* packet)
{
	using namespace std;
	char sensorName[50];
	int laserX=0,laserY=0;
	ofstream write;
	memset(sensorName, 0, sizeof(sensorName));
	memset(laserReadingX, 0, sizeof(laserReadingX));
	memset(laserReadingY, 0, sizeof(laserReadingY));

	laserReadingNo = packet->bufToByte2();   // Number of laser data with X & Y
	packet->bufToStr(sensorName, sizeof(sensorName)); // Sensor's name
	
		for(int i=0;i<laserReadingNo;i++)
		{ 
		laserX = packet->bufToByte4();  // X coordinate for laser reading
		laserY = packet->bufToByte4();  // Y coordinate for laser reading

		laserReadingX[i] = laserX;
		laserReadingY[i] = laserY;
		}
	fflush(stdout);
	//ArUtil::sleep(25);
}
//--------------//


//--- get a list of all goals ---//
void getGoals(ArNetPacket* packet)
{ 
	CString goalName_bind,goalName_trans;
	char goalName[128];
	for(int l=0;l<20;l++)  // bind the string together
	{
	memset(goalName, 0, sizeof(goalName));
	packet->bufToStr(goalName,sizeof(goalName));
	goalName_trans=goalName;
	goalName_bind += " __ " + goalName_trans + "\r\n";
	}
	Goal_list=goalName_bind;
	fflush(stdout);
}
//--------------//


//------ Get path points ------//
void getPath(ArNetPacket* packet)
{
	int pathX=0,pathY=0;
	memset(pathReadingX, 0, sizeof(pathReadingX));
	memset(pathReadingY, 0, sizeof(pathReadingY));

	pathReadingNo=packet->bufToByte2();

		for(int p=0;p<pathReadingNo;p++)
		{
		pathX = packet->bufToByte4();  // path points for X
		pathY = packet->bufToByte4();  // path points for Y

		pathReadingX[p] = pathX;
		pathReadingY[p] = pathY;
		}

	fflush(stdout);
}
//--------------//


//---------- Mouse coordinate ----------//
void CChinaMobileDlg::OnLButtonDblClk()
{
	int mouseX=0,mouseY=0;
	CRect rectP;
	CPoint pointP;

	/*GetDlgItem(IDC_MAP_DRAW)->GetWindowRect(&rectP);
	GetDlgItem(IDC_MAP_DRAW)->GetClientRect(&rectP);*/

	GetCursorPos(&pointP);
	ScreenToClient(&pointP);

	mouseX = pointP.x;
	mouseY = pointP.y;
	mousePoseX= (mouseX-5)*(100+zoomMap)-(19000+shiftX);
	mousePoseY= (mouseY-22)*(100+zoomMap)-(19000+shiftY);
	ArUtil::sleep(50);
}
//-----------------//


//-------- Main thread of the program --------//
UINT CChinaMobileDlg::MainThread(LPVOID lParam)
{
	CChinaMobileDlg *dlg;  // Capture Thread 
	dlg=(CChinaMobileDlg*)lParam; // Get pointer to parent dialog
	dlg->IsThreadRunning=true;

	Aria::init();
	int argc=5; 
	char *argv[6]; //argc是字符串个数，argv是字符串内容
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
	/*ArGlobalFunctor escapeCB(&escape);
	keyHandler.addKeyHandler(ArKeyHandler::ESCAPE, &escapeCB);*/

	InputHandler inputHandler(&client);
	OutputHandler outputHandler(&client);
	inputHandler.safeDrive();
	ArNetPacket requestPacket;

	/* binding requests & the CB functions */
	ArGlobalFunctor1<ArNetPacket *> getMapNameCB(&getMapName);
	client.addHandler("getMapName", &getMapNameCB);
	
	ArGlobalFunctor1<ArNetPacket *> robotInfoCB(&robotInfo);
	client.addHandler("update", &robotInfoCB);

	ArGlobalFunctor1<ArNetPacket *> getLocStateCB(&getLocState);
	client.addHandler("getLocState", &getLocStateCB);

	ArGlobalFunctor1<ArNetPacket *> pathPlannerStatusCB(&pathPlannerStatus);
	client.addHandler("pathPlannerStatus", &pathPlannerStatusCB);

	ArGlobalFunctor1<ArNetPacket *> getMapCB(&getMap);
	client.addHandler("getMap", &getMapCB);

	ArGlobalFunctor1<ArNetPacket *> laserCurrentCB(&laserCurrent);
	client.addHandler("getSensorCurrent",&laserCurrentCB);

	ArGlobalFunctor1<ArNetPacket *> getGoalsCB(&getGoals);
	client.addHandler("getGoals",&getGoalsCB);

	ArGlobalFunctor1<ArNetPacket *> getPathCB(&getPath);
	client.addHandler("getPath",&getPathCB);

	/* sending requests to server */
	client.requestOnce("getMapName");

	client.requestOnce("getGoals");

	client.request("update",TM+1); 

	client.request("getLocState",TM+2);

	client.request("pathPlannerStatus",TM+3);
	
	requestPacket.strToBuf("sim_S3Series_1"); // sim_S3Series_1 for mobileSim use only
	client.request("getSensorCurrent",TM+4,&requestPacket); 

	client.request("getPath",TM*4);

		while(client.getRunningWithLock())
		{
			ArNetPacket pkt, pkt_goal;
			switch(flag){
			case 1:if(GetAsyncKeyState(VK_LBUTTON)&0x8000)inputHandler.up();ArUtil::sleep(50);break;
			case 2:if(GetAsyncKeyState(VK_LBUTTON)&0x8000)inputHandler.down();ArUtil::sleep(50);break;
			case 3:if(GetAsyncKeyState(VK_LBUTTON)&0x8000)inputHandler.left();ArUtil::sleep(25);break;
			case 4:if(GetAsyncKeyState(VK_LBUTTON)&0x8000)inputHandler.right();ArUtil::sleep(25);break;
			case 5:client.requestOnce("stop");ArUtil::sleep(25);flag=0;break;  //inputHandler.doStop();
			case 13:client.requestOnce("localizeToPose");flag=0;break; // for mobileSim use ONLY
			
			case 20:zoomMap=zoomMap-20;flag=0;break; // zoom +
			case 21:zoomMap=zoomMap+20;flag=0;break; // zoom -
			case 22:shiftY=shiftY-2000;flag=0;break; // up
			case 23:shiftY=shiftY+2000;flag=0;break; // down
			case 24:shiftX=shiftX-2000;flag=0;break; // left
			case 25:shiftX=shiftX+2000;flag=0;break; // right

			//case 30:client.requestOnce("getGoals");flag=0;break;
			case 99:client.disconnect();flag=0;break; // disconnect from server
			}
		
			if(safedrive){inputHandler.unsafeDrive();} //安全模式
			else{inputHandler.safeDrive();}

		inputHandler.sendInput(); // don't change this code sequence here!

			/* make sure both point to same robot */
			if(inputHandler.getClient()!=outputHandler.getClient()){outputHandler.setClient( inputHandler.getClient() );}


			/* --------------------------------------------------- *
			 * rewrite the task which needs the long time duration *
			 * --------------------------------------------------- *
			*/

			/* get server MAP */
			if(flag==6)
			{
			client.requestOnce("getMap");
				while(1)
				{
				ArUtil::sleep(200);
				if(flag==5)goto mp;
				}
			mp:
			flag=0;
			client.requestStop("getMap");
			}

			/* localize to the specify point */
			if(flag==12 && GetAsyncKeyState(VK_RBUTTON)&0x8000)
			{
			pkt.byte4ToBuf(mousePoseX);
			pkt.byte4ToBuf(mousePoseY);
			pkt.byte4ToBuf(0); // localize to pose
			client.requestOnce("localizeToPose",&pkt);
			flag=0;
			}

			/* wander mode */
			if(flag==10)
			{
			client.requestOnce("wander");
				while(1)
				{
				ArUtil::sleep(200);
				if(flag==5)goto outside_1;
				}
			outside_1:
			flag=0;
			}

			/* goto a specify point/goal */
			if(flag==11)
			{
			pkt_goal.strToBuf(Goal_name); // Goal_name
			client.requestOnce("gotoGoal",&pkt_goal);
				while(1)
				{
				ArUtil::sleep(200);
				if(flag==5)goto outside_2;
				}
			outside_2:
			flag=0;
			}

			/* send robot to some position */
			if(flag==14 && GetAsyncKeyState(VK_RBUTTON)&0x8000)
			{
			pkt.byte4ToBuf(mousePoseX);
			pkt.byte4ToBuf(mousePoseY);
			pkt.byte4ToBuf(0); // goto pose
			client.requestOnce("gotoPose",&pkt);
				while(1)
				{
				ArUtil::sleep(200);
				if(flag==5)goto outside_3;
				}
			outside_3:
			flag=0;
			}

			/* tour all goals in the MAP */
			if(flag==15)
			{
			client.requestOnce("tourGoals");
				while(1)
				{
				ArUtil::sleep(200);
				if(flag==5)goto outside_4;
				}
			outside_4:
			flag=0;
			}
			
			/* start CMCC task trigger */
			if(flag==31)
			{
			ArNetPacket taskReq;

			taskReq.strToBuf(task_Goal);
			client.requestOnce("gotoGoal",&taskReq);
				while(1)
				{
				ArUtil::sleep(500);
				if(ls_handler!='G' && ls_handler!='C' && ls_handler!='F') goto outside_tk;
				}
			outside_tk:
			client.requestOnce("stop");
			tFlag=99; // trick the tasks
			flag=0;
			}
			
		ArUtil::sleep(25); // set sleep time here to avoid use of too much CPU resource
		}
}
//---------------------//


//---------- Refresh real time data ----------//
void CChinaMobileDlg::OnTimer(UINT_PTR nIDEvent)
{
	UpdateData(true);
	m_Xpose = tempX;
	m_Ypose = tempY;
	m_Th = tempA;
	m_Voltage = tempB;
	m_Velocity = tempV;
	m_px = mousePoseX;
	m_py = mousePoseY;
	m_score=localScore;
	m_status=local_Status;
	m_mapName=map_name;
	UpdateData(false);

	CRect rectPicture;   
	/* 客户区坐标以窗口的左上角为原点，这区别于以屏幕左上角为原点的屏幕坐标 */ 
	m_serverMap.GetClientRect(&rectPicture);
	drawServerMap(m_serverMap.GetDC(), rectPicture); // draw server MAP & current laser on client
	//drawLaserCurrent(m_serverMap.GetDC(), rectPicture);

	if(m_safeDrive.GetCheck()){safedrive=true;}
	else{safedrive=false;}
	if(flag==12 || flag==14){OnLButtonDblClk();}
	if(flag==11){m_goal.GetWindowText(Goal_name);}

	//--- wait for robot reaching to goal ---//
	if(ls_handler!='G' && ls_handler!='F' && tFlag==99)
	{
	tFlag=0;
	//--- if reach the goal, robot starts following CMCC tasks ---//
	ShellExecute(this->m_hWnd,"open","http://www.baidu.com","","", SW_SHOW );
	//--- task end ---//
	goto outTask;
	}
	outTask:

	CDialogEx::OnTimer(nIDEvent);
}
//----------------------------//


//--------------- Connect --------------//
void CChinaMobileDlg::OnBnClickedButton1()
{
	CString strx, m_temp;
	m_IP.GetWindowText(strx);
	AfxBeginThread(MainThread, this);

	UpdateData(true);
	//m_temp += strx;
	Global_IP = strx;
	m_temp += "Connected to:  \r\n" + strx + " \r\n" + "successfully!";
	m_log.SetWindowText(m_temp);
	m_log.LineScroll(m_log.GetLineCount());
	UpdateData(false);
}
//-----------------//


//------------- Disconnect -------------//
void CChinaMobileDlg::OnBnClickedButton2()
{
	flag=99;
}
//-------//


//------------ Exit program ------------//
void CChinaMobileDlg::OnBnClickedButton17()
{
	CDialogEx::OnCancel();
}
//---------------------//


void CChinaMobileDlg::OnBnClickedButton3()
{
	flag=1; // forward
}


void CChinaMobileDlg::OnBnClickedButton4()
{
	flag=2; // backward
}

void CChinaMobileDlg::OnBnClickedButton5()
{
	flag=3; // turn left
}


void CChinaMobileDlg::OnBnClickedButton6()
{
	flag=4; //turn right
}


void CChinaMobileDlg::OnBnClickedButton7()
{
	flag=5; // stop
}


void CChinaMobileDlg::OnBnClickedButton8()
{
	flag=10; // wander mode
}


void CChinaMobileDlg::OnBnClickedButton11()
{
	flag=6; // load server MAP
}


void CChinaMobileDlg::OnBnClickedButton9()
{
	m_goal.GetWindowText(Goal_name);
	flag=11; //goto pose
	//UpdateData(true);
}


void CChinaMobileDlg::OnBnClickedButton12()
{
	flag=12; // localize to pose
}


void CChinaMobileDlg::OnBnClickedButton13()
{
	flag=13; // home position, for MobileSim use ONLY
}


void CChinaMobileDlg::OnBnClickedButton14()
{
	flag=14; // send robot to position
}


void CChinaMobileDlg::OnBnClickedButton15()
{
	flag=15; // tour goals
}


void CChinaMobileDlg::OnBnClickedOk()
{
	flag=20; // zoom +
}


void CChinaMobileDlg::OnBnClickedCancel()
{
	flag=21; // zoom -
}


void CChinaMobileDlg::OnBnClickedButton18()
{
	flag=22; // up
}


void CChinaMobileDlg::OnBnClickedButton19()
{
	flag=23; // down
}


void CChinaMobileDlg::OnBnClickedButton20()
{
	flag=24; // left
}


void CChinaMobileDlg::OnBnClickedButton21()
{
	flag=25; // right
}


void CChinaMobileDlg::OnBnClickedButton22()
{
	zoomMap=0,shiftX=0,shiftY=0; // reset map size
}


void CChinaMobileDlg::OnBnClickedButton10()
{
	MapDlg *dlg1 = new MapDlg;  // open a new dialog of MAP
	dlg1->Create(IDD_DIALOG1); 
	dlg1->ShowWindow(SW_SHOW);
}


void CChinaMobileDlg::OnBnClickedButton23()
{
	//flag=30; // show goal name list
	CString g_info;
	g_info="GOAL LIST: \r\n" + Goal_list + " __ END";
	m_log.SetWindowText(g_info);
}


void CChinaMobileDlg::OnBnClickedButton25()
{
	taskDlg *dlg2 = new taskDlg;  // new dialog for CMCC tasks
	dlg2->Create(IDD_DIALOG2);     
	dlg2->ShowWindow(SW_SHOW);
}


void CChinaMobileDlg::OnBnClickedButton16()
{
	flag=31; // CMCC task starts

	//--- following program can capture every line's text in Edit Ccontrol ---//
	CString strText;
	int TaskNo=0, nLineCount = m_goal.GetLineCount();
	
		for(TaskNo=0;TaskNo<nLineCount;TaskNo++)
		{
		int len = m_goal.LineLength(m_goal.LineIndex(TaskNo));
		m_goal.GetLine(TaskNo, strText.GetBufferSetLength(len), len);
		task_Goal = strText.GetBuffer(len);

		strText.ReleaseBuffer(len);
		}
}
