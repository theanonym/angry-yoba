#include "angry_yoba/gcommon.hpp"

const Material Materials::Wood("wood",       Physics(1.00f, 0.50f, 0.10f));
const Material Materials::Steel("metal",     Physics(7.00f, 0.20f, 0.20f));
const Material Materials::Stone("stone",     Physics(0.75f, 0.75f, 0.50f));
const Material Materials::Textile("textile", Physics(0.10f, 0.75f, 0.50f));


// Block | Yoba  | Enemy | EnemySensor
const Filter Filters::Wall(Filter::A,
                           Filter::B | Filter::C | Filter::D | Filter::E);
// Wall  | Block | Yoba  | Enemy | EnemySensor
const Filter Filters::Block(Filter::B,
                            Filter::A  | Filter::B | Filter::C  | Filter::D | Filter::E);
// Wall  | Block | Yoba  | EnemySensor
const Filter Filters::Yoba(Filter::C,
                           Filter::A | Filter::B /*| Filter::C*/ | Filter::E);
// Wall  | Block | Enemy
const Filter Filters::Enemy(Filter::D,
                            Filter::A | Filter::B | Filter::D);
// Wall | Block | Yoba
const Filter Filters::EnemySensor(Filter::E,
                                  Filter::A | Filter::B | Filter::C);
