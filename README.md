# Graph-store

An application which lets the user build [graphs](https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)) and search [paths](https://en.wikipedia.org/wiki/Path_(graph_theory)) within them. 
The graph can be directed or undirected. 
All graphs are weighted, with the edge weights being positive integers and the default weight being 1.

A graph can be built by inserting vertices and inserting edges between vertices.
In a graph there can be at most one edge between any two vertices.

## The following commands let the user manage graphs.

 - **CREATE GRAPH [id] [type]**
    - Creates a new graph with ID [id] and type [type] and makes it the currently used graph.
  Graphs must have unique IDs. The ID must be a valid string.
  The type of the graph can be *directed* or *undirected*.

 - **DELETE GRAPH [id]**
   - Deletes the graph with ID [id]. 
If this was the currently used graph, there is no currently used graph after executing the command.
The currently used graph can be switched via the USE GRAPH command.

 - **USE GRAPH [id]**
   - Switches the currently used graph with the graph with ID [id].

 - **LIST GRAPH [id]**
   - Lists each vertex of the graph - its ID and a list of its neighbours.


## The following commands can only be used if a graph is (present and) selected as the currently used one.

 - **INSERT VERTEX [id]**
   - Creates and inserts a vertex with ID [id] in the currently used graph. 
All vertices in a graph must have unique IDs. The ID must be a valid string.

 - **DELETE VERTEX [id]**
   - Deletes the vertex with ID [id] from the currently used graph. 

 - **INSERT EDGE [vertexFromId] [vertexToId] *[weight]***
   - Creates and inserts an edge between [vertexFromId] and [vertexToId] in the currently used graph. 
If the graph is directed, the edge has a direction: from [vertexFromId] to [vertexToId].
If the graph is undirected, the edge has no direction. 
Optional parameter: the weight of the edge (a positive integer). The weight is 1 if not specified.

 - **DELETE EDGE [vertexFromId] [vertexToId]**
   - Deletes the edge between [vertexFromId] and [vertexToId]. 
If the graph is directed, the edge from [vertexFromId] to [vertexToId] is deleted.
If the graph is undirected, the order of the sent vertex IDs does not matter.

- **SEARCH [vertexFromId] [vertexToId] [algorithm]**
  - Searches a shortest path starting from [vertexFromId] and finishing at [vertexToId] in the 
currently used graph.
If such path is found, its constituent vertices are listed, else a message is shown.
The algorithm used is specified by the parameter [algorithm].
The currently possible choices for [algorithm] are:
     - BFS : searches an unweighted shortest path using [Breadth-first search](https://en.wikipedia.org/wiki/Breadth-first_search).
     - DFS : searches an unweighted shortest path using [Iterative deepening depth-first search](https://en.wikipedia.org/wiki/Iterative_deepening_depth-first_search).
     - Dijkstra : searches a weighted shortest path using [Dijkstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm).
