

template <typename CreatedGraph>
inline ConcreteGraphCreator<CreatedGraph>::ConcreteGraphCreator(const String& createdGraphType) :
	GraphCreator(createdGraphType)
{
}


template <typename CreatedGraph>
inline GraphCreator::GraphPtr ConcreteGraphCreator<CreatedGraph>::createEmptyGraph(const String& graphID) const
{
	return GraphPtr(new CreatedGraph(graphID));
}