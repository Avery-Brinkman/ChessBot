#pragma once

#include "BitBoard.hpp"
#include <array>

namespace Engine_NS::Precomputed {
// All follow the pattern Array[Color (Black, White)][Index (0, 1, ..., 62, 63)]

const std::array<const std::array<const BitBoard, 64>, 2> PawnMoves = {{{0,
                                                                         0,
                                                                         0,
                                                                         0,
                                                                         0,
                                                                         0,
                                                                         0,
                                                                         0,
                                                                         1,
                                                                         2,
                                                                         4,
                                                                         8,
                                                                         16,
                                                                         32,
                                                                         64,
                                                                         128,
                                                                         256,
                                                                         512,
                                                                         1024,
                                                                         2048,
                                                                         4096,
                                                                         8192,
                                                                         16384,
                                                                         32768,
                                                                         65536,
                                                                         131072,
                                                                         262144,
                                                                         524288,
                                                                         1048576,
                                                                         2097152,
                                                                         4194304,
                                                                         8388608,
                                                                         16777216,
                                                                         33554432,
                                                                         67108864,
                                                                         134217728,
                                                                         268435456,
                                                                         536870912,
                                                                         1073741824,
                                                                         2147483648,
                                                                         4294967296,
                                                                         8589934592,
                                                                         17179869184,
                                                                         34359738368,
                                                                         68719476736,
                                                                         137438953472,
                                                                         274877906944,
                                                                         549755813888,
                                                                         1103806595072,
                                                                         2207613190144,
                                                                         4415226380288,
                                                                         8830452760576,
                                                                         17660905521152,
                                                                         35321811042304,
                                                                         70643622084608,
                                                                         141287244169216,
                                                                         281474976710656,
                                                                         562949953421312,
                                                                         1125899906842624,
                                                                         2251799813685248,
                                                                         4503599627370496,
                                                                         9007199254740992,
                                                                         18014398509481984,
                                                                         36028797018963968},
                                                                        {256,
                                                                         512,
                                                                         1024,
                                                                         2048,
                                                                         4096,
                                                                         8192,
                                                                         16384,
                                                                         32768,
                                                                         16842752,
                                                                         33685504,
                                                                         67371008,
                                                                         134742016,
                                                                         269484032,
                                                                         538968064,
                                                                         1077936128,
                                                                         2155872256,
                                                                         16777216,
                                                                         33554432,
                                                                         67108864,
                                                                         134217728,
                                                                         268435456,
                                                                         536870912,
                                                                         1073741824,
                                                                         2147483648,
                                                                         4294967296,
                                                                         8589934592,
                                                                         17179869184,
                                                                         34359738368,
                                                                         68719476736,
                                                                         137438953472,
                                                                         274877906944,
                                                                         549755813888,
                                                                         1099511627776,
                                                                         2199023255552,
                                                                         4398046511104,
                                                                         8796093022208,
                                                                         17592186044416,
                                                                         35184372088832,
                                                                         70368744177664,
                                                                         140737488355328,
                                                                         281474976710656,
                                                                         562949953421312,
                                                                         1125899906842624,
                                                                         2251799813685248,
                                                                         4503599627370496,
                                                                         9007199254740992,
                                                                         18014398509481984,
                                                                         36028797018963968,
                                                                         72057594037927936,
                                                                         144115188075855872,
                                                                         288230376151711744,
                                                                         576460752303423488,
                                                                         1152921504606846976,
                                                                         2305843009213693952,
                                                                         4611686018427387904,
                                                                         9223372036854775808,
                                                                         0,
                                                                         0,
                                                                         0,
                                                                         0,
                                                                         0,
                                                                         0,
                                                                         0,
                                                                         0}}};
const std::array<const std::array<const BitBoard, 64>, 2> PawnAttacks = {{{0,
                                                                           0,
                                                                           0,
                                                                           0,
                                                                           0,
                                                                           0,
                                                                           0,
                                                                           0,
                                                                           2,
                                                                           5,
                                                                           10,
                                                                           20,
                                                                           40,
                                                                           80,
                                                                           160,
                                                                           64,
                                                                           512,
                                                                           1280,
                                                                           2560,
                                                                           5120,
                                                                           10240,
                                                                           20480,
                                                                           40960,
                                                                           16384,
                                                                           131072,
                                                                           327680,
                                                                           655360,
                                                                           1310720,
                                                                           2621440,
                                                                           5242880,
                                                                           10485760,
                                                                           4194304,
                                                                           33554432,
                                                                           83886080,
                                                                           167772160,
                                                                           335544320,
                                                                           671088640,
                                                                           1342177280,
                                                                           2684354560,
                                                                           1073741824,
                                                                           8589934592,
                                                                           21474836480,
                                                                           42949672960,
                                                                           85899345920,
                                                                           171798691840,
                                                                           343597383680,
                                                                           687194767360,
                                                                           274877906944,
                                                                           2199023255552,
                                                                           5497558138880,
                                                                           10995116277760,
                                                                           21990232555520,
                                                                           43980465111040,
                                                                           87960930222080,
                                                                           175921860444160,
                                                                           70368744177664,
                                                                           562949953421312,
                                                                           1407374883553280,
                                                                           2814749767106560,
                                                                           5629499534213120,
                                                                           11258999068426240,
                                                                           22517998136852480,
                                                                           45035996273704960,
                                                                           18014398509481984},
                                                                          {512,
                                                                           1280,
                                                                           2560,
                                                                           5120,
                                                                           10240,
                                                                           20480,
                                                                           40960,
                                                                           16384,
                                                                           131072,
                                                                           327680,
                                                                           655360,
                                                                           1310720,
                                                                           2621440,
                                                                           5242880,
                                                                           10485760,
                                                                           4194304,
                                                                           33554432,
                                                                           83886080,
                                                                           167772160,
                                                                           335544320,
                                                                           671088640,
                                                                           1342177280,
                                                                           2684354560,
                                                                           1073741824,
                                                                           8589934592,
                                                                           21474836480,
                                                                           42949672960,
                                                                           85899345920,
                                                                           171798691840,
                                                                           343597383680,
                                                                           687194767360,
                                                                           274877906944,
                                                                           2199023255552,
                                                                           5497558138880,
                                                                           10995116277760,
                                                                           21990232555520,
                                                                           43980465111040,
                                                                           87960930222080,
                                                                           175921860444160,
                                                                           70368744177664,
                                                                           562949953421312,
                                                                           1407374883553280,
                                                                           2814749767106560,
                                                                           5629499534213120,
                                                                           11258999068426240,
                                                                           22517998136852480,
                                                                           45035996273704960,
                                                                           18014398509481984,
                                                                           144115188075855872,
                                                                           360287970189639680,
                                                                           720575940379279360,
                                                                           1441151880758558720,
                                                                           2882303761517117440,
                                                                           5764607523034234880,
                                                                           11529215046068469760,
                                                                           4611686018427387904,
                                                                           0,
                                                                           0,
                                                                           0,
                                                                           0,
                                                                           0,
                                                                           0,
                                                                           0,
                                                                           0}}};

} // namespace Engine_NS::Precomputed