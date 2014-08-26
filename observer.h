//************************************
// written by Gareth Hunter DIT Kevin Street artefact's for dissertation 
// MSc advanced software engineering Aug 2014
// The artefacts are available under the GNU license for download and modification
// any questions please email me at gar_hunter@yahoo.com
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//************************************
#include "StdAfx.h"
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

