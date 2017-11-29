/*
 * AstarAlgorithm.h
 *
 *  Created on: 2010-1-20
 *      Author: test
 */

#ifndef ASTARALGORITHM_H_
#define ASTARALGORITHM_H_

#define PRINT_INFO false

#include "Graph.h"
#include "heap.h"
#include "memory_pool.h"

using namespace std;

class AstarAlgorithm{
private:
	Graph *g;
	Point *startPoint;
	Point *endPoint;
	Node *activeNode;
	//vector<Node *> openList;
	vector<unsigned char> closeList;
	vector<Point *> recyclePoints;
	vector<Node *> recycleNodes;
	heap<Node *> openList;

	memory_pool<Node> nodepool;
	memory_pool<Point> pointpool;

	void addNearBy();
	void removeActive();
	void printList();
	bool isValidPath(Point &,Point &);
	bool inClose(const Point &);

	int getIndex(int,int);
	int getG(Point &,Point &);
	Node *getNode(Point &point, unsigned int& index);
	
	void initCloseList();
	void addToCloseList(const Point &point);
	void finalize()
	{
		/*for(unsigned int i = 0; i < recyclePoints.size(); i++)
		{
			//delete recyclePoints[i];
			pointpool.free(recyclePoints[i]);
		}
		recyclePoints.clear();
		for(unsigned int j = 0; j < recycleNodes.size(); j++)
		{
			//delete recycleNodes[j];
			nodepool.free(recycleNodes[j]);
		}*/

		pointpool.reset();
		nodepool.reset();

		recycleNodes.clear();

		openList.clear();
	}

	void initialize(Graph *g, Point *start, Point *end)
	{
		finalize();

		this->g = g;
		this->startPoint = start;
		this->endPoint = end;
		//Node *active = new Node();
		Node *active = nodepool.malloc();
		//recycleNodes.push_back(active);
		//active->point = new Point(*start);
		active->point = pointpool.malloc();
		*(active->point) = *start;
		//recyclePoints.push_back(active->point);
		active->f = 0;
		active->h = 0;
		active->g = 0;
		active->fatherNode = NULL;
		this->constructOpenList();
		openList.push_back(active);
		activeNode = active;
		initCloseList();
		if (PRINT_INFO)
		{
			cout << "activeP:" << activeNode->point->x << ":" << activeNode->point->y << endl;
		}
	}

	void constructOpenList()
	{
		size_t sz = 9 *  max(abs(this->endPoint->x - this->startPoint->x), abs(this->endPoint->y - this->startPoint->y));
		sz *= 2;
		sz =400;
		openList.resize(sz);
	}
public:
	AstarAlgorithm();
	virtual ~AstarAlgorithm();
	bool isValidPoint(Point &);
	void search(list<Point *> &path, Graph *g, Point *start, Point *end);
	char atMap(int,int);
};

#endif /* ASTARALGORITHM_H_ */
