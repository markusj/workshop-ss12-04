/*
 * King.cpp
 *
 *  Created on: 15 Jun 2012
 *      Author: Koios
 */




#include <cmath>

#include "boardgame/BoardgameStatusCodes.h"
#include "AllChessFigures.hpp"
#include "boardgame/Board.h"




namespace chess
{
	namespace figure
	{
		King::King(boardgame::Player* b)
			: OrdinaryChessFigure(b)
			, m_canDoCastling(true)
		{}
		King::King(King const& p, NoCastling)
			: OrdinaryChessFigure( p.getPlayer() )
			, m_canDoCastling(false)
		{}
		King::~King()
		{}

		bool King::apply(boardgame::MutableBoardTransaction& move) const
		{
			// first, check if this is a castling
			bool doesCastling = false;
			if(this->canDoCastling())
			{
			    boardgame::Coords const& to = move.getTo();
				int ownX = static_cast<int>(getPosition().getX());
				int ownY = static_cast<int>(getPosition().getY());
				int toX = static_cast<int>(to.getX());
				int toY = static_cast<int>(to.getY());

				if(2 == std::abs(ownY - toY))
				{// tries to do castling
					if( false == this->canMoveTo_checkThreatened(getPosition()) )
					{// currently in check! not allowed to do castling
						move.setStateCode(boardgame::bsc_invalid);
						return false;
					}

					CoordsIncr dir = { (ownX-toX)/std::abs(ownX-toX),
							           (ownY-toY)/std::abs(ownY-toY) };
					// NOTE: the second check is required (check for target field),
					//       as we'll later avoid the moving test since we're jumping over the Rook
					if(   false == this->canMoveTo_checkThreatened(getPosition() + dir)
					   || false == this->canMoveTo_checkThreatened(getPosition() + dir + dir) )
					{// would move to or through a threatened field
						move.setStateCode(boardgame::bsc_invalid);
						return false;
					}


					int rookY = -1;
					if(ownY - toY < 0) { rookY = 0; }
					else { rookY = getBoard()->getDimensions().getY() - 1; }

					boardgame::Coords rookCoords( getPosition().getX(), static_cast<boardgame::CoordsBase_t>(rookY) );
					Figure const* const pFigAtRookCoords = getBoard()->get(rookCoords);
					if(0 == pFigAtRookCoords)
					{// there's no figure
						move.setStateCode(boardgame::bsc_invalid);
						return false;
					}

					Rook const* const pRook = dynamic_cast < Rook const* >(pFigAtRookCoords);
					if(0 == pRook)
					{// it's not a rook
						move.setStateCode(boardgame::bsc_invalid);
						return false;
					}

					if(false == pRook->canDoCastling())
					{// rook has been moved
						move.setStateCode(boardgame::bsc_invalid);
						return false;
					}

					bool const canMoveRook = pRook->castling(move, to);
					if(false == canMoveRook)
					{// someone is in-between
						move.setStateCode(boardgame::bsc_invalid);
						return false;
					}

					// can do castling. finally!
					// (nothing more to do but move)
					doesCastling = true;
				}
			}


			// same code as in Rook but one Rook->King replacement (new King)
			bool canMove = ( doesCastling || this->apply_internal(move) );

			if(canMove && this->canDoCastling())
			{
				// neat line xD
				King newMe(*this, NoCastling());
				move.getBoardOps().push_back( new boardgame::BoardOp::Replace(move.getTo(), &newMe) );
			}

			return canMove;
		}

		bool King::canMoveTo(boardgame::Coords const& to) const
		{
			int ownX = static_cast<int>(getPosition().getX());
			int ownY = static_cast<int>(getPosition().getY());
			int toX = static_cast<int>(to.getX());
			int toY = static_cast<int>(to.getY());

			if((ownX != toX && ownY != toY) || std::abs(ownX - toX) > 1 || std::abs(toX - toY) > 1)
			{
				return false;
			}

			return canMoveTo_checkThreatened(to);
		}

		bool King::canMoveTo_checkThreatened(boardgame::Coords const& to) const
		{
			int ownX = static_cast<int>(getPosition().getX());
			int ownY = static_cast<int>(getPosition().getY());
			int toX = static_cast<int>(to.getX());
			int toY = static_cast<int>(to.getY());

			CoordsIncr incr = { (ownX-toX)/std::abs(ownX-toX),
								(ownY-toY)/std::abs(ownY-toY) };
			bool canMove = canMoveTo_incr(to, incr);
			if(!canMove) { return false; }

			for(boardgame::Coords i(0, 0);
				i.getX() < getBoard()->getDimensions().getX();
				i.setX( i.getX() + 1))
			{
				for(;
					i.getY() < getBoard()->getDimensions().getY();
					i.setY( i.getY() + 1))
				{
					Figure const* const pf = getBoard()->get(i);
					if(0 != pf && pf->getPlayer() != this->getPlayer() && pf->threatens(to))
					{
						return false;
					}
				}
			}

			return true;
		}
	}
}
