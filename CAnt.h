#pragma once

#include <vector>
#include <algorithm>    // std::find


class CAnt
{
	std::vector<size_t> m_antTour;
    std::vector<bool> m_visted;
    int tourLength;
   
public:
	CAnt::CAnt(size_t n)
		: m_antTour(n+1) , m_visted(n) , tourLength(0)
	{
		tourLength = 0;
	}
	CAnt::~CAnt(void)
	{
	}
	CAnt(){}
	//************************************
	// Method:    getAntTourLength
	// FullName:  CAnt::getAntTourLength
	// Access:    public 
	// Returns:   int
	// Qualifier: const
	//************************************
	int getAntTourLength() const
	{
        return tourLength;
    }
	//************************************
	// Method:    setAntTourLength
	// FullName:  CAnt::setAntTourLength
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int length
	//************************************
	void setAntTourLength(int length)
	{
		tourLength=length;
	}
	//************************************
	// Method:    setAntsTour
	// FullName:  CAnt::setAntsTour
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: std::vector<size_t> & antTour
	//************************************
	void setAntsTour(std::vector<size_t> &antTour)
	{
		m_antTour=antTour;
	}
	//************************************
	// Method:    isCityVisited
	// FullName:  CAnt::isCityVisited
	// Access:    public 
	// Returns:   bool
	// Qualifier: const
	// Parameter: size_t city
	//************************************
	bool isCityVisited(size_t city) const
	{
        return m_visted[city];
    }
	//************************************
	// Method:    setCityVisited
	// FullName:  CAnt::setCityVisited
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: size_t city
	//************************************
	void setCityVisited(size_t city)
	{
        m_visted[city]=true;
    }
	//************************************
	// Method:    ReinitiseAnt
	// FullName:  CAnt::ReinitiseAnt
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void ReinitiseAnt()
	{
		tourLength =0;
		for(unsigned int i=0;i < m_visted.size();i++)
		{
				m_antTour[i]=0;
				m_visted[i]=false;
		}
	}
	//************************************
	// Method:    getCity
	// FullName:  CAnt::getCity
	// Access:    public 
	// Returns:   size_t
	// Qualifier: const
	// Parameter: size_t index
	//************************************
	size_t getCity(size_t index) const {
        return m_antTour[index];
    }
	std::vector<size_t>& getAntsCityTour(){
        return m_antTour;
    }
	//************************************
	// Method:    setAntCity
	// FullName:  CAnt::setAntCity
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: size_t index
	// Parameter: size_t city
	//************************************
	void setAntCity(size_t index, size_t city)
	{
		//make sure city not already there
		//if(std::find(m_antTour.begin(), m_antTour.end(), city)==m_antTour.end())
			m_antTour[index] = city;
	//	else
	//	{
			//serverver error
			//m_antTour[index] = city;

	//	}
    }
	//************************************
	// Method:    getNoNodes
	// FullName:  CAnt::getNoNodes
	// Access:    public 
	// Returns:   size_t
	// Qualifier: const
	//************************************
	size_t getNoNodes() const {
		return m_antTour.size();
	}
	size_t findCity(size_t index)
	{
		std::vector<size_t>::iterator it = std::find(m_antTour.begin(), m_antTour.end(), index);
		return *it; 
	}
	



};

