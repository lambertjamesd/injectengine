#include "injectorkey.h"
#include "classprovide.h"
#include "valueprovide.h"
#include "injector.h"
#include "factoryprovide.h"
#include "gameobject.h"

#include <iostream>

InjectorKey<int> CONFIG_VALUE(1);

class Transform {
public:
    Transform(Transform* parent) : parent(parent) {}

    void DoThing() const {
        std::cout << "Hello From Transform!\n";
    }
private:
    Transform* parent;
    float x, y;
};

InjectorKey<Transform> TRANSFORM_KEY;
ClassProvide<Transform, Transform*> TRANSFORM_PROVIDE(TRANSFORM_KEY.optional().skipSelf());

class Buzz {
public:
    void DoThing() const {
        std::cout << "Hello From Buzz!\n";
    }
private:
    int fooBar;
};

InjectorKey<Buzz> BUZZ_KEY;
ClassProvide<Buzz> BUZZ_PROVIDE;

class Fizz {
public:
    Fizz(Transform& transform, Buzz* buzz, int config);

    void DoThing() const {
        std::cout << "Hello From Fizz! config: " << config << std::endl;
        if (buzz) {
            std::cout << "I haz a buzz: " << buzz << std::endl;
        } else {
            std::cout << "Oh noes! No buzz" << std::endl;
        }
    }
private:
    Transform& transform;
    Buzz* buzz;
    int config;
};

Fizz::Fizz(Transform& transform, Buzz* buzz, int config) : transform(transform), buzz(buzz), config(config) {}

InjectorKey<Fizz> FIZZ_KEY;
ClassProvide<Fizz, Transform, Buzz*, int> FIZZ_PROVIDE(TRANSFORM_KEY, BUZZ_KEY.optional(), CONFIG_VALUE);

void gameObjectTest() {
    Provide* basicProvideList[] = {
        &TRANSFORM_PROVIDE,
    };

    ProvideEntries basicProvides(basicProvideList, sizeof(basicProvideList) / sizeof(*basicProvideList));

    std::cout << "Create root" << std::endl;
    GameObject* root = GameObject::createRoot(basicProvides);

    std::cout << "Create child" << std::endl;
    GameObject* child = root->createChild(basicProvides);

    std::cout << "Create weak ref" << std::endl;
    WeakRef<GameObject> weakRef = child->weakRef();

    std::cout << "Child exists " << weakRef.exists() << std::endl;

    WeakRef<Transform> transformRef = weakRef.getComponent<Transform>();

    std::cout << "Transform exists " << transformRef.exists() << std::endl;

    std::cout << "Delete child" << std::endl;
    delete child;

    std::cout << "Child exists " << weakRef.exists() << std::endl;
    
    std::cout << "Transform exists " << transformRef.exists() << std::endl;

    delete root;
}

int main() {

    int config = 13;
    ValueProvide<int> configProvide(CONFIG_VALUE, config);

    Provide* parentProvides[2] = {
        &TRANSFORM_PROVIDE,
        &configProvide,
    };

    Injector parentInjector(ProvideEntries(parentProvides, sizeof(parentProvides) / sizeof(*parentProvides)));


    Provide* provides[2] = {
        &FIZZ_PROVIDE,
        &BUZZ_PROVIDE,
    };
    Injector* injector = parentInjector.newChildInjector(ProvideEntries(provides, sizeof(provides) / sizeof(*provides)));

    Transform& transform = injector->get(TRANSFORM_KEY);
    Fizz& fizz = injector->get(FIZZ_KEY);
    Buzz* buzz = injector->get(BUZZ_KEY.optional());

    transform.DoThing();
    fizz.DoThing();
    
    if (buzz) {
        buzz->DoThing();
    } else {
        std::cout << "No Buzz" << std::endl;
    }

    std::cout << "Test: " << (injector->get(TRANSFORM_KEY.optional()) == parentInjector.get(TRANSFORM_KEY.optional())) << std::endl;

    gameObjectTest();

    return 0;
}