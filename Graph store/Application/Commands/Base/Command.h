#ifndef __COMMAND_BASE_H_INCLUDED__
#define __COMMAND_BASE_H_INCLUDED__

#include <memory>

namespace args
{
	class Subparser;
	
	struct ValueReader;

	template <typename T, typename Reader>
	class Positional;	
}

namespace IDragnev
{
	class String;

	namespace GraphStore
	{
		class Graph;
		class GraphStore;

		class Command
		{
		protected:
			struct StringReader
			{
				template <typename Name, typename Source, typename Destination>
				void operator()(const Name&, const Source& value, Destination& dest) const
				{
					dest = value.c_str();
				}
			};

			template <typename T>
			using Positional = args::Positional<T, args::ValueReader>;
			using StringPositional = args::Positional<String, StringReader>;

		public:
			Command() = default;
			virtual ~Command() = default;

			Command(const Command&) = delete;
			Command& operator=(const Command&) = delete;

			void execute(args::Subparser& parser);
			virtual const char* getName() const noexcept = 0;
			virtual const char* getDescription() const noexcept = 0;

			static void setManagedStore(GraphStore& store);

		protected:
			static void setIfMatched(String& str, StringPositional& argument);

			static Graph& getUsedGraph();
			static Graph& getGraph(const String& ID);
			static void useGraph(const String& ID);
			static void removeGraph(const String& ID);
			static void insertGraph(std::unique_ptr<Graph> graphPtr);
			
		private:
			virtual void parseArguments(args::Subparser& parser) = 0;
			virtual void execute() = 0;

		private:
			static Graph* usedGraph;
			static GraphStore* graphStore;
		};
	}
}

#endif //__COMMAND_BASE_H_INCLUDED__