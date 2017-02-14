#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <queue>
#include "BasicTypes.h"

using namespace helper;
using namespace std;

#include "Camera2D.h" //debugging
using namespace Camera2D;

template <class DataType, class NodeType, class ArcType> class GraphArc;
template <class DataType, class NodeType, class ArcType> class GraphNode;

template<class DataType, class NodeType, class ArcType>
class Graph {
private:
    typedef GraphArc<DataType, NodeType, ArcType> Arc;
	typedef GraphNode<DataType, NodeType, ArcType> Node;
	
public:           
    // Constructor and destructor functions
						Graph() {};
						Graph( int size );
						~Graph();

    // Accessors
    Node** nodeArray() const {
       return m_pNodes;
    }
	
    // Public member functions.
	void				init(int size);
	bool				addNode(DataType data, int index, helper::Vector2 position);
    void				removeNode( int index );
    bool				addArc( int from, int to, ArcType weight, bool directed = true );
    void				removeArc( int from, int to );
	Arc*				getArc(int from, int to);
	void				reset();
	int					getMaxNodes();
	
	//Pathfinding Assignment
	void				aStar(Node* pStart, Node* pDest, std::vector<Node *>& path);
	void				setHeuristics(Node* pDest);
	void				drawNodes(SDL_Renderer* renderer, Camera* camera) const;
	void				drawArcs(SDL_Renderer* renderer, Camera* camera) const;

private:
	Node**				m_pNodes;
	int					m_maxNodes; //max number of node
	int					m_count; //actual number of node

	class NodeSearchCostComparer {
	public:
		bool operator()(Node * n1, Node * n2) {
			NodeType f1 = n1->hCost() + n1->gCost();
			NodeType f2 = n2->hCost() + n2->gCost();
			// adds H(n) and G(n) to get F(n)
			return f1 > f2;
		}
	};
};

template<class DataType, class NodeType, class ArcType>
Graph<DataType, NodeType, ArcType>::Graph(int size) : m_maxNodes(size) {
   int i;
   m_pNodes = new Node * [m_maxNodes];
   // go through every index and clear it to null (0)
   for( i = 0; i < m_maxNodes; i++ ) {
        m_pNodes[i] = 0;
   }

   // set the node count to 0.
   m_count = 0;
}

template<class DataType, class NodeType, class ArcType>
void Graph<DataType, NodeType, ArcType>::init(int size)
{
	m_maxNodes = size;
	int i;
	m_pNodes = new Node *[m_maxNodes];
	// go through every index and clear it to null (0)
	for (i = 0; i < m_maxNodes; i++) {
		m_pNodes[i] = 0;
	}

	// set the node count to 0.
	m_count = 0;
}


template<class DataType, class NodeType, class ArcType>
Graph<DataType, NodeType, ArcType>::~Graph() {
   int index;
   for( index = 0; index < m_maxNodes; index++ ) {
        if( m_pNodes[index] != 0 ) {
            delete m_pNodes[index];
        }
   }
   // Delete the actual array
   delete m_pNodes;
}


template<class DataType, class NodeType, class ArcType>
bool Graph<DataType, NodeType, ArcType>::addNode(DataType data, int index, helper::Vector2 position) {
   bool nodeNotPresent = false;
   // find out if a node does not exist at that index.
   if ( m_pNodes[index] == 0) {
      nodeNotPresent = true;
      // create a new node, put the data in it, and unmark it.
	  m_pNodes[index] = new Node();
	  m_pNodes[index]->setData(data);
      m_pNodes[index]->setMarked(false);
	  m_pNodes[index]->setPosition(position);

      // increase the count and return success.
      m_count++;
    }
        
    return nodeNotPresent;
}

template<class DataType, class NodeType, class ArcType>
void Graph<DataType, NodeType, ArcType>::removeNode(int index) {
     // Only proceed if node does exist.
     if( m_pNodes[index] != 0 ) {
         // now find every arc that points to the node that
         // is being removed and remove it.
         int node;
         Arc* arc;

         // loop through every node
         for( node = 0; node < m_maxNodes; node++ ) {
              // if the node is valid...
              if( m_pNodes[node] != 0 ) {
                  // see if the node has an arc pointing to the current node.
                  arc = m_pNodes[node]->getArc( m_pNodes[index] );
              }
              // if it has an arc pointing to the current node, then
              // remove the arc.
              if( arc != 0 ) {
                  removeArc( node, index );
              }
         }
        

        // now that every arc pointing to the current node has been removed,
        // the node can be deleted.
        delete m_pNodes[index];
        m_pNodes[index] = 0;
        m_count--;
    }
}

template<class DataType, class NodeType, class ArcType>
bool Graph<DataType, NodeType, ArcType>::addArc(int from, int to, ArcType weight, bool directed) {
     bool proceed = true; 
     // make sure both nodes exist.
     if( m_pNodes[from] == 0 || m_pNodes[to] == 0 ) {
         proceed = false;
     }
        
     // if an arc already exists we should not proceed
     if( m_pNodes[from]->getArc( m_pNodes[to] ) != 0 ) {
         proceed = false;
     }

     if (proceed == true) {
        // add the arc to the "from" node.
		 m_pNodes[from]->addArc(m_pNodes[to], weight);
		 if (directed == false) //add node back the other way if undirected
			 m_pNodes[to]->addArc(m_pNodes[from], weight);
     }
        
     return proceed;
}

template<class DataType, class NodeType, class ArcType>
void Graph<DataType, NodeType, ArcType>::removeArc(int from, int to) {
     // Make sure that the node exists before trying to remove
     // an arc from it.
     bool nodeExists = true;
     if( m_pNodes[from] == 0 || m_pNodes[to] == 0 ) {
         nodeExists = false;
     }

     if (nodeExists == true) {
        // remove the arc.
        m_pNodes[from]->removeArc( m_pNodes[to] );
     }
}


template<class DataType, class NodeType, class ArcType>
// Dev-CPP doesn't like Arc* as the (typedef'd) return type?
GraphArc<DataType, NodeType, ArcType>* Graph<DataType, NodeType, ArcType>::getArc(int from, int to) {
     Arc* pArc = 0;
     // make sure the to and from nodes exist
     if( m_pNodes[from] != 0 && m_pNodes[to] != 0 ) {
         pArc = m_pNodes[from]->getArc( m_pNodes[to] );
     }
                
     return pArc;
}

template<class DataType, class NodeType, class ArcType>
int Graph<DataType, NodeType, ArcType>::getMaxNodes() {

	return m_maxNodes;
}

template<class DataType, class NodeType, class ArcType>
void Graph<DataType, NodeType, ArcType>::reset() {
	int index;
	for (index = 0; index < m_maxNodes; index++) {
		if (m_pNodes[index] != 0) {
			m_pNodes[index]->reset();
		}
	}
}

template<class DataType, class NodeType, class ArcType>
void Graph<DataType, NodeType, ArcType>::aStar(Node* pStart, Node* pDest, std::vector<Node *>& path){
	if (pStart != 0 && pDest != 0) {
		priority_queue<Node*, vector<Node*>, NodeSearchCostComparer> pq;
		pq.push(pStart);
		pStart->setMarked(true);
		pStart->setHCost(NodeType());
		pStart->setGCost(NodeType());

		while (pq.size() != 0 && pq.top() != pDest) {
			list<Arc>::const_iterator iter = pq.top()->arcList().begin();
			list<Arc>::const_iterator endIter = pq.top()->arcList().end();

			for (; iter != endIter; iter++) {
				Node* child = (*iter).node();
				if (child != pq.top()->getPrevious()){
					Arc arc = (*iter);
					//Sleep(1000);
					int Hc = child->hCost();
					int Gc = pq.top()->gCost() + arc.weight();
					int Fc = Hc + Gc;
					if (Fc < child->fCost() || child->gCost() == -1){  //is G(n) not set, H(n) should be always set with setHeuristics()
						child->setHCost(Hc);
						child->setGCost(Gc);
						child->setPrevious(pq.top());
					}

					if (child->marked() == false) {
						pq.push(child);
						child->setMarked(true);
						//child->setColour(SDL_Color{ 0, 128, 128, 255 });
					}
				}
			}
			pq.pop();
		}

		if (pq.size() != 0 && pq.top() == pDest){
			for (Node* previous = pDest; previous->getPrevious() != 0; previous = previous->getPrevious()){
				previous->setColour(SDL_Color{0,0,255,255});
				path.push_back(previous);			
			}
			path.push_back(pStart);
			std::reverse(path.begin(), path.end());
		}
		else if (pq.size() == 0)
			cout << "Couldn't find path." << endl;
	}
}



template<class DataType, class NodeType, class ArcType>
void Graph<DataType, NodeType, ArcType>::setHeuristics(Node* pDest){
	if (pDest != 0) {
		for (int i = 0; i < m_count; i++){
			helper::Vector2 vectorTo = pDest->getPosition() - m_pNodes[i]->getPosition();
			int Hc = (int)(sqrt((vectorTo.x * vectorTo.x) + (vectorTo.y * vectorTo.y)));
			m_pNodes[i]->setHCost(Hc);
		}
	}
}



//draw the nodes
template<class DataType, class NodeType, class ArcType>
void Graph<DataType, NodeType, ArcType>::drawNodes(SDL_Renderer* renderer, Camera* camera) const{
	for (int i = 0; i < m_count; i++){
		m_pNodes[i]->drawNodes(renderer, camera);
	}
}

//draw arcs
template<class DataType, class NodeType, class ArcType>
void Graph<DataType, NodeType, ArcType>::drawArcs(SDL_Renderer* renderer, Camera* camera) const{
	for (int i = 0; i < m_count; i++)
	{
		m_pNodes[i]->drawArcs(renderer, camera);
	}
}



#include "GraphNode.h"
#include "GraphArc.h"


#endif
