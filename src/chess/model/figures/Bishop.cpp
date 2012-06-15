/*
 * Bishop.cpp
 *
 *  Created on: 15 Jun 2012
 *      Author: Koios
 */




#include <cmath>
#include "AllChessFigures.hpp"




namespace chess
{
	namespace figure
	{
		Bishop::Bishop(boardgame::Player* b)
			: OrdinaryChessFigure(b)
		{}
		Bishop::~Bishop()
		{}

		bool Bishop::canMoveTo(boardgame::Coords const& to) const
		{
			int ownX = static_cast<int>(getPosition().getX());
			int ownY = static_cast<int>(getPosition().getY());
			int toX = static_cast<int>(to.getX());
			int toY = static_cast<int>(to.getY());

			if(std::abs(ownX - toX) != std::abs(ownY - toY))
			{
				return false;
			}


			CoordsIncr incr = { (ownX-toX)/std::abs(ownX-toX),
								(ownY-toY)/std::abs(ownY-toY) };
			return canMoveTo_incr(to, incr);
		}
	}
}
