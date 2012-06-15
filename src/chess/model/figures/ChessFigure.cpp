/*
 * ChessFigure.cpp
 *
 *  Created on: 15 Jun 2012
 *      Author: Koios
 */




#include "ChessFigure.hpp"
#include "boardgame/BoardgameStatusCodes.h"
#include "boardgame/BoardTransaction.h"




namespace chess
{
    boardgame::Coords operator+ (boardgame::Coords const& p0, CoordsIncr const& p1)
	{
		return boardgame::Coords(p0.getX() + p1.x, p0.getY() + p1.y);
	}


	bool ChessFigure::moveCapture(boardgame::MutableBoardTransaction& move) const
	{
	    boardgame::Coords const& to = move.getTo();

	    boardgame::BoardOpVec& boardOps = move.getBoardOps();

		if( NULL != getBoard()->get(to) )
		{
		    boardgame::BoardOp::Operation* pReplace = new boardgame::BoardOp::Replace(to, NULL);
			boardOps.push_back(pReplace);
		}

		boardgame::BoardOp::Operation* pMove = new boardgame::BoardOp::Move(this->getPosition(), to);
		boardOps.push_back(pMove);

		return true;
	}


	bool OrdinaryChessFigure::apply_internal(boardgame::MutableBoardTransaction& move) const
	{
	    boardgame::Coords const& to = move.getTo();

		if( false == this->canMoveTo(to) )
		{
			move.setStateCode(boardgame::bsc_invalid);
			return false;
		}

		return this->moveCapture(move);
	}

	bool OrdinaryChessFigure::canMoveTo_incr(boardgame::Coords const& to, CoordsIncr const& incr) const
	{
		for(boardgame::Coords i = this->getPosition();
			i != to;
			i = i + incr)
		{
			boardgame::Figure const* const pf = getBoard()->get(i);

			if(NULL == pf) { continue; }	// field empty
			if(pf->getPlayer() == this->getPlayer()) { return false; }	// field occupied by figure owned by own player
			if(i != to) { return false; } // this is an enemy figure blocking our way
		}

		return true;
	}
}
