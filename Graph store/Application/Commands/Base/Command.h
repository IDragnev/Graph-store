#ifndef __COMMAND_BASE_H_INCLUDED__
#define __COMMAND_BASE_H_INCLUDED__

#include "..\..\args.hxx"
#include <memory>

namespace IDragnev
{
	class String;

	namespace GraphStore
	{
		class Graph;
		class GraphStore;
		struct StringReader;

		class Command
		{
		protected:
			using StringPositional = args::Positional<String, StringReader>;

		public:
			Command() = default;
			virtual ~Command() = default;

			Command(const Command&) = delete;
			Command& operator=(const Command&) = delete;

			void execute(args::Subparser& parser);
			virtual const char* getName() const = 0;
			virtual const char* getDescription() const = 0;

			static void setManagedStore(GraphStore& store);

		protected:
			static Graph& getUsedGraph();
			static void useGraph(const String& ID);
			static void removeGraph(const String& ID);
			static void insertGraph(std::unique_ptr<Graph> graphPtr);

		private:
			virtual void parseArguments(args::Subparser& parser) = 0;
			virtual void execute() const = 0;

		private:
			static Graph* usedGraph;
			static GraphStore* graphStore;
		};
	}
}

#endif //__COMMAND_BASE_H_INCLUDED__