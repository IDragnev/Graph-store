
template <typename T>
T& Iterator<T>::operator*()
{
	return getCurrent();
}


template <typename T>
Iterator<T>& Iterator<T>::operator++()
{
	goToNext();

	return *this;
}


template <typename T>
Iterator<T>::operator bool() const
{
	return ! isFinished();
}


template <typename T>
bool Iterator<T>::operator!() const
{
	return isFinished();
}
