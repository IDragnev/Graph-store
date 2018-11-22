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
			g.insertVertexWithID("v1");
			
			try
			{
				g.insertVertexWithID("v1");
				Assert::Fail(L"The duplicate ID was accepted");
			}
			catch (Exception& e)
			{
				auto message = String{ e.what() };
				Assert::IsTrue(message == String{ "A vertex with such ID already exists" });
			}
		}
		TEST_METHOD(VerticesInDifferentGraphsCanHaveTheSameIDs)
		{
			DirectedGraph g1{ "G1" };
			DirectedGraph g2{ "G2" };

			try 
			{
				g1.insertVertexWithID("v");
				g2.insertVertexWithID("v");
			}
			catch (Exception&)
			{
				Assert::Fail();
			}
		}

	};
}