#include "CppUnitTest.h"
#include "..\..\..\Graph store\GraphStore\GraphStore.h"
#include "..\..\..\Graph store\Graph\Directed Graph\DirectedGraph.h"
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GraphStoretest
{		
	TEST_CLASS(GraphStoreTest)
	{
	private:
		typedef std::unique_ptr<Graph> GraphPtr;

		static bool areEqual(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) == 0;
		}

		static void insertGraph(GraphStore& store, const char* ID)
		{
			GraphPtr graphPtr = createGraph(ID);
			store.insert(*graphPtr);
			graphPtr.release();
		}

		static GraphPtr createGraph(const char* ID)
		{
			return GraphPtr(new DirectedGraph(ID));
		}

	public:	
		TEST_METHOD(testInsertWithNonDiplicateGraph)
		{
			GraphStore store;
			GraphPtr graphPtr = createGraph("ID");

			store.insert(*graphPtr);
			const Graph* expected = graphPtr.release();
			const Graph& actual = store.get(expected->getID());

			Assert::IsTrue(expected == &actual);
		}

		TEST_METHOD(testInsertWithDuplicateIDThrows)
		{
			GraphStore store;
			GraphPtr graphPtr = createGraph("ID");
			try
			{
				store.insert(*graphPtr);
				store.insert(*graphPtr);
				Assert::Fail(L"insert did not throw");
			}
			catch (GraphStoreException& e)
			{
				graphPtr.release();
				Assert::IsTrue(areEqual(e.what(), "A graph with ID \'ID\' already exists"));
			}
		}

		TEST_METHOD(testRemoveWithInvalidIDThrows)
		{
			try
			{
				GraphStore emptyStore;
				emptyStore.remove("ID");
				Assert::Fail(L"remove did not throw");
			}
			catch (GraphStoreException& e)
			{
				Assert::IsTrue(areEqual(e.what(), "No graph with ID \'ID\' exists"));
			}
		}

		TEST_METHOD(testRemove)
		{
			GraphStore store;
			insertGraph(store, "ID");
			store.remove("ID");
			try
			{
				const Graph& graph = store.get("ID");
				Assert::Fail(L"removed graph is still in the store");
			}
			catch (GraphStoreException& e)
			{
				Assert::IsTrue(areEqual(e.what(), "No graph with ID \'ID\' exists"));
			}
		}

	};
}