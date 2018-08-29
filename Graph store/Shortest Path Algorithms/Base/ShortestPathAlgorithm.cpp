#include "ShortestPathAlgorithm.h"
#include <assert.h>


ShortestPathAlgorithm::Path::Path(Path&& source) :
	IDs{ std::move(source.IDs) },
	length{ source.length }
{
	source.length = Distance::Infinity();
}


ShortestPathAlgorithm::Path&
ShortestPathAlgorithm::Path::operator=(Path&& rhs)
{
	if (this != &rhs)
	{
		Path temp{ std::move(rhs) };
		std::swap(IDs, temp.IDs);
		std::swap(length, temp.length);
	}

	return *this;
}


ShortestPathAlgorithm::Path::Path(const VertexWrapper& last) :
	IDs{},
	length{ last.distanceToSource }
{
	auto* wrapper = &last;

	while (wrapper)
	{
		auto* vertex = wrapper->wrappedVertex;
		assert(vertex);
		IDs.insert(vertex->getID());
		wrapper = wrapper->predecessor;
	}
}


const ShortestPathAlgorithm::Distance&
ShortestPathAlgorithm::Path::getLength() const
{
	return length;
}


void ShortestPathAlgorithm::Path::print() const
{
	std::for_each(IDs.getBeginConstIterator(), IDs.getEndConstIterator(),
		[&](const String& ID)
	{
		std::cout << ID << ' ';
	});
	
	std::cout << "\nLength: " << length << std::endl;
}


ShortestPathAlgorithm::ShortestPathAlgorithm(const char* ID) :
	id{ ID },
	searchedGraph{ nullptr },
	goal{ nullptr }
{
	assert(id != String{ "" });
}


ShortestPathAlgorithm::Path 
ShortestPathAlgorithm::findShortestPath(Graph& graph, const Vertex& source, const Vertex& goal)
{
	if (source != goal)
	{
		initBase(graph, goal);
		return findNonTrivialShortestPath(graph, source, goal);
	}
	else
	{
		return buildTrivialPath(goal);
	}
}


void ShortestPathAlgorithm::initBase(Graph& graph, const Vertex& goal)
{
	this->searchedGraph = &graph;
	this->goal = &goal;
}


ShortestPathAlgorithm::Path
ShortestPathAlgorithm::buildTrivialPath(const Vertex& v)
{
	return Path{ VertexWrapper{ &v } };
}


bool ShortestPathAlgorithm::isTheGoal(const Vertex& vertex) const
{
	return vertex == *goal;
}


ShortestPathAlgorithm::EdgeConstIteratorPtr
ShortestPathAlgorithm::getEdgesLeaving(const Vertex& v) const
{
	return searchedGraph->getConstIteratorToEdgesLeaving(v);
}


const String& ShortestPathAlgorithm::getID() const
{
	return id;
}