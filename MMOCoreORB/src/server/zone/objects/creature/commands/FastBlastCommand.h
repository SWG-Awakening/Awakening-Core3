/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef FASTBLASTCOMMAND_H_
#define FASTBLASTCOMMAND_H_

#include "CombatQueueCommand.h"
#include "server/zone/objects/tangible/weapon/WeaponObject.h"

class FastBlastCommand : public CombatQueueCommand {
public:

	FastBlastCommand(const String& name, ZoneProcessServer* server)
		: CombatQueueCommand(name, server) {
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

			if (!checkStateMask(creature))
				return INVALIDSTATE;

			if (!checkInvalidLocomotions(creature))
				return INVALIDLOCOMOTION;

			ManagedReference<WeaponObject*> weapon = creature->getWeapon();
			if (weapon != nullptr && weapon->isHeavyWeapon())
				return INVALIDWEAPON;

			UnicodeString args = "healthDamageMultiplier=0.33f;actionDamageMultiplier=0.33f;mindDamageMultiplier=0.33f;";

			return doCombatAction(creature, target, args);
		}

	};

#endif //FASTBLASTCOMMAND_H_
