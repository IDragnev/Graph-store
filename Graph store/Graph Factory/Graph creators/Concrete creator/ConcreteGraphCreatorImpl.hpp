

template <typename CreatedGraph>
inline ConcreteGraphCreator<CreatedGraph>::ConcreteGraphCreator(const char* createdGraphType) :
	GraphCreator(createdGraphType)
{
}


template <typename CreatedGraph>
inline GraphCreator::GraphPtr ConcreteGraphCreator<CreatedGraph>::createEmptyGraph(const char* graphID) const
{
	return GraphPtr(new CreatedGraph(graphID));
}