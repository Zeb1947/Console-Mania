/*
CONSOLE MANIA
BY DEAN BROSNAN
*/

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <stdexcept>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
#endif

// ==== GLOBAL SETTINGS ====
const int gridWidth = 22;
const int gridHeight = 22;
const int NUM_LEVELS = 2;
const unsigned int speed = 20;  // milliseconds

char username[17];  // 16 characters + null terminator
std::string mainCharacterName = "EZRA";

const char playerSymbol = '@';

// === TILE VALUES ===
enum Tile {
    EMPTY = 0,
    WALL = 1,
    TREE = 2,
    NPC = 3
};

// === LEVELS ===
int levels[NUM_LEVELS][gridHeight][gridWidth];
int currentLevel = 0;

// === PLAYER POSITION ===
int playerX = gridWidth / 2;
int playerY = gridHeight / 2;  // Align with open transition row

// ==== UTILITIES ====
void clearScreen() {
    try {
#ifdef _WIN32
        if (std::system("cls") != 0)
            throw std::runtime_error("Failed to clear screen using 'cls'");
#else
        if (std::system("clear") != 0)
            throw std::runtime_error("Failed to clear screen using 'clear'");
#endif
    } catch (const std::exception& e) {
        std::cerr << "Screen clear error: " << e.what() << std::endl;
    }
}

void delay(unsigned int ms) {
    try {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    } catch (const std::exception& e) {
        std::cerr << "Delay error: " << e.what() << std::endl;
    }
}

// ==== WORLD SETUP ====
void initializeWorld() {
    for (int l = 0; l < NUM_LEVELS; ++l) {
        for (int y = 0; y < gridHeight; ++y) {
            for (int x = 0; x < gridWidth; ++x) {
                bool isBorder = (y == 0 || y == gridHeight - 1);
                bool isSideWall = (x == 0 || x == gridWidth - 1);

                // Leave left/right openings in the center for level transitions
                if ((x == 0 || x == gridWidth - 1) && y == gridHeight / 2) {
                    levels[l][y][x] = EMPTY;
                } else if (isBorder || isSideWall) {
                    levels[l][y][x] = WALL;
                } else {
                    levels[l][y][x] = EMPTY;
                }
            }
        }
    }

    // Decorations for Level 0
    levels[0][5][5] = TREE;
    levels[0][10][10] = NPC;

    // Decorations for Level 1
    levels[1][6][6] = TREE;
    levels[1][8][8] = NPC;
}

// ==== RENDERING ====
void renderGrid() {
    clearScreen();
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            if (x == playerX && y == playerY) {
                std::cout << playerSymbol;
            } else {
                switch (levels[currentLevel][y][x]) {
                    case EMPTY: std::cout << '.'; break;
                    case WALL: std::cout << '#'; break;
                    case TREE: std::cout << '!'; break;
                    case NPC: std::cout << 'N'; break;
                    default: std::cout << '?'; break;
                }
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Level: " << currentLevel + 1 << " | Use W/A/S/D to move. Q to quit." << std::endl;
}

// ==== MOVEMENT LOGIC ====
bool isWalkable(int level, int x, int y) {
    if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight)
        return false;
    return levels[level][y][x] == EMPTY;
}

bool handleInput(char input) {
    input = std::tolower(input);
    int newX = playerX;
    int newY = playerY;

    switch (input) {
        case 'w': newY--; break;
        case 's': newY++; break;
        case 'a': newX--; break;
        case 'd': newX++; break;
        case 'q': return false;
    }

    // === LEVEL TRANSITIONS ===
    if (newX >= gridWidth) {
        if (currentLevel < NUM_LEVELS - 1 && levels[currentLevel][playerY][gridWidth - 1] == EMPTY) {
            currentLevel++;
            playerX = 0;
            return true;
        } else {
            return true;
        }
    }

    if (newX < 0) {
        if (currentLevel > 0 && levels[currentLevel][playerY][0] == EMPTY) {
            currentLevel--;
            playerX = gridWidth - 1;
            return true;
        } else {
            return true;
        }
    }

    // === REGULAR MOVEMENT ===
    if (isWalkable(currentLevel, newX, newY)) {
        playerX = newX;
        playerY = newY;
    }

    return true;
}

// ==== MAIN GAME ====
int main() {
    std::cout << "Welcome to CONSOLE MANIA!" << std::endl;
    std::cout << "There are no saves in this game so beware..." << std::endl << std::endl;
    std::cout << "Please enter your name (limit 16 characters): ";
    std::cin.getline(username, sizeof(username));
    std::cout << "Hello, " << username << "!" << std::endl;
    delay(2500);

    clearScreen();

    std::cout << "Hello " << mainCharacterName << std::endl;
    delay(3000);
    std::cout << "You have been dropped into the underworld and various adventures await you." << std::endl;
    delay(2000);
    std::cout << "Here, take this: ";
    delay(3000);
    std::cout << "YOU HAVE RECEIVED: COMMON SWORD" << std::endl;
    delay(3000);
    std::cout << "Now " << mainCharacterName << ", you must wander through this realm and find the king of the underworld, SATAN!" << std::endl;
    delay(3000);
    std::cout << "Farewell, I wish you luck." << std::endl;
    delay(3000);
    clearScreen();

    // Setup world
    initializeWorld();

    // Game loop
    bool running = true;
    while (running) {
        renderGrid();

        char input;
#ifdef _WIN32
        input = _getch();  // no enter key needed
#else
        std::cin >> input;
#endif
        running = handleInput(input);
        delay(speed);
    }

    std::cout << "Thanks for playing, " << username << "!" << std::endl;
    return 0;
}
