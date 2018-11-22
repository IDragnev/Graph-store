#include "CppUnitTest.h"
#include "..\..\..\Graph store\Graph\Directed Graph\DirectedGraph.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace IDragnev::GraphStore;

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

	};
}