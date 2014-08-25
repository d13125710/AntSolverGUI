#pragma once


// CDExperiments dialog

class CDExperiments : public CDialog
{
	DECLARE_DYNAMIC(CDExperiments)

public:
	int exp;
	CDExperiments(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDExperiments();

// Dialog Data
	enum { IDD = IDD_DEXPERIMENTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedOk();
	virtual INT_PTR DoModal();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
};
