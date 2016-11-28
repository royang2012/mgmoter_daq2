#pragma once


// MGMotorScan dialog

class MGMotorScan : public CDialogEx
{
	DECLARE_DYNAMIC(MGMotorScan)

public:
	MGMotorScan(CWnd* pParent = NULL);   // standard constructor
	virtual ~MGMotorScan();

// Dialog Data
	enum { IDD = IDD_MGMTOR_DAQ2_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
