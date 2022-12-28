#pragma once

#include <functional> /* std::function */
#include <vector> /* std::vector */

template<typename ... Ts>
class Delegate final
{
public:
	using Callback = std::function<void(Ts&&...)>;

	void Bind(const Callback& fn)
	{
		Callbacks.push_back(fn);
	}

	void Bind(Callback&& fn)
	{
		Callbacks.push_back(std::move(fn));
	}

	template<typename ... Us>
	void Invoke(Us&&... vals) const
	{
		for (const Callback& fn : Callbacks)
		{
			fn(std::forward<Us>(vals)...);
		}
	}

private:
	std::vector<Callback> Callbacks;
};