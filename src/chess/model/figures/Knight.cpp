/*
 * Knight.cpp
 *
 *  Created on: 15 Jun 2012
 *      Author: Koios
 */




#include <cassert>
#include <cmath>
#include <cstddef>

#include "AllChessFigures.hpp"
#include "boardgame/Board.h"




namespace chess
{
	namespace figure
	{
		Knight::Knight(boardgame::Player* b)
			: OrdinaryChessFigure(b)
		{}
		Knight::~Knight()
		{}

		bool Knight::canMoveTo(boardgame::Coords const& to) const
		{
			int ownX = static_cast<int>(getPosition().getX());
			int ownY = static_cast<int>(getPosition().getY());
			int toX = static_cast<int>(to.getX());
			int toY = static_cast<int>(to.getY());

			if(   (2 == std::abs(ownX - toX) && 1 == std::abs(ownY - toY))
			   || (1 == std::abs(ownX - toX) && 2 == std::abs(ownY - toY))
			  )
			{// can move
				return (getBoard()->get(to)->getPlayer() != this->getPlayer());
			}else
			{
				return false;
			}
		}
	}
}
