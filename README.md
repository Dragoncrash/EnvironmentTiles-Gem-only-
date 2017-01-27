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
		
--Copy the EnvironmentTileSystem folder into <Lumberyard Root>/1.7.0.0/dev/Gems. Next, copy the FlowGraphs folder into your project's 
directory. So if your project is named "test", the folder would be found at the location <Lumberyard>/1.7.0.0/dev/test.		

--Navigate into the FlowNodes folder and copy all of the C++ header and source files to 
<Lumberyard>/1.6.0.0/dev/Code/Cryengine/CryAction/FlowSystem/Nodes/ComponentEntityNodes		
NOTE: FlowGraph use should be replaced with Lua ScriptBinding [will be added in later]		
		
--Next, open up the Lumberyard Project Configurator, click on your project (which from now I will refer to as "test") 
and then click on "Enable Gems". The following Gems can be enabled.		
Amazon Certificate Manager		
Amazon GameLift		
Camera		
Camera Framework		
Cloud Canvas		
Clouds		
Environment_Tile_System_v2(if you do not see this option you may have pasted the Env_Tile folder in the wrong place)		
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
		
--Also copy the following folders into your test project. They contain a set of textures I already had at my disposal as well as
the sample tile static mesh and associated material.		
		
https://github.com/Dragoncrash/EnvironmentTilesProject/tree/master/Test/Textures		
		
https://github.com/Dragoncrash/EnvironmentTilesProject/tree/master/Test/FBX

		
Part II: Setting up the Tiles in the Lumberyard Editor		
-Once the solution successfully builds, the tiles need to be set up. To do this first open up the editor and load up 
the test level. Next, right click in the editor and create a new component entity. Open up the Entity Inspector view 
and click on the add component button and add in a static mesh component as well as an Env_Tile component. Set the 
static mesh to use the tile model. Set the X, Y, and Z scale values to 10. Change the tile settings as needed (not 
all of them work yet as of 1/14/17).		
		
--When the component entity is set up and configured, right click the entity in the editor and select the option to create
a new slice from the component entity. Next open up the File Browser view and you should see your slice saved in the slices 
folder. Right click the new slice in the File Browser and select the option to set it as a Dynamic Slice. Once the Dynamic
Slice is generated, delete the component entity in the editor as it is no longer needed.		
		
--Next, create another new component entity and add in an Env_Generator component. The settings for the Generator component
are explained below.		
		
Max Row Length: This is the maximum length a single row of the tile grid can be. For this example, set it to 10 so that each
row will consist of 10 tiles.		
		
Max Tiles: This is the overall maximum number of tiles that can be spawned by the generator. Set this value to 60. With this
we now have set the Generator to spawn 6 rows of 10 tiles.		
		
Spawning Method: There are 3 ways to spawn. The first option is Randomized which will randomly select which tile slice to 
spawn out of the slice list. The second option is Ordered which spawns slices in the order in which they were added to the
slice list. Finally there is manual which behaves similar to ordered but takes in the settings of the Local Transform Offset
section into account. Set this to Ordered.		
		
Spawn Repetition: There are two options here. The first is titled "Once", which will limit the number of tiles spawned to the 
number of entries in the slice list. This means that the length of the slice list overrides the Max Tiles setting. The second
option is titled "Repeat" and will allow the generator to loop through the slice list until there are Max Tiles number of tiles
spawned. Set this to Repeat		
		
Constant Offset: This sets the constant X and Y offsets for the tiles. Set both values to 38000.		
		
--Finally, click the little "+" button on the Slice List parameter and add the dynamic slice you created to the list. With this
go into game mode and you should see the environment tiles populating the world. Note that to see all the tiles, you may need to 
adjust the ViewDistance setting. To do this, open up the Rollup Bar, click on the second tab and select Environment. The relevant
setting should be among the first few values shown. Modify the ViewDistance value as needed. At this point, the tiles spawn but 
weather will not work as it has not been set up yet.