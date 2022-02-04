/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef NPCMINDBLAST1COMMAND_H_
#define NPCMINDBLAST1COMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "ForcePowersQueueCommand.h"

class NpcMindBlast1Command : public ForcePowersQueueCommand {
public:

	NpcMindBlast1Command(const String& name, ZoneProcessServer* server)
		: ForcePowersQueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		// not intended for player use
		if (creature->isPlayerCreature()){
			return GENERALERROR;
		}

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (isWearingArmor(creature)) {
			return NOJEDIARMOR;
		}

		ManagedReference<SceneObject*> targetObject = server->getZoneServer()->getObject(target);

		if (targetObject == nullptr || !targetObject->isCreatureObject()) {
			return INVALIDTARGET;
		}

		return doCombatAction(creature, target);
	}

};

#endif //NPCMINDBLAST1COMMAND_H_
