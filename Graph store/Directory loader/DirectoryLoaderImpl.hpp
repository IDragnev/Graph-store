

template <typename Function>
void DirectoryLoader::load(const Function& function)
{
	while (hasRemainingFiles())
	{
		std::unique_ptr<Graph> result = loadCurrentFile();

		if (result)
		{
			function(std::move(result));
		}

		goToNextFile();
	}
}