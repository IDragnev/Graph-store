# Graph-store

A CLI application which lets the user build [graphs](https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)) and search [paths](https://en.wikipedia.org/wiki/Path_(graph_theory)) within them. 
The graphs can be directed or undirected. 
All graphs are weighted, with the edge weights being positive integers and the default weight being 1.

A graph can be built by inserting vertices and inserting edges between vertices.  
Graphs can also be built directly with text files, see the format [here](https://github.com/IDragnev/Graph-store/blob/refactoring/file%20format.md).  
In a graph there can be at most one edge between any two vertices.

The application has no automated build system.   
In order to start it, the user must specify a directory (as a command line argument) in which the graphs are (to be) stored.  
The command-line interface is supported using [Taywee/args](https://github.com/Taywee/args).
Arguments are delimited by whitespaces. If an argument consists of multiple words, it must be surrounded by single quotation marks: 'multiple words argument'.  
Formatting is done using [{fmt}](https://github.com/fmtlib/fmt).  
**A c++17 compliant compiler is needed!**

## EXAMPLE
INSERT-GRAPH G directed  
INSERT-VERTEX v1  
INSERT-VERTEX v2  
INSERT-VERTEX v3  
INSERT-EDGE v1 v2 100  
INSERT-EDGE v2 v3   
INSERT-EDGE v1 v3 10000  
SEARCH-PATH v1 v3 Dijkstra  
*finds the path v1->v2->v3 with length 101.*  

## General commands
 - **HELP**
    - Lists the supported commands.
    
 - **EXIT**
    - Exits the application.

## The following commands let the user manage graphs.

 - **INSERT-GRAPH \<id\> [type]**
    - Creates a new graph with and makes it the currently used graph.
  Graphs must have unique IDs. The ID must be a valid string.
  The type of the graph can be *directed* or *undirected*. If no type is specified, it is 
  undirected.

 - **REMOVE-GRAPH {\<id\>}<sup>+<sup>**
   - Removes the specified graphs.   
If the currently used graph was among them, there is no currently used graph after executing the command.
The currently used graph can be switched via the USE-GRAPH command.

 - **USE-GRAPH \<id\>**
   - Switches the currently used graph with the specified graph.


## The following commands can only be used if a graph is (present and) selected as the currently used one.

 - **SAVE { \<graphId\> }**
   - Saves the specified graphs. If no arguments are supplied, the currently used graph is saved.
   
 - **INSERT-VERTEX \<id\>**
   - Creates and inserts a vertex in the currently used graph. 
All vertices in a graph must have unique IDs. The ID must be a valid string.

 - **REMOVE-VERTEX \<id\>**
   - Removes the specified vertex from the currently used graph. 

 - **INSERT-EDGE \<startVertexId\> \<endVertexId\> [weight]**
   - Inserts an edge between **startVertexId** and **endVertexId** in the currently used graph. 
If the graph is directed, the edge has a direction: from **startVertexId** to **endVertexId**.
If the graph is undirected, the edge has no direction. 
If specified **weight** must be a positive integer, else it is 1.

 - **REMOVE-EDGE \<startVertexId\> \<endVertexId\>**
   - Removes the edge between **startVertexId** and **endVertexId**. 
If the graph is directed, the edge from **startVertexId** to **endVertexId** is deleted.
If the graph is undirected, the order of the sent vertex IDs does not matter.

- **SEARCH \<startVertexId\> \<endVertexId\> \<algorithm\>**
  - Searches a shortest path starting from **startVertexId** and finishing at **endVertexId** in the 
currently used graph.
If such path is found, its constituent vertices are listed along with the path length, else a message is shown.
The algorithm used is specified by **algorithm**.
The currently possible choices for **algorithm** are:
     - BFS : searches an unweighted shortest path using [Breadth-first search](https://en.wikipedia.org/wiki/Breadth-first_search).
     - DFS : searches an unweighted shortest path using [Iterative deepening depth-first search](https://en.wikipedia.org/wiki/Iterative_deepening_depth-first_search).
     - Dijkstra : searches a weighted shortest path using [Dijkstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm).
