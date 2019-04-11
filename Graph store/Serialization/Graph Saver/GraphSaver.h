#ifndef __GRAPH_SAVER_H_INCLUDED__
#define __GRAPH_SAVER_H_INCLUDED__

#include "..\..\Graph\Base Graph\Graph.h"
#include "..\..\Containers\Hash\Hash.h"
#include "..\..\Containers\Dynamic Array\DArray.h"
#include "..\..\HashFunction\HashFunction.h"
#include <fstream>

namespace IDragnev::GraphStore
{
	class GraphSaver
	{
	private:
		struct Pair
		{
			std::size_t index = 0;
			const String* vertexID = nullptr;
		};

		struct KeyAccessor
		{
			const String& operator()(const Pair* p) const noexcept { return *(p->vertexID); }
		};

		using PairArray = Containers::DArray<Pair>;
		using PairPtrMap = Containers::Hash<const Pair*, String, KeyAccessor>;
		using Vertex = Graph::Vertex;
		using Edge = Graph::Edge;

	public:
		GraphSaver() = default;
		~GraphSaver() = default;

		GraphSaver(const GraphSaver& source);
		GraphSaver& operator=(const GraphSaver& rhs);

		void operator()(const Graph& g, const String& filename);

	private:
		void init(const Graph& g, const String& filename);
		void open(const String& filename);
		void setupCollections(std::size_t verticesCount);

		void decorateGraph();
		void registerVertex(const Vertex& v);

		void saveGraphToFile();
		void writeVertexIDs();
		void writeEdges();
		void writeToFile(const Edge& e);
		std::size_t indexOfID(const Vertex& v) const noexcept;

		void clear();

		template <typename T>
		void writeOnASingleLine(const T& item) { file << item << "\n"; }

	private:
		const Graph* graph = nullptr;
		std::ofstream file;
		PairArray pairs;
		PairPtrMap map;
	};
}

#endif //__GRAPH_SAVER_H_INCLUDED__