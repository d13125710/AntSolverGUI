// DExperiments.cpp : implementation file
//

#include "stdafx.h"
#include "GA.h"
#include "DExperiments.h"
#include "afxdialogex.h"


// CDExperiments dialog

IMPLEMENT_DYNAMIC(CDExperiments, CDialog)

CDExperiments::CDExperiments(CWnd* pParent /*=NULL*/)
	: CDialog(CDExperiments::IDD, pParent)
{

}

CDExperiments::~CDExperiments()
{
}

void CDExperiments::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDExperiments, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDExperiments::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDExperiments::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDExperiments::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDExperiments::OnBnClickedButton4)
	ON_BN_CLICKED(IDOK, &CDExperiments::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON5, &CDExperiments::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDExperiments::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON8, &CDExperiments::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CDExperiments::OnBnClickedButton9)
END_MESSAGE_MAP()


// CDExperiments message handlers


void CDExperiments::OnBnClickedButton1()
{
	 exp = 1;
	 CDialog::OnOK();
}


void CDExperiments::OnBnClickedButton2()
{
		 exp = 2;
		 	CDialog::OnOK();
}


void CDExperiments::OnBnClickedButton3()
{
		 exp = 3;
		 	CDialog::OnOK();
}


void CDExperiments::OnBnClickedButton4()
{
			 exp = 4;
			 	CDialog::OnOK();
}


void CDExperiments::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


INT_PTR CDExperiments::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::DoModal();
}


void CDExperiments::OnBnClickedButton5()
{
		 exp = 5;
			CDialog::OnOK();
}


void CDExperiments::OnBnClickedButton6()
{
		exp = 6;
		CDialog::OnOK();
}


void CDExperiments::OnBnClickedButton8()
{
	 exp = 7;
	 	CDialog::OnOK();
}


void CDExperiments::OnBnClickedButton9()
{
	// TODO: Add your control notification handler code here
	exp = 8;
	CDialog::OnOK();
}
