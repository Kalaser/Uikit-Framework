/**
 * @file activity_registry.h
 * @brief App-local Activity registration and explicit Intent routing.
 *
 * The UIKit navigation controller remains the task back-stack.  This small
 * registry gives launcher pages Android-style, named Activity navigation
 * without coupling a page to another page's implementation.
 */
#ifndef LAUNCHER_ACTIVITY_REGISTRY_H
#define LAUNCHER_ACTIVITY_REGISTRY_H

#include "UIKit.hpp"

#include <cstdio>
#include <string>
#include <unordered_map>

namespace launcher {

/** Explicit navigation target.  Targets are registered by Application. */
class Intent {
public:
    explicit Intent(const char *target = "") : m_target(target ? target : "") {}

    const char *target() const { return m_target.c_str(); }
    bool empty() const { return m_target.empty(); }

private:
    std::string m_target;
};

using ActivityFactory = uikit::UIViewController *(*)(uikit::UINavigationController *);

/** Application-owned registry of Activity factories. */
class ActivityRegistry {
public:
    bool registerActivity(const char *target, ActivityFactory factory)
    {
        if (!target || !*target || !factory) {
            return false;
        }
        return m_factories.emplace(target, factory).second;
    }

    uikit::UIViewController *create(const Intent &intent,
                                    uikit::UINavigationController *nav) const
    {
        const auto it = m_factories.find(intent.target());
        if (it == m_factories.end()) {
            std::printf("[ActivityRegistry] unregistered Activity: %s\n", intent.target());
            return nullptr;
        }
        return it->second(nav);
    }

private:
    std::unordered_map<std::string, ActivityFactory> m_factories;
};

} /* namespace launcher */

#endif /* LAUNCHER_ACTIVITY_REGISTRY_H */
