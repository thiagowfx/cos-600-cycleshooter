#ifndef _CONTEXT_HPP_
#define _CONTEXT_HPP_

/**
 * Game context mode.
 * CONTEXT_RUNNER: when the player runs away from the monster.
 * CONTEXT_SHOOTER: when the player stops and tries to shoot off the monster.
 */
enum Context {
    CONTEXT_RUNNER = 0,
    CONTEXT_SHOOTER = 1,
};

#endif
