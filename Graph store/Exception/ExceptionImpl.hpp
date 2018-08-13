

template <typename T>
Exception<T>::Exception(const char* message) :
	std::runtime_error{ message }
{
}


template <typename T>
Exception<T>::Exception(const std::string& message) :
	std::runtime_error{ message }
{
}