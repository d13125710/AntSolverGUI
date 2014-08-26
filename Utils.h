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
#include <cstdarg>
#include <map>
#include "MakeString.h"


#define IDC_INCITTERATION 2096
#define IDC_VERBOSE 2097

#define ID_INDICATOR_NISH WM_USER +100
#define ID_INDICATOR_TIME WM_USER +101


typedef std::vector<std::vector<int> > MatrixArrayTypeInt;
typedef std::vector<std::vector<double> > MatrixArrayTypeDouble;

//AntColonly Sytem;AntSystem;MinMax Ant System;Elitest AntSytem;BWAnt System;Mutated Ant;Gentic System;Brute Force;
enum {MMAS =2 , ACS=0 , AS=1 , GS=6, EAS=3 , BWAS=4 , MA=5 , BF=7 , RA=8};



struct  Tour{
	std::vector<size_t> bestTourToDate;
	std::vector<size_t> worstTourToDate;
	std::vector<size_t> bestTourItter;
	std::vector<size_t> worstTourItter;
	double bestTourToDateLenght;
	double worseTourToDateLenght;
	double worstTourLenghtItter;
	double bestTourLenghtItter;
	size_t currentIrreation;
	std::string extrainfo;
	double bestTourFounditter;
	double worstTourFounditter;
	//extended stats
	double Distance_betweenAnts;
	double branching_factor;
	double pop_stddev;
	double mean;
	double tourAverageTime;
	double foundTime;
	long int noOfToursContructed;
	double timeRunning;

	std::string toString()
	{
		return make_string() << "Distancebetween Ants=" << Distance_betweenAnts
							 << ",branching_factor=" << branching_factor
							 << ",pop_stddev=" << pop_stddev
							 << ",mean=" << mean;
							// << ",extrainfo=" << extrainfo;
	}
};


class Parameters
{
public:
	int ALG; double rho;	int Alpha;	int Beta;	int noAnts;

	int EliteWeight;
	//ACS
	double xi; 	double q0; double r0;
	//search optionscla
	bool LocalSearchOpt2;bool LocalSearchOpt3;bool LocalSearchGreed;
	//min max ant
	double lambda;
	//genetic
	double mutationRatio;
	int maxStagnationIterations;

	int Epochs;

	bool enablelog;

	std::string fileName;
	std::string problemName;
	int problemLength;

	Parameters() : 
		EliteWeight(10) , xi(0.3) , q0(0.90) , r0(0.1) , LocalSearchOpt2(false) , LocalSearchOpt3(false) ,LocalSearchGreed(false) ,
			lambda(0.05) , mutationRatio(0.98) , maxStagnationIterations(100) , Epochs(5000) , enablelog(true) , problemLength(0) ,
			problemName("U"), fileName("U") , rho(0.1)
	{
		//create a random filename with date..

	}
	void setRho(int Q)
	{
		rho = (double) (0.1 * Q);
	}
	std::string getAlgType()
	{
		std::string returnstring;
		switch (ALG)
		{
			case (MMAS) :
				returnstring = "Min Max Ant System";
				break;
			case (ACS) :
				returnstring = "Ant Colony System";
				break;
			case (AS) :
				returnstring = "simple Ant System";
				break;
			case (GS) :
				returnstring = "Genetic System";
				break;
			case (BWAS) :
				returnstring = "Best Worst Ant System";
				break;
			case (MA) :
				returnstring = "Mutated Ant System";
				break;
			case (EAS) :
				returnstring = "Elitist Ant System";
				break;
			case (RA) :
				returnstring = "Random Ant System";
				break;
			default:
				break;
	}

	return returnstring;
	}

	std::map<std::string , std::string> SplitIntoMap()
	{
		std::map<std::string , std::string> parmap;
		parmap["Algorithm"] = getAlgType();
		parmap["rho"]= (make_string() << rho);
		parmap["alpha"]= (make_string() << Alpha);
		parmap["beta"]= (make_string() << Beta);;
		parmap["no of ants"]= (make_string() << noAnts);
		parmap["Elite Weight"]= (make_string() << EliteWeight);;
		parmap["xi"]= make_string() << xi;
		parmap["q0"]= make_string() << q0;
		parmap["r0"]= make_string() << r0;;
		parmap["3opt"]= make_string() << LocalSearchOpt3;;
		parmap["2opt"]= make_string() << LocalSearchOpt2;;
		parmap["LKHopt"]= make_string() << LocalSearchGreed;;
		parmap["lambda"]= make_string() << lambda;;
		parmap["mutationRatio"]= make_string() << mutationRatio;;
		parmap["Max stags"]= make_string() << maxStagnationIterations;;
		parmap["Epochs"]= make_string() << Epochs;;
		parmap["file name"]= fileName;
		parmap["opt solution"]= make_string() << problemLength;
		parmap["Problem Name"]= problemName;
		return parmap;
	}
};


//missing string printf
inline std::string format(const char* fmt, ...){
	int size = 512;
	char* buffer = 0;
	buffer = new char[size];
	va_list vl;
	va_start(vl, fmt);
	int nsize = vsnprintf(buffer, size, fmt, vl);
	if(size<=nsize){ //fail delete buffer and try again
		delete[] buffer;
		buffer = 0;
		buffer = new char[nsize+1]; //+1 for /0
		nsize = vsnprintf(buffer, size, fmt, vl);
	}
	std::string ret(buffer);
	va_end(vl);
	delete[] buffer;
	return ret;
}


