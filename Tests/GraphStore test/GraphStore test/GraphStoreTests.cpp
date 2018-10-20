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

		static void insertGraph(GraphStore& store, const char* ID)
		{
			auto graphPtr = createGraph(ID);
			store.insertGraph(*graphPtr);
			graphPtr.release();
		}

		static std::unique_ptr<Graph> createGraph(const char* ID)
		{
			return std::make_unique<DirectedGraph>(ID);
		}

	public:	
		TEST_METHOD(testInsertWithNonDiplicateGraph)
		{
			auto store = GraphStore{};
			auto graphPtr = createGraph("ID");

			store.insertGraph(*graphPtr);
			auto* expected = graphPtr.release();
			auto& actual = store.getGraph(expected->getID());

			Assert::IsTrue(expected == &actual);
		}

		TEST_METHOD(testInsertWithDuplicateIDThrows)
		{
			auto store = GraphStore{};
			auto graphPtr = createGraph("ID");
			try
			{
				store.insertGraph(*graphPtr);
				store.insertGraph(*graphPtr);
				Assert::Fail(L"insert did not throw");
			}
			catch (Exception& e)
			{
				graphPtr.release();
				Assert::IsTrue(areEqual(e.what(), "A graph with ID \'ID\' already exists"));
			}
		}

		TEST_METHOD(testRemoveWithInvalidIDThrows)
		{
			try
			{
				auto store = GraphStore{};
				store.removeGraph("ID");
				Assert::Fail(L"remove did not throw");
			}
			catch (Exception& e)
			{
				Assert::IsTrue(areEqual(e.what(), "No graph with ID \'ID\' exists"));
			}
		}

		TEST_METHOD(testRemove)
		{
			auto store = GraphStore{};
			insertGraph(store, "ID");
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