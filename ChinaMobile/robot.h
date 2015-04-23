#include "stdafx.h"
/*
MobileRobots Advanced Robotics Interface for Applications (ARIA)
Copyright (C) 2004, 2005 ActivMedia Robotics LLC
Copyright (C) 2006, 2007 MobileRobots Inc.

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

If you wish to redistribute ARIA under different terms, contact 
MobileRobots for information about a commercial version of ARIA at 
robots@mobilerobots.com or 
MobileRobots Inc, 19 Columbia Drive, Amherst, NH 03031; 800-639-9481
*/

/* This is the ArNetworking example client. 
 * It connects to an ArNetworking server, and provides continuous 
 * information about the robot state (position, speed, server mode, etc.),
 * and, if the server supports the commands, lets you drive the robot with
 * the keyboard.  
 *
 * To see the example client in action, first run a server on the robot's 
 * onboard computer, for example, serverDemo, testServer, guiServer (from ARNL),
 * or ARAM. Make sure the robot computer is on a network, and run this
 * clientDemo with the hostname of the robot computer as an argument:
 *
 *    ./clientDemo -host myrobot
 *
 */

/*
   This version modified 10/10 to include joystick control and stereoScan command.
   This version modified 6/11 to include parameters for stereoscan and multiple robot

 */


#include <time.h>

#include "Aria.h"
#include "ArNetworking.h"

#define MAX_ROBOTS 10 /* max num of robots allowed*/

bool G_quiet = false;
int PWM_ratio=38;

/* This class provides callback functions which are called by the ArKeyHandler
 * object when the user presses keys. These callback functions adjust member
 * variables.  The sendInput() method sends a command with those values
 * to the server using the "ratioDrive" request.
 */

class InputHandler
{
public: 
  /** 
   * @param client  Our client networking object 
   * @param keyHandler  Key handler to register command callbacks with
   */
  InputHandler(ArClientBase *client);
  virtual ~InputHandler(void);

  /// Up arrow key handler: drive the robot forward
  void up(void);

  /// Down arrow key handler: drive the robot backward
  void down(void);

  /// Left arrow key handler: turn the robot left 
  void left(void);

  /// Right arrow key handler: turn the robot right
  void right(void);

  /// Send drive request to the server with stored values
  void sendInput(void);

  /// Send a request to enable "safe drive" mode on the server
  void safeDrive();

  /// Send a request to disable "safe drive" mode on the server
  void unsafeDrive();

  void listData();

  void logTrackingTerse();
  void logTrackingVerbose();
  void resetTracking();

  /* dml 6/10/11 */
  void   setPanParameters(); // set the pan increment, num pans,and default tilt
  void   setTiltParameters(); // set the tilt increment, num tits and default pan
  void   setRobotNumber(); // set the robot being controlled
  void   setRobotTo(int n); // set the robot being controlled
  void   addNewRobot(); // open a link to a new robot
  ArClientBase *getClient();
  void   doStereoScan(); // do a scan

  void toggleSalience();
  void enableSalienceMove();
  void doStop();

  /* */

  bool dontCheck;

protected:
  ArClientBase *myClient;
  ArKeyHandler *myKeyHandler;


  int currentRobotNumber;
  int maxRobotSoFar;
  ArClientBase *robotNumberTable[MAX_ROBOTS];

  int panInc, numPans, defTilt;
  int tiltInc, numTilts, defPan;

  /// Set this to true in the constructor to print out debugging information
  bool myPrinting;

  /// Current translation value (a percentage of the  maximum speed)
  double myTransRatio;

  /// Current rotation ration value (a percentage of the maximum rotational velocity)
  double myRotRatio;

  time_t lastScan;
  time_t sinceScan;

  /** Functor objects, given to the key handler, which then call our handler
   * methods above */
  ///@{
  ArFunctorC<InputHandler> myUpCB;
  ArFunctorC<InputHandler> myDownCB;
  ArFunctorC<InputHandler> myLeftCB;
  ArFunctorC<InputHandler> myRightCB;
  ArFunctorC<InputHandler> mySafeDriveCB;
  ArFunctorC<InputHandler> myUnsafeDriveCB;
  ArFunctorC<InputHandler> myListDataCB;
  ArFunctorC<InputHandler> myLogTrackingTerseCB;
  ArFunctorC<InputHandler> myLogTrackingVerboseCB;
  ArFunctorC<InputHandler> myResetTrackingCB;
  /* dml 6/10/11 */
  ArFunctorC<InputHandler> mySetPanParametersCB;
  ArFunctorC<InputHandler> mySetTiltParametersCB;
  ArFunctorC<InputHandler> myAddNewRobotCB;
  ArFunctorC<InputHandler> mySetRobotNumberCB;
  ArFunctorC<InputHandler> myDoStereoScanCB;

  ArFunctorC<InputHandler> myToggleSalienceCB;
  ArFunctorC<InputHandler> myEnableSalienceMoveCB;
  ArFunctorC<InputHandler> myDoStopCB;
  
  /* */

  ///@}
};

InputHandler::InputHandler(ArClientBase *client) : 
  myClient(client),
  myPrinting(false), myTransRatio(0.0), myRotRatio(0.0) ,

  /* Initialize functor objects with pointers to our handler methods: */
  myUpCB(this, &InputHandler::up),
  myDownCB(this, &InputHandler::down),
  myLeftCB(this, &InputHandler::left),
  myRightCB(this, &InputHandler::right),
  mySafeDriveCB(this, &InputHandler::safeDrive),
  myUnsafeDriveCB(this, &InputHandler::unsafeDrive),
  myListDataCB(this, &InputHandler::listData),
  myLogTrackingTerseCB(this, &InputHandler::logTrackingTerse),
  myLogTrackingVerboseCB(this, &InputHandler::logTrackingVerbose),
  myResetTrackingCB(this, &InputHandler::resetTracking),
  /* dml 6/10 /11 */
  mySetPanParametersCB(this, &InputHandler::setPanParameters),
  mySetTiltParametersCB(this, &InputHandler::setTiltParameters),
  myAddNewRobotCB(this, &InputHandler::addNewRobot),
  mySetRobotNumberCB(this, &InputHandler::setRobotNumber),
  myDoStereoScanCB(this,&InputHandler::doStereoScan),

  myToggleSalienceCB(this,&InputHandler::toggleSalience),
  myEnableSalienceMoveCB(this,&InputHandler::enableSalienceMove),
  myDoStopCB(this,&InputHandler::doStop)
  /* */
{
  dontCheck=false;
  currentRobotNumber=0; maxRobotSoFar=1;
  for (int i=0; i<MAX_ROBOTS; i++) robotNumberTable[i]=0;  
  robotNumberTable[0]=client;
  panInc=20; numPans=3; defTilt=20;
  tiltInc=0; numTilts=1; defPan=-1;

  lastScan=time(NULL);
  sinceScan= -10;
}

InputHandler::~InputHandler(void)
{
  // should free up the robot table
}

ArClientBase *InputHandler::getClient() { return myClient; }

void InputHandler::up(void)
{
  if (myPrinting)
    printf("Forwards\n");
  myTransRatio = 100;
}

void InputHandler::down(void)
{
  if (myPrinting)
    printf("Backwards\n");
  myTransRatio = -100;
}

void InputHandler::left(void)
{
  if (myPrinting)
    printf("Left\n");
  myRotRatio = 30;
}

void InputHandler::right(void)
{
  if (myPrinting)
    printf("Right\n");
  myRotRatio = -30;
}

void InputHandler::safeDrive()
{
  /* Construct a request packet. The data is a single byte, with value 
   * 1 to enable safe drive, 0 to disable. */
  ArNetPacket p;
  p.byteToBuf(1);

  /* Send the packet as a single request: */
  if(myPrinting)
    printf("Sending setSafeDrive 1.\n");
  myClient->requestOnce("setSafeDrive",&p);
  if(myPrinting)
    printf("\nSent enable safe drive.\n");
}

void InputHandler::unsafeDrive()
{
  /* Construct a request packet. The data is a single byte, with value 
   * 1 to enable safe drive, 0 to disable. */
  ArNetPacket p;
  p.byteToBuf(0);

  /* Send the packet as a single request: */
  if(myPrinting)
    printf("Sending setSafeDrive 0.\n");
  myClient->requestOnce("setSafeDrive",&p);
  if(myPrinting)
    printf("\nSent disable safe drive command. Your robot WILL run over things if you're not careful.\n");
}

void InputHandler::listData()
{
  myClient->logDataList();
}

void InputHandler::logTrackingTerse()
{
  myClient->logTracking(true);
}

void InputHandler::logTrackingVerbose()
{
  myClient->logTracking(false);
}

void InputHandler::resetTracking()
{
  myClient->resetTracking();
}

/* dml 6/10/11 */

void InputHandler::doStereoScan()
{
  myClient->requestOnce("doStereoScan", NULL);
  ArUtil::sleep(500);
}



void InputHandler::toggleSalience()
{
  myClient->requestOnce("doSalience", NULL);
  ArUtil::sleep(500);
}

void InputHandler::enableSalienceMove()
{
  myClient->requestOnce("doSalienceMove", NULL);
  ArUtil::sleep(500);
}
void InputHandler::doStop()
{
  myClient->requestOnce("doSTOP", NULL);
}

void InputHandler::setPanParameters()
{
  if(!myClient->dataExists("setStereoPan")) return;

  myKeyHandler->restore();
  G_quiet=true;

  printf("Enter pan increment, number of pans, and default tilt during panning\n");
  scanf("%d %d %d",&panInc, &numPans, &defTilt);

  ArNetPacket packet;
  packet.doubleToBuf(double(panInc));
  packet.doubleToBuf(double(numPans));
  packet.doubleToBuf(double(defTilt)); 
  dontCheck=false;

  if (myPrinting)
    printf("Sending\n");
  myClient->requestOnce("setStereoPan", &packet);
  printf("Setting pan parameters.\n");
  myKeyHandler->takeKeys();
  G_quiet=false;

}

void InputHandler::setTiltParameters()
{
  if(!myClient->dataExists("setStereoTilt")) return;

  myKeyHandler->restore();
  G_quiet=true;

  printf("Enter tilt increment, number of tilts, and default pan during tilting\n");
  scanf("%d %d %d",&tiltInc, &numTilts, &defPan);

  ArNetPacket packet;
  packet.doubleToBuf(double(tiltInc));
  packet.doubleToBuf(double(numTilts));
  packet.doubleToBuf(double(defPan)); 
  dontCheck=false;
  if (myPrinting)
    printf("Sending\n");
  myClient->requestOnce("setStereoTilt", &packet);
  printf("Setting tilt parameters.\n");
  myKeyHandler->takeKeys();
  G_quiet=false;

}

void InputHandler::addNewRobot()
{ 
  char ip[100];

  int myArgc=5; char *myArgv[6];

  myKeyHandler->restore();
  G_quiet=true;

  myArgv[0]="./clientDemo";
  myArgv[1]="-host";

  printf("What is the IP address of the new robot\n");
  scanf("%s",ip);
  myArgv[2]="192.168.11.13";
  myArgv[3]="-port";
  myArgv[4]="7272";
  myArgv[5]=0;

  ArClientBase *client = new ArClientBase;

  ArArgumentParser parser(&myArgc,myArgv);
  parser.loadDefaultArguments();
  ArClientSimpleConnector clientConnector(&parser);
  clientConnector.parseArgs();

  /* Connect our client object to the remote server: */
  if (!clientConnector.connectClient(client))
  {
    if (client->wasRejected())
      printf("Server '%s' rejected connection, exiting\n", client->getHost());
    else
      printf("Could not connect to server '%s', exiting\n", client->getHost());
    return;
  } 

  printf("Connected to server on new robot[%d]=%s.\n",maxRobotSoFar,ip);
  client->runAsync();
  robotNumberTable[maxRobotSoFar]=client;
  maxRobotSoFar++;
  myKeyHandler->takeKeys();
  G_quiet=false;

}


void InputHandler::setRobotNumber()
{ int r;
 
  myKeyHandler->restore();
  G_quiet=true;

  printf("Enter the robot number: 0 is the base robot, -1 means all robots\n");
  scanf("%d",&r);

  setRobotTo(r);

  myKeyHandler->takeKeys();
  G_quiet=false;
  ArUtil::sleep(500);

}

void InputHandler::setRobotTo(int r)
{ 
  if (r>=0 && r<MAX_ROBOTS)
    if (robotNumberTable[r]!=0) {
      currentRobotNumber=r;
      myClient=robotNumberTable[r];
      printf("Now controlling robot number %d.\n",r);

    } else printf("Robot %d not started yet\n",r);
  else if (r!=-1) printf("Bad robot number argument %d\n",r);
  else {
    printf("Broadcasting robot commands to all robots\n");
    currentRobotNumber=-1;
  }
  ArUtil::sleep(500);
}


void InputHandler::sendInput(void)
{
  /* This method is called by the main function to send a ratioDrive
   * request with our current velocity values. If the server does 
   * not support the ratioDrive request, then we abort now: */
  if(!myClient->dataExists("ratioDrive")) return;

  /* Construct a ratioDrive request packet.  It consists
   * of three doubles: translation ratio, rotation ratio, and an overall scaling
   * factor. */
  ArNetPacket packet;
  packet.doubleToBuf(myTransRatio);
  packet.doubleToBuf(myRotRatio);
  packet.doubleToBuf(PWM_ratio); // use half of the robot's maximum.
  if (myPrinting)
    printf("Sending\n");

  if (currentRobotNumber<0) 
    for (int i=0; i<maxRobotSoFar; i++)
     robotNumberTable[i]->requestOnce("ratioDrive", &packet);
  else myClient->requestOnce("ratioDrive", &packet);

  sinceScan = time(NULL) - lastScan;
/*
  if ( myJoyHandler.getButton(2) )
    if ( (sinceScan>10) ) {//send stereo scan  command 
      myClient->requestOnce("doStereoScan", NULL);
      lastScan=time(NULL);
    } //else printf("rejecting bounce %d\n",sinceScan);
*/
  myRotRatio = myTransRatio = 0;
}/*  */


/** This class requests continual data updates from the server and prints them
 * out.
 */
class OutputHandler
{
public:

  double myX;
  double myY;
  double myTh;
  double myVel;
  double myRotVel;
  double myVoltage;
  char myStatus[256];
  char myMode[32];
  OutputHandler(ArClientBase *client);
  virtual ~OutputHandler(void);
  
  /// This callback is called when an update on general robot state arrives
  void handleOutput(ArNetPacket *packet);

  /// This callback is called when an update on the battery configuration changes
  void handleBatteryInfo(ArNetPacket *packet);
  /// This is called when the physical robot information comes back
  void handlePhysicalInfo(ArNetPacket *packet);

  void sData(ArNetPacket* packet);

  ArClientBase *getClient();
  void setClient(ArClientBase *c);

protected:

  /// The results from the data update are stored in these variables
  //@{
  
  //@}
  ArClientBase *myClient;

  /** These functor objects are given to the client to receive updates when they
   * arrive from the server.
   */
  //@{
  ArFunctor1C<OutputHandler, ArNetPacket *> myHandleOutputCB;
  ArFunctor1C<OutputHandler, ArNetPacket *> myHandleBatteryInfoCB;
  ArFunctor1C<OutputHandler, ArNetPacket *> myHandlePhysicalInfoCB;

  ArFunctor1C<OutputHandler, ArNetPacket *> mysDataCB;
  //@}
  
  /// A header for the columns in the data printout is sometimes printed
  bool myNeedToPrintHeader;
  /// Don't print any information until we get the battery info
  bool myGotBatteryInfo;
};

OutputHandler::OutputHandler(ArClientBase *client) :
  myClient(client),
  myHandleOutputCB(this, &OutputHandler::handleOutput),
  myHandleBatteryInfoCB(this, &OutputHandler::handleBatteryInfo),
  myHandlePhysicalInfoCB(this, &OutputHandler::handlePhysicalInfo),
  mysDataCB(this, &OutputHandler::sData),
  myNeedToPrintHeader(false),
  myGotBatteryInfo(false)
{
  /* Add a handler for battery info, and make a single request for it  */
  myClient->addHandler("physicalInfo", &myHandlePhysicalInfoCB);
  myClient->requestOnce("physicalInfo");


  /* Add a handler for battery info, and make a single request for it  */
  myClient->addHandler("batteryInfo", &myHandleBatteryInfoCB);
  myClient->requestOnce("batteryInfo");

  myClient->addHandler("getSensorList", &mysDataCB);
  myClient->requestOnce("getSensorList");

  /* Add a handler for general info, and request it to be called every 1000 ms */
  myClient->addHandler("update", &myHandleOutputCB);
  myClient->request("update", 500);
}

OutputHandler::~OutputHandler(void)
{
  /* Halt the request for data updates */
  myClient->requestStop("update");
}


ArClientBase *OutputHandler::getClient() { return myClient; }

void OutputHandler::setClient(ArClientBase *c) { 
  if (c==NULL) return;
 
  myClient->requestStop("update");
  myClient = c;
  printf("\nSwitching Robot telemetry Source.\n");
  myClient->requestOnce("physicalInfo");
  myClient->requestOnce("batteryInfo");
  myClient->addHandler("update", &myHandleOutputCB);
  myClient->request("update", 500);
}


void OutputHandler::handleOutput(ArNetPacket *packet)
{
  /* Extract the data from the update packet. Its format is status and
   * mode (null-terminated strings), then 6 doubles for battery voltage, 
   * x position, y position and orientation (theta) (from odometry), current
   * translational velocity, and current rotational velocity. Translation is
   * always milimeters, rotation in degrees.
   */
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

  if(myNeedToPrintHeader && !G_quiet) 
  {
    printf("\n%6s|%6s|%6s|%6s|%6s|%6s|%15s|%20s|\n",
      "x","y","theta", "vel", "rotVel", "volts", "mode","status");
    fflush(stdout);
    myNeedToPrintHeader = false;
  }
  if (myGotBatteryInfo && !G_quiet)
    printf("%6.0f|%6.0f|%6.1f|%6.1f|%6.1f|%6.1f|%15s|%20s|\r",
	   myX, myY, myTh, myVel, myRotVel, myVoltage, myMode, myStatus);
  
  fflush(stdout);
}

void OutputHandler::handleBatteryInfo(ArNetPacket *packet)
{
  /* Get battery configuration parameters: when the robot will begin beeping and 
   * warning about low battery, and when it will automatically disconnect and
   * shutdown. */
  double lowBattery = packet->bufToDouble();
  double shutdown = packet->bufToDouble();
  if (!G_quiet)
   printf("Low battery voltage: %6g       Shutdown battery voltage: %6g\n", lowBattery, shutdown);
  fflush(stdout);
  myNeedToPrintHeader = true;
  myGotBatteryInfo = true;
}


void OutputHandler::handlePhysicalInfo(ArNetPacket *packet)
{
  /* Get phyiscal configuration parameters: */
  char robotType[512];
  char robotSubtype[512];
  int width;
  int lengthFront;
  int lengthRear;

  packet->bufToStr(robotType, sizeof(robotType));
  packet->bufToStr(robotSubtype, sizeof(robotSubtype));
  width = packet->bufToByte2();
  lengthFront = packet->bufToByte2();
  lengthRear = packet->bufToByte2();

  if (!G_quiet)
   printf("Type: %s Subtype: %s Width %d: LengthFront: %d LengthRear: %d\n",
	 robotType, robotSubtype, width, lengthFront, lengthRear);
  fflush(stdout);
}

void OutputHandler::sData(ArNetPacket *packet)
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
}

/* Key handler for the escape key: shutdown all of Aria. */
void escape(void)
{
  printf("esc pressed, shutting down aria\n");
  Aria::shutdown();
}
