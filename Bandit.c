#include <stdlib.h>
#include <stdio.h>
#include "Bandit.h"

Site moveBandit(Game* game) {
    Site move;
    move.i = 0;
    move.j = 0;
    if (!game) return move;

    Site bandit = getBanditSite(game);
    Site police = getPoliceSite(game);
    int N = game->N;
    Scenario* sc = game->scenario;

    move = bandit;
    int bestDist = manhattanToSite(bandit, police);

    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) continue;
            Site w;
            w.i = bandit.i + di;
            w.j = bandit.j + dj;
            if (w.i < 0 || w.j < 0 || w.i >= N || w.j >= N) continue;
            if (!isLegalMoveScenario(sc, bandit, w)) continue;

            int d = manhattanToSite(w, police);
            if (d > bestDist) {
                bestDist = d;
                move = w;
            }
        }
    }

    return move;
}
