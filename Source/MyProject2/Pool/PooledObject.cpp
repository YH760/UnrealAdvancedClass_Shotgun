#include "PooledObject.h"
#include "PoolObjectSubsystem.h"

void UPooledObject::Init(UPoolObjectSubsystem* Owner)
{
	bIsPoolActive = false;
	ObjectPool = Owner;
}

void UPooledObject::RecycleSelf()
{
	if (ObjectPool)
	{
		ObjectPool->RecyclePooledObject(this);
	}
}

void UPooledObject::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (ObjectPool)
	{
		ObjectPool->OnPoolerCleanup.RemoveDynamic(this, &UPooledObject::RecycleSelf);
	}
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}
