/* ------------------------------------------------------- *
 * Last modification data 2015/3/12 00:52  by: Xiaoxi Gong *
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

#include "Aria.h"
#include "ArNetworking.h"
#include "robot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define TM 120
#endif

CString Global_IP;
CString Warning;
CString Goal_name;

int flag=0,laserReadingNo=0,laserReadingX[512],laserReadingY[512],mouseX=0,mouseY=0,selectMap=0,selectLaser=0;
double tempX=0,tempY=0,tempA=0,tempB=0,tempV=0,mousePoseX=0,mousePoseY=0;
float windowMaxX=452,windowMinX=6,windowMaxY=340,windowMinY=18,zoomMap=0,shiftX=0,shiftY=0;
bool safedrive=0;

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
ON_BN_CLICKED(IDC_BUTTON16, &CChinaMobileDlg::OnBnClickedButton16)
ON_BN_CLICKED(IDC_BUTTON17, &CChinaMobileDlg::OnBnClickedButton17)
ON_BN_CLICKED(IDC_BUTTON18, &CChinaMobileDlg::OnBnClickedButton18)
ON_BN_CLICKED(IDC_BUTTON19, &CChinaMobileDlg::OnBnClickedButton19)
ON_BN_CLICKED(IDC_BUTTON20, &CChinaMobileDlg::OnBnClickedButton20)
ON_BN_CLICKED(IDC_BUTTON21, &CChinaMobileDlg::OnBnClickedButton21)
ON_BN_CLICKED(IDC_BUTTON22, &CChinaMobileDlg::OnBnClickedButton22)
ON_BN_CLICKED(IDC_BUTTON10, &CChinaMobileDlg::OnBnClickedButton10)
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
	SetTimer(1, TM, NULL);  // set TIMER 
	m_IP.SetAddress(192,168,1,120);

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


//---------------------- Draw server MAP ----------------------//
void CChinaMobileDlg::drawServerMap(CDC *pDC, CRect &rectPicture)
{
	using std::vector;
	float serverMapX1=0,serverMapX2=0,serverMapY1=0,serverMapY2=0,botX=0,botY=0;
	CPen newPen;       
	CPen *pOldPen;    
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
	//DeleteObject(newBrush.m_hObject);

	newPen.CreatePen(PS_SOLID, 1, RGB(0,0,0));     
	pOldPen = pDC->SelectObject(&newPen); 

		for(vector<ArLineSegment>::iterator it = armap.getLines()->begin();it != armap.getLines()->end();it++)
		{ 

		serverMapX1 = ((*it).getX1()+(19000)+shiftX*selectMap)/(100+zoomMap*selectMap);
		serverMapY1 = ((*it).getY1()+(19000)+shiftY*selectMap)/(100+zoomMap*selectMap);
		serverMapX2 =((*it).getX2()+(19000)+shiftX*selectMap)/(100+zoomMap*selectMap);
		serverMapY2 = ((*it).getY2()+(19000)+shiftY*selectMap)/(100+zoomMap*selectMap);

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
	ReleaseDC(pDC);
}
//--------------//


//-------------------- Draw laser current MAP --------------------//
void CChinaMobileDlg::drawLaserCurrent(CDC *pDC, CRect &rectPicture)
{
	float currentLaserX1=0,currentLaserX2=0,currentLaserY1=0,currentLaserY2=0,botX=0,botY=0,originX=0,originY=0;
	/* draw current laser MAP */
	CPen newPen_lsr;   // 用于创建新画笔   
	CPen *pOldPen_lsr; // 用于存放旧画笔   
	newPen_lsr.CreatePen(PS_SOLID, 2, RGB(0,249,0)); // 创建实心画笔，粗度为2，颜色为绿色    
	pOldPen_lsr = pDC->SelectObject(&newPen_lsr); // 选择新画笔，并将旧画笔的指针保存到pOldPen 

	originX = (19000+shiftX*selectMap)/(100+zoomMap*selectMap);
	originY = (19000+shiftY*selectMap)/(100+zoomMap*selectMap);

		for(int t=0;t<laserReadingNo;t++)
		{
		currentLaserX1 = (laserReadingX[t]+(19000)+shiftX*selectMap)/(100+zoomMap*selectMap);
		currentLaserY1 = (laserReadingY[t]+(19000)+shiftY*selectMap)/(100+zoomMap*selectMap);
		currentLaserX2 = (laserReadingX[t+1]+(19000)+shiftX*selectMap)/(100+zoomMap*selectMap);
		currentLaserY2 = (laserReadingY[t+1]+(19000)+shiftY*selectMap)/(100+zoomMap*selectMap);
	
			if(currentLaserX1<windowMaxX && currentLaserX2<windowMaxX && currentLaserX1>windowMinX && currentLaserX2>windowMinX && 
				currentLaserY1<windowMaxY && currentLaserY2<windowMaxY && currentLaserY1>windowMinY && currentLaserY2>windowMinY)
			{
			    /* avoid (0, 0, 0.0) point be plotted on the screen */
				if(currentLaserX1!=originX && currentLaserX2!=originX && 
					currentLaserY1!=originY && currentLaserY2!=originY)
				{
				pDC->MoveTo(currentLaserX1, currentLaserY1);
				pDC->LineTo(currentLaserX2, currentLaserY2); 
				}
			}
		}  

	pDC->SelectObject(pOldPen_lsr); // 恢复旧画笔  
	newPen_lsr.DeleteObject(); // 删除新画笔


    /* draw current robot position on MAP */
	CPen newPen_bot;    // 用于创建新画笔   
	CPen *pOldPen_bot;  // 用于存放旧画笔    
	newPen_bot.CreatePen(PS_SOLID, 4.5, RGB(255,0,0)); // 创建实心画笔，粗度为4.5，颜色为red   
	pOldPen_bot = pDC->SelectObject(&newPen_bot);    // 选择新画笔，并将旧画笔的指针保存到pOldPen 

	botX = (tempX+(19000)+shiftX*selectMap)/(100+zoomMap*selectMap);
	botY = (tempY+(19000)+shiftX*selectMap)/(100+zoomMap*selectMap);

		if(botX<windowMaxX && botX>windowMinX && botY<windowMaxY && botY>windowMinY)
		{
		pDC->MoveTo(botX, botY);
		pDC->LineTo(botX, botY); 
		}

	pDC->SelectObject(pOldPen_bot);   
	newPen_bot.DeleteObject(); 
	ReleaseDC(pDC);
}
//--------------//


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
//---------//


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
}
//--------------//


//---------- Mouse coordinate ----------//
void CChinaMobileDlg::OnLButtonDblClk()
{
	CRect rectP;
	CPoint pointP;

	/*GetDlgItem(IDC_MAP_DRAW)->GetWindowRect(&rectP);
	GetDlgItem(IDC_MAP_DRAW)->GetClientRect(&rectP);*/

	GetCursorPos(&pointP);
	ScreenToClient(&pointP);

	mouseX = pointP.x;
	mouseY = pointP.y;
	mousePoseX= (mouseX-5)*(100)-(19000);
	mousePoseY= (mouseY-18)*(100)-(19000);
}
//-------//


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
	ArNetPacket requestPacket, pkt, pkt_1, pkt_goal;

	/* binding requests & the CB functions */
	ArGlobalFunctor1<ArNetPacket *> robotInfoCB(&robotInfo);
	client.addHandler("update", &robotInfoCB);

	ArGlobalFunctor1<ArNetPacket *> getMapCB(&getMap);
	client.addHandler("getMap", &getMapCB);

	ArGlobalFunctor1<ArNetPacket *> laserCurrentCB(&laserCurrent);
	client.addHandler("getSensorCurrent",&laserCurrentCB);

	/* sending requests to server */
	client.request("update",TM); 

	requestPacket.strToBuf("sim_S3Series_1"); // sim_S3Series_1 for mobileSim use only
	client.request("getSensorCurrent",TM/2,&requestPacket); 

		while(client.getRunningWithLock())
		{
			switch(flag){
			case 1:if(GetAsyncKeyState(VK_LBUTTON)&0x8000)inputHandler.up();ArUtil::sleep(50);break;
			case 2:if(GetAsyncKeyState(VK_LBUTTON)&0x8000)inputHandler.down();ArUtil::sleep(50);break;
			case 3:if(GetAsyncKeyState(VK_LBUTTON)&0x8000)inputHandler.left();ArUtil::sleep(50);break;
			case 4:if(GetAsyncKeyState(VK_LBUTTON)&0x8000)inputHandler.right();ArUtil::sleep(50);break;
			case 5:client.requestOnce("stop");ArUtil::sleep(50);flag=0;break;  //inputHandler.doStop();
			case 6:client.requestOnce("getMap");ArUtil::sleep(50);client.requestStop("getMap");flag=0;break;

			case 10:client.requestOnce("wander");ArUtil::sleep(3600);break;
			case 11:pkt_goal.strToBuf(Goal_name);client.requestOnce("gotoGoal");ArUtil::sleep(30000);break;
			case 13:client.requestOnce("localizeToPose");flag=0;break; // for mobileSim use ONLY
			case 15:client.requestOnce("tourGoals");ArUtil::sleep(30000);break;

			case 20:zoomMap=zoomMap-20;flag=0;break; // zoom +
			case 21:zoomMap=zoomMap+20;flag=0;break; // zoom -
			case 22:shiftY=shiftY-2000;flag=0;break; // up
			case 23:shiftY=shiftY+2000;flag=0;break; // down
			case 24:shiftX=shiftX-2000;flag=0;break; // left
			case 25:shiftX=shiftX+2000;flag=0;break; // right
	
			case 99:client.disconnect();flag=0;break; // disconnect from server
			}
		inputHandler.sendInput();

			if(safedrive){inputHandler.unsafeDrive();} //安全模式
			else{inputHandler.safeDrive();}

			/* make sure both point to same robot */
			if(inputHandler.getClient()!=outputHandler.getClient()){outputHandler.setClient( inputHandler.getClient() );}

			if(flag==12 && GetAsyncKeyState(VK_RBUTTON)&0x8000)
			{
			pkt.byte4ToBuf(mousePoseX);
			pkt.byte4ToBuf(mousePoseY);
			pkt.byte4ToBuf(0); // localize to pose
			client.requestOnce("localizeToPose",&pkt);
			flag=0;
			}

			if(flag==14 && GetAsyncKeyState(VK_RBUTTON)&0x8000)
			{
			pkt_1.byte4ToBuf(mousePoseX);
			pkt_1.byte4ToBuf(mousePoseY);
			pkt_1.byte4ToBuf(0); // goto pose
			client.requestOnce("gotoPose",&pkt_1);
			ArUtil::sleep(12000);
			flag=0;
			}

		ArUtil::sleep(200); // set sleep time here to avoid use of too much CPU resource
		}
}
//-------------------//


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
	UpdateData(false);

	CRect rectPicture;   
	/* 客户区坐标以窗口的左上角为原点，这区别于以屏幕左上角为原点的屏幕坐标 */ 
	m_serverMap.GetClientRect(&rectPicture);
	drawServerMap(m_serverMap.GetDC(), rectPicture); // draw server MAP on client
	drawLaserCurrent(m_serverMap.GetDC(), rectPicture);  // draw current laser points on client

	if(m_safeDrive.GetCheck()){safedrive=true;}
	else{safedrive=false;}
	if(flag==12 || flag==14){OnLButtonDblClk();}
	
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
	m_temp += "Connected to " + strx + " successfully! \r\n";
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
	flag=6;
}


void CChinaMobileDlg::OnBnClickedButton9()
{
	CString goal;
	m_goal.GetWindowText(goal);
	(LPSTR)(LPCTSTR)Goal_name;
	flag=11; //goto pose
	UpdateData(true);
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


void CChinaMobileDlg::OnBnClickedButton16()
{
	selectMap=1; // static map
}


void CChinaMobileDlg::OnBnClickedButton17()
{
	selectLaser=1; // current map
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
	zoomMap=0,shiftX=0,shiftY=0;// reset map size
}


void CChinaMobileDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
}
