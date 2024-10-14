// DijkstraMap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <iostream>
#include <queue>

#define TIMING 0
#define REC0  1 // debug 8 - release 3
#define REC1  2 // debug 6 - release 3
#define BSF0  4 // debug 101 - release 3

#define ACTIVE  REC0

constexpr int WID = 41, HEI = 41;

int map[WID][HEI];

bool inBounds(int x, int y) {
    return (x > -1 && y > -1 && x < WID && y < HEI);
}

#if ACTIVE == REC0
void setDistValue(int x, int y, int dx, int dy, int v, bool top) {
    int xx = dx + x, yy = dy + y;

    if (inBounds(xx, yy) && map[xx][yy] < 0) {

        map[xx][yy] = v;

        if (top) setDistValue(xx, yy, 0, -1, v + 1, top);
        else setDistValue(xx, yy, 0, 1, v + 1, top);
        setDistValue(xx, yy, 1, 0, v + 1, top);
        setDistValue(xx, yy, -1, 0, v + 1, top);

    }
}

void createDMap(int sx, int sy) {
    
    setDistValue(sx, sy, 0, 0, 0, true);
    map[sx][sy] = -1;
    setDistValue(sx, sy, 0, 0, 0, false);
}

#elif ACTIVE == REC1
void setDistValue(int x, int y, int dx, int dy, int v, int dir) {
    int xx = dx + x, yy = dy + y;

    if (inBounds(xx, yy) && map[xx][yy] < 0) {

        map[xx][yy] = v;
        switch (dir) {
            case 0:
                setDistValue(xx, yy, 0, -1, v + 1, dir);
                setDistValue(xx, yy, 1, -1, v + 1, dir);
                setDistValue(xx, yy, -1, -1, v + 1, dir);
                break;
            case 1:
                setDistValue(xx, yy, 1, 1, v + 1, dir);
                setDistValue(xx, yy, 0, 1, v + 1, dir);
                setDistValue(xx, yy, -1, 1, v + 1, dir);
                break;
            case 2:
                setDistValue(xx, yy, -1, -1, v + 1, dir);
                setDistValue(xx, yy, -1, 0, v + 1, dir);
                setDistValue(xx, yy, -1, 1, v + 1, dir);
                break;
            case 3:
                setDistValue(xx, yy, 1, -1, v + 1, dir);
                setDistValue(xx, yy, 1, 0, v + 1, dir);
                setDistValue(xx, yy, 1, 1, v + 1, dir);
                break;
        }

    }
}

void createDMap(int sx, int sy) {
    setDistValue(sx, sy, 0, 0, 0, 0);
    map[sx][sy] = -1;
    setDistValue(sx, sy, 0, 0, 0, 1);
    map[sx][sy] = -1;
    setDistValue(sx, sy, 0, 0, 0, 2);
    map[sx][sy] = -1;
    setDistValue(sx, sy, 0, 0, 0, 3);
}

#elif ACTIVE == BSF0

void setDistValueBFS(int sx, int sy) {

    std::queue<std::pair<int, int>> q;
    map[sx][sy] = 0;
    q.push({ sx, sy });

    int directions[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

    while (!q.empty()) {
        auto qf = q.front();
        q.pop();

        int x = qf.first, y = qf.second;

        int currentDist = map[x][y];

        for (int i = 0; i < 4; i++) {
            int newX = x + directions[i][0];
            int newY = y + directions[i][1];

            if (inBounds(newX, newY) && map[newX][newY] == -1) {
                map[newX][newY] = currentDist + 1;
                q.push({ newX, newY });
            }
        }
    }
}

void createDMap(int sx, int sy) {
    setDistValueBFS(sx, sy);
}

#endif


void initMap() {
    for (int y = 0; y < HEI; y++) {
        for (int x = 0; x < WID; x++) {
            map[x][y] = -1;
        }
    }
}

int main() {

#if TIMING == 1
    time_t t = time(0);

    for (int u = 0; u < 100000; u++) {
        initMap();
        createDMap(20, 20);
    }

    time_t d = time(0) - t;

#else
    initMap();
    createDMap(20, 20);

#endif

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int y = 0; y < HEI; y++) {
        for (int x = 0; x < WID; x++) {
            int k = map[x][y] % 10+1;
            SetConsoleTextAttribute(hConsole, k << 4);
            if(map[x][y] == -1) std::cout << "  " << " ";
            else std::cout << (map[x][y] < 10 ? " " : "") << map[x][y];
        }
        std::cout << std::endl;
    }
#if TIMING == 1
    std::cout << d;
#endif

    SetConsoleTextAttribute(hConsole, 7);

    return 0;
}
