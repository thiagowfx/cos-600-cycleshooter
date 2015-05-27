#ifndef _CONTEXT_HPP_
#define _CONTEXT_HPP_

/**
 * Current game context mode.
 * CONTEXT_RUNNER: when the player runs away from the monster.
 * CONTEXT_SHOOTER: when the player stops and tries to shoot off the monster.
 */
enum Context {
    CONTEXT_RUNNER,
    CONTEXT_SHOOTER,
};

#endif