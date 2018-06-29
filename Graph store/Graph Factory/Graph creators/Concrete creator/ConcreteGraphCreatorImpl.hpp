

template <typename CreatedGraph>
inline ConcreteGraphCreator<CreatedGraph>::ConcreteGraphCreator(const char* createdGraphType) :
	GraphCreator(createdGraphType)
{
}


template <typename CreatedGraph>
inline std::unique_ptr<Graph> ConcreteGraphCreator<CreatedGraph>::createEmptyGraph(const char* graphID) const
{
	return std::unique_ptr<Graph>(new CreatedGraph(graphID));
}