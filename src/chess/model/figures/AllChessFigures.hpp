/*
 * Bishop.hpp
 *
 *  Created on: 15 Jun 2012
 *      Author: Koios
 */

#ifndef BISHOP_HPP_
#define BISHOP_HPP_




#include <boardgame/Figure.h>
#include "ChessFigure.hpp"




namespace chess
{
	namespace figure
	{
		class King
			: public OrdinaryChessFigure
		{
		public:
			King(boardgame::Player* b);
			virtual boardgame::Figure* clone() const { return new King(*this); }
			virtual ~King();

			virtual bool canMoveTo(boardgame::Coords const& to) const;
			virtual bool apply(boardgame::MutableBoardTransaction& move) const;

			bool canDoCastling() const { return m_canDoCastling; }

		private:
			bool canMoveTo_checkThreatened(boardgame::Coords const& to) const;

			struct NoCastling{};
			King(King const&, NoCastling);

			bool const m_canDoCastling;
		};


		class Queen
			: public OrdinaryChessFigure
		{
		public:
			Queen(boardgame::Player* b);
			virtual boardgame::Figure* clone() const { return new Queen(*this); }
			virtual ~Queen();

			virtual bool canMoveTo(boardgame::Coords const& to) const;
		};


		class Bishop
			: public OrdinaryChessFigure
		{
		public:
			Bishop(boardgame::Player* b);
			virtual boardgame::Figure* clone() const { return new Bishop(*this); }
			virtual ~Bishop();

			virtual bool canMoveTo(boardgame::Coords const& to) const;
		};


		class Rook
			: public OrdinaryChessFigure
		{
		public:
			Rook(boardgame::Player* b);
			virtual boardgame::Figure* clone() const { return new Rook(*this); }
			virtual ~Rook();

			virtual bool canMoveTo(boardgame::Coords const& to) const;
			virtual bool apply(boardgame::MutableBoardTransaction& move) const;

			bool canDoCastling() const { return m_canDoCastling; }

		private:
			friend class King;
			bool castling(boardgame::MutableBoardTransaction& move, boardgame::Coords const& target) const;

			struct NoCastling{};
			Rook(Rook const&, NoCastling);

			bool const m_canDoCastling;
		};


		class Knight
			: public OrdinaryChessFigure
		{
		public:
			Knight(boardgame::Player* b);
			virtual boardgame::Figure* clone() const { return new Knight(*this); }
			virtual ~Knight();

			virtual bool canMoveTo(boardgame::Coords const& to) const;
		};


		class Pawn
			: public ChessFigure
		{
		public:
			Pawn(boardgame::Player* b);
			virtual boardgame::Figure* clone() const { return new Pawn(*this); }
			virtual ~Pawn();

			virtual void setPosition(const boardgame::Coords& c);

			virtual bool canMoveTo(boardgame::Coords const& to) const;
			virtual bool threatens(boardgame::Coords const& to) const;
			virtual bool apply(boardgame::MutableBoardTransaction& move) const;

			bool hasJustJumped() const;

		private:
			Pawn(Pawn const&, boardgame::TransactionNumber_t);

			bool const canJump;
			boardgame::TransactionNumber_t const jumpedAt;
			signed short movingDir;
		};
	}
}




#endif /* BISHOP_HPP_ */
