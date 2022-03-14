#include "Registry.h"


namespace ECS
{
    Entity Registry::Create()
    {
        Entity entity;

        if (myFreeIDs.size() > 0)
        {
            entity = myFreeIDs.front();
            myFreeIDs.pop();

        }
        else
        {
            myEntities.emplace_back();
            entity = static_cast<Entity>(myEntities.size() - 1);
        }

        return entity;
    }

    void Registry::Destroy(Entity aEntity)
    {
        myFreeIDs.push(aEntity);
        myEntities[aEntity].reset();
    }
}