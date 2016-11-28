
// Mgmtor_DAQ2Dlg.h : header file
//

#pragma once
#include "mgmotorctrl1.h"


// CMgmtor_DAQ2Dlg dialog
class CMgmtor_DAQ2Dlg : public CDialogEx
{
// Construction
public:
	CMgmtor_DAQ2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MGMTOR_DAQ2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMgmotorctrl1 Motor1;
	afx_msg void OnBnClickedButton1();
	CMgmotorctrl1 Motor2;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
