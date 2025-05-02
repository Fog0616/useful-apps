#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>

// 游戏常量定义
#define MAZE_HEIGHT 25   // 迷宫高度(必须为奇数)
#define MAZE_WIDTH 25    // 迷宫宽度(必须为奇数)
#define BORDER_SIZE 2    // 迷宫边界大小

// 地图元素类型
typedef enum {
    WALL = 1,
    ROAD = 0,
    START = 2,
    END = 3
} CellType;

// 方向枚举
typedef enum {
    DIR_UP = 1,
    DIR_DOWN = 2,
    DIR_LEFT = 3,
    DIR_RIGHT = 4,
    DIR_ESC = 5
} Direction;

// 全局地图数组
int maze[MAZE_HEIGHT + BORDER_SIZE][MAZE_WIDTH + BORDER_SIZE];

// 函数声明
void moveCursor(int x, int y);
void hideCursor();
void initializeMaze();
void generateMaze(int x, int y);
Direction getInput();
void drawCell(int x, int y);
void runGame();
void drawMaze();

// 移动光标到指定位置
void moveCursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 隐藏控制台光标
void hideCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}

// 初始化迷宫(全部设为墙)
void initializeMaze() {
    for (int i = 0; i <= MAZE_HEIGHT + 1; i++) {
        for (int j = 0; j <= MAZE_WIDTH + 1; j++) {
            if (i == 0 || i == MAZE_HEIGHT + 1 || j == 0 || j == MAZE_WIDTH + 1) {
                maze[i][j] = ROAD;  // 边界设为路
            } else {
                maze[i][j] = WALL;  // 内部设为墙
            }
        }
    }
}

// 使用递归分割法生成迷宫
void generateMaze(int x, int y) {
    // 四个方向: 右, 下, 左, 上
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    
    // 随机打乱方向顺序
    for (int i = 0; i < 4; i++) {
        int j = rand() % 4;
        // 交换x方向
        int temp = directions[i][0];
        directions[i][0] = directions[j][0];
        directions[j][0] = temp;
        // 交换y方向
        temp = directions[i][1];
        directions[i][1] = directions[j][1];
        directions[j][1] = temp;
    }
    
    maze[x][y] = ROAD;
    
    // 尝试四个方向
    for (int i = 0; i < 4; i++) {
        int nx = x + 2 * directions[i][0];
        int ny = y + 2 * directions[i][1];
        
        // 检查是否在边界内且是墙
        if (nx > 0 && nx <= MAZE_HEIGHT && ny > 0 && ny <= MAZE_WIDTH && maze[nx][ny] == WALL) {
            // 打通当前格和下一格
            maze[x + directions[i][0]][y + directions[i][1]] = ROAD;
            generateMaze(nx, ny);
        }
    }
}

// 获取用户输入
Direction getInput() {
    char c = _getch();
    if (c == 27) return DIR_ESC;  // ESC键
    
    if (c == -32) {  // 方向键前缀
        c = _getch();
        switch (c) {
            case 72: return DIR_UP;
            case 80: return DIR_DOWN;
            case 75: return DIR_LEFT;
            case 77: return DIR_RIGHT;
        }
    }
    return DIR_ESC;  // 默认返回ESC
}

// 绘制单个地图单元
void drawCell(int x, int y) {
    moveCursor(2 * y - 2, x - 1);
    switch (maze[x][y]) {
        case START: printf("入"); break;
        case END:   printf("出"); break;
        case WALL:  printf("▇"); break;
        case ROAD:  printf(" "); break;
    }
}

// 绘制整个迷宫
void drawMaze() {
    for (int i = 1; i <= MAZE_HEIGHT; i++) {
        for (int j = 1; j <= MAZE_WIDTH; j++) {
            drawCell(i, j);
        }
    }
}

// 运行游戏主循环
void runGame() {
    int playerX = 2, playerY = 1;  // 玩家初始位置(入口)
    
    while (true) {
        // 绘制玩家位置
        moveCursor(2 * playerY - 2, playerX - 1);
        printf("A");
        
        // 检查是否到达终点
        if (maze[playerX][playerY] == END) {
            moveCursor(30, 24);
            printf("到达终点，按任意键结束");
            _getch();
            break;
        }
        
        // 获取输入并处理移动
        Direction dir = getInput();
        if (dir == DIR_ESC) {
            moveCursor(0, 24);
            break;
        }
        
        // 根据输入方向移动
        int newX = playerX, newY = playerY;
        switch (dir) {
            case DIR_UP:    newX--; break;
            case DIR_DOWN:  newX++; break;
            case DIR_LEFT:  newY--; break;
            case DIR_RIGHT: newY++; break;
            default: break;
        }
        
        // 检查移动是否有效(不是墙)
        if (maze[newX][newY] != WALL) {
            // 恢复原来位置的显示
            drawCell(playerX, playerY);
            // 更新玩家位置
            playerX = newX;
            playerY = newY;
        }
    }
}

int main() {
    // 初始化随机种子
    srand((unsigned)time(NULL));
    
    // 设置控制台
    hideCursor();
    
    // 初始化并生成迷宫
    initializeMaze();
    generateMaze(2 * (rand() % (MAZE_HEIGHT / 2) + 1), 2 * (rand() % (MAZE_WIDTH / 2) + 1));
    
    // 设置迷宫边界
    for (int i = 0; i <= MAZE_HEIGHT + 1; i++) {
        maze[i][0] = WALL;
        maze[i][MAZE_WIDTH + 1] = WALL;
    }
    for (int j = 0; j <= MAZE_WIDTH + 1; j++) {
        maze[0][j] = WALL;
        maze[MAZE_HEIGHT + 1][j] = WALL;
    }
    
    // 设置起点和终点
    maze[2][1] = START;
    maze[MAZE_HEIGHT - 1][MAZE_WIDTH] = END;
    
    // 绘制迷宫并开始游戏
    drawMaze();
    runGame();
    
    return 0;
}
