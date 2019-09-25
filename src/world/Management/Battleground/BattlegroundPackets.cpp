#include "BattlegroundPackets.h"
#include "BattlegroundInfo.h"

namespace BattlegroundPackets {

    BattlefieldList::BattlefieldList()
    {
        BattlemasterGuid = 0;
        BattlemasterListID = 0;
        BattlegroundType = 0;
        MinLevel = 0;
        MaxLevel = 0;
        HasRandomWinToday = false;
        RbgHonorPointsForWinning = 0;
        RbgArenaPointsForWinning = 0;
        RbgHonorPointsForLosing = 0;
    }

    WorldPacket BattlefieldList::Write() const
    {
        WorldPacket data(SMSG_BATTLEFIELD_LIST, 18);

        data << BattlemasterGuid;   // m_session->GetPlayer()->getGuid()
        data << BattlemasterListID; // uint8(0);           // from ?
        data << BattlegroundType;

        data << uint8(0);           // unk
        data << uint8(0);           // unk

        // Rewards
        data << uint8(0);           // 3.3.3 hasWin
        data << uint32(0);          // 3.3.3 winHonor
        data << uint32(0);          // 3.3.3 winArena
        data << uint32(0);          // 3.3.3 lossHonor

        uint8 isRandom = BattlegroundType == BATTLEGROUND_RANDOM;
        data << isRandom;           // 3.3.3 isRandom
        
        if (isRandom == 1)
        {
            // rewards
            data << uint8(HasRandomWinToday);
            data << RbgHonorPointsForWinning;
            data << RbgArenaPointsForWinning;
            data << RbgHonorPointsForLosing;
        }

        data << uint32(0);

        return data;
    }
}