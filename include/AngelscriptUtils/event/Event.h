#pragma once

#include <type_traits>
#include <vector>

#include "AngelscriptUtils/util/CASBaseClass.h"
#include "AngelscriptUtils/util/CASRefPtr.h"

#include "AngelscriptUtils/event/EventArgs.h"

class asIScriptContext;
class asIScriptFunction;

namespace asutils
{
/**
*	@brief Represents an event, stores listeners, handles dispatching
*/
class Event : public CASRefCountedBaseClass
{
protected:
	Event(const CASRefPtr<asIScriptContext>& context);

public:
	virtual ~Event();

	Event(const Event&) = delete;
	Event& operator=(const Event&) = delete;

	bool IsSubscribed(asIScriptFunction& function) const;

	void Subscribe(asIScriptFunction& function);

	void Unsubscribe(asIScriptFunction& function);

	void Release() const
	{
		if (InternalRelease())
		{
			delete this;
		}
	}

	/**
	*	@brief Removes all functions defined in the given module from this event
	*/
	void RemoveFunctionsOfModule(asIScriptModule& module);

protected:
	/**
	*	@brief Dispatches the given arguments object to all listeners
	*/
	void Dispatch(EventArgs& arguments);

private:
	//TODO: could store a reference to the event system and allow the context to be swapped out there
	CASRefPtr<asIScriptContext> m_Context;

	std::vector<CASRefPtr<asIScriptFunction>> m_Functions;
};

/**
*	@brief Type-safe event for C++ event dispatching
*/
template<typename T>
class TypedEvent : public Event
{
public:
	TypedEvent(const CASRefPtr<asIScriptContext>& context)
		: Event(context)
	{
	}

	/**
	*	@brief Dispatches an event of type T with arguments args
	*	@tparam T Type of the event to dispatch. Must be a type that inherits from EventArgs
	*/
	void Dispatch(const T& args)
	{
		//Create a dynamically allocated instance so scripts won't retain a reference to the stack
		CASRefPtr<T> object{new T(args)};

		Event::Dispatch(*object);
	}

	/**
	*	@brief Dispatches an event of type T with arguments args
	*	This overload allows you to pass an event object directly and get any changes back in the original object
	*	Take care to dynamically allocate the object to avoid allowing scripts to retain references to it
	*	@tparam T Type of the event to dispatch. Must be a type that inherits from EventArgs
	*/
	void Dispatch(T& args)
	{
		Event::Dispatch(args);
	}
};
}