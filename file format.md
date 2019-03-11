## The file format is:
\<graph type\>
\<graph id\>
\<N: number of vertices\>
\<vertex0 id\>
\<vertex1 id\>
  .
  .
  .
\<vertexN id\>
\<K: number of edges\>
\<edge1\>
\<edge2\>
  .
  .
  .
\<edgeK\> 

# Where:
 - \<graph type\> must be either directed or undirected
 - \<graph id\> and \<vertex id\> must be valid strings
 - the edge format is: (startIDNumber, endIDNumber, weight)
    - where weight is a positive integer  
   	- each IDNumber is a non-negative integer and corresponds to the vertex with id \<vertex(IDNumber) id\>
 - the name of the (text) file must be identical to the name of the graph

## Example:  
#*my graph.txt
undirected
my graph
6
v0
v1
v2
v3
v4
v5
8
(0, 1, 10)
(0, 2, 12)
(1, 2, 13)
(1, 3, 8)
(1, 5, 4)
(3, 2, 100)
(2, 4, 1)
(5, 4, 3)