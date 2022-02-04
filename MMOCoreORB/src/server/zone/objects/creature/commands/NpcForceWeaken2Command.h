/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef NPCFORCEWEAKEN2COMMAND_H_
#define NPCFORCEWEAKEN2COMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "ForcePowersQueueCommand.h"

class NpcForceWeaken2Command : public ForcePowersQueueCommand {
public:

	NpcForceWeaken2Command(const String& name, ZoneProcessServer* server)
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

		int res = doCombatAction(creature, target);

		if (res == SUCCESS) {

			// Setup debuff.

			ManagedReference<CreatureObject*> creatureTarget = targetObject.castTo<CreatureObject*>();

			if (creatureTarget != nullptr) {
				Locker clocker(creatureTarget, creature);

				ManagedReference<Buff*> buff = new Buff(creatureTarget, getNameCRC(), 120, BuffType::JEDI);

				Locker locker(buff);

				buff->setAttributeModifier(CreatureAttribute::HEALTH, -600);
				buff->setAttributeModifier(CreatureAttribute::ACTION, -600);
				buff->setAttributeModifier(CreatureAttribute::MIND, -600);

				creatureTarget->addBuff(buff);

				CombatManager::instance()->broadcastCombatSpam(creature, creatureTarget, nullptr, 0, "cbt_spam", combatSpam + "_hit", 1);
			}

		}

		return res;
	}

};

#endif //NPCFORCEWEAKEN2COMMAND_H_
