#include "ShortestPathAlgorithm.h"
#include "UtilityFunctions.h"
#include <assert.h>

namespace IDragnev::GraphStore
{
	ShortestPathAlgorithm::Path::Path(Path&& source) :
		IDs{ std::move(source.IDs) },
		length{ source.length }
	{
		source.length = {};
	}

	auto ShortestPathAlgorithm::Path::operator=(Path&& rhs) -> Path&
	{
		using std::swap;

		if (this != &rhs)
		{
			auto temp = std::move(rhs);
			swap(IDs, temp.IDs);
			swap(length, temp.length);
		}

		return *this;
	}

	ShortestPathAlgorithm::Path::Path(const VertexDecorator& last) :
		length{ last.distance }
	{
		for (auto decorator = &last;
			decorator != nullptr;
			decorator = decorator->predecessor)
		{
			auto vertex = decorator->vertex;
			assert(vertex != nullptr);
			IDs.insertFront(vertex->getID());
		}
	}

	auto ShortestPathAlgorithm::Path::getLength() const -> const Distance&
	{
		return length;
	}

	ShortestPathAlgorithm::ShortestPathAlgorithm(const String& ID) :
		id{ ID },
		searchedGraph{ nullptr },
		goal{ nullptr }
	{
		assert(id != String{ "" });
	}

	auto ShortestPathAlgorithm::findShortestPath(const Graph& graph, const Vertex& source, const Vertex& goal) -> Path
	{
		using Utility::CallOnDestruction;

		if (source != goal)
		{
			auto x = CallOnDestruction{ [this]() noexcept { clear(); } };
			init(graph, goal);
			return findNonTrivialShortestPath(graph, source, goal);
		}
		else
		{
			return buildTrivialPath(goal);
		}
	}

	void ShortestPathAlgorithm::init(const Graph& graph, const Vertex& goal)
	{
		this->searchedGraph = &graph;
		this->goal = &goal;
	}

	auto ShortestPathAlgorithm::buildTrivialPath(const Vertex& v) -> Path
	{
		auto decorator = VertexDecorator{ &v };
		decorator.distance = 0;

		return { decorator };
	}

	bool ShortestPathAlgorithm::isTheGoal(const VertexDecorator& decorator) const
	{
		assert(decorator.vertex != nullptr);
		return *decorator.vertex == *goal;
	}

	const String& ShortestPathAlgorithm::getID() const noexcept
	{
		return id;
	}
}