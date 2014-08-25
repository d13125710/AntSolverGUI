#include "StdAfx.h"
#include "Experiments.h"
#include "ProblemSolver.h"

#include "CTSPLIBFileReader.h"

#include "TspsolverFactory.h"

#include "xlsLog.h"

#include "StopWatch.h"

#include <numeric>
#include <vector>















UINT AlphaBeta(LPVOID pParam);
UINT MyExperiment2(LPVOID pParam);
UINT ElitestTest(LPVOID pParam);
UINT branchingToursTest(LPVOID pParam);
UINT ShorestTourTest(LPVOID pParam);
UINT AvgTourTest(LPVOID pParam);
UINT MAnt(LPVOID pParam);
UINT NoOfAnts(LPVOID pParam);
UINT EvaporitionTest(LPVOID pParam);
void RunAverages(std::map<string , int> &instancesArray , std::vector<int> &alg , std::vector<Parameters> &parametersArray , xlsLog& Log  , Experiments *tsp );
UINT LocalSearchTest(std::map<string , int> &instancesArray , Experiments *tsp , Parameters par , xlsLog &Log, std::vector<int>& alg);
UINT LocalSearchTestLK(LPVOID pParam);
UINT LocalSearchTestOp2(LPVOID pParam);
UINT LocalSearchTestOp3(LPVOID pParam);
UINT AntBattle(LPVOID pParam);
UINT NormalTest(LPVOID pParam);
UINT runLocal(Parameters &par , Experiments *tsp , std::string &filename , std::vector<int>& alg);



UINT ACSTest(LPVOID pParam);

int noofAvg = 10;

UINT RunMe(LPVOID pParam);


Experiments::Experiments(void)
{
}


Experiments::~Experiments(void)
{
}

void Experiments::Run(Parameters &par ,  std::vector<std::vector<int> > &matrix, int ex)
{
	
	
	if(!m_pThread)
	{
		PopulateInstances();
		constructParameterExp1();
		m_pmatrix_ = &matrix;
		m_EndThread=false;

		switch (ex)
		{
		case -1: 
			break;
		case 1:
			m_pThread = AfxBeginThread(AntBattle,((LPVOID)this));
			break;
		case 2:
			m_pThread = AfxBeginThread(RunMe,((LPVOID)this));
			break;
		case 3:
			m_pThread = AfxBeginThread(ElitestTest,((LPVOID)this));
			break;
		case 4:
			m_pThread = AfxBeginThread(AvgTourTest,((LPVOID)this));
			break;
		case 5:
			m_pThread = AfxBeginThread(NormalTest,((LPVOID)this));
			break;
		case 6:
			m_pThread = AfxBeginThread(LocalSearchTestOp2,((LPVOID)this));
			break;
		case 7:
			m_pThread = AfxBeginThread(LocalSearchTestOp3,((LPVOID)this));
			break;
		case 8:
			m_pThread = AfxBeginThread(LocalSearchTestLK,((LPVOID)this));
			break;
		default:
			// Do something 
			break;
		};
	}

}
void Experiments::PopulateInstances()
{
	//use map 
	//m_problemFiles["C:\\tsp\\ei8246.tsp");
//	m_problemFiles["C:\\tsp\\ulysses16.tsp"] =6859;
//	m_problemFiles["C:\\tsp\\ulysses22.tsp"] = 7013;
//	m_problemFiles["C:\\tsp\\eil51.tsp"]=426;
	m_problemFiles["C:\\tsp\\berlin52.tsp"] = 7542;
//	m_problemFiles["C:\\tsp\\eil101.tsp"]=629;
	m_problemFiles["C:\\tsp\\bier127.tsp"]=118282;

	m_problemFiles["C:\\tsp\\att532.tsp"]=202339;
//	m_problemFiles["C:\\tsp\\tsp225.tsp"] =3916;
//	m_problemFiles["C:\\tsp\\rat575.tsp"] =6773;
//	m_problemFiles["C:\\tsp\\p654.tsp"] =34643;

	//m_problemFiles["C:\\tsp\\pr2392.tsp"] =378032;
//	m_problemFiles["C:\\tsp\\vm1748.tsp"] =336556;
//	m_problemFiles["C:\\tsp\\vm1084.tsp"] =239297;


}

void Experiments::constructParameterExp1()
{
	Parameters par;
	par.rho = 0.2;
	par.Epochs = 5000;
	par.EliteWeight = 5;

	for(int i=0 ; i< 5; i++) //loop alpha
	{
		par.Alpha = 1;
		for(int j=0 ; j< 10; j++) //loop beta
		{
			par.Beta = j;
			m_parameters.push_back(par);
		}
	}
}
//************************************
// Method:    MyExperiment1
// FullName:  MyExperiment1
// Access:    public 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID pParam
// runs expirement 1 as in disseration
//************************************
UINT AlphaBeta(LPVOID pParam)
{
	Experiments *tsp = (Experiments *)pParam;

	std::map<string , int> instancesArray;// = tsp->GetInstancesArray();

	instancesArray["C:\\tsp\\ulysses16.tsp"] =6859;
	instancesArray["C:\\tsp\\ulysses22.tsp"] = 7013;
	instancesArray["C:\\tsp\\eil51.tsp"]=426;
	instancesArray["C:\\tsp\\berlin52.tsp"] = 7542;
	instancesArray["C:\\tsp\\eil101.tsp"]=629;
	instancesArray["C:\\tsp\\bier127.tsp"]=118282;

	std::vector<Parameters> parametersArray;


	Parameters par;
	par.rho = 0.2;
	par.Epochs = 1;
	par.EliteWeight = 5;
	par.noAnts = 25;
	par.noAnts = 25;

	for(int i=0 ; i< 5; i++) //loop alpha
	{
		par.Alpha = i;
		for(int j=0 ; j< 10; j++) //loop beta
		{
			par.Beta = j;
			parametersArray.push_back(par);
		}
	}


	std::vector<int> alg;
	alg.push_back(AS);
	alg.push_back(EAS);
	alg.push_back(BWAS);
	alg.push_back(MMAS);
	alg.push_back(ACS);
	alg.push_back(MA);


	xlsLog Log;
	Log.Add();

	Log.chartsaddTitle();
	Log.ShowPar(parametersArray[0]);
	Log.AddParameters(parametersArray);

//	Log.save();	


	
	map <string, int>::iterator it;
	for (it = instancesArray.begin(); it != instancesArray.end(); ++it)
	{
			
		CTSPLIBFileReader fileReader(it->first);
		fileReader.Read();
		std::vector<std::vector<int> >  &m_pmatrix_ = fileReader.getMatrixbyref();

		for(int a=0; a < alg.size();a++)
		{
			//Log.chartsaddvalue(par.getAlgType(), itter , d);
			Parameters p;
			p.fileName = it->first;
			p.problemLength = it->second;
			p.ALG = alg[a];
			Log.startRow2(p);
			Log.AddResults("");

		

			for(int j=0; j < parametersArray.size();j++)
			{
				std::vector<size_t> avg(5);

				for(int i=0;i<5;i++)
				{
					Parameters par = parametersArray[j];
					par.ALG = alg[a];
					iProblemSolver *pMM = TSPSolverFactory::create(par, &m_pmatrix_);
					int d =(std::numeric_limits<int>::max)();
					int itter = 0;
					pMM->initSystem();
				
						while(pMM->getTourNo() < par.Epochs)
						//{//for(int i = 0; i <par.Epochs; i++)
						{
					
							if(tsp->m_EndThread){
								delete pMM;
								Log.save("AlphabetaPar.xlsx");	
								tsp->m_pThread =0;
								return 0;
							}
					
							pMM->constructSolutions();
							pMM->localSearch();
							pMM->update();
							pMM->incrementItteration();
							int dist= (int)pMM->getBestPathLengthSofar();
							if(dist < d ){
								d = dist;
								itter = pMM->getTourNo();
							}

						
						}
						//average for one run..
						avg[i]=d;
						delete pMM;
						
				}
				long int sum = std::accumulate(avg.begin(), avg.end(), 0);
				double mean = sum / avg.size();
				Log.AddResults(mean);
		
			}
			Log.EndRow();
			
			static std::string s;
			s= par.getAlgType();
			AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s);
		}
		Log.insertBlankRow();
		Log.insertBlankRow();
		Log.insertBlankRow();
	}
	Log.save("AlphabetaPar.xlsx");	
	tsp->m_pThread =0;
	return 0;
}

UINT MyExperiment2(LPVOID pParam)
{
	Experiments *tsp = (Experiments *)pParam;

	std::map<string , int> instancesArray = tsp->GetInstancesArray();


	std::vector<int> alg;
	alg.push_back(AS);
	alg.push_back(EAS);
	//alg.push_back(BWAS);
	alg.push_back(MMAS);
	alg.push_back(ACS);


	xlsLog Log;
	Log.Add();

	Parameters par;
	par.rho = 0.2;
	par.Epochs = 5000;
	par.EliteWeight = 5;
	par.Alpha = 1;
	par.Beta = 4;
	par.noAnts = 50;

	std::vector<int> avg(par.Epochs);

	Log.experiment2Title(par.Epochs);

	map <string, int>::iterator it;
	for (it = instancesArray.begin(); it != instancesArray.end(); ++it)
	{

		CTSPLIBFileReader fileReader(it->first);
		fileReader.Read();
		std::vector<std::vector<int> >  &m_pmatrix_ = fileReader.getMatrixbyref();

		for(int a=0; a < alg.size();a++)
		{
			//Log.chartsaddvalue(par.getAlgType(), itter , d);
			Parameters p;
			p.fileName = it->first;
			p.problemLength = it->second;
			p.ALG = alg[a];
			Log.startRow(p.getAlgType(), p.fileName);
			
			par.ALG = alg[a];
		for(int i=0;i<5;i++)
		{
			iProblemSolver *pMM = TSPSolverFactory::create(par, &m_pmatrix_);
			int d =(std::numeric_limits<int>::max)();
			int itter = 0;
				pMM->initSystem();
					for(int i = 0; i <par.Epochs; i++)
					{

						if(tsp->m_EndThread){
							delete pMM;
							Log.save();	
							tsp->m_pThread =0;
							return 0;
						}

						pMM->constructSolutions();
						pMM->localSearch();
						pMM->update();
						pMM->incrementItteration();
						int dist= (int)pMM->getBestPathLengthSofar();

						Tour Tour1= pMM->getStats();
						if(dist < d ){
							d = dist;
							itter = i;
						}
						Log.AddResults(Tour1.pop_stddev);
						Log.AddResults(Tour1.branching_factor);
						Log.AddResults(Tour1.bestTourToDateLenght);
						Log.AddResults(Tour1.worseTourToDateLenght);
						Log.AddResults(0);


						avg[i]=dist;
					}
					//average for one run..
					long int sum = std::accumulate(avg.begin(), avg.end(), 0);
					double mean = sum / avg.size();

					//double sq_sum = std::inner_product(avg.begin(), avg.end(), avg.begin(), 0.0);
					//double stdev = std::sqrt(sq_sum / avg.size() - mean * mean);
					Log.AddResults(mean);
				

			}
			tsp->NotifyFinished(0);
			Log.EndRow();
			Log.insertBlankRow();
			Log.insertBlankRow();
			Log.insertBlankRow();
			static std::string s;
			s= par.getAlgType();
			AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s);
		}//end algo

	}
	Log.save();	
	tsp->m_pThread =0;
	return 0;


}

UINT ElitestTest(LPVOID pParam)
{
	Experiments *tsp = (Experiments *)pParam;
	
	std::map<string , int> instancesArray;//=otherReseachers();// = tsp->GetInstancesArray();



		instancesArray["C:\\tsp\\ulysses16.tsp"] =6859;
		instancesArray["C:\\tsp\\ulysses22.tsp"] = 7013;
		instancesArray["C:\\tsp\\eil51.tsp"]=426;
	    instancesArray["C:\\tsp\\berlin52.tsp"] = 7542;
	    instancesArray["C:\\tsp\\bier127.tsp"]=118282;
		instancesArray["C:\\tsp\\eil101.tsp"]=629;

	//instancesArray["C:\\tsp\\att532.tsp"]=27686;
		//	i
		instancesArray["C:\\tsp\\tsp225.tsp"] =3916;
	//	instancesArray["C:\\tsp\\rat575.tsp"] =6773;
	//	instancesArray["C:\\tsp\\p654.tsp"] =34643;

	//instancesArray["C:\\tsp\\pr2392.tsp"] =378032;
	//	instancesArray["C:\\tsp\\vm1748.tsp"] =336556;
	//	instancesArray["C:\\tsp\\vm1084.tsp"] =239297;
		//instancesArray["C:\\tsp\\ei8246.tsp");



	std::vector<Parameters> parametersArray;
	std::vector<Parameters> parametersArray2;

	xlsLog Log;
	Log.Add();

	Parameters par;
	par.rho = 0.3;
	par.Epochs = 500;
	par.Alpha = 1;
	par.Beta = 4;
	par.ALG = EAS;
	par.EliteWeight = 1;
	parametersArray.push_back(par);

	


	Log.chartsaddTitle();
	Log.ShowPar(parametersArray[0]);
	Log.AddEliteParameters(parametersArray);

	map <string, int>::iterator it;
	for (it = instancesArray.begin(); it != instancesArray.end(); ++it)
	{

		CTSPLIBFileReader fileReader(it->first);
		fileReader.Read();
		std::vector<std::vector<int> >  &m_pmatrix_ = fileReader.getMatrixbyref();
		
		
		
		for(int elite=1; elite< m_pmatrix_.size(); elite=elite + 10)
		{

			par.EliteWeight = elite;
			parametersArray2.push_back(par);

		}
	
		par.EliteWeight = 0;
		parametersArray2.push_back(par);
		parametersArray2[0].fileName = it->first;
		parametersArray2[0].problemLength = it->second;
		Log.startRow2( parametersArray2[0]);

	
		for(int j=0; j < parametersArray.size();j++)
		{
				
			std::vector<size_t> avg2(noofAvg);
		
				

				parametersArray2[j].fileName = it->first;
				parametersArray2[j].problemLength = it->second;
				
			
		

			
				for(int b=0;b<noofAvg;b++) // run averages
				{
					std::vector<size_t> avg(par.Epochs);
					Parameters par = parametersArray2[j];
					par.noAnts = fileReader.getMatrixSize();
					par.fileName = it->first;
					par.problemLength = it->second;

					iProblemSolver *pMM = TSPSolverFactory::create(par, &m_pmatrix_);
					int d =(std::numeric_limits<int>::max)();
					int itter = 0;
					pMM->initSystem();
				
	

					for(int i = 0; i <par.Epochs; i++)
					{
						if(tsp->m_EndThread){
							delete pMM;
							Log.save("elitestExperiment.xlsx");	
							tsp->m_pThread =0;
							return 0;
						}
						pMM->constructSolutions();
					//	pMM->localSearch();
						pMM->update();
						pMM->incrementItteration();
						int dist= (int)pMM->getBestPathLengthSofar();
				//		Tour Tour1= pMM->getStats();
						if(dist < d ){
							d = dist;
							itter = pMM->getItteration();
						}
						//found solution so no need to find more
						if(d ==  it->second)
						{
							for(int zz=i; zz < par.Epochs; zz++ )
							{
								avg[zz] =dist;
							}
							break;
						}

						avg[i]=dist;
					}

					long int sum = std::accumulate(avg.begin(), avg.end(), 0);
					double mean = sum / par.Epochs;
 					avg2[b]=mean;
					delete pMM;
					
				}//end averages
				long int sum = std::accumulate(avg2.begin(), avg2.end(), 0);
				double mean = sum / noofAvg;
				Log.AddResults(mean);
			
				static std::string s;
				s= par.getAlgType();
				AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s);
			}//end averages
			Log.EndRow();	
		
	} //end problem

	//}
	Log.save("elitestExperiment.xlsx");	
	tsp->m_pThread =0;
	return 0;

}






UINT branchingToursTest(LPVOID pParam)
{
	
	//plot itteration againts branching factors
	
	std::vector<int> alg;
	alg.push_back(AS);
	alg.push_back(EAS);
	alg.push_back(BWAS);
	alg.push_back(MMAS);
	alg.push_back(ACS);


	Experiments *tsp = (Experiments *)pParam;
	std::map<string , int> instancesArray;// = tsp->GetInstancesArray();
	instancesArray["C:\\tsp\\bier127.tsp"]=118282;

	xlsLog Log;
	Log.Add();

	Parameters par;
	par.rho = 0.2;
	par.Epochs = 500;
	par.Alpha = 5;
	par.Beta = 2;
	par.noAnts = 50;
	
	par.EliteWeight = 5;

	Log.insertParametersHeader(par);
	Log.insertBlankRow();
	//Log.experiment2Title(par.Epochs);
	std::string s = make_string() << "*Branching Factors*";
	Log.insertTitleLine(s);

	map <string, int>::iterator it;
	for (it = instancesArray.begin(); it != instancesArray.end(); ++it)
	{

		CTSPLIBFileReader fileReader(it->first);
		fileReader.Read();
		std::vector<std::vector<int> >  &m_pmatrix_ = fileReader.getMatrixbyref();
		par.fileName = it->first;
		par.problemLength = it->second;
		

		for(int a=0; a < alg.size();a++)
		{
			
			 par.ALG = alg[a];
			
				Log.startRow(par.getAlgType(), par.fileName);

				iProblemSolver *pMM = TSPSolverFactory::create(par, &m_pmatrix_);
				int d =(std::numeric_limits<int>::max)();
				int itter = 0;
				pMM->initSystem();
				for(int i = 0; i <par.Epochs; i++)
				{
					if(tsp->m_EndThread){
						delete pMM;
						Log.save();	
						tsp->m_pThread =0;
						return 0;
					}
					pMM->constructSolutions();
					pMM->localSearch();
					pMM->update();
					pMM->incrementItteration();
					int dist= (int)pMM->getBestPathLengthSofar();
					Tour Tour1= pMM->getStats();
					if(dist < d ){
						d = dist;
						itter = i;
					}
					//Log.AddResults(Tour1.pop_stddev);
					Log.AddResults(Tour1.branching_factor);
					//Log.AddResults(Tour1.bestTourToDateLenght);
					//Log.AddResults(Tour1.worseTourToDateLenght);
				//	Log.AddResults(0);
					if(it->second == d) //opt found
						break;
				}
				tsp->NotifyFinished(0);
				delete pMM;
				Log.EndRow();
		}
		static std::string s;
		s= par.getAlgType();
		AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s);
		Log.insertBlankRow();
	} //end instance
	Log.save("branchingfactor.xlsx");	
	tsp->m_pThread =0;
	return 0;

}



UINT ShorestTourTest(LPVOID pParam)
{

	//plot itteration againts branching factors

	std::vector<int> alg;
	alg.push_back(AS);
	alg.push_back(EAS);
	alg.push_back(BWAS);
	alg.push_back(MMAS);
	alg.push_back(ACS);


	Experiments *tsp = (Experiments *)pParam;
	std::map<string , int> instancesArray;// = tsp->GetInstancesArray();
	//instancesArray["C:\\tsp\\bier127.tsp"]=118282;
	instancesArray["C:\\tsp\\berlin52.tsp"] = 7542;

	xlsLog Log;
	Log.Add();

	Parameters par;
	par.rho = 0.3;
	par.Epochs = 1500;
	par.Alpha = 1;
	par.Beta = 4;
	par.noAnts = 50;

	par.EliteWeight = 5;

	Log.insertParametersHeader(par);
	Log.insertBlankRow();
	//Log.experiment2Title(par.Epochs);
	std::string s = make_string() << "*shorest tour*";
	Log.insertTitleLine(s);

	map <string, int>::iterator it;
	for (it = instancesArray.begin(); it != instancesArray.end(); ++it)
	{

		CTSPLIBFileReader fileReader(it->first);
		fileReader.Read();
		std::vector<std::vector<int> >  &m_pmatrix_ = fileReader.getMatrixbyref();
		par.fileName = it->first;
		par.problemLength = it->second;


		for(int a=0; a < alg.size();a++)
		{

			par.ALG = alg[a];

			Log.startRow(par.getAlgType(), par.fileName);

			iProblemSolver *pMM = TSPSolverFactory::create(par, &m_pmatrix_);
			int d =(std::numeric_limits<int>::max)();
			int itter = 0;
			pMM->initSystem();
			for(int i = 0; i <par.Epochs; i++)
			{
				if(tsp->m_EndThread){
					delete pMM;
					Log.save();	
					tsp->m_pThread =0;
					return 0;
				}
				pMM->constructSolutions();
				pMM->localSearch();
				pMM->update();
				pMM->incrementItteration();
				int dist= (int)pMM->getBestPathLengthSofar();
				Tour Tour1= pMM->getStats();
				if(dist < d ){
					d = dist;
					itter = i;
				}
				//Log.AddResults(Tour1.pop_stddev);
				Log.AddResults(Tour1.bestTourToDateLenght);
				//Log.AddResults(Tour1.bestTourToDateLenght);
				//Log.AddResults(Tour1.worseTourToDateLenght);
				//	Log.AddResults(0);
				if(it->second == d) //opt found
					break;
			}
			tsp->NotifyFinished(0);
			delete pMM;
			Log.EndRow();
		}
		static std::string s;
		s= par.getAlgType();
		AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s);
		Log.insertBlankRow();
	} //end instance
	Log.save("bestTourToDateLenght.xlsx");	
	tsp->m_pThread =0;
	return 0;

}
std::map<string , int> otherReseachers()
{
	std::map<string , int> instancesArray;// = tsp->GetInstancesArray();



	//	instancesArray["C:\\tsp\\ulysses16.tsp"] =6859;
	//	instancesArray["C:\\tsp\\ulysses22.tsp"] = 7013;
	//	instancesArray["C:\\tsp\\eil51.tsp"]=426;
	instancesArray["C:\\tsp\\kroA100.tsp"] = 7542;
	instancesArray["C:\\tsp\\eil51.tsp"] = 7542;
	instancesArray["C:\\tsp\\d198.tsp"] = 7542;
	return instancesArray;

}



UINT AvgTourTest(LPVOID pParam)
{

	//plot itteration againts branching factors

	std::vector<int> alg;
	alg.push_back(AS);
	alg.push_back(EAS);
	alg.push_back(BWAS);
	alg.push_back(MMAS);
	alg.push_back(ACS);
	alg.push_back(GS);

	//to get avg store all tours in epoch size vector
	//them sum them and / by eproch

	Experiments *tsp = (Experiments *)pParam;
	std::map<string , int> instancesArray;//=otherReseachers();// = tsp->GetInstancesArray();



	//	instancesArray["C:\\tsp\\ulysses16.tsp"] =6859;
	//	instancesArray["C:\\tsp\\ulysses22.tsp"] = 7013;
	//	instancesArray["C:\\tsp\\eil51.tsp"]=426;
	instancesArray["C:\\tsp\\berlin52.tsp"] = 7542;

	//instancesArray["C:\\tsp\\bier127.tsp"]=118282;

	//instancesArray["C:\\tsp\\att532.tsp"]=27686;
		//	instancesArray["C:\\tsp\\eil101.tsp"]=629;
		instancesArray["C:\\tsp\\tsp225.tsp"] =3916;
	//	instancesArray["C:\\tsp\\rat575.tsp"] =6773;
	//	instancesArray["C:\\tsp\\p654.tsp"] =34643;

	//instancesArray["C:\\tsp\\pr2392.tsp"] =378032;
	//	instancesArray["C:\\tsp\\vm1748.tsp"] =336556;
	//	instancesArray["C:\\tsp\\vm1084.tsp"] =239297;
		//instancesArray["C:\\tsp\\ei8246.tsp");


	

	xlsLog Log;
	Log.Add();

	Parameters par;
	par.rho = 0.3;
	par.r0 = 0.1;
	par.Epochs = 1000;
	par.Alpha = 1;
	par.Beta = 4;
	//par.noAnts = 25;
	par.xi = 0.3;
	par.q0 = 0.90;
	par.EliteWeight = 10;
	//par.LocalSearchGreed = true;
	std::vector<int> avg(par.Epochs);

	Log.insertParametersHeader(par);
	Log.insertBlankRow();
	//Log.experiment2Title(par.Epochs);
	std::string s = make_string() << "*Average and best tour*";
	Log.insertTitleLine(s);

	Log.insertAvgBestHeader(alg);

	map <string, int>::iterator it;
	for (it = instancesArray.begin(); it != instancesArray.end(); ++it)
	{

		CTSPLIBFileReader fileReader(it->first);
		fileReader.Read();
		std::vector<std::vector<int> >  &m_pmatrix_ = fileReader.getMatrixbyref();
		par.fileName = it->first;
		par.problemLength = it->second;

		par.noAnts = m_pmatrix_.size();

		for(int z=0;z<noofAvg; z++)
		{
			Log.startRow(par.fileName);

			for(int a=0; a < alg.size();a++)
			{

				par.ALG = alg[a];
				int irr=0;
				iProblemSolver *pMM = TSPSolverFactory::create(par, &m_pmatrix_);
				int d =(std::numeric_limits<int>::max)();
				int itter = 0;
				double foundtimesec=0;
				pMM->initSystem();
				for(int i = 0; i <par.Epochs; i++)
				{
				
					if(tsp->m_EndThread){
						delete pMM;
						Log.save("averagetour.xlsx");	
						tsp->m_pThread =0;
						return 0;
					}
					pMM->constructSolutions();
					pMM->localSearch();
					pMM->update();
					pMM->incrementItteration();
					int dist= (int)pMM->getBestPathLengthSofar();
					
				
					if(dist < d ){
						Tour Tour1= pMM->getStats();
						d = dist;
						itter = i;
						foundtimesec = Tour1.foundTime;
					}
					avg[irr]=dist;
					irr++;
					//int no=pMM->getTourNo();
					//if(no > 10000)
				//	{
				//		break;
				//	}
				}
				//average for one run..
				long int sum = std::accumulate(avg.begin(), avg.end(), 0);
				double mean = sum /irr;

				//double sq_sum = std::inner_product(avg.begin(), avg.end(), avg.begin(), 0.0);
				//double stdev = std::sqrt(sq_sum / avg.size() - mean * mean);
				Log.AddResults(d);
				Log.AddResults(mean);
				Log.AddResults(itter);
				Log.AddResults(foundtimesec);
				tsp->NotifyFinished(0);
				delete pMM;

			}
		  Log.EndRow();
			
		}
		static std::string s;
		s= par.getAlgType();
		AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s);
		Log.EndRow();
		Log.insertBlankRow();
	} //end instance
	Log.save("averagetournosearch.xlsx");	
	tsp->m_pThread =0;
	return 0;

}


//************************************
// Method:    ACSTest
// FullName:  ACSTest
// Access:    public 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID pParam
// test parameters of q0
//xi
//ro
//seting a = 1
//b =5
//r0 = 0.1
//************************************
UINT ACSTest(LPVOID pParam)
{
	Experiments *tsp = (Experiments *)pParam;
	std::vector<Parameters> m_parameters;
	std::map<string , int> instancesArray= tsp->GetInstancesArray();
	xlsLog Log;
	Log.Add();
	for(int i=1 ; i< 6; i++) //loop alpha
	{
		for(int j=1 ; j< 10; j++) //loop beta
		{
			Parameters par;
			par.rho = 0.1;
			par.r0 = 0.1;
			par.Epochs = 5000;
			par.Alpha = 1;
			par.Beta = 4;
			par.noAnts = 25;
			par.xi =(double) i* 0.1;
			par.q0 = (double) j* 0.1;
			par.ALG= ACS;
			m_parameters.push_back(par);
		}
	}

	for(int j=1 ; j< 10; j++) //loop beta
	{
		Parameters par;
		par.rho = 0.1;
		par.r0 = 0.1;
		par.Epochs = 5000;
		par.Alpha = 1;
		par.Beta = 4;
		par.noAnts = 25;
		par.xi =0.3;
		par.q0 = (double)  0.9 + (double)j*0.01;
			par.ALG= ACS;
		m_parameters.push_back(par);
	}


	for(int j=1 ; j< 10; j++) //loop beta
	{
		Parameters par;
		par.rho = 0.1;
		par.r0 = 0.1;
		par.Epochs = 5000;
		par.Alpha = 1;
		par.Beta = 4;
		par.noAnts = 25;
		par.xi =0.1;
		par.q0 = (double)  0.9 + (double)j*0.01;
		par.ALG= ACS;
		m_parameters.push_back(par);
	}

	Log.chartsaddTitle();
	Log.ShowPar(m_parameters[0]);
	
	Log.insertBlankRow();
	Log.insertBlankRow();
	
	CStopWatch stopwatch;
	stopwatch.startTimer();

	Log.AddACOParameters(m_parameters);

	map <string, int>::iterator it;
	for (it = instancesArray.begin(); it != instancesArray.end(); ++it)
	{

		CTSPLIBFileReader fileReader(it->first);
		int solution = it->second;
		fileReader.Read();
		std::vector<std::vector<int> >  &m_pmatrix_ = fileReader.getMatrixbyref();
				m_parameters[0].fileName =  it->first;
			
				Log.startRow2(m_parameters[0]);
			for(int j=0; j < m_parameters.size();j++)
			{
				Parameters par = m_parameters[j];
				par.fileName =  it->first;
				std::vector<int> avg2(noofAvg);
			
			
				for(int z=0;z<noofAvg;z++)
				{
						
							std::vector<int> avg(par.Epochs);
							iProblemSolver *pMM = TSPSolverFactory::create(par, &m_pmatrix_);
							int d =(std::numeric_limits<int>::max)();
							int itter = 0;
							pMM->initSystem();
							while(pMM->getTourNo() < par.Epochs)
							{

								if(tsp->m_EndThread){
									delete pMM;
									Log.save("acoparmeterstest.xlsx");	
									tsp->m_pThread =0;
									return 0;
								}

								pMM->constructSolutions();
								pMM->localSearch();
								pMM->update();
								pMM->incrementItteration();
								int dist= (int)pMM->getBestPathLengthSofar();
								if(dist < d ){
									d = dist;
						            //tter = pMM->getItteration();
								}
								avg[itter]= dist;
								itter++;
							
							}

							long int sum = std::accumulate(avg.begin(), avg.end(), 0);
							double mean = sum / itter-1;

							avg2[z] =mean;

							delete pMM;
					
						} // end averages
				
				long int sum = std::accumulate(avg2.begin(), avg2.end(), 0);
				double mean = sum / noofAvg;
				Log.AddResults(mean);
			}//end par
		
			Log.EndRow();
			static std::string s;
			s= make_string() << "ACS" << " " << it->first << " xi= " << m_parameters[0].xi << " q0=" << m_parameters[0].q0;
			AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s);	
			Log.insertBlankRow();
			Log.insertBlankRow();
			Log.insertBlankRow();

			Log.insertBlankRow();
		
				
		}



	tsp->NotifyFinished(0);
	double stoptime = stopwatch.getRunningTime();
	Log.insertComment("Time taken" , stoptime);

	static std::string s1;
	s1= make_string() << "finished";
	AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s1);	

	Log.save("acoparmeterstest.xlsx");	
	tsp->m_pThread =0;
	return 0;
}

UINT MAnt(LPVOID pParam)
{

//plot itteration againts branching factors

std::vector<int> alg;
alg.push_back(MMAS);
alg.push_back(ACS);
alg.push_back(MA);

//to get avg store all tours in epoch size vector
//them sum them and / by eproch

Experiments *tsp = (Experiments *)pParam;
std::map<string , int> instancesArray= tsp->GetInstancesArray();


xlsLog Log;
Log.Add();

Parameters par;
par.rho = 0.2;
par.Epochs = 10000;
par.Alpha = 1;
par.Beta = 3;
par.noAnts = 30;
par.LocalSearchGreed =true;


par.EliteWeight = 5;

std::vector<int> avg(par.Epochs);

Log.insertParametersHeader(par);
Log.insertBlankRow();
std::string s = make_string() << "*Average  best tour MM AS*";
Log.insertTitleLine(s);

Log.insertAvgBestHeader(alg);

map <string, int>::iterator it;
for (it = instancesArray.begin(); it != instancesArray.end(); ++it)
{

	CTSPLIBFileReader fileReader(it->first);
	fileReader.Read();
	std::vector<std::vector<int> >  &m_pmatrix_ = fileReader.getMatrixbyref();
	par.fileName = it->first;
	par.problemLength = it->second;





		Log.startRow(par.fileName);

		for(int a=0; a < alg.size();a++)
		{

			par.ALG = alg[a];
			par.LocalSearchGreed =true;
			iProblemSolver *pMM = TSPSolverFactory::create(par, &m_pmatrix_);
			int d =(std::numeric_limits<int>::max)();
			int itter = 0;
			long int tourfound=0;
			double foundtimesec=0;
			pMM->initSystem();
			while(pMM->getTourNo() < par.Epochs)
			{
				if(tsp->m_EndThread){
					delete pMM;
					Log.save();	
					tsp->m_pThread =0;
					return 0;
				}
				pMM->constructSolutions();
				pMM->localSearch();
				pMM->update();
				pMM->incrementItteration();
				int dist= (int)pMM->getBestPathLengthSofar();
				Tour Tour1= pMM->getStats();
				if(dist < d ){
					d = dist;
					itter = pMM->getItteration();
					foundtimesec = Tour1.foundTime;
					tourfound = pMM->getTourNo();

				}
				if(d == it->second  ){
					break;
				}
				//avg[i]=dist;
			}
			//average for one run..
			//long int sum = std::accumulate(avg.begin(), avg.end(), 0);
		//	double mean = sum / avg.size();

			//double sq_sum = std::inner_product(avg.begin(), avg.end(), avg.begin(), 0.0);
			//double stdev = std::sqrt(sq_sum / avg.size() - mean * mean);

			if(d == it->second  ){
				std::string s = make_string() << "@" <<tourfound;	
				Log.AddResults(s);
			}
			else{
				Log.AddResults(d);
			}

			Log.AddResults("");
			Log.AddResults(tourfound);
			Log.AddResults(foundtimesec);
			tsp->NotifyFinished(0);
			delete pMM;

	}
	Log.EndRow();
	Log.insertBlankRow();
} //end instance
Log.save("bestaverageMA.xlsx");	
tsp->m_pThread =0;
return 0;

}


UINT NoOfAnts(LPVOID pParam)
{
	Experiments *tsp = (Experiments *)pParam;
	std::map<string , int> instancesArray = tsp->GetInstancesArray();
	std::vector<Parameters> parametersArray;


	std::vector<int> alg;
	alg.push_back(AS);
	alg.push_back(EAS);
	alg.push_back(BWAS);
	alg.push_back(MMAS);
	alg.push_back(ACS);
	//alg.push_back(MA);


	xlsLog Log;
	Log.Add();

	Parameters par;
	par.rho = 0.1;
	par.Epochs = 500;
	par.Alpha = 1;
	par.Beta = 4;
	par.noAnts = 5;
	par.xi = 0.1;
	par.q0 = 0.95;
	parametersArray.push_back(par);
	par.noAnts = 10;
	parametersArray.push_back(par);
	par.noAnts = 25;
	parametersArray.push_back(par);
	par.noAnts = 50;
	parametersArray.push_back(par);
	par.noAnts = 100;
	parametersArray.push_back(par);
	par.noAnts = 0;
	parametersArray.push_back(par);



	Log.chartsaddTitle();
	Log.ShowPar(parametersArray[0]);
	Log.AddNoOfAntsParameters(parametersArray);

	map <string, int>::iterator it;
	for (it = instancesArray.begin(); it != instancesArray.end(); ++it)
	{

		CTSPLIBFileReader fileReader(it->first);
		fileReader.Read();
		std::vector<std::vector<int> >  &m_pmatrix_ = fileReader.getMatrixbyref();


		
		for(int a=0; a < alg.size();a++) //run al
		{
		
			Parameters par1= parametersArray[0];
			par1.ALG = alg[a];
			par1.fileName = it->first;
			par1.problemLength = it->second;
			
			Log.startRow2( par1);

			for(int j=0; j < parametersArray.size();j++)
			{
			

						std::vector<size_t> avg_alg(5);
						Parameters par = parametersArray[j];
						if(par.noAnts == 0)
							par.noAnts = fileReader.getMatrixSize();
						par.fileName = it->first;
						par.problemLength = it->second;
						par.ALG = alg[a];
						int found_count=0;
						for(int b=0;b<5;b++) // run averages
						{
							std::vector<size_t> avg(parametersArray[j].Epochs);
							iProblemSolver *pMM = TSPSolverFactory::create(par, &m_pmatrix_);
							int d =(std::numeric_limits<int>::max)();
							int itter = 0;
							pMM->initSystem();
							int i=0;
						
							//while(pMM->getTourNo() < par.Epochs)
							for(int i = 0; i <par.Epochs; i++)
							{
								if(tsp->m_EndThread){
									delete pMM;
									Log.save("noofants.xlsx");	
									tsp->m_pThread =0;
									return 0;
								}
								pMM->constructSolutions();
								//pMM->localSearch();
								pMM->update();
								pMM->incrementItteration();
								int dist= (int)pMM->getBestPathLengthSofar();
								//Tour Tour1= pMM->getStats();
								if(dist < d ){
									d = dist;
									itter = pMM->getItteration();
								}
								//found solution so no need to find more
								if(d ==  it->second)
								{
									found_count++;
									for(int zz=i; zz < par.Epochs; zz++ )
									{
										avg[zz] =dist;
									}
									break;
								}
								
								
								

								avg[i] =dist;
							}
							
							delete pMM;
							long int sum = std::accumulate(avg.begin(), avg.end(), 0);
							double mean = sum / par.Epochs;
							avg_alg[b] = mean;
						
							
							


						}//end averages
					
				long int sum = std::accumulate(avg_alg.begin(), avg_alg.end(), 0);
				double mean = sum /5;
				Log.AddResults(mean);
				Log.AddResults(found_count);
	
			}//end parameters
			Log.EndRow();
			Log.insertBlankRow();
			Log.insertBlankRow();
			Log.insertBlankRow();
			Log.insertBlankRow();
			static std::string s;
			s= make_string() << par.getAlgType() << " " << par.fileName << " " << par.rho;
			AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s);
		
		//	Log.insertBlankRow();
		} //end alg
 		Log.insertBlankRow();
		Log.insertBlankRow();
	//	Log.insertBlankRow();
		//Log.insertBlankRow();
	//	Log.insertBlankRow();
	} //end problem
	//}
	Log.save("noofants.xlsx");	
	tsp->m_pThread =0;
	return 0;

}




UINT EvaporitionTest(LPVOID pParam)
{

	Experiments *tsp = (Experiments *)pParam;

	std::map<string , int> instancesArray= tsp->GetInstancesArray();

	
	std::vector<Parameters> parametersArray;




	for(double i=0.0 ; i<0.7; i=i+0.1) //loop alpha
	{
		
		Parameters par;
		par.rho = 0.1;
		par.Epochs = 2500;
		par.EliteWeight = 10;
		par.noAnts = 25;
		par.Alpha = 1;
		par.Beta = 4;
		
		par.rho = i;
		par.r0 = i*0.01;
		parametersArray.push_back(par);
	}


	std::vector<int> alg;
	alg.push_back(AS);
	alg.push_back(EAS);
	alg.push_back(BWAS);
	alg.push_back(MMAS);
	alg.push_back(ACS);
	//alg.push_back(MA);
	
	xlsLog Log;
	Log.Add();

	Log.chartsaddTitle();
	Log.ShowPar(parametersArray[0]);
	Log.AddEvapParameters(parametersArray);

	RunAverages(instancesArray,alg,parametersArray,Log,tsp);
	Log.insertBlankRow();
	Log.save("evapvalues.xlsx");
	return 0;

}

void RunAverages(std::map<string , int> &instancesArray , std::vector<int> &alg , std::vector<Parameters> &parametersArray , xlsLog& Log  , Experiments *tsp )
{
	map <string, int>::iterator it;
	Parameters par;

	for(int a=0; a < alg.size();a++)
	{
		
	for (it = instancesArray.begin(); it != instancesArray.end(); ++it)
	{

		CTSPLIBFileReader fileReader(it->first);
		fileReader.Read();
		std::vector<std::vector<int> >  &m_pmatrix_ = fileReader.getMatrixbyref();
		par.fileName = it->first;
		par.problemLength = it->second;
		par.ALG = alg[a];
		Log.startRow(par.getAlgType(),par.fileName);

		

			for(int j=0; j < parametersArray.size();j++)
			{
				Parameters par = parametersArray[j];
				par.ALG = alg[a];

				if(par.noAnts == 0 && (par.ALG == AS || par.ALG == EAS || par.ALG == BWAS))
					par.noAnts = fileReader.getMatrixSize()-1;


				std::vector<size_t> avg2(noofAvg);	
				int found_count=0;

				for(int average = 0 ; average < noofAvg ; average++)
				{

					std::vector<size_t> avg(par.Epochs);	
					iProblemSolver *pMM = TSPSolverFactory::create(par, &m_pmatrix_);
					int d =(std::numeric_limits<int>::max)();
					int itter = 0;
					double foundtimesec=0;
					pMM->initSystem();
					for(int i = 0; i <par.Epochs; i++)
					{
						if(tsp->m_EndThread){
							delete pMM;
							//Log.save();	
							tsp->m_pThread =0;
							return;
						}
						pMM->constructSolutions();
						//pMM->localSearch();
						pMM->update();
						pMM->incrementItteration();
						int dist= (int)pMM->getBestPathLengthSofar();
						//Tour Tour1= pMM->getStats();
						if(dist < d ){
							d = dist;
							itter = i;
						//	foundtimesec = Tour1.foundTime;
						}
						if(d ==  it->second)
						{
							found_count++;
							for(int zz=i; zz < par.Epochs; zz++ )
							{
								avg[zz] =dist;
							}
							break;
						}
						avg[i]=dist;
					}

					//average for one run..
					long int sum = std::accumulate(avg.begin(), avg.end(), 0);
					double mean = sum / avg.size();
					avg2[average]= mean;
					delete pMM;

				}
				//aver over all run
				long int sum = std::accumulate(avg2.begin(), avg2.end(), 0);
				double mean = sum / avg2.size();
				Log.AddResults(mean);
				Log.AddResults(found_count);
		
		

			}
			Log.EndRow();
			static std::string s;
			s= make_string() << par.getAlgType() << " " << par.fileName << " " << par.rho;
			AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s);
			

		}//end 
	Log.insertBlankRow();
	Log.insertBlankRow();
	Log.insertBlankRow();
	Log.insertBlankRow();
	Log.insertBlankRow();
	} //end instance
}

UINT runLocal(Parameters &par , Experiments *tsp , std::string &filename , std::vector<int>& alg)
{
	std::map<string , int> instancesArray1;// = tsp->GetInstancesArray();
	std::map<string , int> instancesArray2;
	std::map<string , int> instancesArray3;
	std::map<string , int> instancesArray4;
	std::map<string , int> instancesArray5;
	std::map<string , int> instancesArray6;
	std::map<string , int> instancesArray7;
	std::map<string , int> instancesArray8;
	std::map<string , int> instancesArray9;
	instancesArray1["C:\\tsp\\ulysses16.tsp"] =6859;
	instancesArray1["C:\\tsp\\ulysses22.tsp"] = 7013;
	instancesArray1["C:\\tsp\\eil51.tsp"]=426;
	instancesArray1["C:\\tsp\\berlin52.tsp"] = 7542;
	instancesArray1["C:\\tsp\\eil101.tsp"]=629;
	instancesArray2["C:\\tsp\\bier127.tsp"]=118282;
	instancesArray2["C:\\tsp\\ch150.tsp"] =6528;
//	instancesArray3["C:\\tsp\\d198.tsp"] =15780;
	
	//instancesArray4["C:\\tsp\\tsp225.tsp"] =3916;
	instancesArray5["C:\\tsp\\gil262.tsp"] =2378;
	instancesArray6["C:\\tsp\\att532.tsp"]=27686;

	//battle
	//instancesArray6["C:\\tsp\\p654.tsp"] =34643;
	//instancesArray7["C:\\tsp\\d1291.tsp"] =50801;
	//instancesArray8["C:\\tsp\\vm1748.tsp"] =336556;
	


	//instancesArray3["C:\\tsp\\p654.tsp"] =34643;
//	instancesArray4["C:\\tsp\\d1291.tsp"] =50801;
//	instancesArray5["C:\\tsp\\fl1400.tsp"]=20127;
//	instancesArray6["C:\\tsp\\d2103.tsp"] =80450;
	//instancesArray9["C:\\tsp\\p2392.tsp"] =378032;

	//instancesArray9["C:\\tsp\\usa13509.tsp"] =0;

	
	//instancesArray9["C:\\tsp\\p2392.tsp"] =378032;
	//instancesArray8["C:\\tsp\\ei8246.tsp"] =0;

	instancesArray8["C:\\tsp\\ei8246.tsp"] =0;

	
	xlsLog Log1;
//	LocalSearchTest(instancesArray1 , tsp , par , Log1 , alg);
	std::string file= filename + "1.xlsx";
	//Log1.save(file);	
	xlsLog Log2;
	file= filename + "2.xlsx";
//	LocalSearchTest(instancesArray2 , tsp , par , Log2 , alg);
//	Log2.save(file);	
	xlsLog Log3;
	file= filename + "3.xlsx";
	//LocalSearchTest(instancesArray3 , tsp , par , Log3 , alg);
	//Log3.save(file);	
	xlsLog Log4;
	file= filename + "4.xlsx";
	//LocalSearchTest(instancesArray4 , tsp , par , Log4, alg);
	//Log4.save(file);	
	xlsLog Log5;
	file= filename + "5.xlsx";
	//LocalSearchTest(instancesArray5 , tsp , par , Log5, alg);
	//Log5.save(file);
	xlsLog Log6;
	file= filename + "att532.xlsx";
	//LocalSearchTest(instancesArray6 , tsp , par , Log6, alg);
	Log6.save(file);

	xlsLog Log7;
	file= filename + "7.xlsx";
	//LocalSearchTest(instancesArray7 , tsp , par , Log7, alg);
	//Log7.save(file);

	xlsLog Log8;
	file= filename + "ei8246.xlsx";
	LocalSearchTest(instancesArray8 , tsp , par , Log8, alg);
	Log8.save(file);

	xlsLog Log9;
	file= filename + "usa13509.xlsx";
	//LocalSearchTest(instancesArray9 , tsp , par , Log9, alg);
	Log9.save(file);
//	tsp->NotifyFinished(0);
//	tsp->m_pThread =0;
	return 0;
}


UINT LocalSearchTestLK(LPVOID pParam)
{
	std::vector<int> alg1, alg2 , alg3  , alg4 , alg5 , alg6 , alg7;
	
	alg1.push_back(MA);
	alg2.push_back(MMAS);
	alg3.push_back(ACS);
	alg4.push_back(GS);
	alg5.push_back(AS);
	alg6.push_back(EAS);
	alg7.push_back(BWAS);


	Experiments *tsp = (Experiments *)pParam;
	Parameters par;
	par.rho = 0.3;
	par.Epochs = 10000;
	par.Alpha = 1;
	par.Beta = 4;
	par.noAnts = 25;
	par.r0 = 0.1;
	par.q0 = 0.90;
	par.xi = 0.3;
	par.EliteWeight = 10;
	par.LocalSearchGreed =true;
	
	std::string file="localSerachaverageLK_MA";
	runLocal(par , tsp , file,alg1);

	file="localSerachaverageLK_MMAS";
	runLocal(par , tsp , file,alg2);

	file="localSerachaverageLK_ACS";
	runLocal(par , tsp , file,alg3);
	file="localSerachaverageLK_GS";
	runLocal(par , tsp , file,alg4);
	file="localSerachaverageLK_AS";
	runLocal(par , tsp , file,alg5);
	file="localSerachaverageLK_EAS";
	runLocal(par , tsp , file,alg6);
	file="localSerachaverageLK_BWAS";
	runLocal(par , tsp , file,alg7);
	
	return 0;
}
UINT LocalSearchTestOp2(LPVOID pParam)
{
	std::vector<int> alg;
	alg.push_back(AS);
	alg.push_back(EAS);
	alg.push_back(BWAS);
	alg.push_back(MMAS);
	alg.push_back(ACS);
	alg.push_back(GS);
	Experiments *tsp = (Experiments *)pParam;
	Parameters par;
	par.rho = 0.5;
	par.Epochs = 10000;
	par.Alpha = 1;
	par.Beta = 4;
	par.noAnts = 25;
	par.r0 = 0.1;
	par.q0 = 0.90;
	par.xi = 0.3;
	par.EliteWeight = 10;
	par.LocalSearchOpt2 =true;
	std::string file="localSerachaverageOpt2_";
		return runLocal(par , tsp , file,alg);

}
UINT LocalSearchTestOp3(LPVOID pParam)
{
	std::vector<int> alg;
	alg.push_back(AS);
	alg.push_back(EAS);
	alg.push_back(BWAS);
	alg.push_back(MMAS);
	alg.push_back(ACS);
	alg.push_back(GS);
	Experiments *tsp = (Experiments *)pParam;
	Parameters par;
	par.rho = 0.3;
	par.Epochs = 10000;
	par.Alpha = 1;
	par.Beta = 4;
	par.noAnts = 25;
	par.r0 = 0.1;
	par.q0 = 0.90;
	par.xi = 0.3;
	par.EliteWeight = 10;
	par.LocalSearchOpt3 =true;
	std::string file="localSerachaverageOpt3_";
		return runLocal(par , tsp , file,alg);

}
UINT NormalTest(LPVOID pParam)
{
	std::vector<int> alg;
	alg.push_back(AS);
	alg.push_back(EAS);
	alg.push_back(BWAS);
	alg.push_back(MMAS);
	alg.push_back(ACS);
	alg.push_back(GS);
	Experiments *tsp = (Experiments *)pParam;
	Parameters par;
	par.rho = 0.5;
	par.Epochs = 10000;
	par.Alpha = 1;
	par.Beta = 4;
	par.noAnts = 25;
	par.r0 = 0.1;
	par.q0 = 0.90;
	par.xi = 0.3;
	par.EliteWeight = 10;
	//par.LocalSearchOpt3 =true;
	std::string file="Normalaverage_";
	return runLocal(par , tsp , file,alg);

}
UINT AntBattle(LPVOID pParam)
{

	std::vector<int> alg1, alg2 , alg3  , alg4 , alg5 , alg6 , alg7;
	alg1.push_back(MA);
	alg2.push_back(MMAS);
	alg3.push_back(ACS);

	Experiments *tsp = (Experiments *)pParam;
	Parameters par;
	par.rho = 0.3;
	par.Epochs = 10000;
	par.Alpha = 1;
	par.Beta = 4;
	par.noAnts = 25;
	par.r0 = 0.1;
	par.q0 = 0.90;
	par.xi = 0.3;
	par.EliteWeight = 0;
	par.mutationRatio =1.5;
	par.LocalSearchGreed =true;
//	std::string file="localSerachaverageLK_";
	std::string file="battleoftheants_MA";
//	runLocal(par , tsp , file, alg1);

	file="battleoftheants_MMAS";
	//runLocal(par , tsp , file, alg2);

	file="battleoftheants_ACS";
	runLocal(par , tsp , file, alg3);

	return 0;
}





UINT LocalSearchTest(std::map<string , int> &instancesArray , Experiments *tsp , Parameters par , xlsLog &Log , std::vector<int>& alg)
{
	Log.Add();
	Log.insertParametersHeader(par);
	Log.insertBlankRow();
	std::string s = make_string() << "*LocalSearchTest and best tour*";
	Log.insertTitleLine(s);
	Log.insertAvgBestHeader(alg);
	map <string, int>::iterator it;
	for (it = instancesArray.begin(); it != instancesArray.end(); ++it)
	{

		CTSPLIBFileReader fileReader(it->first);
		fileReader.Read();
		std::vector<std::vector<int> >  &m_pmatrix_ = fileReader.getMatrixbyref();
		par.fileName = it->first;
		par.problemLength = it->second;
		std::vector<int> avg2(noofAvg);
		for(int z=0;z<10; z++)
		{
			
			Log.startRow(par.fileName);
			for(int a=0; a < alg.size();a++)
			{
				std::vector<int> avg(par.Epochs);
				par.ALG = alg[a];
				par.EliteWeight = m_pmatrix_.size();
				iProblemSolver *pMM = TSPSolverFactory::create(par, &m_pmatrix_);
				int d =(std::numeric_limits<int>::max)();
				int itter = 0;
				double foundtimesec=0;
				pMM->initSystem();
				CStopWatch stop;
				stop.startTimer();
				int i = 0;
				while( pMM->getTourNo() < par.Epochs)
				{
					if(tsp->m_EndThread){
						delete pMM;
						Log.save("localsearchaveragetour.xlsx");	
						tsp->m_pThread =0;
						return 0;
					}
					pMM->constructSolutions();
					pMM->localSearch();
					pMM->update();
					pMM->incrementItteration();
					int dist= (int)pMM->getBestPathLengthSofar();
					if(dist < d ){
						d = dist;
						itter = i;
						Tour Tour1= pMM->getStats();
						foundtimesec = Tour1.foundTime;
						static std::string zs;
						zs= make_string() << "found d " << par.getAlgType() << " distabce=" << d << "@"<<z << " itt=" << foundtimesec; 
						AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&zs);
					}
					avg[i++]=dist;
					double runningtime = stop.getRunningTime();
					if(d ==  it->second)
					{
						break;
					}
					if(runningtime > 1000)
					{
						break;
					}
				
				}
				//average for one run..
				long int sum = std::accumulate(avg.begin(), avg.end(), 0);
				double mean = sum / i;
				Log.AddResults(d);
				Log.AddResults(mean);
				Log.AddResults(itter);
				Log.AddResults(foundtimesec);
				delete pMM;
				static std::string s;
				s= make_string() << "Finished run " << par.getAlgType() << " Itt=" << z; 
				AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s);


			}
			Log.EndRow();
			static std::string s1;
			s1= "finished row";
			AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s1);


		}
		Log.EndRow();
		Log.insertBlankRow();
		static std::string s2;
		s2= make_string() << "finished row" << " " << it->first;
		AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&s2);
	} //end instance
	return 0;

}


UINT RunMe(LPVOID pParam)
{
	//AlphaBeta(pParam); done 
	//ACSTest(pParam); done
	//ElitestTest(pParam);
//NoOfAnts( pParam);
	//EvaporitionTest(pParam);
//	MAnt(pParam);
	//AvgTourTest(pParam);
//	LocalSearchTest(pParam);
	return 0;


}

