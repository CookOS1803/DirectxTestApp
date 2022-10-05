#pragma once

template<class T>
class DXDeleter
{
public:

	void operator()(T* obj)
	{
		if (obj) obj->Release();
	}
};

