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
#include "StdAfx.h"
#include "CChromo.h"


#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector

 


CChromo::CChromo(int noGenes, MatrixArrayTypeInt *distance)
{
	this->m_noGenes= noGenes;
	m_dist = distance;
	m_Vgenes.resize(noGenes);
	fitness=0;

}

CChromo::~CChromo(void)
{

}


std::vector<size_t>& CChromo::getPath()
{
	return m_Vgenes;
}
int CChromo::getNoGenes()
{
    return m_noGenes;
}
int CChromo::getGene(int idx)
{
    return m_Vgenes[idx];
}
void CChromo::setGene(int idx, int val)
{
    m_Vgenes[idx] = val;
}
void CChromo::mutate()
{
    int idx1 = getRandomNumber(0, m_noGenes-1);
	int idx2  = getRandomNumber(0,m_noGenes-1, idx1);
    int swap = m_Vgenes[idx1];
    m_Vgenes[idx1] = m_Vgenes[idx2];
    m_Vgenes[idx2] = swap;
 
}
void CChromo::shuffle(){
  
    for(int i = 0; i < m_noGenes; i++)
	{
        m_Vgenes[i] = i;
    }

  std::random_shuffle( m_Vgenes.begin() , m_Vgenes.end() );

}
 CChromo CChromo::CrossOver2(CChromo *pParent1)
 {

	CChromo offspring (this->m_noGenes, this->m_dist);
	
	for (int i = 0; i<offspring.m_Vgenes.size(); i++)
	{
			offspring.m_Vgenes[i]=-1;
	}

		
        // Get start and end sub tour positions for parent1's tour
        int startPos = (int) getRandomNumber(0 , m_noGenes);
        int endPos = (int)  getRandomNumber(0 , m_noGenes , startPos);

        // Loop and add the sub tour from parent1 to our child
		for (int i = 0; i < 	offspring.m_noGenes; i++) {
            // If our start position is less than the end position
            if (startPos < endPos && i > startPos && i < endPos)
			{
				offspring.m_Vgenes[i] = this->m_Vgenes[i];
              //  child.setCity(i, parent1.getCity(i));
            } // If our start position is larger
            else if (startPos > endPos) 
			{
                if (!(i < startPos && i > endPos)) {
                   	offspring.m_Vgenes[i] = this->m_Vgenes[i];
                }
            }
        }

        // Loop through parent2's city tour
        for (int i = 0; i < pParent1->m_noGenes; i++) {
            // If child doesn't have the city add it
			if (!offspring.containsGene(pParent1->m_Vgenes[i])) 
			{
                // Loop to find a spare position in the child's tour
				for (int ii = 0; ii < 	offspring.m_noGenes; ii++) 
				{
                    // Spare position found, add city
                    if (offspring.m_Vgenes[ii] == -1) 
					{
                        offspring.m_Vgenes[ii]=pParent1->m_Vgenes[i];
                        break;
                    }
                }
            }
        }
        return offspring;


 }


void CChromo::crossover(CChromo  *father,  CChromo *offspring1,  CChromo  *offspring2)
{
    offspring1->m_Vgenes[0] = m_Vgenes[0];
    offspring2->m_Vgenes[0] = father->m_Vgenes[0];
	int max = this->m_noGenes-1;
        
	std::vector<bool> visited1(m_noGenes), visited2(m_noGenes);

	for(int i = 0; i < m_noGenes; i++){
        visited1[i] = false;
        visited2[i] = false;
    }

    visited1[offspring1->m_Vgenes[0]] = true;
    visited2[offspring2->m_Vgenes[0]] = true;

    for(int i = 1; i < m_noGenes; i++){
        int prevNode = offspring1->m_Vgenes[i-1];
        int node1 = 0, node2 = 0;
        for(int j = 0; j < m_noGenes-1; j++){
            if(m_Vgenes[j] == prevNode)
                node1 = m_Vgenes[j+1];
            if(father->m_Vgenes[j] == prevNode)
                node2 = father->m_Vgenes[j+1];

        }
        if((!visited1[node1]) && (!visited1[node2])){
            if(getDistance(prevNode,node1) < getDistance(prevNode, node2)){
                offspring1->m_Vgenes[i] = node1;
                visited1[node1] = true;
            }
            else{
                offspring1->m_Vgenes[i] = node2;
                visited1[node2] = true;
            }
            continue;
        }
        if((!visited1[node1]) && visited1[node2]){
            offspring1->m_Vgenes[i] = node1;
            visited1[node1] = true;
            continue;
        }
        if((!visited1[node2]) && visited1[node1]){
            offspring1->m_Vgenes[i] = node2;
            visited1[node2] = true;
            continue;
        }
        if(visited1[node1] && visited1[node2]){
      		int node = getRandomNumber(0, max);
	        while(visited1[node]){
				
				node = getRandomNumber(0,max , node);
	      }
            visited1[node] = true;
            offspring1->m_Vgenes[i] = node;
            continue;
        }
    }
    //===================================================
    for(int i = 1; i < m_noGenes; i++){
        int prevNode = offspring2->m_Vgenes[i-1];
        int node1 = 0, node2 = 0;
        for(int j = 0; j < m_noGenes-1; j++){
            if(m_Vgenes[j] == prevNode)
                node1 = m_Vgenes[j+1];
            if(father->m_Vgenes[j] == prevNode)
                node2 = father->m_Vgenes[j+1];

        }
        if((!visited2[node1]) && (!visited2[node2])){
            if(getDistance(prevNode,node1) < getDistance(prevNode,node2)){
                offspring2->m_Vgenes[i] = node1;
                visited2[node1] = true;
            }
            else{
                offspring2->m_Vgenes[i] = node2;
                visited2[node2] = true;
            }
            continue;
        }
        if((!visited2[node1]) && visited2[node2]){
            offspring2->m_Vgenes[i] = node1;
            visited2[node1] = true;
            continue;
        }
        if((!visited2[node2]) && visited2[node1]){
            offspring2->m_Vgenes[i] = node2;
            visited2[node2] = true;
            continue;
        }
        if(visited2[node1] && visited2[node2]){
      	int node = getRandomNumber(0, max);
				while(visited2[node]){
				node = getRandomNumber(0,max , node);
	       }
            visited2[node] = true;
            offspring2->m_Vgenes[i] = node;
            continue;
        }

    }
//	m_Vgenes.resize(m_noGenes+1);
//	m_Vgenes[m_noGenes] = m_Vgenes[0];
  
}

double CChromo::getDistance()
{
    if(!fitness)
	{
	    for(int i = 0; i < m_noGenes-1; i++)
			fitness+=(*m_dist)[m_Vgenes[i]][m_Vgenes[i+1]];
		 fitness += (*m_dist)[m_Vgenes[m_Vgenes.size()-1]][m_Vgenes[0]];
    }
	return fitness;
}





