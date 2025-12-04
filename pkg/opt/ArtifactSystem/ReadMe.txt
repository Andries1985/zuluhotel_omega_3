Renamed to Artifacts by Wintermute for ZHO
Originally Unique System by Clemens

Description

The system works like this:
There is a global bankbox, which can be opened with .openartifact
In this bankbox you can store items that you wish to distribute as artifacts.
If a unique item decays or is removed then it will return to the uniquebox and be "recycled" if you wish to destroy an artifact item completely you will have to use the .destroyartifact on that specific item.
You can also make an abitrary item unique by using the .makeartifact command. Then it will be treated as an artifact item, and can be added to the box, if not added, it will be moved to the uniquebox on destruction.
The way players can get these unique items is getting the uniquebag, described below, as loot from monsters or what ever.

To make an item unique from a script, you will have to add to the item:

CProp artifact i1
DestroyScript	::maindestroy


Installation:
On your maindestroy script, add the following:

	// send artifacts back to their box
	if (GetObjProperty(item,"Artifact"))
		toArtifactBox(item);
		return 0;
	endif

You can then add the artifactbox item described in the package onto your loot tables either as it's own lootgroup, or onto other item groups, such as GMItems

When the box is created and doubleclicked, it moves a random item from the artifact container to the players backpack.

There is currently a limitation with POL where you cannot change an item's DestroyScript, so any item without one will not be recovered if destroyed!

Now just compile the new and edited .src files and you are ready to go.

Have fun,
Wintermute

Original author:
Clemens
Mail: cnk@mail1.stofanet.dk
ICQ: 4409233


