
template <typename T>
const T& ConstIterator<T>::operator*() const
{
	return getCurrent();
}


template <typename T>
ConstIterator<T>& ConstIterator<T>::operator++()
{
	goToNext();

	return *this;
}


template <typename T>
ConstIterator<T>::operator bool() const
{
	return ! isFinished();
}


template <typename T>
bool ConstIterator<T>::operator!() const
{
	return isFinished();
}