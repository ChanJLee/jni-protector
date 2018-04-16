// AddSectionDlg.h : header file
//

#if !defined(AFX_ADDSECTIONDLG_H__C729F0A4_902B_4F37_8B7B_7025FEEA1766__INCLUDED_)
#define AFX_ADDSECTIONDLG_H__C729F0A4_902B_4F37_8B7B_7025FEEA1766__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAddSectionDlg dialog

class CAddSectionDlg : public CDialog
{
// Construction
public:
	BOOL IsELF(char *lpPath);
	int addSectionFun(char *lpPath, char *szSecname, unsigned int nNewSecSize);
	CAddSectionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAddSectionDlg)
	enum { IDD = IDD_ADDSECTION_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddSectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAddSectionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnButtonExit();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonLl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDSECTIONDLG_H__C729F0A4_902B_4F37_8B7B_7025FEEA1766__INCLUDED_)
