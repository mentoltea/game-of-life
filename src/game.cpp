#include "definitions.h"
#include <random>
#include <time.h>

#define BLOCKSIZE 10
#define BLOCKCOUNT 50

size_t WINX=BLOCKCOUNT*BLOCKSIZE, WINY=BLOCKCOUNT*BLOCKSIZE;
float FPS = 10;
int FPSi = (int)FPS; 
size_t tm = 0;


class GameMap: public DrawableObject {
    int map0[BLOCKCOUNT][BLOCKCOUNT], map1[BLOCKCOUNT][BLOCKCOUNT];
    int curr = 0;
public:
    GameMap() {
        ObjectsToDraw.push_back(this);
        
        for (int i=0; i<BLOCKCOUNT; i++) {
            for (int j=0; j<BLOCKCOUNT; j++) {
                map0[i][j] = rand()%8 == 1;
            }
        }
    }

    ~GameMap() {
        RemoveFromVector(this);
    }
    void update() {
        int n;
        if (curr) {
            for (int i=0; i<BLOCKCOUNT; i++) {
                for (int j=0; j<BLOCKCOUNT; j++) {
                    n = neighbours(i,j);
                    if (map1[i][j]) {
                        map0[i][j] = n==2 || n==3;
                    } else {
                        map0[i][j] = n==3;
                    }
                    //if (n==5) map[i][j] = 2;
                }
            }
        } else {
            for (int i=0; i<BLOCKCOUNT; i++) {
                for (int j=0; j<BLOCKCOUNT; j++) {
                    n = neighbours(i,j);
                    if (map0[i][j]) {
                        map1[i][j] = n==2 || n==3;
                    } else {
                        map1[i][j] = n==3;
                    }
                    //if (n==5) map[i][j] = 2;
                }
            }
        }

        curr = (curr+1) % 2;
    }

    int neighbours(int y, int x) {
        int s = 0;
        if (curr) {
            for (int i=y-1; i<y+2; i++) {
                for (int j=x-1; j<x+2; j++) {
                    if ( (i==y && j==x)
                        || i < 0 || i > BLOCKCOUNT
                        || j < 0 || j > BLOCKCOUNT) continue;
                
                    s += map1[i][j];
                }
            }
        } else {
            for (int i=y-1; i<y+2; i++) {
                for (int j=x-1; j<x+2; j++) {
                    if ( (i==y && j==x)
                        || i < 0 || i > BLOCKCOUNT
                        || j < 0 || j > BLOCKCOUNT) continue;
                
                    s += map0[i][j];
                }
            }
        }
        return s;
    }
    
    void draw() override {
        for (int i=0; i<BLOCKCOUNT; i++) {
            draw_line({i*BLOCKSIZE, 0}, {i*BLOCKSIZE, (int)WINY}, {0,0,0}, 3);
        }
        for (int i=0; i<BLOCKCOUNT; i++) {
            draw_line({0, i*BLOCKSIZE}, {(int)WINX, i*BLOCKSIZE}, {0,0,0}, 3);
        }
        
        if (curr){    
            for (int i=0; i<BLOCKCOUNT; i++) {
                for (int j=0; j<BLOCKCOUNT; j++) {
                    draw_filled_rect({i*BLOCKSIZE, j*BLOCKSIZE}, BLOCKSIZE, BLOCKSIZE, {0, 0, 0.1f + ((map1[i][j]) ? (1/3.0f + (float)(i+j)/3/BLOCKCOUNT) : 0)});
                }
            }
        } else {
            for (int i=0; i<BLOCKCOUNT; i++) {
                for (int j=0; j<BLOCKCOUNT; j++) {
                    draw_filled_rect({i*BLOCKSIZE, j*BLOCKSIZE}, BLOCKSIZE, BLOCKSIZE, {0, 0, 0.1f + ((map0[i][j]) ? (1/3.0f + (float)(i+j)/3/BLOCKCOUNT) : 0)});
                }
            }
        }
    }
};


GameMap *map;
void prepare() {
    map = new GameMap;
    srand(time(NULL));
}

void call() {
    tm = (tm+1)%FPSi;
    map->update();
}

void end() {
    if (map) delete map;
}

void keyboardKeys(unsigned char key, int x, int y) {
    //if (tm > 4*FPSi/7) return;
    switch (key) {
        case 'r': case 'R':
            if (map) delete map;
            map = new GameMap;
            break;
        default:
            break;
    }
}