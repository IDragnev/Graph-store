#ifndef __PATH_UTIL_H_INCLUDED__
#define __PATH_UTIL_H_INCLUDED__

#include "..\..\..\Graph\Vertex\Vertex.h"

void printPath(const Vertex& start, const Vertex& end);
void printTrivialPath(const Vertex& start, const Vertex& end);
void printNonExistingPath(const Vertex& start, const Vertex& end);
void printPathLength(const Vertex& start, const Vertex& end);

#endif //__PATH_UTIL_H_INCLUDED__