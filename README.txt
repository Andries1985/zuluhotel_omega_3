#############################################################################
## Getting started
## Author: Nagash
#############################################################################
Extract ConfigPath.zip (incase of missing config files)

Edit the IP settings in the following file:
	- config/servers.cfg

Edit the path settings in the following files:	
	- scripts/ecompile.cfg
	- pol.cfg
	
Make sure the data folder in the root directory contains the following files (they can be empty):
 - accounts.txt
 - datastore.txt
 - guilds.txt
 - items.txt
 - multi's.txt
 - npcequip.txt
 - npcs.txt
 - objects.txt
 - parties.txt
 - pcequip.txt
 - pcs.txt
 - pol.txt
 - resource.txt
 - storage.txt
 
#############################################################################
## Working with GIT
#############################################################################
Step 01: Log onto the website (atlassian/bitbucket, github etc...) and create a feature/bugfix branch from a develop or master branch
Step 02: Log into your favorite git tool
Step 03: Assuming you have cloned the repository. Run the following command/action: git fetch
Step 04: You will now see the new branch you created on the website
Step 05: Do a "git checkout <branch>" for example: git checkout feature/helloworld
Step 06: Do your modifications and once you're done check the status with the command/action: git status
Step 07: In case of new files you will have to add them to the staging area by running the following command: git add . (don't forget the dot)
Step 08: Before you are ready to commit your changes you will want to check if someone else made any changes. Run the following command/action: git pull
Step 09: You are now ready to commit your modifications. Run the following command/action: git commit -a -m "My commit message"
Step 10: After commiting you will need to do a push to the remote. Run the following command/action: git push
Step 11: Now that your changes are on the remote it's time to do a pull request. Use the website's pull request feature to suggest a merge with your branch as a source and the desired branch as a destination.