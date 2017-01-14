# EnvironmentTiles-Gem-only-
Contains only the Environment Tile Gem code, FlowNode code, and exported FlowNode XML   
(This is more updated than the full project version at this point. There may be compile errors. Use with caution :) )   
--1/9/2017

INSTRUCTIONS		
To get this Gem integrated into Lumberyard, do the following:		
		
Part I: Adding Files To Your Project		
--The first step is to clone this repo to your system. Once this is done, your locally cloned folder should have 3 subfolders:
Env_Tile which is the Gem itself, FlowGraphs which has relevant flowgraphs, and FlowNodes which has the C++ definitions for
custom flownodes associated with the Gem.		
		
--Copy the Env_Tile folder into <Lumberyard Root>/1.6.0.0/dev/Gems. Next, copy the FlowGraphs folder into your project's 
directory. So if your project is named "test", the folder would be found at the location <Lumberyard>/1.6.0.0/dev/test.		

--Navigate into the FlowNodes folder and copy all of the C++ header and source files to 
<Lumberyard>/1.6.0.0/dev/Code/Cryengine/CryAction/FlowSystem/Nodes/ComponentEntityNodes		
		
--Next, open up the Lumberyard Project Configurator, click on your project (which from now I will refer to as "test") 
and then click on "Enable Gems". The following Gems can be enabled.		
Amazon Certificate Manager		
Amazon GameLift		
Camera		
Camera Framework		
Cloud Canvas		
Clouds		
Env_Tile (if you do not see this option you may have pasted the Env_Tile folder in the wrong place)		
Game Effect System		
Input Management Framework		
Lightning Arc		
Multiplayer		
Physics Entities		
Rain		
Snow		
Starting Point Camera	
Starting Point Input		
Starting Point Movement		
Tornadoes		
UI Basics		
User Login:Default		
		
Once these Gems are enabled, click save and then close the Project Configurator.		
		
--Next, open up the command prompt and navigate to <Lumberyard>/1.6.0.0/dev and then type in the commmand: lmbr_waf configure.
If you receive errors, you may need to open up the Setup Assistant application to install missing SDKs or Redistributables.
Once the output indicates that the process was successful, close command prompt and open up the Visual Studio solution found
at <Lumberyard>/1.6.0.0/dev/Solutions/LumberyardSDK.sln. If the Solutions folder does not exist, then you need to run the 
lmbr_waf command in the windows command prompt and check the output for errors. Select the build target Profile[All] and then build
the solution by selecting the "Build Solution" option from the build menu or by hitting F7. The initial build process can take
a half hour or more depending on your CPU.		
		
Part II: Setting up the Tiles in the Lumberyard Editor		
