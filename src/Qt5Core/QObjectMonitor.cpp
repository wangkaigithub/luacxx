#include "QObjectMonitor.hpp"

#include <cassert>

namespace lua {

QObjectMonitor::QObjectMonitor(
        const QObject* const object,
        LuaUserdata* const userdata) :
    object(object),
    userdata(userdata),
    _destroyOnGC(false)
{
    if (!userdata->managed()) {
        throw std::logic_error("Refusing to manage a non-managed userdata");
    }

    // Watch for destruction from C++
    connect(object, SIGNAL(destroyed(QObject*)), this, SLOT(destroyedFromC()));

    // Watch for destruction from Lua
    userdata->setManager([this]() { destroyedFromLua(); });
}

void QObjectMonitor::destroyedFromC()
{
    if (!userdata) {
        // Already destroyed from Lua
        assert(!object);
        return;
    }
    object = nullptr;
    if (userdata) {
        auto tmp = userdata;
        userdata = nullptr;
        tmp->reset();
    }
}

void QObjectMonitor::destroyedFromLua()
{
    if (!object) {
        // Already destroyed from C++
        assert(!userdata);
        return;
    }
    userdata = nullptr;
    if (_destroyOnGC && object) {
        auto tmp = object;
        object = nullptr;
        delete tmp;
    }
}

void QObjectMonitor::setDestroyOnGC(const bool destroy)
{
    _destroyOnGC = destroy;
    if (!userdata) {
        destroyedFromLua();
    }
}

QObjectMonitor::~QObjectMonitor()
{
    destroyedFromC();
}

} // namespace lua

// vim: set ts=4 sw=4 :
