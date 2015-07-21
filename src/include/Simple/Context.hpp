#ifndef _CONTEXT_HPP_
#define _CONTEXT_HPP_

/**
 * Game context mode.
 * CONTEXT_RUNNER: the player runs away from the monster.
 * CONTEXT_SHOOTER: the player stops then tries to shoot off the monster.
 */
enum Context {
    CONTEXT_RUNNER,
    CONTEXT_SHOOTER,
};

#endif
