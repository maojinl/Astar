/*
 * Graph.h
 *
 *  Created on: 2010-1-20
 *      Author: test
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <list>

using namespace std;

struct Point
{
	int x, y;

	bool operator ==(const Point &p) const;
};

class Graph {
public:
	std::string *map;
	int width;
	int height;
public:
	Graph(){};
	Graph(std::string *graph,const int w,const int h)
	{
		this->map = graph;
		width = w;
		height = h;
	};
	virtual ~Graph(){};

	bool isValid()
	{
		return this->map->length() >= (unsigned int)(width * height);
	}

	bool isValidPoint(Point &point)
	{
		if (!isValid())
			return false;
		if(point.x < 0
		 ||point.y < 0
		 ||(point.x >= width)
		 ||(point.y >= height)
		 ||map->at(point.y * width + point.x) == '1')
		{
			return false;
		}
		else
		{
			return true;
		}
	}

};




/**
 *  Node �������
 */
struct Node
{
	Point *point;
	int h;  //��������
	int g;  //����ʼ�����
	int f;  //Ԥ�ƾ���
	Node *fatherNode; //��Node

	bool operator <(const Node &) const;
	bool operator ()(const Node *, const Node *) const;
	int getKey() const { return f; }

	/*~Node() 
	{
		cout<<"destructor"<<endl;
	}*/
};

inline bool Node::operator <(const Node & node) const
{
	return (f > node.f);
}
inline bool Node::operator ()(const Node *node1, const Node *node2) const
{
	return (node1->f < node2->f);
}

inline bool Point::operator ==(const Point & p) const
{
	return (x == p.x && y == p.y);
}



#endif /* GRAPH_H_ */
