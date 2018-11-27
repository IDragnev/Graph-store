#include "CppUnitTest.h"
#include "..\..\..\Graph store\Graph\Directed Graph\DirectedGraph.h"
#include "..\..\..\Graph store\Graph\Undirected Graph\UndirectedGraph.h"
#include "..\..\..\Graph store\General Exceptions\Exception.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace IDragnev::GraphStore;
using IDragnev::String;

namespace GraphTest
{		
	TEST_CLASS(GraphTest)
	{
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
			for (const auto& ID : IDs)
			{
				if (!g.hasVertexWithID(ID))
				{
					return false;
				}
			}
		}

		static bool existsEdge(const Graph& g, const Vertex& start, const Vertex& end, Weight weight)
		{
			auto result = false;
			auto iteratorPtr = g.getConstIteratorToEdgesLeaving(start);
			
			forEach(*iteratorPtr, [&](const IncidentEdge& e)
			{
				if (e.getIncidentVertex() == end && e.getWeight() == weight)
				{
					result = true;
					return;
				}
			});

			return result;
		}

		static bool hasNeighbour(const Graph& g, const Vertex& v, const String& ID) 
		{
			auto result = false;
			auto iteratorPtr = g.getConstIteratorToEdgesLeaving(v);

			forEach(*iteratorPtr, [&](const IncidentEdge& e)
			{
				auto& neighbour = e.getIncidentVertex();

				if (neighbour.ID() == ID)
				{
					result = true;
					return;
				}
			});

			return result;
		}

	public:
		TEST_METHOD(DirectedGraphCtorMakesEmptyGraph)
		{
			testCtorMakesEmptyGraph<DirectedGraph>("G");
		}

		TEST_METHOD(UndirectedGraphCtorMakesEmptyGraph)
		{
			testCtorMakesEmptyGraph<UndirectedGraph>("G");
		}

		template <typename GraphT>
		static inline void testCtorMakesEmptyGraph(const String& ID)
		{
			GraphT g{ "G" };

			auto vertexItPtr = g.getConstIteratorToVertices();
			auto uniqueEdgesIt = g.getUniqueEdgesConstIterator();

			Assert::IsFalse(*vertexItPtr);
			Assert::IsFalse(uniqueEdgesIt);
		}
		TEST_METHOD(DirectedGraphsMustHaveValidStringsAsIDs)
		{
			testTheEmptyStringIsNotAcceptedAsID<DirectedGraph>();
		}

		TEST_METHOD(UndirectedGraphsMustHaveValidStringsAsIDs)
		{
			testTheEmptyStringIsNotAcceptedAsID<UndirectedGraph>();
		}

		template <typename GraphT>
		inline static void testTheEmptyStringIsNotAcceptedAsID()
		{
			try
			{
				GraphT{ "" };
				Assert::Fail(L"Ctor did not throw");
			}
			catch (Exception& e)
			{
				auto message = String{ e.what() };
				Assert::IsTrue(message == String{ "A Graph ID must be a valid string" });
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
			catch (Exception& e)
			{
				auto message = String{ e.what() };
				Assert::IsTrue(message == String{ "A Vertex ID must be a valid string" });
			}
		}
		TEST_METHOD(VerticesInADirectedGraphMustHaveUniqueIDs)
		{
			testDuplicateVertexIDsAreNotAccepted<DirectedGraph>();
		}

		TEST_METHOD(VerticesInAnUndirectedGraphMustHaveUniqueIDs)
		{
			testDuplicateVertexIDsAreNotAccepted<UndirectedGraph>();
		}

		template <typename GraphT>
		static inline void testDuplicateVertexIDsAreNotAccepted()
		{
			GraphT g{ "G" };
			g.insertVertexWithID("v");

			try
			{
				g.insertVertexWithID("v");
				Assert::Fail(L"The duplicate ID was accepted");
			}
			catch (Exception& e)
			{
				auto message = String{ e.what() };
				Assert::IsTrue(message == String{ "A vertex with such ID already exists" });
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
			GraphT g{ "Cities" };

			insertVerticesWithIDs(g, { "Sofia", "Varna", "Shumen", "Plovdiv" });
			removeVerticesWithIDs(g, { "Varna", "Plovdiv" });

			Assert::IsTrue(hasVertices(g, { "Sofia", "Shumen" }));
			Assert::IsFalse(hasVertices(g, { "Varna", "Plovdiv" }));
		}

		TEST_METHOD(RemovingAVertexInADirectedGraphRemovesAllEdgesToIt)
		{
			testVertexRemovalRemovesAllEdgesToIt<DirectedGraph>();
		}
	
		TEST_METHOD(RemovingAVertexInAnUndirectedGraphRemovesAllEdgesToIt)
		{
			testVertexRemovalRemovesAllEdgesToIt<UndirectedGraph>();
		}

		template <typename GraphT>
		static inline void testVertexRemovalRemovesAllEdgesToIt()
		{
			GraphT g{ "Cities" };
			insertVerticesWithIDs(g, { "Sofia", "Varna" });

			auto& toRemove = g.getVertex("Varna");
			auto& other = g.getVertex("Sofia");
			auto distance = 330U;

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
			Assert::IsFalse(existsEdge(g, end, start, distance), L"The opposite edge is also inserted");
		}

		TEST_METHOD(DirectedGraphsAllowSingleEdgeInASpecificDirection)
		{
			DirectedGraph g{ "Cities" };
			insertVerticesWithIDs(g, { "Sofia", "Varna" });

			auto& start = g.getVertex("Sofia");
			auto& end = g.getVertex("Varna");
			auto distance = 330U;

			try
			{
				g.insertEdge(start, end, distance);
				g.insertEdge(start, end, distance + 1);
				Assert::Fail(L"The duplicate edge was accepted");
			}
			catch (Exception& e)
			{
				auto message = String{ e.what() };
				Assert::IsTrue(message == String{ "Such edge already exists" });
			}		
		}

		TEST_METHOD(DirectedGraphsDifferentiateBetweenEdgesWithOppositeDirections)
		{
			DirectedGraph g{ "Cities" };
			insertVerticesWithIDs(g, { "Sofia", "Varna" });

			auto& start = g.getVertex("Sofia");
			auto& end = g.getVertex("Varna");
			auto distance = 330U;

			try
			{
				g.insertEdge(start, end, distance);
				g.insertEdge(end, start, distance);
			}
			catch (Exception&)
			{
				Assert::Fail(L"The edge was not accepted");
			}
		}

		TEST_METHOD(DirectedGraphEdgeRemoval)
		{
			DirectedGraph g{ "Cities" };
			insertVerticesWithIDs(g, { "Sofia", "Varna" });

			auto& start = g.getVertex("Sofia");
			auto& end = g.getVertex("Varna");
			auto distance = 330U;
			g.insertEdge(start, end, distance);
			g.insertEdge(end, start, distance);

			g.removeEdge(start, end);

			Assert::IsTrue(existsEdge(g, end, start, distance));
			Assert::IsFalse(existsEdge(g, start, end, distance));
		}
	};
}