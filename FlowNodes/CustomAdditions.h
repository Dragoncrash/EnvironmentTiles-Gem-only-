#include <ISystem.h>
#include <IFlowSystem.h>
#include "../FlowBaseNode.h"

#pragma region Additional Transform Nodes

class ComponentEntityAddRotation
	: public CFlowBaseNode<eNCT_Instanced>
{
public:
	ComponentEntityAddRotation(SActivationInfo*){}
	
	void GetMemoryUsage(ICrySizer* sizer) const override { sizer->Add(*this); }
	void GetConfiguration(SFlowNodeConfig&) override;
	IFlowNodePtr Clone(SActivationInfo* info) override { return new ComponentEntityAddRotation(info); }
	void ProcessEvent(EFlowEvent evt, SActivationInfo* info) override;

	enum class CoordSys{
		Local = 0,
		World
	};

private:
	enum InputPorts{
		Activate = 0,
		Rotation,
		CoordSystem
	};

	enum OutputPorts{
		Result
	};
};

#pragma endregion Additional Transform Nodes