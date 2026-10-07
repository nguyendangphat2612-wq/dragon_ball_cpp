#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <conio.h>
#include <windows.h>
using namespace std;

const int MAP_W = 12;
const int MAP_H = 10;
const int MAX_DRAGON_BALLS = 7;

struct Player {
    int x;
    int y;
    int hp;
    int ki;
    int dragonBalls;
    string name;
};

struct Enemy {
    int x;
    int y;
    int hp;
    bool alive;
    string name;
};

struct DragonBall {
    int x;
    int y;
    bool collected;
};

vector<DragonBall> dragonBalls;
vector<Enemy> enemies;
Player player;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void initGame() {
    srand((unsigned)time(0));
    player.x = 0;
    player.y = 0;
    player.hp = 100;
    player.ki = 30;
    player.dragonBalls = 0;
    player.name = "Goku";

    dragonBalls.clear();
    for (int i = 0; i < MAX_DRAGON_BALLS; ++i) {
        DragonBall db;
        db.x = rand() % MAP_W;
        db.y = rand() % MAP_H;
        db.collected = false;
        dragonBalls.push_back(db);
    }

    enemies.clear();
    for (int i = 0; i < 3; ++i) {
        Enemy e;
        e.x = rand() % MAP_W;
        e.y = rand() % MAP_H;
        e.hp = 50;
        e.alive = true;
        e.name = "Enemy" + to_string(i + 1);
        enemies.push_back(e);
    }
}

void drawMap() {
    system("cls");

    for (int y = 0; y < MAP_H; ++y) {
        for (int x = 0; x < MAP_W; ++x) {
            bool drawn = false;

            for (size_t i = 0; i < dragonBalls.size(); ++i) {
                if (!dragonBalls[i].collected && dragonBalls[i].x == x && dragonBalls[i].y == y) {
                    cout << "O";
                    drawn = true;
                    break;
                }
            }

            if (!drawn) {
                for (size_t i = 0; i < enemies.size(); ++i) {
                    if (enemies[i].alive && enemies[i].x == x && enemies[i].y == y) {
                        cout << "E";
                        drawn = true;
                        break;
                    }
                }
            }

            if (!drawn) {
                if (player.x == x && player.y == y) {
                    cout << "P";
                } else {
                    cout << ".";
                }
            }
        }
        cout << endl;
    }

    cout << "Ten: " << player.name << " | HP: " << player.hp << " | Ki: " << player.ki
         << " | Ngoc Rong: " << player.dragonBalls << "/" << MAX_DRAGON_BALLS << endl;
}

bool isInsideMap(int x, int y) {
    return x >= 0 && x < MAP_W && y >= 0 && y < MAP_H;
}

void collectDragonBall() {
    for (size_t i = 0; i < dragonBalls.size(); ++i) {
        if (!dragonBalls[i].collected && dragonBalls[i].x == player.x && dragonBalls[i].y == player.y) {
            dragonBalls[i].collected = true;
            player.dragonBalls++;
            player.ki += 10;
            cout << "Ban da thu thap duoc vien Ngoc Rong!\n";
            return;
        }
    }
    cout << "Khong co vien Ngoc Rong o vi tri nay.\n";
}

void enemyTurn() {
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (!enemies[i].alive) continue;

        int moveX = 0, moveY = 0;
        if (enemies[i].x < player.x) moveX = 1;
        else if (enemies[i].x > player.x) moveX = -1;

        if (enemies[i].y < player.y) moveY = 1;
        else if (enemies[i].y > player.y) moveY = -1;

        int nx = enemies[i].x + moveX;
        int ny = enemies[i].y + moveY;
        if (isInsideMap(nx, ny)) {
            enemies[i].x = nx;
            enemies[i].y = ny;
        }

        if (enemies[i].x == player.x && enemies[i].y == player.y) {
            int damage = rand() % 15 + 5;
            player.hp -= damage;
            cout << enemies[i].name << " danh ban, mat " << damage << " mau!\n";
        }
    }
}

void attackEnemy() {
    bool hit = false;
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (!enemies[i].alive) continue;

        if (abs(enemies[i].x - player.x) <= 1 && abs(enemies[i].y - player.y) <= 1) {
            int damage = rand() % 20 + 10;
            enemies[i].hp -= damage;
            hit = true;
            cout << "Ban tan cong " << enemies[i].name << " gay " << damage << " sat thuong!\n";

            if (enemies[i].hp <= 0) {
                enemies[i].alive = false;
                cout << enemies[i].name << " da bi defeat!\n";
                player.ki += 5;
                player.dragonBalls += 1;
            }
        }
    }

    if (!hit) {
        cout << "Khong co ke dich nao trong tam tan cong.\n";
    }
}

void checkWinLose() {
    int aliveEnemies = 0;
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i].alive) aliveEnemies++;
    }

    if (player.hp <= 0) {
        cout << "Ban da thua!\n";
        exit(0);
    }

    if (player.dragonBalls >= MAX_DRAGON_BALLS && aliveEnemies == 0) {
        cout << "CHUC MUNG! BAN DA THU THAP 7 VIEN NGOC RONG VA DANH BAI TAT CA DOI THU!\n";
        exit(0);
    }
}

void handleInput(char key) {
    int nx = player.x;
    int ny = player.y;

    switch (key) {
        case 'w': case 'W': ny--; break;
        case 's': case 'S': ny++; break;
        case 'a': case 'A': nx--; break;
        case 'd': case 'D': nx++; break;
        case 'c': case 'C': collectDragonBall(); return;
        case 'f': case 'F': attackEnemy(); return;
        case 'q': case 'Q':
            cout << "Ban da thoat game!\n";
            exit(0);
        default:
            return;
    }

    if (isInsideMap(nx, ny)) {
        player.x = nx;
        player.y = ny;
    }

    // tranh va cham voi doi thu
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i].alive && enemies[i].x == player.x && enemies[i].y == player.y) {
            player.hp -= 10;
            cout << "Ban va cham voi " << enemies[i].name << ", mat 10 mau!\n";
        }
    }

    if (player.x == 0 && player.y == 0) {
        cout << "Ban da quay ve diem xuat phat.\n";
    }
}

void showHelp() {
    cout << "-----------------------------------\n";
    cout << "W/A/S/D: Di chuyen\n";
    cout << "C: Thu thap vien Ngoc Rong\n";
    cout << "F: Tan cong ke dich\n";
    cout << "Q: Thoat game\n";
    cout << "-----------------------------------\n";
}

int main() {
    hideCursor();
    initGame();

    cout << "GAME NGOC RONG - 7 VIEN NGOC RONG\n";
    showHelp();

    while (true) {
        drawMap();
        checkWinLose();

        cout << "Nhap lenh (W/A/S/D/C/F/Q): ";
        char key = _getch();
        handleInput(key);
        enemyTurn();

        if (player.dragonBalls >= MAX_DRAGON_BALLS) {
            cout << "Ban da thu thap du 7 vien Ngoc Rong! Tim kiem doi thu giu lai!\n";
        }

        Sleep(150);
    }

    return 0;
}
