#pragma once
#include "resource.h"
#include "MakeString.h"
#include "Utils.h"
#include "log.h"

class Observer  
{  
public:  
     virtual void newBestTour( Tour &tour ) = 0;  
	 virtual void finished( int i ) = 0; 
}; 

class MfcObserver:  public Observer 
{

public:
	virtual void newBestTour( Tour &tour )
	{
		AfxGetMainWnd()->PostMessage(IDC_STATICFILENAME ,0, (LPARAM)&tour );
	}
	virtual void finished( int i ) 
	{
		AfxGetMainWnd()->PostMessage(IDC_INCITTERATION ,0, (LPARAM)&i );
	}


};
class LogObserver:  public Observer 
{
private:

	antlogger::CLog m_alog;

public:
	LogObserver(const std::string &file , Parameters &p) :
	  m_alog(file.c_str() , p)
	{
		m_alog << p;
	}
	LogObserver(){
	
	}
	void Init(Parameters &p)
	{
		CTime t = CTime::GetCurrentTime(); 
		CString ts = t.Format(_T("%H%M%S_%A%B%d%Y"));
		std::string s = make_string() << p.problemName << "_solution_" << ts<<".sol";
		m_alog.init(s.c_str() , p);
		m_alog << p;
	}

	virtual void newBestTour( Tour &tour )
	{
		m_alog << tour;
	}
	virtual void finished( int i ) 
	{

		m_alog.close();
	}
};

