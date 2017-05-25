// Mgmtor_DAQ2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Mgmtor_DAQ2.h"
#include "Mgmtor_DAQ2Dlg.h"
#include "afxdialogex.h"
#include <fstream>
#include "Boolean28th.h"
#include <time.h>
#include <Windows.h>
//c
#include <stdio.h>
#include "C:\Program Files (x86)\National Instruments\Shared\ExternalCompilerSupport\C\Include\NIDAQmx.h"
//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//c
#define DAQmxErrChk1(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error1; else
//

using namespace std;
// Struct to pass args to DAQ thread
typedef struct SParam{
	TaskHandle  tHandle;
	int32       NumSamPerChan;
	float64     TimeOut;
	bool32      FillMode;
	uInt16      *ReadArray;
	uInt32      ArraySize;
	int32       *SampPerChanRead;
	bool32      *reserved;
}uParam, *sParam;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
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


// CMgmtor_DAQ2Dlg dialog



CMgmtor_DAQ2Dlg::CMgmtor_DAQ2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMgmtor_DAQ2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMgmtor_DAQ2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MGMOTORCTRL1, Motor1);
	DDX_Control(pDX, IDC_MGMOTORCTRL2, Motor2);
}

BEGIN_MESSAGE_MAP(CMgmtor_DAQ2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMgmtor_DAQ2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CMgmtor_DAQ2Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, &CMgmtor_DAQ2Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMgmtor_DAQ2Dlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CMgmtor_DAQ2Dlg message handlers

BOOL CMgmtor_DAQ2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);         // Set big icon
	SetIcon(m_hIcon, FALSE);        // Set small icon

	// TODO: Add extra initialization here

	Motor1.StartCtrl();
	Motor2.StartCtrl();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMgmtor_DAQ2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMgmtor_DAQ2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMgmtor_DAQ2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD WINAPI DAQThread(LPVOID lpParameter);

void CMgmtor_DAQ2Dlg::OnBnClickedButton1()
{

}





void CMgmtor_DAQ2Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CMgmtor_DAQ2Dlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	//DAQ Initiation
	int32       error = 0;
	TaskHandle  taskHandle = 0;
	int32       read;
	uInt16      data[336000];
	float64     timeout = 2100;  // data acquisition time span in sec	
	char        errBuff[2048] = { '\0' };
	float64     sampRate = 80;  // sampling rate in samples/sec
	float64     sampPerChan = 168000;
	clock_t     RealTime;
	clock_t     AcqStartTime;
	clock_t     pauseTime = 500;  // sleep time in mili sec
	///////////////////////


	// Write to file
	// File Operation 
	//ofstream FILEMotor("U:\\eng_research_biomicroscopy\\Ron\\Results\\1017\\PoTi_PHA_6.txt");
	ofstream FILEMotor("C:\\Users\\jingyis\\Documents\\us_obm_data\\05242017\\PoTi_PHA_6.txt");

	// DAQmx Configure Code
	DAQmxErrChk1(DAQmxCreateTask("", &taskHandle));
	DAQmxErrChk1(DAQmxCreateAIVoltageChan(taskHandle, "Dev1/ai0:1", "", DAQmx_Val_Cfg_Default, -0, 1, DAQmx_Val_Volts, NULL));
	DAQmxErrChk1(DAQmxCfgSampClkTiming(taskHandle, "OnBoardClock", sampRate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, sampPerChan));
	// DAQmx Start Code

	DAQmxErrChk1(DAQmxStartTask(taskHandle));

	// DAQ Thread
	SParam sparam;
	SParam *ParamPointer;
	ParamPointer = &sparam;
	sparam.tHandle = taskHandle;
	sparam.TimeOut = timeout;
	sparam.FillMode = DAQmx_Val_GroupByChannel;
	sparam.ArraySize = sampPerChan * 2;
	sparam.NumSamPerChan = -1;
	sparam.ReadArray = data;
	sparam.reserved = NULL;
	sparam.SampPerChanRead = &read;

	LPDWORD dwThreadId;
	HANDLE hThrd = NULL;
	AcqStartTime = clock();

	hThrd = (HANDLE)CreateThread(NULL, 0, DAQThread, ParamPointer, 0, NULL);

	CloseHandle(hThrd);

	// Scanning Configuration
	float       xResolution = 0.15;
	float       yResolution = 0.15;
	float       xStart = 0;
	float       xEnd = 15.15;
	float       ySteps = 70;
	int         ScanCount;
	float       Motor1Position = 0;
	float       Motor2Position = 0;
	float       LastPosition;
	long        Motor1Status;
	bool        MoveStatus; // 1 = moving, 0 = not moving

	// Home the motors
	Motor1.MoveHome(0, 1);
	Motor2.MoveHome(0, 1);

	// Every for loop contains four steps
	Motor1.SetAbsMovePos(0, xStart);    // Move to the start position
	Motor1.MoveAbsolute(0, 1);


	// Perform Scan
	for (ScanCount = 1; ScanCount < ySteps + 1; ScanCount++){
		// Stage one
		Motor1.SetVelParams(0, 0, 1.5, 1);
		LastPosition = -1;
		Motor2.GetPosition(0, &Motor2Position);
		//Motor1.LLGetStatusBits(0, &Motor1Status);
		//MoveStatus = Boolean28th(Motor1Status);
		Motor1.SetAbsMovePos(0, xEnd);
		Motor1.MoveAbsolute(0, 0);
		Sleep(32);      // If the program enters the while loop too fast, motion will not be detected due to slow accelaration of the motor.

		while (Motor1Position - LastPosition){
			LastPosition = Motor1Position;  // Use the position acquired last time to check the moving status
			Motor1.GetPosition(0, &Motor1Position);
			RealTime = clock();

			// Write directly to file
			FILEMotor << Motor1Position << '\t' << Motor2Position << '\t' << RealTime << endl;
		}
		// Stage two
		Motor1.SetVelParams(0, 0, 1.5, 2.5);
		Motor2.SetRelMoveDist(0, yResolution);
		Motor2.MoveRelative(0, 1);
		Motor1.SetAbsMovePos(0, xStart);
		Motor1.MoveAbsolute(0, 1);
		Sleep(pauseTime);

	}

	FILEMotor.close(); // Close the file stream
Error1:
	if (DAQmxFailed(error))
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	if (taskHandle != 0) {
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
}

void CMgmtor_DAQ2Dlg::OnBnClickedButton3()
{

}

DWORD WINAPI DAQThread(LPVOID lpParameter){
	sParam sparam;
	sparam = (sParam)lpParameter;
	// DAQmx Read Code
	clock_t startTime;
	clock_t endTime;
	startTime = clock();
	DAQmxReadBinaryU16(sparam->tHandle, sparam->NumSamPerChan, sparam->TimeOut, sparam->FillMode, sparam->ReadArray, sparam->ArraySize, sparam->SampPerChanRead, sparam->reserved);
	endTime = clock();
	// File Operation 
	//ofstream FILEDAQP("U:\\eng_research_biomicroscopy\\Ron\\Results\\1017\\DAQP_PHA_6.txt");
	//ofstream FILEDAQV("U:\\eng_research_biomicroscopy\\Ron\\Results\\1017\\DAQV_PHA_6.txt");
	//ofstream FILEDAQP("U:\\eng_research_biomicroscopy\\Ron\\Results\\20161115\\c6.txt");
	ofstream FILEDAQV("C:\\Users\\jingyis\\Documents\\us_obm_data\\05242017\\DAQV66.txt");

	int i;
	for (i = 0; i < sparam->ArraySize; i++){
		FILEDAQV << sparam->ReadArray[i] << endl;
		//FILEDAQP << sparam->ReadArray[i + sparam->ArraySize / 2] << endl;

	}
	FILEDAQV << startTime << endl << endTime << endl;
	FILEDAQV.close();
	//FILEDAQP.close(); 
	return 0;
}