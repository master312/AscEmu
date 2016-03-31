/*
Copyright (c) 2016 AscEmu Team <http://www.ascemu.org/>
This file is released under the MIT license. See README-MIT for more information.
*/

#include "StdAfx.h"

namespace Movement
{
    UnitMovementManager::UnitMovementManager() : m_spline()
    {

    }

    UnitMovementManager::UnitMovementManager(Spline::MoveSpline pSpline) : m_spline(pSpline)
    {

    }
} 