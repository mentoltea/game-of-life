#include "definitions.h"
#include <random>
#include <time.h>

#define BLOCKSIZE 10
#define BLOCKCOUNT 50

size_t WINX=BLOCKCOUNT*BLOCKSIZE, WINY=BLOCKCOUNT*BLOCKSIZE;
float FPS = 60;


class GameMap: public DrawableObject {
    int map[BLOCKCOUNT][BLOCKCOUNT];
public:
    GameMap() {
        ObjectsToDraw.push_back(this);
        
        srand(time(NULL));
        for (int i=0; i<BLOCKCOUNT; i++) {
            for (int j=0; j<BLOCKCOUNT; j++) {
                map[i][j] = rand()%15 == 1;
            }
        }
    }

    ~GameMap() {
        RemoveFromVector(this);
    }
    void update() {
        int n;
        for (int i=0; i<BLOCKCOUNT; i++) {
            for (int j=0; j<BLOCKCOUNT; j++) {
                n = neighbours(i,j);
                if (map[i][j]) {
                    map[i][j] = n==2 || n==3;
                } else {
                    map[i][j] = n==3;
                }
                //if (n==5) map[i][j] = 2;
            }
        }
    }

    void draw() override {
        for (int i=0; i<BLOCKCOUNT; i++) {
            draw_line({i*BLOCKSIZE, 0}, {i*BLOCKSIZE, WINY}, {0,0,0}, 3);
        }
        for (int i=0; i<BLOCKCOUNT; i++) {
            draw_line({0, i*BLOCKSIZE}, {WINX, i*BLOCKSIZE}, {0,0,0}, 3);
        }
        
        for (int i=0; i<BLOCKCOUNT; i++) {
            for (int j=0; j<BLOCKCOUNT; j++) {
                draw_filled_rect({i*BLOCKSIZE, j*BLOCKSIZE}, BLOCKSIZE, BLOCKSIZE, {0, 0, 0.1f + ((map[i][j]) ? (1/3.0f + (float)(i+j)/3/BLOCKCOUNT) : 0)});
            }
        }
    }

    int neighbours(int y, int x) {
        int s = 0;
        for (int i=y-1; i<y+2; i++) {
            for (int j=x-1; j<x+2; j++) {
                if ( (i==y && j==x)
                    || i < 0 || i > BLOCKCOUNT
                    || j < 0 || j > BLOCKCOUNT) continue;
            
                s += map[i][j];
            }
        }
        return s;
    }
};


GameMap *map;
void prepare() {
    map = new GameMap;
}

void call() {
    map->update();
}

void end() {
    if (map) delete map;
}