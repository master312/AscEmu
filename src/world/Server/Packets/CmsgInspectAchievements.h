/*
Copyright (c) 2014-2018 AscEmu Team <http://www.ascemu.org>
This file is released under the MIT license. See README-MIT for more information.
*/

#pragma once
#include <cstdint>

#include "ManagedPacket.h"
#include "WorldPacket.h"

namespace AscEmu { namespace Packets
{
    class CmsgInspectAchievements : public ManagedPacket
    {
    public:
        WoWGuid guid;

        CmsgInspectAchievements() : CmsgInspectAchievements(0)
        {
        }

        CmsgInspectAchievements(uint64_t guid) :
            ManagedPacket(CMSG_QUERY_INSPECT_ACHIEVEMENTS, 8),
            guid(guid)
        {
        }

        bool internalSerialise(WorldPacket& packet) override
        {
            return false;
        }

        bool internalDeserialise(WorldPacket& packet) override
        {
            guid.Init(packet.unpackGUID());
            return true;
        }
    };
}}