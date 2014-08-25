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

