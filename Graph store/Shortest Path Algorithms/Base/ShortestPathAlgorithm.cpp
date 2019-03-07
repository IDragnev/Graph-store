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

		auto ShortestPathAlgorithm::Path::operator=(Path&& rhs) -> Path&
		{
			if (this != &rhs)
			{
				using std::swap;

				auto temp = std::move(rhs);
				swap(IDs, temp.IDs);
				swap(length, temp.length);
			}

			return *this;
		}

		ShortestPathAlgorithm::Path::Path(const VertexDecorator& last) :
			length{ last.distance }
		{
			for(auto decorator = &last;
				decorator != nullptr;
				decorator = decorator->predecessor)
			{
				auto vertex = decorator->vertex;
				assert(vertex != nullptr);
				IDs.insertAsHead(vertex->ID());
			}
		}

		auto ShortestPathAlgorithm::Path::getLength() const -> const Distance&
		{
			return length;
		}

		void ShortestPathAlgorithm::Path::print() const
		{
			for (const auto& ID : IDs)
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

		auto ShortestPathAlgorithm::findShortestPath(const Graph& graph, const Vertex& source, const Vertex& goal) -> Path
		{
			if (source != goal)
			{
				init(graph, goal);
				auto result = findNonTrivialShortestPath(graph, source, goal);
				clear();
				return result;
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

		const String& ShortestPathAlgorithm::getID() const
		{
			return id;
		}
	}
}