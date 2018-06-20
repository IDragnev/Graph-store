

template <typename CreatedGraph>
inline ConcreteGraphCreator<CreatedGraph>::ConcreteGraphCreator(const char* createdGraphType) :
	GraphCreator(createdGraphType)
{
}


template <typename CreatedGraph>
inline std::unique_ptr<Graph> ConcreteGraphCreator<CreatedGraph>::createEmptyGraph(const char* graphName) const
{
	return std::unique_ptr<Graph>(new CreatedGraph(graphName));
}