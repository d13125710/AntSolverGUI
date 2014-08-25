
#include "StdAfx.h"
#include "CTSPLIBFileReader.h"




std::string::size_type sz;     // alias of size_t
 

//************************************
// Method:    CTSPLIBFileReader
// FullName:  CTSPLIBFileReader::CTSPLIBFileReader
// Access:    public 
// Returns:   
// Qualifier:
// Parameter: const string & str
//************************************
CTSPLIBFileReader::CTSPLIBFileReader(const string &str)
{
	Init(str);
}

//************************************
// Method:    ShowSolution
// FullName:  CTSPLIBFileReader::ShowSolution
// Access:    public 
// Returns:   double
// Qualifier:
//************************************
double CTSPLIBFileReader::ShowSolution()
{
	std::ifstream m_solutionFile; 
	m_solutionFile.open(m_fileName + ".opt.tour");
	bool Read = false;
	std::vector<int>  strVector;
	double distance = 0;

	if(m_solutionFile.is_open())
	{
		m_solutionFile.setf(ios::skipws);

		while(!m_solutionFile.eof() && m_solutionFile.good())
		{
			//we need to read up to the newline
			getline(m_solutionFile , m_sInput , '\n');
						
			if(Read)
			{
				//split up the input into tokens of whit spaces
				Tk ti(m_sInput.c_str() , TokenFinder(" ")); // find white spaces
						
				while( ti != Tk()) // skip the first as format in "place" , distance , distance , dont need the first
				{
					//convert the string to a double
					int position = std::stoi(*ti,&sz);
					if(position == -1)
					{
						Read = false;
						break;
					}
					strVector.push_back(position - 1); //our arrays start at 0
					++ti;
				}
				
			}
			//start of the COORDS in the TSP format , so read the coords from here....
			if(m_sInput == "TOUR_SECTION")
				Read = true;
		}
		m_solutionFile.close();	
		int start, end;

		for(size_t  i = 1; i < strVector.size() ; i++)
		{
			start = strVector[i-1];
			end = strVector[i];
			distance += ReadMatrix(start , end);
		}
		//get the last city to first..
		distance += ReadMatrix(end, strVector[0]);
	
	}


	return distance;
}

bool CTSPLIBFileReader::Read()
{
	bool status = true;
	bool ReadCoords = false;

	maxx =-1;
	minx=(std::numeric_limits<int>::max)();
	miny=(std::numeric_limits<int>::max)();
	maxy=-1;



	if(m_inFileStream.is_open())
	{
		m_inFileStream.setf(ios::skipws);
		while(!m_inFileStream.eof() && m_inFileStream.good())
		{
			//we need to read up to the newline
			getline(m_inFileStream , m_sInput , '\n');
			if(ReadCoords)
			{
				//split up the input into tokens of whit spaces
				Tk ti(m_sInput.c_str() , TokenFinder(" ")); // find white spaces
				Coords cd;
				while( ti++ != Tk()) // skip the first as format in "place" , distance , distance , dont need the first
				{
					//convert the string to a double
					cd.x = std::stod (*ti,&sz);
					++ti; //get the next token
					cd.y = std::stod (*ti,&sz);
					//add the struct to the vector to to be stored
					m_coordsVector.push_back(cd);
					++ti;

					// Get node extremities
					if ( cd.x > maxx ) maxx = static_cast<int>(cd.x);
					if ( cd.y > maxy ) maxy = static_cast<int>(cd.y);
					if ( cd.x < minx ) minx = static_cast<int>(cd.x);
					if ( cd.y < miny ) miny = static_cast<int>(cd.y);	

				}
			}
			//start of the COORDS in the TSP format , so read the coords from here....
			std::size_t found = m_sInput.find("EDGE_WEIGHT_TYPE");
	
			if(found!=std::string::npos)
			{
				//find the type ..
				Tk ti1(m_sInput.c_str() , TokenFinder(":")); // find white spaces
				ti1++;
				if(*ti1 == "EUC_2D")
					p_mDistanceCalculator =new CRoundDistance();
					//round_distance
				else if(*ti1 == "CEIL_2D")
					p_mDistanceCalculator = new CEuclidianDistance();
					//calculateEuclidianDistance
				else if(*ti1 == "GEO")//geo_distance
					p_mDistanceCalculator = new CGeoDistance();
				else if(*ti1 == "ATT")//att_distance
					p_mDistanceCalculator = new CAttDistance();
				else
					p_mDistanceCalculator = new CEuclidianDistance();
			}
			if(m_sInput == "NODE_COORD_SECTION" || m_sInput == "EDGE_WEIGHT_SECTION")
				ReadCoords = true;
		}
		calculateDistanceMatrix();

		delete p_mDistanceCalculator;
		p_mDistanceCalculator=0;
		m_inFileStream.close();
	}

	return status;

}

bool CTSPLIBFileReader::ReadPoint()
{
	bool status = true;
	bool ReadCoords = false;

	maxx =-1;
	minx=(std::numeric_limits<int>::max)();
	miny=(std::numeric_limits<int>::max)();
	maxy=-1;



	if(m_inFileStream.is_open())
	{
		m_inFileStream.setf(ios::skipws);
		while(!m_inFileStream.eof() && m_inFileStream.good())
		{
			//we need to read up to the newline
			getline(m_inFileStream , m_sInput , '\n');
	
			//split up the input into tokens of whit spaces
			Tk ti(m_sInput.c_str() , TokenFinder(" ")); // find white spaces
			Coords cd;
			while( ti != Tk()) // skip the first as format in "place" , distance , distance , dont need the first
			{
				//convert the string to a double
				cd.x = std::stod (*ti,&sz);
				++ti; //get the next token
				cd.y = std::stod (*ti,&sz);
				//add the struct to the vector to to be stored
				m_coordsVector.push_back(cd);
				++ti;

				// Get node extremities
				if ( cd.x > maxx ) maxx = static_cast<int>(cd.x);
				if ( cd.y > maxy ) maxy = static_cast<int>(cd.y);
				if ( cd.x < minx ) minx = static_cast<int>(cd.x);
				if ( cd.y < miny ) miny = static_cast<int>(cd.y);	
			}
		   	p_mDistanceCalculator = new CEuclidianDistance();
		}
		calculateDistanceMatrix();

		delete p_mDistanceCalculator;
		p_mDistanceCalculator=0;
		m_inFileStream.close();
		
	}



	return status;

}

void CTSPLIBFileReader::calculateDistanceMatrix()
{
	if(p_mDistanceCalculator)
	{
		size_t Vsize = m_coordsVector.size();
		m_theMatrix.resize(m_coordsVector.size());
		for(size_t i = 0 ; i< Vsize ; i++)
			m_theMatrix[i].resize(Vsize);
		
		for (size_t i=0; i<m_coordsVector.size(); i++) 
		{
			for (size_t j=0; j<m_coordsVector.size(); j++)
				m_theMatrix[i][j]  = (int) p_mDistanceCalculator->calculate(m_coordsVector[i].x, m_coordsVector[i].y, m_coordsVector[j].x, m_coordsVector[j].y);
			
		}
	}
}

CTSPLIBFileReader::~CTSPLIBFileReader(void)
{
	if(p_mDistanceCalculator)
		delete p_mDistanceCalculator;
}

