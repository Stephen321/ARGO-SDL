#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <list>
#include "BasicTypes.h"
#include <string>


#include "Camera2D.h" //debugging
using namespace Camera2D;


using namespace std;

// Forward references
//class GraphArc;

class GraphArc;
class GraphNode {
private:    

// -------------------------------------------------------
// Description: data inside the node
// -------------------------------------------------------
	string m_data;
// -------------------------------------------------------
// Description: cost inside node
// -------------------------------------------------------
	int m_hCost;
	int m_gCost;
	
// -------------------------------------------------------
// Description: list of arcs that the node has.
// -------------------------------------------------------
    list<GraphArc> m_arcList;
	
// -------------------------------------------------------
// Description: This remembers if the node is marked.
// -------------------------------------------------------
    bool m_marked;
	
	GraphNode* m_prevNode;

	helper::Vector2 m_position;
	//sf::CircleShape m_shape;

	//sf::Text m_nameTxt;
	//sf::Text m_gCostTxt;
	//sf::Text m_hCostTxt;

	const int RADIUS = 25;
	SDL_Color _color;
public:
	int m_fCost;
    // Accessor functions
    list<GraphArc> const & arcList() const {
        return m_arcList;              
    }

    bool marked() const {
        return m_marked;
    }

    string const & data() const {
        return m_data;
    }
    // Manipulator functions
	void setData(string data) {
		m_data = data;
		//m_nameTxt.setString(m_data);
    }

	void setHCost(int hCost) {
		m_hCost = hCost;
		//m_hCostTxt.setString("H(n)= " + to_string(m_hCost));
	}

	void setGCost(int gCost) {
		m_gCost = gCost;
		//m_gCostTxt.setString("G(n)= " + to_string(m_gCost));
	}

	int const & hCost() const {
		return m_hCost;
	}

	int const & gCost() const {
		return m_gCost;
	}

	int const & fCost() const {
		return m_hCost + m_gCost;
	}

	void setMarked(bool mark) {
		m_marked = mark;
	}

	void setPrevious(GraphNode* previous) {
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
	
	GraphNode* getPrevious() {
		return m_prevNode;
	}


	GraphNode();



	GraphArc* getArc(GraphNode* pNode);


	void  reset();


	void addArc(GraphNode* pNode, float weight);

	// ----------------------------------------------------------------
	//  Name:           removeArc
	//  Description:    This finds an arc from this node to input node 
	//                  and removes it.
	//  Arguments:      None.
	//  Return Value:   None.
	// ----------------------------------------------------------------

	void removeArc(GraphNode* pNode);



	void drawArcs(SDL_Renderer* renderer, Camera* camera) const;


	void drawNodes(SDL_Renderer* renderer, Camera* camera) const;
};
#endif
