#include "Move.h"

namespace Engine_NS {

Move::Move(MoveBits start, MoveBits end, MoveBits flags) : Move(flags | end | start) {}
Move::Move(MoveBits move) : m_move(move) {}

} // namespace Engine_NS