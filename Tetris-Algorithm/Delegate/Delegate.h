#pragma once

#include <functional> /* std::function */
#include <vector> /* std::vector */

template<typename ... Ts>
struct Delegate
{
	using Callback = std::function<void(Ts...)>;

	void Bind(Callback fn)
	{
		Callbacks.push_back(fn);
	}
	template<typename T>
	void Bind(T* pObj, void (T::* fn)(Ts...))
	{
		Callbacks.push_back([pObj, fn](Ts... args)->void
			{
				(pObj->*fn)(std::forward<Ts>(args)...);
			});
	}

	template<typename ... Us>
	void Invoke(Us&&... args) const
	{
		for (const Callback& cb : Callbacks)
			cb(std::forward<Us>(args)...);
	}

	std::vector<Callback> Callbacks;
};