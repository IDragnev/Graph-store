#include "ShortestPathAlgorithm.h"
#include <assert.h>


ShortestPathAlgorithm::Path::Path(Path&& source) :
	IDs{ std::move(source.IDs) },
	len{ source.len }
{
	source.len = Length::Infinity();
}


ShortestPathAlgorithm::Path&
ShortestPathAlgorithm::Path::operator=(Path&& rhs)
{
	if (this != &rhs)
	{
		Path temp{ std::move(rhs) };
		std::swap(IDs, temp.IDs);
		std::swap(len, temp.len);
	}

	return *this;
}


const ShortestPathAlgorithm::Path::Length&
ShortestPathAlgorithm::Path::getLength() const
{
	return len;
}


void ShortestPathAlgorithm::Path::print() const
{
	std::for_each(IDs.getBeginConstIterator(), IDs.getEndConstIterator(),
		[&](const String& str)
	{
		std::cout << str << ' ';
	});
	
	std::cout << "\nLength: " << len << std::endl;
}


ShortestPathAlgorithm::ShortestPathAlgorithm(const char* ID) :
	id{ ID },
	searchedGraph{ nullptr },
	goal{ nullptr }
{
	assert(id != String{ "" });
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


void ShortestPathAlgorithm::initBase(Graph& graph, const Vertex& goal)
{
	this->searchedGraph = &graph;
	this->goal = &goal;
}


const String& ShortestPathAlgorithm::getID() const
{
	return id;
}