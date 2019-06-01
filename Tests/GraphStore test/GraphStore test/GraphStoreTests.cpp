#include "CppUnitTest.h"
#include "GraphStore\GraphStore.h"
#include "Graph\Directed Graph\DirectedGraph.h"
#include "Exceptions\Exceptions.h"
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using IDragnev::GraphStore::Graph;
using IDragnev::GraphStore::GraphStore;
using IDragnev::GraphStore::DirectedGraph;
using IDragnev::GraphStore::Exception;

namespace GraphStoretest
{		
	TEST_CLASS(GraphStoreTest)
	{
	private:
		static std::unique_ptr<Graph> createGraph(const char* ID)
		{
			return std::make_unique<DirectedGraph>(ID);
		}

	public:	
		TEST_METHOD(insertWithDuplicateIDThrows)
		{
			GraphStore store;
			try
			{
				store.insert(createGraph("ID"));
				store.insert(createGraph("ID"));
				Assert::Fail(L"insert did not throw");
			}
			catch (Exception&)
			{
			}
		}

		TEST_METHOD(removingNonExistingGraphThrows)
		{
			GraphStore store;
			try
			{
				store.remove("ID");
				Assert::Fail(L"remove did not throw");
			}
			catch (Exception&)
			{
			}
		}

		TEST_METHOD(removingAnExistingGraph)
		{
			GraphStore store;
			store.insert(createGraph("ID"));
			store.remove("ID");

			try
			{
				auto& graph = store["ID"];
				Assert::Fail(L"removed graph is still in the store");
			}
			catch (Exception&)
			{
			}
		}
	};
}