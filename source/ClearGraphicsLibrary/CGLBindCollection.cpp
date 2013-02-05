#include "cgl.h"

template <class T>
bool cgl::core::CGLBindCollection<T>::Update()
{
	return restore();
}

template <class T>
HRESULT cgl::core::CGLBindCollection<T>::onRestore()
{
	// reset ptr
	onReset();

	// get collection from parent class
	std::vector<std::tr1::shared_ptr<CGLBase<T>>>& collection = GetCollection();

	// allocate memory for the ptr arr
	set((T**)malloc( sizeof(T*) * collection.size()));

	// set all ptr NULL
	ZeroMemory(get(), sizeof(T*) * collection.size());

	// extract ptr from the collection
	for (UINT i = 0; i < collection.size(); i++)
	{
		if (collection[i])
			((T**)get())[i] = collection[i]->get();
	}

	return S_OK;
}

template <class T>
void cgl::core::CGLBindCollection<T>::onReset()
{
	if (get())
	{
		free(get());
		set(NULL);
	}
}





