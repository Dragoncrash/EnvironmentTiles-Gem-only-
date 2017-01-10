#include "StdAfx.h"
#include "EnvTileNode.h"
#include "../../../Gems/Env_Tile/Code/Include/Env_Tile/Env_TileBus.h"
#include "../../../Gems/Env_Tile/Code/Include/Env_Tile/WeatherStructs.h"

#pragma region Env_TileSystemComponent
//Simple Weather Enable
void EnvTileNode_SimpleEnables::GetConfiguration(SFlowNodeConfig& config){
	static const SInputPortConfig in_config[] = {
		InputPortConfig_Void("Get", _HELP("Get the values and enable node outputs")),
		InputPortConfig_Void("Set", _HELP("Set the values and then return them via node outputs")),
		InputPortConfig<bool>("Cloudy", _HELP("Enable Cloudy Weather as an option for randomized weather.")),
		InputPortConfig<bool>("Windy", _HELP("Enable Cloudy Weather as an option for randomized weather.")),
		{0}
	};

	static const SOutputPortConfig out_config[] = {
		OutputPortConfig<bool>("Cloudy",_HELP("The current value of Cloudy.")),
		OutputPortConfig<bool>("Windy", _HELP("The current value of Windy")),
		{0}
	};

	config.sDescription = _HELP("Gets or Sets Simple Weather parameters for the Environment Tile. If both Get and Set inputs are active, Get is ignored and Set alone is evaluated.");
	config.nFlags |= EFLN_TARGET_ENTITY;
	config.pInputPorts = in_config;
	config.pOutputPorts = out_config;
	config.SetCategory(EFLN_APPROVED);
}

void EnvTileNode_SimpleEnables::ProcessEvent(EFlowEvent event, SActivationInfo* info){
	FlowEntityType fet = GetFlowEntityTypeFromFlowEntityId(info->entityId);
	AZ_Assert((fet == FlowEntityType::Component) || (fet == FlowEntityType::Invalid),
		"This node is incompatible with Legacy Entities");
	if (fet == FlowEntityType::Component){
		switch (event){
		case IFlowNode::eFE_Initialize:
			break;
		case IFlowNode::eFE_Activate:
			int result;
			if (IsPortActive(info, InputPorts::Set)){
				EBUS_EVENT_ID_RESULT(result, info->entityId, Env_Tile::Env_TileRequestBus, setSimple, GetPortBool(info,InputPorts::in_Cloudy),GetPortBool(info,InputPorts::in_Windy));
			}
			else if (IsPortActive(info, InputPorts::Get)){
				EBUS_EVENT_ID_RESULT(result, info->entityId, Env_Tile::Env_TileRequestBus, getSimple);
			}

			switch (result){
			case 0: 
				break;
			case 1:
				ActivateOutput(info, OutputPorts::out_Cloudy, true);
				ActivateOutput(info, OutputPorts::out_Windy, false);
				break;
			case 2:
				ActivateOutput(info, OutputPorts::out_Cloudy, false);
				ActivateOutput(info, OutputPorts::out_Windy, true);
				break;
			case 3:
				ActivateOutput(info, OutputPorts::out_Cloudy, true);
				ActivateOutput(info, OutputPorts::out_Windy, true);
				break;
			default:
				CryLog("Node: Error processing simple weather integer!");
			}
			break;
		}
	}
}

//Rain
void EnvTileNode_Rain::GetConfiguration(SFlowNodeConfig& config){
	static const SInputPortConfig in_config[] = {
		InputPortConfig_Void("Get", _HELP("Get the values and enable node outputs")),
		InputPortConfig_Void("Set", _HELP("Set the values and then return them via node outputs")),
		InputPortConfig<bool>("Light Rain", _HELP("Allow light rain (drizzle)")),
		InputPortConfig<bool>("Regular Showers", _HELP("Allow regular rain showers")),
		InputPortConfig<bool>("Heavy Rain", _HELP("Allow heavy rain (storm)")),
		InputPortConfig<int>("TOD Start", _HELP("Time of day at which rain will be possible.")),
		InputPortConfig<int>("TOD Stop", _HELP("Time of day at which rain will no longer be possible.")),
		{ 0 }
	};

	static const SOutputPortConfig out_config[] = {
		OutputPortConfig<bool>("Light Rain", _HELP("Allows light rain if enabled.")),
		OutputPortConfig<bool>("Regular Showers", _HELP("Allows regular rain showers if enabled.")),
		OutputPortConfig<bool>("Heavy Rain", _HELP("Allows heavy storm showers if enabled.")),
		OutputPortConfig<int>("TOD Start", _HELP("The time to start considering rain weather.")),
		OutputPortConfig<int>("TOD Stop", _HELP("The time to stop considering rain weather.")),
		{ 0 }
	};

	config.sDescription = _HELP("Gets or Sets Rain Weather parameters for the Environment Tile. If both Get and Set inputs are active, Get is ignored and Set alone is evaluated.");
	config.nFlags |= EFLN_TARGET_ENTITY;
	config.pInputPorts = in_config;
	config.pOutputPorts = out_config;
	config.SetCategory(EFLN_APPROVED);
}

void EnvTileNode_Rain::ProcessEvent(EFlowEvent event, SActivationInfo* info){
	FlowEntityType fet = GetFlowEntityTypeFromFlowEntityId(info->entityId);
	AZ_Assert((fet == FlowEntityType::Component) || (fet == FlowEntityType::Invalid),
		"This node is incompatible with Legacy Entities");
	if (fet == FlowEntityType::Component){
		switch (event){
		case IFlowNode::eFE_Initialize:
			break;
		case IFlowNode::eFE_Activate:
			int intensity, todstart, todstop;
			if (IsPortActive(info, InputPorts::Set)){
				EBUS_EVENT_ID_RESULT(intensity, info->entityId, Env_Tile::Env_TileRequestBus, setRainStrengths, GetPortBool(info, InputPorts::in_Light), GetPortBool(info, InputPorts::in_Medium), GetPortBool(info, InputPorts::in_Heavy));
				EBUS_EVENT_ID_RESULT(todstart, info->entityId, Env_Tile::Env_TileRequestBus, setRainTODStart, GetPortInt(info, InputPorts::in_TOD_Start));
				EBUS_EVENT_ID_RESULT(todstop, info->entityId, Env_Tile::Env_TileRequestBus, setRainTODStop, GetPortInt(info, InputPorts::in_TOD_End));
			}
			else if (IsPortActive(info, InputPorts::Get)){
				EBUS_EVENT_ID_RESULT(intensity, info->entityId, Env_Tile::Env_TileRequestBus, getRainStrengths);
				EBUS_EVENT_ID_RESULT(todstart, info->entityId, Env_Tile::Env_TileRequestBus, getRainTODStart);
				EBUS_EVENT_ID_RESULT(todstop, info->entityId, Env_Tile::Env_TileRequestBus, getRainTODStop);
			}

			//Intensity bitmask extraction [TEST]
			ActivateOutput(info, OutputPorts::out_Light, intensity & 1);
			ActivateOutput(info, OutputPorts::out_Medium, intensity & 2);
			ActivateOutput(info, OutputPorts::out_Heavy, intensity & 4);

			//TOD
			ActivateOutput(info, OutputPorts::out_TOD_Start, todstart);
			ActivateOutput(info, OutputPorts::out_TOD_End, todstop);

			break;
		}
	}
}

//Snow
void EnvTileNode_Snow::GetConfiguration(SFlowNodeConfig& config){
	static const SInputPortConfig in_config[] = {
		InputPortConfig_Void("Get", _HELP("Get the values and enable node outputs")),
		InputPortConfig_Void("Set", _HELP("Set the values and then return them via node outputs")),
		InputPortConfig<bool>("Light Snow", _HELP("Allow light snowfall")),
		InputPortConfig<bool>("Regular Snow", _HELP("Allow regular snowfall")),
		InputPortConfig<bool>("Heavy Snow", _HELP("Allow heavy snowfall (blizzard)")),
		InputPortConfig<int>("TOD Start", _HELP("Time of day at which snow will be possible.")),
		InputPortConfig<int>("TOD Stop", _HELP("Time of day at which snow will no longer be possible.")),
		{ 0 }
	};

	static const SOutputPortConfig out_config[] = {
		OutputPortConfig<bool>("Light Snow", _HELP("Allows light snowfall if enabled.")),
		OutputPortConfig<bool>("Regular Snow", _HELP("Allows regular snowfall if enabled.")),
		OutputPortConfig<bool>("Heavy Snow", _HELP("Allows blizzard level snowfall if enabled.")),
		OutputPortConfig<int>("TOD Start", _HELP("The time to start considering snow weather.")),
		OutputPortConfig<int>("TOD Stop", _HELP("The time to stop considering snow weather.")),
		{ 0 }
	};

	config.sDescription = _HELP("Gets or Sets Snow Weather parameters for the Environment Tile. If both Get and Set inputs are active, Get is ignored and Set alone is evaluated.");
	config.nFlags |= EFLN_TARGET_ENTITY;
	config.pInputPorts = in_config;
	config.pOutputPorts = out_config;
	config.SetCategory(EFLN_APPROVED);
}

void EnvTileNode_Snow::ProcessEvent(EFlowEvent event, SActivationInfo* info){
	FlowEntityType fet = GetFlowEntityTypeFromFlowEntityId(info->entityId);
	AZ_Assert((fet == FlowEntityType::Component) || (fet == FlowEntityType::Invalid),
		"This node is incompatible with Legacy Entities");
	if (fet == FlowEntityType::Component){
		switch (event){
		case IFlowNode::eFE_Initialize:
			break;
		case IFlowNode::eFE_Activate:
			int intensity, todstart, todstop;
			if (IsPortActive(info, InputPorts::Set)){
				EBUS_EVENT_ID_RESULT(intensity, info->entityId, Env_Tile::Env_TileRequestBus, setSnowStrengths, GetPortBool(info, InputPorts::in_Light), GetPortBool(info, InputPorts::in_Medium), GetPortBool(info, InputPorts::in_Heavy));
				EBUS_EVENT_ID_RESULT(todstart, info->entityId, Env_Tile::Env_TileRequestBus, setSnowTODStart, GetPortInt(info, InputPorts::in_TOD_Start));
				EBUS_EVENT_ID_RESULT(todstop, info->entityId, Env_Tile::Env_TileRequestBus, setSnowTODStop, GetPortInt(info, InputPorts::in_TOD_End));
			}
			else if (IsPortActive(info, InputPorts::Get)){
				EBUS_EVENT_ID_RESULT(intensity, info->entityId, Env_Tile::Env_TileRequestBus, getSnowStrengths);
				EBUS_EVENT_ID_RESULT(todstart, info->entityId, Env_Tile::Env_TileRequestBus, getSnowTODStart);
				EBUS_EVENT_ID_RESULT(todstop, info->entityId, Env_Tile::Env_TileRequestBus, getSnowTODStop);
			}

			//Intensity bitmask extraction [TEST]
			ActivateOutput(info, OutputPorts::out_Light, intensity & 1);
			ActivateOutput(info, OutputPorts::out_Medium, intensity & 2);
			ActivateOutput(info, OutputPorts::out_Heavy, intensity & 4);

			//TOD
			ActivateOutput(info, OutputPorts::out_TOD_Start, todstart);
			ActivateOutput(info, OutputPorts::out_TOD_End, todstop);

			break;
		}
	}
}

//Rain-Extra
void EnvTileNode_RainExtra::GetConfiguration(SFlowNodeConfig& config){
	static const SInputPortConfig in_config[] = {
		InputPortConfig_Void("Get", _HELP("Get the values and enable node outputs")),
		InputPortConfig_Void("Set", _HELP("Set the values and then return them via node outputs")),
		InputPortConfig<bool>("Thunder", _HELP("Enable the possibility of Thunder during Rain.")),
		InputPortConfig<bool>("Lightning", _HELP("Enable the possibility of Lightning during Rain")),
		{ 0 }
	};

	static const SOutputPortConfig out_config[] = {
		OutputPortConfig<bool>("Thunder", _HELP("The current value of Thunder.")),
		OutputPortConfig<bool>("Lightning", _HELP("The current value of Lightning")),
		{ 0 }
	};

	config.sDescription = _HELP("Gets or Sets the extra Rain parameters for the Environment Tile. If both Get and Set inputs are active, Get is ignored and Set alone is evaluated.");
	config.nFlags |= EFLN_TARGET_ENTITY;
	config.pInputPorts = in_config;
	config.pOutputPorts = out_config;
	config.SetCategory(EFLN_APPROVED);
}

void EnvTileNode_RainExtra::ProcessEvent(EFlowEvent event, SActivationInfo* info){
	FlowEntityType fet = GetFlowEntityTypeFromFlowEntityId(info->entityId);
	AZ_Assert((fet == FlowEntityType::Component) || (fet == FlowEntityType::Invalid),
		"This node is incompatible with Legacy Entities");
	if (fet == FlowEntityType::Component){
		switch (event){
		case IFlowNode::eFE_Initialize:
			break;
		case IFlowNode::eFE_Activate:
			int result;
			if (IsPortActive(info, InputPorts::Set)){
				EBUS_EVENT_ID_RESULT(result, info->entityId, Env_Tile::Env_TileRequestBus, setRainExtra, GetPortBool(info, InputPorts::in_Thunder), GetPortBool(info, InputPorts::in_Lightning));
			}
			else if (IsPortActive(info, InputPorts::Get)){
				EBUS_EVENT_ID_RESULT(result, info->entityId, Env_Tile::Env_TileRequestBus, getRainExtra);
			}

			ActivateOutput(info, OutputPorts::out_Thunder, result & 1);
			ActivateOutput(info, OutputPorts::out_Lightning, result & 2);

			break;
		}
	}
}

//Snow-Extra
void EnvTileNode_SnowExtra::GetConfiguration(SFlowNodeConfig& config){
	static const SInputPortConfig in_config[] = {
		InputPortConfig_Void("Get", _HELP("Get the values and enable node outputs")),
		InputPortConfig_Void("Set", _HELP("Set the values and then return them via node outputs")),
		InputPortConfig<bool>("Freeze", _HELP("Enable the possibility of the ground freezing during Snow. (Currently does not work with static meshes)")),
		InputPortConfig<float>("Frost Amount", _HELP("The amount of freezing to apply to the terrain if freezing is enabled. (Currently does not work with static meshes)")),
		{ 0 }
	};

	static const SOutputPortConfig out_config[] = {
		OutputPortConfig<bool>("Freeze", _HELP("The current value of Freeze.")),
		OutputPortConfig<float>("Frost Amount", _HELP("The current amount of frost.")),
		{ 0 }
	};

	config.sDescription = _HELP("Gets or Sets the extra Snow parameters for the Environment Tile. If both Get and Set inputs are active, Get is ignored and Set alone is evaluated.");
	config.nFlags |= EFLN_TARGET_ENTITY;
	config.pInputPorts = in_config;
	config.pOutputPorts = out_config;
	config.SetCategory(EFLN_APPROVED);
}

void EnvTileNode_SnowExtra::ProcessEvent(EFlowEvent event, SActivationInfo* info){
	FlowEntityType fet = GetFlowEntityTypeFromFlowEntityId(info->entityId);
	AZ_Assert((fet == FlowEntityType::Component) || (fet == FlowEntityType::Invalid),
		"This node is incompatible with Legacy Entities");
	if (fet == FlowEntityType::Component){
		switch (event){
		case IFlowNode::eFE_Initialize:
			break;
		case IFlowNode::eFE_Activate:
			bool freeze;
			float amount;
			if (IsPortActive(info, InputPorts::Set)){
				EBUS_EVENT_ID_RESULT(freeze, info->entityId, Env_Tile::Env_TileRequestBus, setSnowFreezeGround, GetPortBool(info, InputPorts::in_FreezeGround));
				EBUS_EVENT_ID_RESULT(amount, info->entityId, Env_Tile::Env_TileRequestBus, setSnowFreezeAmount, GetPortFloat(info, InputPorts::in_FreezeAmount));
			}
			else if (IsPortActive(info, InputPorts::Get)){
				EBUS_EVENT_ID_RESULT(freeze, info->entityId, Env_Tile::Env_TileRequestBus, getSnowFreezeGround);
				EBUS_EVENT_ID_RESULT(amount, info->entityId, Env_Tile::Env_TileRequestBus, getSnowFreezeAmount);
			}

			ActivateOutput(info, OutputPorts::out_FreezeGround, freeze);
			ActivateOutput(info, OutputPorts::out_FreezeAmount, amount);

			break;
		}
	}
}

#pragma endregion Node Definitions for Env_TileSystemComponent

#pragma region Env_TileGenerator
void EnvGenNode_Generator::GetConfiguration(SFlowNodeConfig& config){
	static const SInputPortConfig in_config[] = {
		InputPortConfig_Void("Get", _HELP("Get the current generator values.")),
		{ 0 }
	};

	static const SOutputPortConfig out_config[] = {
		OutputPortConfig<int>("Row Length", _HELP("Number of Tiles per row.")),
		OutputPortConfig<int>("Max Instances", _HELP("Maximum number of tiles that can spawn. Only used if repetition is true.")),
		OutputPortConfig<float>("xOffset", _HELP("Grid spawning offset on the World-X axis.")),
		OutputPortConfig<float>("yOffset", _HELP("Grid spawning offset on the World-Y axis.")),
		OutputPortConfig<bool>("Repetition", _HELP("True if using Repeat mode.")),
		OutputPortConfig<int>("Slice Count", _HELP("Number of slices in Slice List")),
		{ 0 }
	};

	config.sDescription = _HELP("Retrieves the Generator properties for the specified generator.");
	config.nFlags |= EFLN_TARGET_ENTITY;
	config.pInputPorts = in_config;
	config.pOutputPorts = out_config;
	config.SetCategory(EFLN_APPROVED);
}

void EnvGenNode_Generator::ProcessEvent(EFlowEvent event, SActivationInfo* info){
	FlowEntityType fet = GetFlowEntityTypeFromFlowEntityId(info->entityId);
	AZ_Assert((fet == FlowEntityType::Component) || (fet == FlowEntityType::Invalid),
		"This node is incompatible with Legacy Entities");
	if (fet == FlowEntityType::Component){
		info->pGraph->SetRegularlyUpdated(info->myID, true);
		switch (event){
		case IFlowNode::eFE_Initialize:
			break;
		case IFlowNode::eFE_Activate:
			if (IsPortActive(info, InputPorts::Get)){
				int rowLen, maxInstances, xoff, yoff, sliceNum;
				bool repeat;

				EBUS_EVENT_ID_RESULT(rowLen, info->entityId, Env_Tile::Env_GeneratorRequestBus, getRowSize);
				EBUS_EVENT_ID_RESULT(maxInstances, info->entityId, Env_Tile::Env_GeneratorRequestBus, getMaxTiles);
				EBUS_EVENT_ID_RESULT(xoff, info->entityId, Env_Tile::Env_GeneratorRequestBus, getXOffset);
				EBUS_EVENT_ID_RESULT(yoff, info->entityId, Env_Tile::Env_GeneratorRequestBus, getYOffset);
				EBUS_EVENT_ID_RESULT(repeat, info->entityId, Env_Tile::Env_GeneratorRequestBus, getRepetition);
				EBUS_EVENT_ID_RESULT(sliceNum, info->entityId, Env_Tile::Env_GeneratorRequestBus, getSliceCount);

				ActivateOutput(info, OutputPorts::maxRowSize, rowLen);
				ActivateOutput(info, OutputPorts::maxTiles, maxInstances);
				ActivateOutput(info, OutputPorts::xOffset, xoff);
				ActivateOutput(info, OutputPorts::yOffset, yoff);
				ActivateOutput(info, OutputPorts::Repetition, repeat);
				ActivateOutput(info, OutputPorts::SliceCount, sliceNum);

			}
			break;
		}
	}
}

void EnvGenNode_TOD_Activation::GetConfiguration(SFlowNodeConfig& config){
	static const SInputPortConfig in_config[] = {
		InputPortConfig<float>("Current TOD", _HELP("The current Time of Day")),
		{ 0 }
	};

	static const SOutputPortConfig out_config[] = {
		OutputPortConfig<string>("Entity Name", "The Name of the Weather Entity to modify."),
		OutputPortConfig<float>("Amount", "Global Amount of Weather Effect to apply."),
		{ 0 }
	};

	config.sDescription = _HELP("");
	config.nFlags |= EFLN_TARGET_ENTITY;
	config.pInputPorts = in_config;
	config.pOutputPorts = out_config;
	config.SetCategory(EFLN_APPROVED);
}

void EnvGenNode_TOD_Activation::ProcessEvent(EFlowEvent event, SActivationInfo* info){
	FlowEntityType fet = GetFlowEntityTypeFromFlowEntityId(info->entityId);
	AZ_Assert((fet == FlowEntityType::Component) || (fet == FlowEntityType::Invalid),
		"This node is incompatible with Legacy Entities");
	if (fet == FlowEntityType::Component){
		info->pGraph->SetRegularlyUpdated(info->myID, true);//Check Node output to see if it fires constantly
		switch (event){
		case IFlowNode::eFE_Initialize:
			break;
		case IFlowNode::eFE_Update:
			float tod = GetPortFloat(info, InputPorts::CurrentTOD);
			float integralPart;
			
			//Preload Check
			if (modf(tod, &integralPart) == 0.0){
				//If the decimal portion of the float is zero, trigger the appropriate tile weather systems
				int index = (int)tod;

				//These should never actually be necessary
				if (index < 0) index = 0;
				if (index > 24) index = 24;
				
				EBUS_EVENT_ID(info->entityId, Env_Tile::Env_GeneratorRequestBus, preloadTriggersAtTime, index);
			}

			//Process Triggers
			bool queue_empty;
			EBUS_EVENT_ID_RESULT(queue_empty, info->entityId, Env_Tile::Env_GeneratorRequestBus, triggerListEmpty);

			if (!queue_empty){
				Env_Tile::WeatherUnit w;
				EBUS_EVENT_ID_RESULT(w, info->entityId, Env_Tile::Env_GeneratorRequestBus, processNextTrigger);

				//Decode struct

			}


			break;
		}
	}
}
#pragma endregion Node Definitions for Env_TileGenerator


//REGISTER FLOW NODES
REGISTER_FLOW_NODE("ComponentEntity:Env_Tile:SimpleWeather", EnvTileNode_SimpleEnables)
REGISTER_FLOW_NODE("ComponentEntity:Env_Tile:Rain:Default", EnvTileNode_Rain)
REGISTER_FLOW_NODE("ComponentEntity:Env_Tile:Snow:Default", EnvTileNode_Snow)
REGISTER_FLOW_NODE("ComponentEntity:Env_Tile:Rain:Extra", EnvTileNode_RainExtra)
REGISTER_FLOW_NODE("ComponentEntity:Env_Tile:Snow:Extra", EnvTileNode_SnowExtra)
REGISTER_FLOW_NODE("ComponentEntity:Generator:GeneratorParams", EnvGenNode_Generator)