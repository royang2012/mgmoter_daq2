// MGMotorScan.cpp : implementation file
//

#include "stdafx.h"
#include "Mgmtor_DAQ2.h"
#include "MGMotorScan.h"
#include "afxdialogex.h"


// MGMotorScan dialog

IMPLEMENT_DYNAMIC(MGMotorScan, CDialogEx)

MGMotorScan::MGMotorScan(CWnd* pParent /*=NULL*/)
	: CDialogEx(MGMotorScan::IDD, pParent)
{

}

MGMotorScan::~MGMotorScan()
{
}

void MGMotorScan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MGMotorScan, CDialogEx)
END_MESSAGE_MAP()


// MGMotorScan message handlers
