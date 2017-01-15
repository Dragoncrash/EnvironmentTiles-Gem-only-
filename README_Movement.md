Setting up the Camera		
		
To have a functioning movement system in game mode, do the following:		
1. Create 2 new component entities. Make one the child of the other via the Entity Outliner (click and drag one onto the other)		
2. Add a Camera Component to the child entity.		
3. Create one flowgraph for the parent and one for the child.		
4. Open up the parentmovement flowgraph and copy the nodes into the parent flowgraph. Make sure that the entity value fed into the various Transform nodes is set to
be the child camera entity. You can do this by selecting the child camera entity in the editor, going back to the flow graph,
right clicking on the node which needs the entity id set, and then click the "Assign Selected Entity" option.		
5. Then open up the chlid camera flow graph and the camera flowgraph from the repo. Copy the camera flow graph into the child camera flow graph.		
6. If everything is set up properly, you should now be able to move in game mode.