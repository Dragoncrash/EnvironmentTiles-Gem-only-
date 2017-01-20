
#include "StdAfx.h"
#include <platform_impl.h>

#include "Env_TileSystemComponent.h"
#include <Env_Tile/Env_TileBus.h>

#include <IGem.h>

namespace Env_Tile
{
    class Env_TileModule
        : public CryHooksModule
    {
    public:
        AZ_RTTI(Env_TileModule, "{13D41E5E-DAC3-400F-83C0-028ECFE7C1E1}", CryHooksModule);

        Env_TileModule()
            : CryHooksModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                //Env_TileSystemComponent::CreateDescriptor(),
				Env_TileGenerator::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<Env_TileGenerator>(),
            };
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(Env_Tile_a290d1afec6044d5a35898a12dcd7859, Env_Tile::Env_TileModule)
