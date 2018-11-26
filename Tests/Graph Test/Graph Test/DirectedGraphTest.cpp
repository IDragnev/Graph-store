#include "CppUnitTest.h"
#include "..\..\..\Graph store\Graph\Directed Graph\DirectedGraph.h"
#include "..\..\..\Graph store\General Exceptions\Exception.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace IDragnev::GraphStore;
using IDragnev::String;

namespace GraphTest
{		
	TEST_CLASS(DirectedGraphTest)
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

	public:
		TEST_METHOD(CtorMakesEmptyGraph)
		{
			DirectedGraph g{ "G" };

			auto vertexItPtr = g.getConstIteratorToVertices();
			auto uniqueEdgesIt = g.getUniqueEdgesConstIterator();

			Assert::IsFalse(*vertexItPtr);
			Assert::IsFalse(uniqueEdgesIt);
		}
		TEST_METHOD(GraphsMustHaveValidStringsAsIDs)
		{
			try
			{
				DirectedGraph{ "" };
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
		TEST_METHOD(VerticesInAGraphMustHaveUniqueIDs)
		{
			DirectedGraph g{ "G" };
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
		TEST_METHOD(VertexInsertion)
		{
			DirectedGraph g{ "Cities" };

			insertVerticesWithIDs(g, { "Sofia", "Varna", "Shumen" });

			Assert::IsTrue(hasVertices(g, { "Sofia", "Shumen", "Varna" }));
		}

		TEST_METHOD(SimpleVertexRemoval)
		{
			DirectedGraph g{ "Cities" };

			insertVerticesWithIDs(g, { "Sofia", "Varna", "Shumen", "Plovdiv" });
			removeVerticesWithIDs(g, { "Varna", "Plovdiv" });

			Assert::IsTrue(hasVertices(g, { "Sofia", "Shumen" }));
			Assert::IsFalse(hasVertices(g, { "Varna", "Plovdiv" }));
		}
		TEST_METHOD(EdgeInsertion)
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

		TEST_METHOD(SingleEdgeAllowedInASpecificDirection)
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

		TEST_METHOD(EdgesWithOppositeDirectionsAreDifferent)
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
			catch (Exception& e)
			{
				Assert::Fail(L"The edge was not accepted");
			}
		}

		TEST_METHOD(EdgeRemoval)
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