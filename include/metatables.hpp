#ifndef LUA_METATABLES_HPP
#define LUA_METATABLES_HPP

#include <memory>

class LuaStack;
class QObject;

namespace lua {
namespace metatable {

void qobject(LuaStack& stack, std::shared_ptr<QObject> obj);

} // namespace metatable
} // namespace lua

#endif