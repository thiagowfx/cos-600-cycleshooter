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

/**
 * Represents which condition led the game to finish.
 * TL;DR Victory or loss?
 */
enum EndGameType {
    // monster has been killed by the player
    GAME_END_MONSTER_KILLED = 0,

    // the monster caught the player
    GAME_END_CAUGHT_BY_MONSTER,

    // the player collided with a wall
    GAME_END_WALL_CRASH,

    // the player manually closed the game
    GAME_END_MANUAL_SHUTDOWN,
};

inline bool isGameVictory(const EndGameType& type) {
    return type == GAME_END_MONSTER_KILLED;
}

static const char* EndGameTypeToString(const EndGameType& type) {
    static const char* names[] = {
        "Game VICTORY :: Monster killed",
        "Game LOSS :: Caught by monster",
        "Game LOSS :: Crashed in the wall",
        "Game LOSS :: Manual Shutdown"
    };
    return names[type];
}

#endif
