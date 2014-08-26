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

#include <vector>
#include <random>
#include <time.h>
#include <math.h>


template <class RandomAccessIterator, class URNG>
void shuffleVector (RandomAccessIterator first, RandomAccessIterator last, URNG&& g)
{
  for (auto i=(last-first)-1; i>0; --i) {
    std::uniform_int_distribution<decltype(i)> d(0,i);
    std::swap (first[i], first[d(g)]);
  }
}


typedef std::vector<std::vector<int> > MatrixArrayTypeInt;

class CChromo
{



public:
	//cctor
	 CChromo( ){ }

	 CChromo( const CChromo& other )
	 {
		 if(this != &other)
		 {
			 m_Vgenes = other.m_Vgenes;
			 m_noGenes = other.m_noGenes;
			 m_dist = other.m_dist;
			 fitness = other.fitness;
		 }


	 }


	std::vector<size_t> m_Vgenes;
	int m_noGenes;
	MatrixArrayTypeInt *m_dist;
	double fitness;

	std::vector<size_t>& getPath();
	int getNoGenes();
	int getGene(int idx);
	void setGene(int idx, int val);
	void mutate();
	void shuffle();
	void crossover(CChromo  *father,  CChromo  *offspring1,  CChromo  *offspring2);
	CChromo CrossOver2(CChromo *pParent1);

	double getDistance();


	CChromo(int noGenes, MatrixArrayTypeInt *distance);
	~CChromo(void);
//	double getFitness();
	void setDistance(double newfit){
		fitness= newfit;
	}
	int calulatePathLength();
	int getDistance(int indx1,int indx2) const
	{
		return (*m_dist)[indx1][indx2];
	}

	int getRandomNumber(int low, int high)
	{
		int temp = (std::rand() % ((high + 1) - low)) + low;
		return temp;
	}
	 
	int getRandomNumber(int low, int high, int except)
	{
		//Generate random value between low and high (inclusive), except 'except'.
		int temp;
		bool done = false;

		while(!done)
		{
			temp = (std::rand() % ((high + 1) - low)) + low;
			if(temp != except)
			{
				done = true;
			}
		}
		return temp;
	}

	bool containsGene(int gene) const 
	{
      
	for(int i =0; i< m_Vgenes.size(); i++){
		if(m_Vgenes[i]==gene)
			return true;
	}
	return false;
	}

};

