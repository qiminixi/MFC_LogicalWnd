// LogicalWndDlg.h : header file
//

#pragma once


// CLogicalWndDlg dialog
class CLogicalWndDlg : public CDialog
{
// Construction
public:
	CLogicalWndDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LOGICALWND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	virtual BOOL OnEraseBkgnd(CDC* pDC);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};
