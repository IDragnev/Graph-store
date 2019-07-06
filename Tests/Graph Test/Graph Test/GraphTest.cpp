#include "CppUnitTest.h"
#include "Graph\Directed Graph\DirectedGraph.h"
#include "Graph\Undirected Graph\UndirectedGraph.h"
#include "Exceptions\Exceptions.h"
#include "Graph\Base Graph\GraphUtilities.h"
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace IDragnev::GraphStore;
using IDragnev::String;
using namespace IDragnev::Ranges;

namespace GraphTest
{		
	TEST_CLASS(GraphTest)
	{
	public:
		TEST_METHOD(DirectedGraphCtorMakesEmptyGraph)
		{
			assertCtorMakesEmptyGraph<DirectedGraph>();
		}

		TEST_METHOD(UndirectedGraphCtorMakesEmptyGraph)
		{
			assertCtorMakesEmptyGraph<UndirectedGraph>();
		}

		template <typename GraphT>
		inline void assertCtorMakesEmptyGraph()
		{
			GraphT g("G");
			assertIsEmpty(g);
		}

		static void assertIsEmpty(const Graph& g)
		{
			auto vertexItPtr = g.getConstIteratorToVertices();
			auto edgesItPtr = g.getConstIteratorToEdges();

			Assert::IsFalse(*vertexItPtr);
			Assert::IsFalse(*edgesItPtr);
		}

		TEST_METHOD(DirectedGraphsMustHaveValidStringsAsIDs)
		{
			assertTheEmptyStringIsNotAcceptedAsID<DirectedGraph>();
		}
		TEST_METHOD(UndirectedGraphsMustHaveValidStringsAsIDs)
		{
			assertTheEmptyStringIsNotAcceptedAsID<UndirectedGraph>();
		}

		template <typename GraphT>
		inline static void assertTheEmptyStringIsNotAcceptedAsID()
		{
			try
			{
				GraphT{ "" };
				Assert::Fail(L"Ctor did not throw");
			}
			catch (Exception&)
			{
			}
		}
		TEST_METHOD(VerticesMustHaveValidStringsAsIDs)
		{
			DirectedGraph g{ "G" };

			try
			{
				g.insertVertexWithID("");
				Assert::Fail(L"The empty string was accepted");
			}
			catch (Exception&)
			{
			}
		}
		TEST_METHOD(VerticesInADirectedGraphMustHaveUniqueIDs)
		{
			assertDuplicateVertexIDsAreNotAccepted<DirectedGraph>();
		}

		TEST_METHOD(VerticesInAnUndirectedGraphMustHaveUniqueIDs)
		{
			assertDuplicateVertexIDsAreNotAccepted<UndirectedGraph>();
		}

		template <typename GraphT>
		inline void assertDuplicateVertexIDsAreNotAccepted()
		{
			GraphT g{ "G" };
			assertDuplicateVertexIDsAreNotAccepted(g);
		}

		static void assertDuplicateVertexIDsAreNotAccepted(Graph& g)
		{
			g.insertVertexWithID("v");

			try
			{
				g.insertVertexWithID("v");
				Assert::Fail(L"The duplicate ID was accepted");
			}
			catch (Exception&)
			{
			}
		}
		TEST_METHOD(DirectedGraphVertexInsertion)
		{
			testVertexInsertion<DirectedGraph>();
		}

		TEST_METHOD(UndirectedGraphVertexInsertion)
		{
			testVertexInsertion<UndirectedGraph>();
		}

		template <typename GraphT>
		static inline void testVertexInsertion()
		{
			GraphT g{ "Cities" };

			insertVerticesWithIDs(g, { "Sofia", "Varna", "Shumen" });

			Assert::IsTrue(hasVertices(g, { "Sofia", "Shumen", "Varna" }));
		}

		TEST_METHOD(DirectedGraphSimpleVertexRemoval)
		{
			testSimpleVertexRemoval<DirectedGraph>();
		}

		TEST_METHOD(UndirectedGraphSimpleVertexRemoval)
		{
			testSimpleVertexRemoval<UndirectedGraph>();
		}

		template <typename GraphT>
		static inline void testSimpleVertexRemoval()
		{
			GraphT g("Cities");
			testSimpleVertexRemoval(g);
		}

		static void testSimpleVertexRemoval(Graph& g)
		{
			insertVerticesWithIDs(g, { "Sofia", "Varna", "Shumen", "Plovdiv" });
			removeVerticesWithIDs(g, { "Varna", "Plovdiv" });

			Assert::IsTrue(hasVertices(g, { "Sofia", "Shumen" }));
			Assert::IsFalse(hasVertices(g, { "Varna", "Plovdiv" }));
		}

		TEST_METHOD(RemovingAVertexInADirectedGraphRemovesAllEdgesToIt)
		{
			assertReomvingAVertexRemovesAllEdgesToIt<DirectedGraph>();
		}

		TEST_METHOD(RemovingAVertexInAnUndirectedGraphRemovesAllEdgesToIt)
		{
			assertReomvingAVertexRemovesAllEdgesToIt<UndirectedGraph>();
		}

		template <typename GraphT>
		static inline void assertReomvingAVertexRemovesAllEdgesToIt()
		{
			GraphT g{ "Cities" };
			assertReomvingAVertexRemovesAllEdgesToIt(g);
		}

		static void assertReomvingAVertexRemovesAllEdgesToIt(Graph& g)
		{
			insertVerticesWithIDs(g, { "Sofia", "Varna" });

			auto& toRemove = g.getVertex("Varna");
			auto& other = g.getVertex("Sofia");
			auto distance = 330u;
			g.insertEdge(other, toRemove, distance);

			g.removeVertex(toRemove);

			Assert::IsFalse(hasNeighbour(g, other, "Varna"));
		}

		TEST_METHOD(DirectedGraphEdgeInsertion)
		{
			DirectedGraph g{ "Cities" };
			insertVerticesWithIDs(g, { "Sofia", "Varna" });

			auto& start = g.getVertex("Sofia");
			auto& end = g.getVertex("Varna");
			auto distance = 330U;

			g.insertEdge(start, end, distance);

			Assert::IsTrue(existsEdge(g, start, end, distance), L"The correct edge is not inserted");
			Assert::IsFalse(existsEdge(g, end, start, distance), L"The opposite edge is inserted");
		}

		TEST_METHOD(UndirectedGraphEdgeInsertion)
		{
			UndirectedGraph g{ "Debts" };
			insertVerticesWithIDs(g, { "Ivan", "Peter" });

			auto& start = g.getVertex("Ivan");
			auto& end = g.getVertex("Peter");
			auto debt = 330U;

			g.insertEdge(start, end, debt);

			Assert::IsTrue(existsEdge(g, start, end, debt), L"The edge is not inserted");
			Assert::IsTrue(existsEdge(g, end, start, debt), L"The edge does not exist in the reverse direction");
		}
		TEST_METHOD(DirectedGraphsAllowSingleEdgeInASpecificDirection)
		{
			DirectedGraph g{ "Debts" };
			insertVerticesWithIDs(g, { "Ivan", "Stoyan" });

			auto& start = g.getVertex("Ivan");
			auto& end = g.getVertex("Stoyan");
			auto debt = 330U;

			try
			{
				g.insertEdge(start, end, debt);
				g.insertEdge(start, end, debt + 1);
				Assert::Fail(L"The duplicate edge was accepted");
			}
			catch (Exception&)
			{
			}
		}

		TEST_METHOD(UndirectedGraphsAllowSingleEdgeBetweenAnyPairOfVertices)
		{
			UndirectedGraph g{ "Cities" };
			insertVerticesWithIDs(g, { "Sofia", "Varna" });

			auto& start = g.getVertex("Sofia");
			auto& end = g.getVertex("Varna");
			auto distance = 330U;

			try
			{
				g.insertEdge(start, end, distance);
				g.insertEdge(end, start, distance + 1);
				Assert::Fail(L"The duplicate edge was accepted");
			}
			catch (Exception&)
			{
			}
		}

		TEST_METHOD(DirectedGraphsDifferentiateBetweenEdgesWithOppositeDirections)
		{
			DirectedGraph g{ "Debts" };
			insertVerticesWithIDs(g, { "Kiril", "Ivaylo" });

			auto& start = g.getVertex("Kiril");
			auto& end = g.getVertex("Ivaylo");
			auto debt = 5000U;

			try
			{
				g.insertEdge(start, end, debt);
				g.insertEdge(end, start, debt + 100U);
			}
			catch (Exception&)
			{
				Assert::Fail(L"The edge was not accepted");
			}
		}

		TEST_METHOD(DirectedGraphEdgeRemoval)
		{
			DirectedGraph g{ "Debts" };
			insertVerticesWithIDs(g, { "Philip", "George" });

			auto& start = g.getVertex("Philip");
			auto& end = g.getVertex("George");
			auto debt = 100U;
			g.insertEdge(start, end, debt);
			g.insertEdge(end, start, debt);

			g.removeEdge(start, end);

			Assert::IsFalse(existsEdge(g, start, end, debt), L"The edge is not removed");
			Assert::IsTrue(existsEdge(g, end, start, debt), L"The opposite edge is also removed");
		}

		TEST_METHOD(UndirectedGraphEdgeRemoval)
		{
			UndirectedGraph g{ "Cities" };
			insertVerticesWithIDs(g, { "Sofia", "Varna" });

			auto& start = g.getVertex("Sofia");
			auto& end = g.getVertex("Varna");
			auto distance = 330U;
			g.insertEdge(start, end, distance);

			g.removeEdge(end, start);

			Assert::IsFalse(existsEdge(g, end, start, distance), L"The edge is not removed");
			Assert::IsFalse(existsEdge(g, start, end, distance), L"There is still an edge in the reverse direction");
		}

	private:
		using IDList = std::vector<String>;
		using Vertex = Graph::Vertex;
		using IncidentEdge = Graph::IncidentEdge;
		using Weight = IncidentEdge::Weight;

		static void insertVerticesWithIDs(Graph& g, IDList IDs)
		{
			for (const auto& ID : IDs)
			{
				g.insertVertexWithID(ID);
			}
		}

		static void removeVerticesWithIDs(Graph& g, IDList IDs)
		{
			for (const auto& ID : IDs)
			{
				g.removeVertex(ID);
			}
		}

		static bool hasVertices(const Graph& g, IDList IDs)
		{
			return std::all_of(std::cbegin(IDs), 
				               std::cend(IDs), 
				               [&g](const auto& ID) { return g.hasVertexWithID(ID); });
		}

		static bool existsEdge(const Graph& g, const Vertex& start, const Vertex& end, Weight w)
		{
			auto iteratorPtr = g.getConstIteratorToEdgesLeaving(start);
			auto isWantedEdge = [w, &end](const auto& edge) { return edge.getIncidentVertex() == end && edge.getWeight() == w; };

			return anyOf(*iteratorPtr, isWantedEdge);
		}

		static bool hasNeighbour(const Graph& g, const Vertex& v, const String& ID)
		{
			auto iteratorPtr = g.getConstIteratorToEdgesLeaving(v);
			auto predicate = [&ID](const auto& edge) { return edge.getIncidentVertex().getID() == ID; };

			return anyOf(*iteratorPtr, predicate);
		}
	};
}