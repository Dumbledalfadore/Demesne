#include "Settlements/BuildingData.h"

UBuildingData::UBuildingData()
{
	/* Make sure we have population set to at least one to prevent UI issues */
	BuildingModifiers.Add(FLocalResourceData(ELocalResourceType::Population, 1.0f));
}
