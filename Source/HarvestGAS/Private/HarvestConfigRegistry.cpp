#include "HarvestConfigRegistry.h"

UHarvestableConfig* UHarvestConfigRegistry::GetForTag(FGameplayTag Tag) const
{
	return MappingAsset ? MappingAsset->GetDataForTag(Tag) : nullptr;
}
