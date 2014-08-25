#include "StdAfx.h"
#include "resource.h"
#include "TSPalgorithm.h"
#include "ProblemSolver.h"
#include "TspsolverFactory.h"

//************************************
// Method:    MyThreadNew
// FullName:  MyThreadNew
// Access:    public 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID pParam
//************************************
UINT MyThreadNew(LPVOID pParam)
{
	TSPalgorithm *tsp = (TSPalgorithm *)pParam;
	Parameters par= tsp->GetParameters();
	iProblemSolver *pMM = TSPSolverFactory::create(par, &*tsp->m_pmatrix_);
	pMM->initSystem();
	for(int i = 0; i < par.Epochs; i++)
	{
		if(tsp->m_EndThread){
			break;
		}
		pMM->constructSolutions();
		pMM->localSearch();
		pMM->update();
		pMM->incrementItteration();
		int dist= (int)pMM->getBestPathLengthSofar();
		static Tour tour;
		tour= pMM->getStats();
		tsp->Notify(tour);
	}
	tsp->m_pThread =0;
	tsp->NotifyFinished(0);
	delete pMM;
	return 0;
}
//************************************
// Method:    TSPalgorithm
// FullName:  TSPalgorithm::TSPalgorithm
// Access:    public 
// Returns:   
// Qualifier:
// Parameter: void
//************************************
TSPalgorithm::TSPalgorithm(void)
{
	createSolutionsMap();
	m_pThread = 0;
	m_EndThread = false;
}
TSPalgorithm::~TSPalgorithm(void)
{
}
//************************************
// Method:    createSolutionsMap
// FullName:  TSPalgorithm::createSolutionsMap
// Access:    private 
// Returns:   void
// Qualifier:
//************************************
void TSPalgorithm::createSolutionsMap()
{
	populateSolutionMap(string("berlin52") , 7542);
	populateSolutionMap(string("eil51") , 426);
	populateSolutionMap(string("pr2392") , 378032);
	populateSolutionMap(string("ulysses16") , 6859);
	populateSolutionMap(string("fl1400") , 20127);
	populateSolutionMap(string("rat783") , 8806);
	populateSolutionMap(string("pr2392") , 378032);

}
//************************************
// Method:    Run
// FullName:  TSPalgorithm::Run
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: Parameters & par
// Parameter: std::vector<std::vector<int> > & matrix
// Run the optimization algorithm
//************************************
void TSPalgorithm::Run(Parameters &par ,   std::vector<std::vector<int> >  &matrix )
{
	if(!m_pThread)
	{
		this->m_par =par;
		m_pmatrix_ = &matrix;
		m_EndThread=false;
		m_pThread = AfxBeginThread(MyThreadNew,((LPVOID)this));
	}
}
//************************************
// Method:    stopThread
// FullName:  TSPalgorithm::stopThread
// Access:    public 
// Returns:   bool
// Qualifier:
//************************************
bool TSPalgorithm::stopThread()
{
	if(m_pThread)
	{
		m_pThread->SuspendThread();
		m_EndThread = true;
		m_pThread->ResumeThread();
	}
	return true;
}
//************************************
// Method:    Notify
// FullName:  TSPalgorithm::Notify
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: newTour & tour
// Notify observers of changes
//************************************
void TSPalgorithm::Notify( Tour &tour )  
{ 
	for ( std::vector<Observer*>::iterator it = ob_set.begin(); it != ob_set.end();it++){  
		Observer* ob = *it;  
		ob->newBestTour(tour);
    }  
}  
//************************************
// Method:    NotifyFinished
// FullName:  TSPalgorithm::NotifyFinished
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int i
// Notify the main thread that we have finished
//************************************
void TSPalgorithm::NotifyFinished(int i) 
{ 
	for ( std::vector<Observer*>::iterator it = ob_set.begin(); it != ob_set.end();it++){  
		Observer* ob = *it;  
		ob->finished(i);
	} 
	ob_set.resize(0);
}  
//************************************
// Method:    AddObserver
// FullName:  TSPalgorithm::AddObserver
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: Observer * ob
// Add an observer
//************************************
void TSPalgorithm::AddObserver( Observer* ob )
{
	ob_set.push_back( ob );
}

