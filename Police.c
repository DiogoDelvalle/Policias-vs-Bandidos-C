#include <stdlib.h>
#include <stdio.h>
#include "Police.h"

static int** allocIntMatrix(int N, int initVal) {
    int** m = (int**)malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        m[i] = (int*)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) m[i][j] = initVal;
    }
    return m;
}

static void freeIntMatrix(int** m, int N) {
    if (!m) return;
    for (int i = 0; i < N; i++) free(m[i]);
    free(m);
}

static int** bfsDistances(Scenario* sc, int N, Site source) {
    int** dist = allocIntMatrix(N, -1);
    Site* queue = (Site*)malloc(N * N * sizeof(Site));
    int head = 0, tail = 0;
    dist[source.i][source.j] = 0;
    queue[tail++] = source;
    while (head < tail) {
        Site v = queue[head++];
        for (int di = -1; di <= 1; di++) {
            for (int dj = -1; dj <= 1; dj++) {
                if (di == 0 && dj == 0) continue;
                Site w;
                w.i = v.i + di;
                w.j = v.j + dj;
                if (w.i < 0 || w.j < 0 || w.i >= N || w.j >= N) continue;
                if (!isLegalMoveScenario(sc, v, w)) continue;
                if (dist[w.i][w.j] != -1) continue;
                dist[w.i][w.j] = dist[v.i][v.j] + 1;
                queue[tail++] = w;
            }
        }
    }
    free(queue);
    return dist;
}

Site movePolice(Game* game) {
    Site police;
    police.i = 0;
    police.j = 0;
    if (!game) return police;

    police = getPoliceSite(game);
    Site bandit = getBanditSite(game);
    int N = game->N;
    Scenario* sc = game->scenario;

    if (equalsSite(police, bandit)) return police;

    int** dist = bfsDistances(sc, N, bandit);

    Site best = police;
    int bestDist = dist[police.i][police.j]; /* peor caso: quedarse quieto */
    int bestManhattan = manhattanToSite(police, bandit);

    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) continue;
            Site w;
            w.i = police.i + di;
            w.j = police.j + dj;
            if (w.i < 0 || w.j < 0 || w.i >= N || w.j >= N) continue;
            if (!isLegalMoveScenario(sc, police, w)) continue;
            int d = dist[w.i][w.j];
            if (d < 0) continue; /* inalcanzable desde el bandido */
            int man = manhattanToSite(w, bandit);
            if (d < bestDist || (d == bestDist && man < bestManhattan)) {
                bestDist = d;
                bestManhattan = man;
                best = w;
            }
        }
    }

    freeIntMatrix(dist, N);
    return best;
}
