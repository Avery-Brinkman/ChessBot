#include "Piece.h"

namespace Engine_NS::Pieces {

Piece getColor(Piece piece) { return piece & ColorFlag; }

Piece getType(Piece piece) { return piece & TypeFlag; }

bool isWhite(Piece piece) { return getColor(piece) == White; }

} // namespace Engine_NS::Pieces