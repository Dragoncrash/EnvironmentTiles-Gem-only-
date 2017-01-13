#include "StdAfx.h"
#include "CustomAdditions.h"

#include <AzCore/Math/Transform.h>
#include <AzCore/Component/TransformBus.h>
#include <MathConversion.h>

void AddEntityRotation(FlowEntityId id, Vec3 addRot, ComponentEntityAddRotation::CoordSys c_space){
	AZ::Transform newRot(AZ::Transform::CreateRotationX(AZ::DegToRad(addRot.x))
		* AZ::Transform::CreateRotationY(AZ::DegToRad(addRot.y))
		* AZ::Transform::CreateRotationZ(AZ::DegToRad(addRot.z)));
	
	AZ::Transform currTM;

	switch (c_space){
	case ComponentEntityAddRotation::CoordSys::Local:
		EBUS_EVENT_ID_RESULT(currTM, id, AZ::TransformBus, GetWorldTM);
		newRot *= currTM;
		EBUS_EVENT_ID(id, AZ::TransformBus, SetWorldTM, newRot);
		break;
	case ComponentEntityAddRotation::CoordSys::World:
		EBUS_EVENT_ID_RESULT(currTM, id, AZ::TransformBus, GetLocalTM);
		newRot *= currTM;
		EBUS_EVENT_ID(id, AZ::TransformBus, SetLocalTM, newRot);
		break;
	}

}

void ComponentEntityAddRotation::GetConfiguration(SFlowNodeConfig& config){
	static const SInputPortConfig in_config[] =
	{
		InputPortConfig<Vec3>("Rotation", _HELP("New Rotation to add to current rotation")),
		InputPortConfig<int>("CoordSystem", 1, _HELP("The coordinate system to use"),
			_HELP("Coordinate System"), _UICONFIG("enum_int:Local=0,World=1")),
		{0}
	};
	static const SOutputPortConfig out_config[] =
	{
		OutputPortConfig<Vec3>("Resulting Rotation", _HELP("The value of the vector after rotation")),
		{0}
	};

	config.pInputPorts = in_config;
	config.pOutputPorts = out_config;
	config.nFlags |= EFLN_ACTIVATION_INPUT | EFLN_TARGET_ENTITY;
	config.SetCategory(EFLN_APPROVED);
}

void ComponentEntityAddRotation::ProcessEvent(EFlowEvent evt, SActivationInfo* info){
	FlowEntityType fet = GetFlowEntityTypeFromFlowEntityId(info->entityId);

	AZ_Assert((fet == FlowEntityType::Component) || (fet == FlowEntityType::Invalid),
		"This node is incompatible with Legacy Entities");

	switch (evt){
	case eFE_Activate:
		if (IsPortActive(info, InputPorts::Activate)){
			if (fet == FlowEntityType::Component){
				auto c_system = static_cast<CoordSys>(GetPortInt(info, InputPorts::CoordSystem));
				Vec3 addRot = GetPortVec3(info, InputPorts::Rotation);
				AddEntityRotation(info->entityId, addRot, c_system);
			}
		}
		break;
	}
}

REGISTER_FLOW_NODE("ComponentEntity:TransformComponent:AddEntityRotation", ComponentEntityAddRotation)