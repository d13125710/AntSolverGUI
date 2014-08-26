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



#include <string>
#include <vector>
#include <map>

#include "Utils.h"
#include "observer.h"




UINT MyThreadProc(LPVOID pParam);


class TSPalgorithm
{
	enum {MMAS =5 , ACS=0 , AS=1 , GS=4, EAS=3 , BWAS=2 , RAS=6};
	Parameters m_par;
public:

	TSPalgorithm(void);
	~TSPalgorithm(void);
	CWinThread* m_pThread;
	std::vector<std::vector<int> >  *m_pmatrix_;
	virtual void Run(Parameters &par ,  std::vector<std::vector<int> > &matrix);	
	void AddObserver( Observer* ob );  
  	void Notify(Tour &tour);  
	void NotifyFinished(int i);  
	volatile bool m_EndThread;
		
private:
	//int iterations;
	std::vector<Observer*> ob_set; 
	Tour bestTour;
	std::map<std::string,unsigned int> m_Soltions;
	void populateSolutionMap(std::string &file  , unsigned int distance)
	{
		
	}
	void createSolutionsMap();

public:
	const Parameters &GetParameters() const{
		return m_par;
	}

	bool stopThread();


};

