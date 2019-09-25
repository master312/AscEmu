#pragma once
#include "WorldPacket.h"

namespace BattlegroundPackets
{
    class BattlefieldList
    {
        public:
            BattlefieldList();

            WorldPacket Write() const;

            uint64 BattlemasterGuid;
            uint8 BattlemasterListID;
            uint32 BattlegroundType;
            uint8 MinLevel;
            uint8 MaxLevel;

            bool HasRandomWinToday;
            uint32 RbgHonorPointsForWinning;
            uint32 RbgArenaPointsForWinning;
            uint32 RbgHonorPointsForLosing;
    };
} // namespace BattlegroundPackets
