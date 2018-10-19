#ifndef __COMMAND_REGISTRATOR_H_INCLUDED__
#define __COMMAND_REGISTRATOR_H_INCLUDED__

#include "..\Application.h"
#include "..\Commands\Base\Command.h"

namespace IDragnev
{
	namespace GraphStore
	{
		template <typename ConcreteCommand>
		class CommandRegistrator
		{
		public:
			CommandRegistrator() { Application::instance().insertCommand(command); }

			CommandRegistrator(const CommandRegistrator&) = delete;
			CommandRegistrator& operator=(const CommandRegistrator&) = delete;

		private:
			ConcreteCommand command{};
		};
	}
}

#endif //__COMMAND_REGISTRATOR_H_INCLUDED__
