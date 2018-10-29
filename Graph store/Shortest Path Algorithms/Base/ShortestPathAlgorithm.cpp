#include "ShortestPathAlgorithm.h"
#include <assert.h>

namespace IDragnev
{
	namespace GraphStore
	{
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
				using std::swap;

				auto temp = Path{ std::move(rhs) };
				swap(IDs, temp.IDs);
				swap(length, temp.length);
			}

			return *this;
		}

		ShortestPathAlgorithm::Path::Path(const VertexDecorator& last) :
			IDs{},
			length{ last.distance }
		{
			auto* decorator = &last;

			while (decorator)
			{
				auto* vertex = decorator->vertex;
				assert(vertex);
				IDs.insertAsHead(vertex->getID());
				decorator = decorator->predecessor;
			}
		}

		const ShortestPathAlgorithm::Distance&
		ShortestPathAlgorithm::Path::getLength() const
		{
			return length;
		}

		void ShortestPathAlgorithm::Path::print() const
		{
			for (auto&& ID : IDs)
			{
				std::cout << ID << ' ';
			}

			std::cout << "Length: " << length << std::endl;
		}

		ShortestPathAlgorithm::ShortestPathAlgorithm(const String& ID) :
			id{ ID },
			searchedGraph{ nullptr },
			goal{ nullptr }
		{
			assert(id != String{ "" });
		}

		ShortestPathAlgorithm::Path
		ShortestPathAlgorithm::findShortestPath(const Graph& graph, const Vertex& source, const Vertex& goal)
		{
			if (source != goal)
			{
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

		ShortestPathAlgorithm::Path
		ShortestPathAlgorithm::buildTrivialPath(const Vertex& v)
		{
			auto decorator = VertexDecorator{ &v };
			decorator.distance = 0;

			return Path{ decorator };
		}

		bool ShortestPathAlgorithm::isTheGoal(const VertexDecorator& decorator) const
		{
			assert(decorator.vertex);
			return *decorator.vertex == *goal;
		}

	    auto ShortestPathAlgorithm::getEdgesLeaving(const VertexDecorator& decorator) const -> IncidentEdgeConstIteratorPtr
		{
			auto* vertex = decorator.vertex;
			assert(vertex);

			return searchedGraph->getConstIteratorToEdgesLeaving(*vertex);
		}

		const String& ShortestPathAlgorithm::getID() const
		{
			return id;
		}
	}
}