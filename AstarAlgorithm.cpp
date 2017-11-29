/*
 * AstarAlgorithm.cpp
 *
 *  Created on: 2010-1-20
 *      Author: test
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iostream>

#include "AstarAlgorithm.h"

using namespace std;

bool AstarAlgorithm::isValidPoint(Point &point)
{
	if(point.x < 0
	 ||point.y < 0
	 ||(point.x >= g->width)
	 ||(point.y >= g->height)
	 ||atMap(point.x, point.y) == '1')
	{
		return false;
	}
	else
	{
		return true;
	}
}


void AstarAlgorithm::printList()
{
	for(unsigned int i = 0;i < (this->openList.size()); i++)
	{
		Node *node = this->openList.at(i);
		if (PRINT_INFO)
		{
			printf("[%d,%d]<%d,%d,%d>",node->point->x,node->point->y,
				node->f,node->h,node->g);
		}
	}
	if (PRINT_INFO)
	{
		printf("\n");
	}
}


void AstarAlgorithm::removeActive()
{
	//openList.erase(openList.begin());
	//openList.EraseFirst();
	openList.pop_front();
	//closeList.push_back(activeNode);
	addToCloseList(*activeNode->point);
	if (PRINT_INFO)
	{
		cout << "close point:" << activeNode->point->x << ":" << activeNode->point->y << endl;
	}

	openList.adjust_heap(openList.begin(), openList.size(), 0, Node());
	//sort(openList.begin(), openList.end(), Node());
}


void AstarAlgorithm::addNearBy()
{
	if (PRINT_INFO)
	{
		cout << "active point:" << activeNode->point->x << ":" << activeNode->point->y << endl;
	}
	int variation[] = {-1, 0, 1};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if( !(i == 1 && j == 1))
			{
				Point temp;
				temp.x = activeNode->point->x + variation[i];
				temp.y = activeNode->point->y + variation[j];

				//if(isValidPoint(temp) && isValidPath(*(activeNode->point), temp) && !inClose(temp)) 
				//CAN WALK through diagono points
				if(isValidPoint(temp) && !inClose(temp)) 
				{
					//Manhattan ��������
					int h = getIndex(temp.x, temp.y);
					int g = activeNode->g + getG(temp, *(activeNode->point));
					int f = h + g;
					unsigned int index = 0;
					Node *tp = getNode(temp, index);
					if (tp != NULL)
					{
						if (tp->f > f)
						{
							tp->fatherNode = activeNode;
							tp->f = f;
							tp->g = g;
							openList.adjust_heap_reverse(openList.begin(), index, Node());
						}
					}
					else
					{
						//Node *newNode = new Node();
						Node* newNode =nodepool.malloc();
						//recycleNodes.push_back(newNode);

						//newNode->point = new Point(temp);
						newNode->point = pointpool.malloc();
						*(newNode->point) = temp;
						//recyclePoints.push_back(newNode->point);
						newNode->h = h;
						newNode->g = g;
						newNode->f = f;
						newNode->fatherNode = activeNode;

						//openList.push_back(newNode);
						openList.push_back(newNode);
						openList.adjust_heap_reverse(openList.begin(), openList.size()-1, Node());
					}
					if (PRINT_INFO)
					{
						cout << "add nearby point:" << temp.x << ":" << temp.y << endl;
					}
				}
			}
		}
	}
}

bool AstarAlgorithm::isValidPath(Point &point, Point &point1)
{
	int x1 = point.x;
	int x2 = point1.x;
	int y1 = point.y;
	int y2 = point1.y;
	int pos1 = y2 * this->g->width + x1;
	int pos2 = y1 * this->g->width + x2;
	if (this->g->map->at(pos1) == '1'
	  ||this->g->map->at(pos2) == '1')
	{
		return false;
	}
	else
	{
		return true;
	}
}

void AstarAlgorithm::initCloseList()
{
	/*size_t sz = g->map->length();
	closeList.resize(sz);
	memset(&closeList[0], 0, sz);*/
	size_t sz = g->map->length() / 8;
	closeList.resize(sz);
	memset(&closeList[0], 0, sz);
}

AstarAlgorithm::AstarAlgorithm():closeList(0),openList(0)
{
	nodepool.set_pool(128*128);
	pointpool.set_pool(128*128);
}

AstarAlgorithm::~AstarAlgorithm() 
{
	finalize();
}

bool AstarAlgorithm::inClose(const Point &point)
{
	/*int pos = point.y * g->width + point.x;
	if (closeList[pos]=='1')
		return true;
	else
		return false;*/
	int pos = point.y * g->width + point.x;
	int idx1 = pos>>3;
	int idx2 = pos&(~-8);
	unsigned char itemp = 1 << idx2;
	unsigned char ret = closeList[idx1] & itemp;
	if ( ret > 0 )
		return true;
	else
		return false;
}

void AstarAlgorithm::addToCloseList(const Point &point)
{
	int pos = point.y * g->width + point.x;
	int idx1 = pos / 8;
	int idx2 = pos % 8;
	unsigned char itemp = 1 << idx2;
	closeList[idx1] = closeList[idx1] | itemp;

	/*int pos = point.y * g->width + point.x;
	closeList[pos] = '1';*/
}

char AstarAlgorithm::atMap(int x, int y)
{
	return g->map->at(y * g->width + x);
}

int AstarAlgorithm::getIndex(int x, int y)
{
	//Manhattan
	//return abs(this->endPoint->x - x) * 10
			//+ abs(this->endPoint->y - y) * 10;
	return 14 * max(abs(this->endPoint->x - x), abs(this->endPoint->y - y));
}

int AstarAlgorithm::getG(Point &point, Point &point1)
{
	if(point.x == point1.x || point.y == point1.y)
	{
		return 10;
	}
	else
	{
		return 14;
	}
}

Node *AstarAlgorithm::getNode(Point &point, unsigned int& index)
{
	for (unsigned int i = 0; i < this->openList.size(); i++)
	{
		Node *node = this->openList.at(i);
		if (node->point->x == point.x
		  &&node->point->y == point.y)
		{
			index = i;
			return node;
		}
	}
	return NULL;
}

void AstarAlgorithm::search(list<Point *> &path, Graph *g, Point *start, Point *end)
{
	this->initialize(g, start, end);
		
	memset(&closeList[0], 0, closeList.size());
	Point *p;
	while(true)
	{
		if (*(this->startPoint) == *(this->endPoint))
		{
			p = new Point();
			p->x = this->endPoint->x;
			p->y = this->endPoint->y;
			path.push_back(p);
			break;
		}
		
		removeActive();

		addNearBy(); 
		
		if (PRINT_INFO)
		{
			printList();
		}
		if(openList.empty())
		{
			break; 
		}
		activeNode = openList.at(0);
		//activeNode = openList.GetNode();
		if (*(activeNode->point) == *endPoint)
		{
			p = new Point();
			p->x = activeNode->point->x;
			p->y = activeNode->point->y;
			path.push_back(p);
			while (activeNode->fatherNode != NULL)
			{
				activeNode = activeNode->fatherNode;
				p = new Point();
				p->x = activeNode->point->x;
				p->y = activeNode->point->y;
				path.push_back(p);
			}
			break;
		}
	}
//	return path;
}

