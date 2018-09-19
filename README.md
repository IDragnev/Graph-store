# Graph-store

An application which lets the user build [graphs](https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)) and search [paths](https://en.wikipedia.org/wiki/Path_(graph_theory)) within them. 
The graph can be directed or undirected. 
All graphs are weighted, with the edge weights being positive integers and the default weight being 1.

A graph can be built by inserting vertices and inserting edges between vertices.
In a graph there can be at most one edge between any two vertices.

To start the application, the user must specify a directory (as a command line argument) in which the graphs are (to be) stored.
It is then run via command-line interface, supported thanks to https://github.com/Taywee/args.
Arguments are delimited by whitespaces. If an argument consists of multiple words, it must be surrounded by single quotation marks: 'multiple words argument'.

## General commands
 - **HELP**
    - Lists the supported commands.
    
 - **EXIT**
    - Exits the application.

## The following commands let the user manage graphs.

 - **INSERT-GRAPH [id] *[type]***
    - Creates a new graph with ID [id] and type [type] and makes it the currently used graph.
  Graphs must have unique IDs. The ID must be a valid string.
  The type of the graph can be *directed* or *undirected*. If no [type] is specified, it is 
  undirected.

 - **REMOVE-GRAPH [id]**
   - Removes the graph with ID [id]. 
If this was the currently used graph, there is no currently used graph after executing the command.
The currently used graph can be switched via the USE GRAPH command.

 - **USE-GRAPH [id]**
   - Switches the currently used graph with the graph with ID [id].

 - **LIST-GRAPH [id]**
   - Lists each vertex of the graph - its ID and a list of its neighbours.


## The following commands can only be used if a graph is (present and) selected as the currently used one.

 - **INSERT-VERTEX [id]**
   - Creates and inserts a vertex with ID [id] in the currently used graph. 
All vertices in a graph must have unique IDs. The ID must be a valid string.

 - **REMOVE-VERTEX [id]**
   - Removes the vertex with ID [id] from the currently used graph. 

 - **INSERT-EDGE [startVertexId] [endVertexId] *[weight]***
   - Creates and inserts an edge with weight [weight] between [startVertexId] and [endVertexId] in the currently used graph. 
If the graph is directed, the edge has a direction: from [startVertexId] to [endVertexId].
If the graph is undirected, the edge has no direction. 
If specified [weight] must be a positive integer, else it is 1.

 - **REMOVE-EDGE [startVertexId] [endVertexId]**
   - Removes the edge between [startVertexId] and [endVertexId]. 
If the graph is directed, the edge from [startVertexId] to [endVertexId] is deleted.
If the graph is undirected, the order of the sent vertex IDs does not matter.

- **SEARCH [startVertexId] [endVertexId] [algorithm]**
  - Searches a shortest path starting from [startVertexId] and finishing at [endVertexId] in the 
currently used graph.
If such path is found, its constituent vertices are listed along with the path length, else a message is shown.
The algorithm used is specified by [algorithm].
The currently possible choices for [algorithm] are:
     - BFS : searches an unweighted shortest path using [Breadth-first search](https://en.wikipedia.org/wiki/Breadth-first_search).
     - DFS : searches an unweighted shortest path using [Iterative deepening depth-first search](https://en.wikipedia.org/wiki/Iterative_deepening_depth-first_search).
     - Dijkstra : searches a weighted shortest path using [Dijkstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm).
