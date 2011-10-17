#ifndef LUAGLOBAL_HPP
#define LUAGLOBAL_HPP

#include "LuaValue.hpp"

class LuaGlobal : public LuaValue
{
	const QString key;
protected:
	void push(LuaStack& stack) const
	{
		stack.global(key);
	}
public:
	LuaGlobal(Lua& lua, const QString& key) :
		LuaValue(lua), key(key) {}

	template<typename T>
	const LuaValue& operator=(const T& value)
	{
		LuaStack(lua).setGlobal(key, value);
		return *this;
	}

};

#endif // LUAGLOBAL_HPP
