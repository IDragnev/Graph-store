#include "CppUnitTest.h"
#include "..\..\..\Graph store\GraphStore\GraphStore.h"
#include "..\..\..\Graph store\Graph\Directed Graph\DirectedGraph.h"
#include "..\..\..\Graph store\General Exceptions\Exception.h"
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
		static bool areEqual(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) == 0;
		}

		static std::unique_ptr<Graph> createGraph(const char* ID)
		{
			return std::make_unique<DirectedGraph>(ID);
		}

	public:	
		TEST_METHOD(InsertWithDuplicateIDThrows)
		{
			GraphStore store;
			try
			{
				store.insertGraph(createGraph("ID"));
				store.insertGraph(createGraph("ID"));
				Assert::Fail(L"insert did not throw");
			}
			catch (Exception& e)
			{
				Assert::IsTrue(areEqual(e.what(), "A graph with ID \'ID\' already exists"));
			}
		}

		TEST_METHOD(RemoveWithInvalidIDThrows)
		{
			try
			{
				GraphStore store;
				store.removeGraph("ID");
				Assert::Fail(L"remove did not throw");
			}
			catch (Exception& e)
			{
				Assert::IsTrue(areEqual(e.what(), "No graph with ID \'ID\' exists"));
			}
		}

		TEST_METHOD(Remove)
		{
			GraphStore store;
			store.insertGraph(createGraph("ID"));
			store.removeGraph("ID");
			try
			{
				auto& graph = store.getGraph("ID");
				Assert::Fail(L"removed graph is still in the store");
			}
			catch (Exception& e)
			{
				Assert::IsTrue(areEqual(e.what(), "No graph with ID \'ID\' exists"));
			}
		}

	};
}