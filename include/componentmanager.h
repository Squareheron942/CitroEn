#pragma once

#include <unordered_map>
#include <utility>
#include "script.h"
#include "entt.hpp"
#include <stdio.h>
#include "gameobject.h"
#include "console.h"


class ComponentManager {
    ComponentManager() {}
    public:
    template<typename T> static Script* createScriptInstance(GameObject& owner) { return new T(owner); }
    template<typename T> static void createComponentInstance(GameObject& obj, void* data) { obj.reg.emplace<T>(obj.id, obj, data); }
    
    template<typename T> static bool registerComponent(const char* name) {
        if constexpr (std::is_base_of_v<Script, T>) {
            getScriptMap()[name] = ComponentManager::createScriptInstance<T>;
        } else {
            getComponentMap()[name] = ComponentManager::createComponentInstance<T>;
        }
        return true;
    }

    static bool addComponent(const char* name, GameObject& obj, void* data = NULL) {
        if (getComponentMap().find(name) == getComponentMap().end()) {
            Console::error("Unknown component %s", name);
            return false;
        }
        getComponentMap()[name](obj, data);
        return true;
    }

    static bool addScript(const char* name, GameObject& obj) {
        if (getScriptMap().find(name) == getScriptMap().end()) {
            Console::error("Unknown script %s", name);
            return false;
        }
        obj.scripts.push_back((getScriptMap()[name](obj)));
        return true;
    }

    private:
        // Use Meyer's singleton to prevent SIOF
        static std::unordered_map<const char*, Script*(*)(GameObject&)>& getScriptMap() {
            static std::unordered_map<const char*, Script*(*)(GameObject&)> map;
            return map;
        }
        static std::unordered_map<const char*, void(*)(GameObject&, void*)>& getComponentMap() {
            static std::unordered_map<const char*, void(*)(GameObject&, void*)> map;
            return map;
        }
};

#define COMPONENT_REGISTER(component) bool component ## _component = ComponentManager::registerComponent<component>(#component);