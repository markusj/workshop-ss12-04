/*
 * ChessFigure.hpp
 *
 *  Created on: 15 Jun 2012
 *      Author: Koios
 */

#ifndef CHESSFIGURE_HPP_
#define CHESSFIGURE_HPP_




#include <boardgame/Figure.h>
#include <boardgame/Coords.h>




namespace chess
{
	struct CoordsIncr
	{
		int x;
		int y;
	};
	boardgame::Coords operator+ (boardgame::Coords const& p0, CoordsIncr const& p1);


	class ChessFigure
		: public boardgame::AbstractFigure
	{
	public:
		ChessFigure(boardgame::Player* const p)
			: AbstractFigure(p)
		{}

	protected:
		bool moveCapture(boardgame::MutableBoardTransaction& move) const;
	};


	class OrdinaryChessFigure
		: public ChessFigure
	{
	public:
		OrdinaryChessFigure(boardgame::Player* const p)
			: ChessFigure(p)
		{}

		virtual bool apply(boardgame::MutableBoardTransaction& move) const
		{
			return this->apply_internal(move);
		}
		virtual bool threatens(boardgame::Coords const& pos) const
		{
			return this->canMoveTo(pos);
		}

	protected:
		bool apply_internal(boardgame::MutableBoardTransaction& move) const;
		bool canMoveTo_incr(boardgame::Coords const& to, CoordsIncr const& incr) const;
	};
}




#endif /* CHESSFIGURE_HPP_ */
