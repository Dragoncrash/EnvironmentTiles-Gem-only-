#include <ISystem.h>
#include "../FlowBaseNode.h"
#include <IFlowSystem.h>


#pragma region Env_TileSystemComponent
/////////////////////////////////////////////
//Env_TileSystemComponent Nodes
/////////////////////////////////////////////

class EnvTileNode_SimpleEnables
	: public CFlowBaseNode<eNCT_Instanced>
{
public:
	EnvTileNode_SimpleEnables(SActivationInfo*){}
	IFlowNodePtr Clone(SActivationInfo* info) override
	{
		return new EnvTileNode_SimpleEnables(info);
	}

	void GetConfiguration(SFlowNodeConfig&) override;
	void ProcessEvent(EFlowEvent, SActivationInfo*) override;
	void GetMemoryUsage(ICrySizer* sizer) const override { sizer->Add(*this); }

private:
	enum InputPorts{
		Get,
		Set,
		in_Cloudy,
		in_Windy
	};
	enum OutputPorts{
		out_Cloudy,
		out_Windy
	};
};

class EnvTileNode_Rain
	: public CFlowBaseNode<eNCT_Instanced>
{
public:
	EnvTileNode_Rain(SActivationInfo*){}
	IFlowNodePtr Clone(SActivationInfo* info) override
	{
		return new EnvTileNode_Rain(info);
	}

	void GetConfiguration(SFlowNodeConfig&) override;
	void ProcessEvent(EFlowEvent, SActivationInfo*) override;
	void GetMemoryUsage(ICrySizer* sizer) const override { sizer->Add(*this); }

private:
	enum InputPorts{
		Get,
		Set,
		in_Light,
		in_Medium,
		in_Heavy,
		in_TOD_Start,
		in_TOD_End
	};
	enum OutputPorts{
		out_Light,
		out_Medium,
		out_Heavy,
		out_TOD_Start,
		out_TOD_End
	};
};

class EnvTileNode_Snow
	: public CFlowBaseNode<eNCT_Instanced>
{
public:
	EnvTileNode_Snow(SActivationInfo*){}
	IFlowNodePtr Clone(SActivationInfo* info) override
	{
		return new EnvTileNode_Snow(info);
	}

	void GetConfiguration(SFlowNodeConfig&) override;
	void ProcessEvent(EFlowEvent, SActivationInfo*) override;
	void GetMemoryUsage(ICrySizer* sizer) const override { sizer->Add(*this); }

private:
	enum InputPorts{
		Get,
		Set,
		in_Light,
		in_Medium,
		in_Heavy,
		in_TOD_Start,
		in_TOD_End
	};
	enum OutputPorts{
		out_Light,
		out_Medium,
		out_Heavy,
		out_TOD_Start,
		out_TOD_End
	};
};

class EnvTileNode_RainExtra
	: public CFlowBaseNode<eNCT_Instanced>
{
public:
	EnvTileNode_RainExtra(SActivationInfo*){}
	IFlowNodePtr Clone(SActivationInfo* info) override
	{
		return new EnvTileNode_RainExtra(info);
	}

	void GetConfiguration(SFlowNodeConfig&) override;
	void ProcessEvent(EFlowEvent, SActivationInfo*) override;
	void GetMemoryUsage(ICrySizer* sizer) const override { sizer->Add(*this); }

private:
	enum InputPorts{
		Get,
		Set,
		in_Thunder,
		in_Lightning
	};
	enum OutputPorts{
		out_Thunder,
		out_Lightning
	};
};

class EnvTileNode_SnowExtra
	: public CFlowBaseNode<eNCT_Instanced>
{
public:
	EnvTileNode_SnowExtra(SActivationInfo*){}
	IFlowNodePtr Clone(SActivationInfo* info) override
	{
		return new EnvTileNode_SnowExtra(info);
	}

	void GetConfiguration(SFlowNodeConfig&) override;
	void ProcessEvent(EFlowEvent, SActivationInfo*) override;
	void GetMemoryUsage(ICrySizer* sizer) const override { sizer->Add(*this); }

private:
	enum InputPorts{
		Get,
		Set,
		in_FreezeGround,
		in_FreezeAmount
	};
	enum OutputPorts{
		out_FreezeGround,
		out_FreezeAmount
	};
};

#pragma endregion Node Definitions for Env_TileSystemComponent

#pragma region Env_TileGenerator
/////////////////////////////////////////////
//Env_TileGenerator Nodes
/////////////////////////////////////////////

/*NOTE: Generation is static in that it should only ever run once.
 * Therefore, the flownodes will only get values. Any set functions
 * should be handled at design time (rows, tiles, etc.) 
 */

class EnvGenNode_Generator
	: public CFlowBaseNode<eNCT_Instanced>
{
public:
	EnvGenNode_Generator(SActivationInfo*){}
	IFlowNodePtr Clone(SActivationInfo* info) override
	{
		return new EnvGenNode_Generator(info);
	}

	void GetConfiguration(SFlowNodeConfig&) override;
	void ProcessEvent(EFlowEvent, SActivationInfo*) override;
	void GetMemoryUsage(ICrySizer* sizer) const override { sizer->Add(*this); }

private:
	enum InputPorts{
		Get
	};
	enum OutputPorts{
		maxRowSize,
		maxTiles,
		xOffset,
		yOffset,
		Repetition,
		SliceCount
	};
};

class EnvGenNode_TOD_Activation
	: public CFlowBaseNode<eNCT_Instanced>
{
public:
	EnvGenNode_TOD_Activation(SActivationInfo*){}
	IFlowNodePtr Clone(SActivationInfo* info) override
	{
		return new EnvGenNode_Generator(info);
	}

	void GetConfiguration(SFlowNodeConfig&) override;
	void ProcessEvent(EFlowEvent, SActivationInfo*) override;
	void GetMemoryUsage(ICrySizer* sizer) const override { sizer->Add(*this); }

private:
	enum InputPorts{
		CurrentTOD
	};
	enum OutputPorts{
		EntityName,
		Amount
		//Add more later
	};
};

#pragma endregion Node Definitions for Env_TileGenerator