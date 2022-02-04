
#ifndef PLAYERBOUNTYTASK_H_
#define PLAYERBOUNTYTASK_H_

#include "server/zone/managers/mission/MissionManager.h"
#include "server/zone/objects/player/PlayerObject.h"


namespace server {
namespace zone {
namespace objects {
namespace player {
namespace events {

class PlayerBountyTask: public Task {
	ManagedWeakReference<CreatureObject*> creature;

public:
	PlayerBountyTask(CreatureObject* creo) {
		creature = creo;
	}

	void run() {
		ManagedReference<CreatureObject*> player = creature.get();

		if (player == nullptr)
			return;

		ManagedReference<PlayerObject*> ghost = player->getPlayerObject().get();

		if (ghost == nullptr)
			return;

		ManagedReference<ZoneServer*> zoneServer = player->getZoneServer();

		if (zoneServer == nullptr)
			return;

		ManagedReference<CreatureObject*> bountyPlacer = zoneServer->getObject(ghost->getBountyPlacerId()).castTo<CreatureObject*>();

		if (bountyPlacer == nullptr)
			return;

		MissionManager* missionManager = player->getZoneServer()->getMissionManager();

		if (missionManager == nullptr)
			return;

		Locker locker(player);

		uint64 playerId = player->getObjectID();
		int reward = ghost->getBountyReward();
		bool isJedi = player->hasSkill("force_title_jedi_rank_02");
		bool online = false;

		if (ghost->hasPlayerBounty()) {
			if (!missionManager->hasPlayerBountyTargetInList(playerId)) {
				missionManager->addPlayerToBountyList(playerId, reward);
			} else {
				if (ghost->isOnline())
					online = true;

				missionManager->updatePlayerBountyReward(playerId, reward);
				missionManager->updatePlayerBountyOnlineStatus(playerId, online);
			}
			this->reschedule(llabs(ghost->getPlayerBountyTaskTimestamp().miliDifference()));
		} else if (missionManager->hasPlayerBountyTargetInList(playerId) && reward > 0) {
			missionManager->removePlayerFromBountyList(playerId);
			ghost->refundPlayerBountyCredits();
		}
	}
};

}
}
}
}
}

using namespace server::zone::objects::player::events;

#endif /* PLAYERBOUNTYTASK_H_ */
