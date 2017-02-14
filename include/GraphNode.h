#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <list>
#include "BasicTypes.h"

#include "Camera2D.h" //debugging
using namespace Camera2D;


using namespace std;

// Forward references
template <typename DataType, typename NodeType, typename ArcType> class GraphArc;

// -------------------------------------------------------
// Name:        GraphNode
// Description: This is the node class. The node class 
//              contains data, and has a linked list of 
//              arcs.
// -------------------------------------------------------
template<class DataType, class NodeType, class ArcType>
class GraphNode {
private:    
// typedef the classes to make our lives easier.
	typedef GraphArc<DataType, NodeType, ArcType> Arc;
	typedef GraphNode<DataType, NodeType, ArcType> Node;
// -------------------------------------------------------
// Description: data inside the node
// -------------------------------------------------------
	DataType m_data;
// -------------------------------------------------------
// Description: cost inside node
// -------------------------------------------------------
	NodeType m_hCost;
	NodeType m_gCost;
// -------------------------------------------------------
// Description: list of arcs that the node has.
// -------------------------------------------------------
    list<Arc> m_arcList;
	
// -------------------------------------------------------
// Description: This remembers if the node is marked.
// -------------------------------------------------------
    bool m_marked;
	
	Node* m_prevNode; 

	helper::Vector2 m_position;
	//sf::CircleShape m_shape;

	//sf::Text m_nameTxt;
	//sf::Text m_gCostTxt;
	//sf::Text m_hCostTxt;

	const int RADIUS = 25;
	SDL_Color _color;
public:
    // Accessor functions
    list<Arc> const & arcList() const {
        return m_arcList;              
    }

    bool marked() const {
        return m_marked;
    }

    DataType const & data() const {
        return m_data;
    }
    // Manipulator functions
	void setData(DataType data) {
		m_data = data;
		//m_nameTxt.setString(m_data);
    }

	void setHCost(NodeType hCost) {
		m_hCost = hCost;
		//m_hCostTxt.setString("H(n)= " + to_string(m_hCost));
	}

	void setGCost(NodeType gCost) {
		m_gCost = gCost;
		//m_gCostTxt.setString("G(n)= " + to_string(m_gCost));
	}

	NodeType const & hCost() const {
		return m_hCost;
	}

	NodeType const & gCost() const {
		return m_gCost;
	}

	NodeType const & fCost() const {
		return m_hCost + m_gCost;
	}

	void setMarked(bool mark) {
		m_marked = mark;
	}

	void setPrevious(Node* previous) {
		m_prevNode = previous;
	}

	void setColour(const SDL_Color& color) {
		_color = color;
	}

	void setPosition(helper::Vector2 newPosition){
		/*
		m_nameTxt.setPosition(newPosition);
		m_hCostTxt.setPosition(newPosition.x - RADIUS, newPosition.y - RADIUS);
		m_gCostTxt.setPosition(newPosition.x - RADIUS, newPosition.y - (RADIUS / 2));
		m_shape.setPosition(newPosition);*/
		m_position = newPosition;
	}

	helper::Vector2 getPosition(){
		return m_position;
	}
	


	void reset();

	Node* getPrevious() {
		return m_prevNode;
	}
           
    Arc* getArc( Node* pNode );    
    void addArc( Node* pNode, ArcType pWeight );
	void removeArc(Node* pNode);
	void drawArcs(SDL_Renderer* renderer, Camera* camera) const;
	void drawNodes(SDL_Renderer* renderer, Camera* camera) const;
	GraphNode();
};


template<typename DataType, typename NodeType, typename ArcType>
GraphNode<DataType, NodeType, ArcType>::GraphNode() :
m_prevNode(0),
m_hCost(-1),
m_gCost(-1){

	_color = { 255,255,255,255 };
	/*
	m_shape = sf::CircleShape(RADIUS);
	m_shape.setOrigin(RADIUS, RADIUS);

	
	m_nameTxt = sf::Text("", font, 15);
	m_nameTxt.setOrigin(4, 8);
	m_hCostTxt = sf::Text("H(n)= ?", font, 11);
	m_hCostTxt.setOrigin(7, 7);
	m_gCostTxt = sf::Text("G(n)= ?", font, 11);
	m_gCostTxt.setOrigin(7, 7);*/
}


template<typename DataType, typename NodeType, typename ArcType>
GraphArc<DataType, NodeType, ArcType>* GraphNode<DataType, NodeType, ArcType>::getArc(Node* pNode) {

     list<Arc>::iterator iter = m_arcList.begin();
     list<Arc>::iterator endIter = m_arcList.end();
     Arc* pArc = 0;
     
     // find the arc that matches the node
     for( ; iter != endIter && pArc == 0; ++iter ) {         
          if ( (*iter).node() == pNode) {
               pArc = &( (*iter) );
          }
     }

     // returns null if not found
     return pArc;
}

template<typename DataType, typename NodeType, typename ArcType>
void  GraphNode<DataType, NodeType, ArcType>::reset() {
	m_marked = false;
	m_prevNode = 0;
	//m_shape.setFillColor(sf::Color::Blue);
	m_hCost = -1;
	m_gCost = -1;

	//m_hCostTxt.setString("H(n)= ?");
	//m_gCostTxt.setString("G(n)= ?");
}

template<typename DataType, typename NodeType, typename ArcType>
void GraphNode<DataType, NodeType, ArcType>::addArc(Node* pNode, ArcType weight) {
   // Create a new arc.
   Arc a;
   a.setNode(pNode);
   a.setWeight(weight);
   a.setLine(m_position, pNode->getPosition());
   // Add it to the arc list.
   m_arcList.push_back( a );
}


// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This finds an arc from this node to input node 
//                  and removes it.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<typename DataType, typename NodeType, typename ArcType>
void GraphNode<DataType, NodeType, ArcType>::removeArc(Node* pNode) {
     list<Arc>::iterator iter = m_arcList.begin();
     list<Arc>::iterator endIter = m_arcList.end();

     int size = m_arcList.size();
     // find the arc that matches the node
     for( ; iter != endIter && m_arcList.size() == size;  
                                                    ++iter ) {
          if ( (*iter).node() == pNode) {
             m_arcList.remove( (*iter) );
          }                           
     }
}


template<typename DataType, typename NodeType, typename ArcType>
void GraphNode<DataType, NodeType, ArcType>::drawArcs(SDL_Renderer* renderer, Camera* camera) const{
	int arcs = m_arcList.size();
	for (auto a : m_arcList)
		a.draw(renderer, camera);
}

template<typename DataType, typename NodeType, typename ArcType>
void GraphNode<DataType, NodeType, ArcType>::drawNodes(SDL_Renderer* renderer, Camera* camera) const{
	SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);
	Point point = { (float)(m_position.x -10), (float)(m_position.y - 10) };
	point = camera->worldToScreen(point);
	
	SDL_Rect rect = { point.x,point.y, 20, 20 };
	SDL_RenderFillRect(renderer, &rect);
}

#include "GraphArc.h"

#endif
