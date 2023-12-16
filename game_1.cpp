#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"

#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

int Length = 20, Width = 10; // 定義地圖的長度和寬度
HICON hIcon_player1 = (HICON)LoadImage(NULL, L"JNG (2).ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_player2 = (HICON)LoadImage(NULL, L"HCR (2).ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_grass = (HICON)LoadImage(NULL, L"grass_try.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_water = (HICON)LoadImage(NULL, L"water_try.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_rock = (HICON)LoadImage(NULL, L"rock_try.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_flag = (HICON)LoadImage(NULL, L"flag.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_location = (HICON)LoadImage(NULL, L"location.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_des = (HICON)LoadImage(NULL, L"snowman (1).ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);

// Brushes
HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
HBRUSH grassBrush = CreateSolidBrush(RGB(0, 201, 87));
HBRUSH waterBrush = CreateSolidBrush(RGB(0, 255, 255));
HBRUSH rockBrush = CreateSolidBrush(RGB(192, 192, 192));

///--------------------------

// 方格的種類
enum TileType {
    GRASS, // 草地
    WATER, // 水域
    ROCK   // 岩石
};

struct Vector{
    int x;
    int y;
};

// 生成隨機數字的函數
int generateRandomNumber(int minValue, int maxValue) {
    return rand() % (maxValue - minValue + 1) + minValue;
}

#pragma region Tile// Tile 類別代表地圖上的單個方格
class Tile {
private:
    TileType type; // 方格的類型

public:
    Tile(TileType type){
        this->type = type;
    }
    Tile(){}
    TileType getTile(){
        return type;
    }
    void setTile(TileType type){
        this->type = type;
    }
};
#pragma endregion

#pragma region TileMap// TileMap 類別代表整個地圖
class TileMap {
private:
    int width;       // 地圖寬度
    int length;      // 地圖長度
    Tile*** TileMapObject; // 二維陣列，儲存地圖上的每個方格

public:
    TileMap(int width, int length){
        this->width = width;
        this->length = length;

        // 初始化地圖陣列
        TileMapObject = new Tile** [length];
        for (int i = 0; i < length; i++){
            TileMapObject[i] = new Tile* [width];
            for (int j = 0; j < width; j++){
                TileMapObject[i][j] = new Tile();
            }
        }
        srand(static_cast<unsigned>(time(0)));
    }
    ~TileMap() {
        // 清理記憶體
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < width; j++) {
                delete TileMapObject[i][j];
            }
            delete[] TileMapObject[i];
        }
        delete[] TileMapObject;
    }

    // 獲取指定位置的方格類型
    TileType getTileType(int x, int y) const {
        return TileMapObject[y][x]->getTile();
    }

    // 設置指定位置的方格類型
    void setTileType(int x, int y, TileType type) {
        TileMapObject[y][x]->setTile(type);
    }

    // 打印地圖到控制台
    void printTileMap() const {
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < width; j++) {
                switch (getTileType(j, i)) {
                    case GRASS:
                        cout << " 1 "; break;
                    case WATER:
                        cout << " 2 "; break;
                    case ROCK:
                        cout << " 3 "; break;
                }
            }
            cout << endl;
        }
    }

    // 生成水域
    void generateWaterMass() {
        int waterMassWidth = rand() % 3 + 2; // 水域寬度
        int waterMassLength = rand() % 3 + 2; // 水域長度

        int startX = rand() % (width - waterMassWidth + 1);
        int startY = rand() % (length - waterMassLength + 1);

        int endX = startX + waterMassWidth - 1;
        int endY = startY + waterMassLength - 1;

        for (int i = startY; i <= endY; i++) {
            for (int j = startX; j <= endX; j++) {
                setTileType(j, i, WATER);
            }
        }
    }

    // 隨機生成岩石
    void randomRocks(int rockCnt) {
        for (int i = 0; i < rockCnt; i++) {
            int x = rand() % width;
            int y = rand() % length;

            // 只有當方格是草地時，才設置為岩石
            if (getTileType(x, y) == GRASS) {
                setTileType(x, y, ROCK);
            } else {
                i--;
            }
        }
    }
};
#pragma endregion

int** mapArray; // 儲存地圖數據的陣列

///---------------------------

int cnt_UpDown = 0;
int cnt_LeftRight = 0;

int cnt_UpDown_player2 = 100;
int cnt_LeftRight_player2 = 100;

int p1_x = 1, p1_y = 1, p1_moves = generateRandomNumber(2, 6);
int p2_x = Length, p2_y = Width, p2_moves = generateRandomNumber(2, 6);

bool isInitialDrawDone = false;

int currentPlayer = 1;

int pixel = 32;

int** passing_arr_p1;
int** passing_arr_p2;

//int** desAndOpt;

int currentCharacter = 0;

bool isGameStarted = false;

double moveChange_p1 = 1, moveChange_p2 = 1;

wchar_t* stepText_p1;
wchar_t* stepText_p2;
wchar_t* condition_text;

int conditionTextCnt = 0;

wchar_t* text;
void HandleTimer(HWND hwnd, wchar_t* displayText)
{
    HDC hdc = GetDC(hwnd);
    const wchar_t currentChar = displayText[currentCharacter];
    SetBkColor(hdc, RGB(0, 0, 0));  // 設置黑色背景顏色
    SetTextColor(hdc, RGB(255, 255, 255));  // 設置白色文本顏色
    TextOutW(hdc, pixel + 10 + currentCharacter * 10, (Width + 2) * pixel + 10, &currentChar, 1); // 逐個字符顯示

    currentCharacter++; // 增加字符index

    // 如果所有字符都已經顯示，停止定時器
    if (currentCharacter >= wcslen(displayText)) KillTimer(hwnd, 1);   // 停止定時器
    ReleaseDC(hwnd, hdc);
}

void printOutAll(HWND hwnd)
{
    HDC hdc = GetDC(hwnd);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    // clear all
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    RECT customRect = {(Length + 2) * pixel, 190, screenWidth-pixel-10, screenHeight-pixel-10};
    ExcludeClipRect(hdc, customRect.left, customRect.top, customRect.right, customRect.bottom);
    FillRect(hdc, &clientRect, blackBrush);

    for (int i = 0; i <= screenWidth/pixel; i++)
    {
        DrawIcon(hdc, i * pixel, 0, hIcon_rock);
        DrawIcon(hdc, i * pixel, screenHeight-pixel, hIcon_rock);
    }
    for (int j = 0; j <= screenHeight/pixel; j++)
    {
        DrawIcon(hdc, 0, j * pixel, hIcon_rock);
        DrawIcon(hdc, screenWidth-pixel, j * pixel, hIcon_rock);
    }
    // Draw the balack rectangles here
    for (int i = 1; i <= Length; i++) {
        for (int j = 1; j <= Width; j++) {
            if(mapArray[j][i] == 1) DrawIcon(hdc, i * pixel, j * pixel, hIcon_grass);
            else if(mapArray[j][i] == 2) DrawIcon(hdc, i * pixel, j * pixel, hIcon_water);
            else if(mapArray[j][i] == 3) DrawIcon(hdc, i * pixel, j * pixel, hIcon_rock);
        }
    }

    for (int i = 1; i <= screenHeight/pixel; i++)
    {
        DrawIcon(hdc, (Length + 1) * pixel, i * pixel, hIcon_rock);
    }
    for (int i = 1; i <= Length + 1; i++)
    {
        DrawIcon(hdc, i * pixel, (Width + 1) * pixel, hIcon_rock);
    }
    for (int i = Length + 2; i <= screenWidth/pixel ; i++)
    {
        DrawIcon(hdc, i * pixel, 150, hIcon_rock);
    }

    DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_player1);
    DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_player2);

    stepText_p1 = L"p1 steps : ";
    stepText_p2 = L"p2 steps : ";
    SetBkColor(hdc, RGB(0, 0, 0));  // 設置黑色背景顏色
    SetTextColor(hdc, RGB(255, 255, 255));  // 設置白色文本顏色
    TextOutW(hdc, (Length + 2) * pixel + 10, 40, stepText_p1, static_cast<int>(wcslen(stepText_p1)));
    TextOutW(hdc, (Length + 2) * pixel + 10, 90, stepText_p2, static_cast<int>(wcslen(stepText_p2)));
    for(int i = 0 ; i < p1_moves ; i++)
    {
        DrawIcon(hdc, (Length + 5) * pixel + i * pixel, 40, hIcon_player1);
    }
}

int cntAroundObstacles(int player, int x, int y)
{
    if(x == 1 || x == Length || y == 1 || y == Width) return 0;
    int cnt = 0;
    if(player == 1)
    {
        if(passing_arr_p1[y-1][x] == 1) cnt++;
        if(passing_arr_p1[y+1][x] == 1) cnt++;
        if(passing_arr_p1[y][x+1] == 1) cnt++;
        if(passing_arr_p1[y][x-1] == 1) cnt++;
    }
    else if(player == 2)
    {
        if(passing_arr_p2[y-1][x] == 1) cnt++;
        if(passing_arr_p2[y+1][x] == 1) cnt++;
        if(passing_arr_p2[y][x+1] == 1) cnt++;
        if(passing_arr_p2[y][x-1] == 1) cnt++;
    }
    return cnt;
}

void BlackHole(int player, int x, int y, HWND hwnd)
{
    HDC hdc = GetDC(hwnd);
    mapArray[y][x] = 1;
    if(player == 1)
    {
        DrawIcon(hdc, x * pixel, y * pixel, hIcon_player1);
        DrawIcon(hdc, x * pixel, y * pixel, hIcon_flag);
        int isNewPositionLegal = false;
        while(isNewPositionLegal == false)
        {
            p1_x = generateRandomNumber(1, Length);
            p1_y = generateRandomNumber(1, Width);
            if(passing_arr_p1[p1_y][p1_x] == 0 && mapArray[p1_y][p1_x] == 1 && p1_x != x && p1_y != y && p1_x != p2_x && p1_y != p2_y && cntAroundObstacles(1, p1_x, p1_y) <= 2)
            {
                isNewPositionLegal = true;
                DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_player1);
            }
        }
    }
    else if(player == 2)
    {
        DrawIcon(hdc, x * pixel, y * pixel, hIcon_player2);
        DrawIcon(hdc, x * pixel, y * pixel, hIcon_flag);
        int isNewPositionLegal = false;
        while(isNewPositionLegal == false)
        {
            p2_x = generateRandomNumber(1, Length);
            p2_y = generateRandomNumber(1, Width);
            if(passing_arr_p2[p2_y][p2_x] == 0 && mapArray[p2_y][p2_x] == 1 && p2_x != x && p2_y != y && p2_x != p1_x && p2_y != p1_y && cntAroundObstacles(2, p2_x, p2_y) <= 2)
            {
                isNewPositionLegal = true;
                DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_player2);
            }
        }
    }
}

void conditionText(int player, int DandO, int Case, HWND hwnd)
{
    HDC hdc = GetDC(hwnd);
    SetBkColor(hdc, RGB(0, 0, 0));  // 設置黑色背景顏色
    SetTextColor(hdc, RGB(255, 255, 255));  // 設置白色文本顏色

    wchar_t* player1or2;
    if(player == 1) player1or2 = L"player 1 : ";
    else if(player == 2) player1or2 = L"player 2 : ";
    TextOutW(hdc, (Length + 2) * pixel + 10, 200 + conditionTextCnt * pixel, player1or2, static_cast<int>(wcslen(player1or2)));

    if(DandO == 4)
    {
        switch(Case)
        {
            case 1:
                condition_text = L"next steps x 2";
                break;
            case 2:
                condition_text = L"next steps x 0.5";
                break;
            case 3:
                condition_text = L"opponent's next steps x 0.5";
                break;
            case 4:
                condition_text = L"next steps x 0";
                break;
            case 5:
                condition_text = L"opponent's next steps x 0";
                break;
            case 6:
                condition_text = L"next steps x 1.5";
                break;
        }
    }
    else if(DandO == 5)
    {
        switch(Case)
        {
            case 1:
                condition_text = L"add a rock above you";
                break;
            case 2:
                condition_text = L"add a rock below you";
                break;
            case 3:
                condition_text = L"add a rock on your right";
                break;
            case 4:
                condition_text = L"add a rock on your left";
                break;
            case 5:
                condition_text = L"remove all obstacles around you";
                break;
            case 10:    // 黑洞傳送
                condition_text = L"there's no way to go, transfer to a new place.";
                break;
        }
    }
    TextOutW(hdc, (Length + 2) * pixel + 100, 200 + conditionTextCnt * pixel, condition_text, static_cast<int>(wcslen(condition_text)));
    conditionTextCnt++;
    if(conditionTextCnt > 21)
    {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        RECT customRect = {(Length + 2) * pixel, 190, screenWidth-pixel-10, screenHeight-pixel-10};
        FillRect(hdc, &customRect, blackBrush);
        conditionTextCnt = 0;
    }
}

void destinyModule(int player, int x, int y, int destinyCase, HWND hwnd) //此xy是加減過的
{
    HDC hdc = GetDC(hwnd);
    if(player == 1)
    {
        mapArray[y][x] = 3;
        passing_arr_p1[y][x] = 1;
        DrawIcon(hdc, x * pixel, y * pixel, hIcon_rock);
        conditionText(1, 5, destinyCase, hwnd);
    }
    else if(player == 2)
    {
        mapArray[y][x] = 3;
        passing_arr_p2[y][x] = 1;
        DrawIcon(hdc, x * pixel, y * pixel, hIcon_rock);
        conditionText(2, 5, destinyCase, hwnd);
    }
}

void destinyCase5Close(int player, int x, int y, HWND hwnd) // xy是加減過的
{
    HDC hdc = GetDC(hwnd);
    if(player == 1)
    {
        if(x != p2_x && y != p2_y)
        {
            mapArray[y][x] = 1;
            DrawIcon(hdc, x * pixel, y * pixel, hIcon_grass);
        }
    }
    else if(player == 2)
    {
        if(x != p1_x && y != p1_y)
        {
            mapArray[y][x] = 1;
            DrawIcon(hdc, x * pixel, y * pixel, hIcon_grass);
        }
    }
}

void DesAndOpt_p1(int x, int y, HWND hwnd)
{
    HDC hdc = GetDC(hwnd);
    if(mapArray[y][x] == 4)
    {
        int opportunityCase = generateRandomNumber(1, 6); // opportinity機會 : 改變步數
        switch(opportunityCase)
        {
            case 1:
                moveChange_p1 = 2;
                break;
            case 2:
                moveChange_p1 = 0.5;
                break;
            case 3:
                moveChange_p2 = 0.5;
                break;
            case 4:
                moveChange_p1 = 0;
                break;
            case 5:
                moveChange_p2 = 0;
                break;
            case 6:
                moveChange_p1 = 1.5;
                break;
        }
        mapArray[y][x] = 1;
        DrawIcon(hdc, x * pixel, y * pixel, hIcon_grass);
        conditionText(1, 4, opportunityCase, hwnd);
    }
    else if(mapArray[y][x] == 5)
    {
        int destinyCase = generateRandomNumber(1, 5); // destiny命運 : 改變障礙物跟位置
        int cnt = cntAroundObstacles(1, x, y);
        if(destinyCase == 5)
        {
            mapArray[y][x] = 1;
            DrawIcon(hdc, x * pixel, y * pixel, hIcon_grass);

            destinyCase5Close(1, x-1, y, hwnd);
            destinyCase5Close(1, x+1, y, hwnd);
            destinyCase5Close(1, x, y-1, hwnd);
            destinyCase5Close(1, x, y+1, hwnd);

            conditionText(1, 5, destinyCase, hwnd);
        }
        else
        {
            if(cnt <= 3)
            {
                mapArray[y][x] = 1;
                DrawIcon(hdc, x * pixel, y * pixel, hIcon_grass);
                switch(destinyCase)
                {
                    case 1:
                        destinyModule(1, x, y-1, destinyCase, hwnd);
                        break;
                    case 2:
                        destinyModule(1, x, y+1, destinyCase, hwnd);
                        break;
                    case 3:
                        destinyModule(1, x+1, y, destinyCase, hwnd);
                        break;
                    case 4:
                        destinyModule(1, x-1, y, destinyCase, hwnd);
                        break;
                }
            }
            else if(cnt > 3)
            {
                BlackHole(1, x, y, hwnd);
                conditionText(1, 5, 10, hwnd);
            }
        }
    }

    RECT rect;
    rect = {(Length + 5) * pixel, 40, (Length + 5 + 15) * pixel + 50, 40 + 50};
    FillRect(hdc, &rect, blackBrush);
    p1_moves = 1;
}

void DesAndOpt_p2(int x, int y, HWND hwnd)
{
    HDC hdc = GetDC(hwnd);
    if(mapArray[y][x] == 4)
    {
        int opportunityCase = generateRandomNumber(1, 6); // opportinity機會 : 改變步數
        switch(opportunityCase)
        {
            case 1:
                moveChange_p2 = 2;
                break;
            case 2:
                moveChange_p2 = 0.5;
                break;
            case 3:
                moveChange_p1 = 0.5;
                break;
            case 4:
                moveChange_p2 = 0;
                break;
            case 5:
                moveChange_p1 = 0;
                break;
            case 6:
                moveChange_p2 = 1.5;
                break;
        }
        mapArray[y][x] = 1;
        DrawIcon(hdc, x * pixel, y * pixel, hIcon_grass);
        conditionText(2, 4, opportunityCase, hwnd);
    }
    else if(mapArray[y][x] == 5)
    {
        int destinyCase = generateRandomNumber(1, 5); // destiny命運 : 改變障礙物跟位置
        int cnt = cntAroundObstacles(2, x, y);
        if(destinyCase == 5)
        {
            mapArray[y][x] = 1;
            DrawIcon(hdc, x * pixel, y * pixel, hIcon_grass);

            destinyCase5Close(2, x-1, y, hwnd);
            destinyCase5Close(2, x+1, y, hwnd);
            destinyCase5Close(2, x, y-1, hwnd);
            destinyCase5Close(2, x, y+1, hwnd);

            conditionText(2, 5, destinyCase, hwnd);
        }
        else
        {
            if(cnt <= 3)
            {
                mapArray[y][x] = 1;
                DrawIcon(hdc, x * pixel, y * pixel, hIcon_grass);
                switch(destinyCase)
                {
                    case 1:
                        destinyModule(2, x, y-1, destinyCase, hwnd);
                        break;
                    case 2:
                        destinyModule(2, x, y+1, destinyCase, hwnd);
                        break;
                    case 3:
                        destinyModule(2, x+1, y, destinyCase, hwnd);
                        break;
                    case 4:
                        destinyModule(2, x-1, y, destinyCase, hwnd);
                        break;
                }
            }
            else if(cnt > 3)
            {
                BlackHole(2, x, y, hwnd);
                conditionText(2, 5, 10, hwnd);
            }
        }
    }

    RECT rect;
    rect = {(Length + 5) * pixel, 90, (Length + 5 + 15) * pixel + 50, 90 + 50};
    FillRect(hdc, &rect, blackBrush);
    p2_moves = 1;
}

void UpDownLeftRight(int player, HWND hwnd)
{
    RECT rect;
    HDC hdc = GetDC(hwnd);
    if(player == 1)
    {
        rect = {(Length + 5 + p1_moves - 1) * pixel, 40, (Length + 5 + p1_moves - 1) * pixel + 50, 40 + 50};
        FillRect(hdc, &rect, blackBrush);
        if(mapArray[p1_y][p1_x] == 4 || mapArray[p1_y][p1_x] == 5) DesAndOpt_p1(p1_x, p1_y, hwnd);
        DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_player1);
        p1_moves--;
        passing_arr_p1[p1_y][p1_x] = 1;

        if(cntAroundObstacles(1, p1_x, p1_y) > 3)
        {
            BlackHole(1, p1_x, p1_y, hwnd);
            conditionText(1, 5, 10, hwnd);
        }
    }
    else if(player == 2)
    {
        rect = {(Length + 5 + p2_moves - 1) * pixel, 90, (Length + 5 + p2_moves - 1) * pixel + 50, 90 + 50};
        FillRect(hdc, &rect, blackBrush);
        if(mapArray[p2_y][p2_x] == 4 || mapArray[p2_y][p2_x] == 5) DesAndOpt_p2(p2_x, p2_y, hwnd);
        DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_player2);
        p2_moves--;
        passing_arr_p2[p2_y][p2_x] = 1;

        if(cntAroundObstacles(2, p2_x, p2_y) > 3)
        {
            BlackHole(2, p2_x, p2_y, hwnd);
            conditionText(2, 5, 10, hwnd);
        }
    }
}

#pragma region WindowProc
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);

    switch (uMsg) {
        case WM_CREATE:
        {
            srand(static_cast<unsigned int>(time(nullptr)));
            // Make the window fullscreen
            SetWindowPos(hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
            break;
        }
        case WM_TIMER:
        {
            HandleTimer(hwnd, text);
            break;
        }
        case WM_PAINT:
        {
            passing_arr_p1[1][1] = 1;
            passing_arr_p2[Width][Length] = 1;

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if (!isGameStarted) //起始畫面
            {
                // 設置黑色背景
                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                FillRect(hdc, &clientRect, blackBrush);

                currentCharacter = 0;
                text = L"Press Enter to start...";
                SetTimer(hwnd, 1, 30, NULL);
            }
            else if(isGameStarted)
            {
                currentCharacter = 0;
                text = L"Please choose small (press 1)";
                SetTimer(hwnd, 1, 30, NULL);

                // Check if initial drawing is done
                if (!isInitialDrawDone) {
                    printOutAll(hwnd);
                    isInitialDrawDone = true;
                }
            }

            EndPaint(hwnd, &ps);
            break;
        }
        default:
            break;
    }


    wchar_t msg[32];
    switch (uMsg)
    {
        case WM_KEYDOWN:
        {
            if(wParam == VK_ESCAPE)
            {
                PostQuitMessage(0); // 結束程序
                ShowCursor(TRUE);   // 顯示鼠標
            }
            if(wParam == VK_RETURN && isGameStarted == false) { // 假設 Enter 鍵啟動遊戲
                isGameStarted = true;
                InvalidateRect(hwnd, NULL, TRUE); // 通知窗口需要重新繪製
            }
            swprintf_s(msg, L"WM_KEYDOWN: 0x%x\n", wParam);
            OutputDebugStringW(msg);
            cout << "WM_KEYDOWN" << endl;

            RECT rect;

            if (currentPlayer == 1 && isGameStarted == true)
            {
                switch (wParam)
                {
                    case VK_UP:
                        if(p1_y > 1 && passing_arr_p1[p1_y - 1][p1_x] == 0)
                        {
                            cout << "up" << endl;
                            DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_flag);
                            p1_y -= 1;
                            UpDownLeftRight(1, hwnd);
                            break;
                        }
                        else break;


                    case VK_DOWN:
                        if(p1_y < Width && passing_arr_p1[p1_y + 1][p1_x] == 0)
                        {
                            cout << "down" << endl;
                            DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_flag);
                            p1_y += 1;
                            UpDownLeftRight(1, hwnd);
                            break;
                        }
                        else break;


                    case VK_LEFT:
                        if(p1_x > 1 && passing_arr_p1[p1_y][p1_x - 1] == 0)
                        {
                            cout << "left" << endl;
                            DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_flag);
                            p1_x -= 1;
                            UpDownLeftRight(1, hwnd);
                            break;
                        }
                        else break;


                    case VK_RIGHT:
                        if(p1_x < Length && passing_arr_p1[p1_y][p1_x + 1] == 0)
                        {
                            cout << "right" << endl;
                            DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_flag);
                            p1_x += 1;
                            UpDownLeftRight(1, hwnd);
                            break;
                        }
                        else break;

                    default:
                        break;
                }
                if(p1_moves == 0)
                {
                    currentPlayer = 2;
                    for(int i = 0 ; i < p2_moves ; i++)
                    {
                        DrawIcon(hdc, (Length + 5) * pixel + i * pixel, 90, hIcon_player2);
                    }
                }
            }
            else if (currentPlayer == 2 && isGameStarted == true)
            {
                switch (wParam)
                {
                    case 'W':
                        if(p2_y > 1 && passing_arr_p2[p2_y - 1][p2_x] == 0)
                        {
                            cout << "W pressed" << endl;
                            DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_flag);
                            p2_y -= 1;
                            UpDownLeftRight(2, hwnd);
                            break;
                        }
                        else break;

                    case 'S':
                        if(p2_y < Width && passing_arr_p2[p2_y + 1][p2_x] == 0)
                        {
                            cout << "S pressed" << endl;
                            DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_flag);
                            p2_y += 1;
                            UpDownLeftRight(2, hwnd);
                            break;
                        }
                        else break;


                    case 'A':
                        if(p2_x > 1 && passing_arr_p2[p2_y][p2_x - 1] == 0)
                        {
                            cout << "A pressed" << endl;
                            DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_flag);
                            p2_x -= 1;
                            UpDownLeftRight(2, hwnd);
                            break;
                        }
                        else break;


                    case 'D':
                        if(p2_x < Length && passing_arr_p2[p2_y][p2_x + 1] == 0)
                        {
                            cout << "D pressed" << endl;
                            DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_flag);
                            p2_x += 1;
                            UpDownLeftRight(2, hwnd);
                            break;
                        }
                        else break;

                    default:
                        break;
                }
                if(p2_moves == 0)
                {
                    // Switch turns after a move
                    isInitialDrawDone = 0;
                    // Reset moves for the next player

                    p1_moves = int(generateRandomNumber(2, 6) * moveChange_p1);
                    p2_moves = int(generateRandomNumber(2, 6) * moveChange_p1);

                    moveChange_p1 = 1;
                    moveChange_p2 = 1;

                    printOutAll(hwnd);

                    currentPlayer = 1;

                    for(int i = 1 ; i <= Width ; i++)
                    {
                        for(int j = 1 ; j <= Length ; j++)
                        {
                            if(mapArray[i][j] == 1 || mapArray[i][j] == 4 || mapArray[i][j] == 5)
                            {
                                passing_arr_p1[i][j] = 0;
                                passing_arr_p2[i][j] = 0;
                            }
                        }
                    }

                    passing_arr_p1[p1_y][p1_x] = 1;
                    passing_arr_p2[p2_y][p2_x] = 1;
                }
            }

            if (p1_x == p2_x && p1_y == p2_y)
            {
                ShowCursor(TRUE);
                PostQuitMessage(0); // 結束程序
            }
            ReleaseDC(hwnd, hdc);
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
#pragma endregion

int main()
{
    ///--------------------------------
    mapArray = new int*[Width + 1];
    passing_arr_p1 = new int*[Width + 1];
    passing_arr_p2 = new int*[Width + 1];
    for (int i = 0; i < Width + 1; i++) {
        mapArray[i] = new int[Length + 1];
        passing_arr_p1[i] = new int[Length + 1];
        passing_arr_p2[i] = new int [Length + 1];
        for (int j = 0; j < Length + 1; j++) {
            mapArray[i][j] = 0; // 初始化陣列值為 0
            passing_arr_p1[i][j] = 0;
            passing_arr_p2[i][j] = 0;
        }
    }
    //建立地圖
    TileMap myTileMap(Length + 1, Width + 1);

    for (int i = 1; i <= Width; i++) {
        for (int j = 1; j <= Length; j++) {
            myTileMap.setTileType(j, i, GRASS);
        }
    }

    //加入湖水
    int lakeCnt = generateRandomNumber(static_cast<int>(Length * Width * 0.02),
                                       static_cast<int>(Length * Width * 0.04));
    //cout << lakeCnt << endl;
    for (int i = 1; i <= lakeCnt; i++)
        myTileMap.generateWaterMass();

    //加入岩石
    int rockCnt = generateRandomNumber(static_cast<int>(Length * Width * 0.05),
                                       static_cast<int>(Length * Width * 0.1));
    //cout << rockCnt << endl;
    myTileMap.randomRocks(rockCnt);

    for (int i = 1; i <= Width; i++){
        for (int j = 1; j <= Length; j++){
            if (i == 1 || i == Width)
                myTileMap.setTileType(j, i, GRASS);
            if (j == 1 || j == Length)
                myTileMap.setTileType(j, i, GRASS);
        }
    }

    myTileMap.printTileMap();

    for (int i = 1; i <= Width; i++) {
            for (int j = 1; j <= Length; j++) {
                switch (myTileMap.getTileType(j, i)) {
                    case GRASS:
                        mapArray[i][j] = 1;
                        break;
                    case WATER:
                        mapArray[i][j] = 2;
                        passing_arr_p1[i][j] = 1;
                        passing_arr_p2[i][j] = 1;
                        break;
                    case ROCK:
                        mapArray[i][j] = 3;
                        passing_arr_p1[i][j] = 1;
                        passing_arr_p2[i][j] = 1;
                        break;
//                    case []:
//                        mapArray[i][j] = 4;
//                        desAndOpt[i][j] = 1; // des
//                    case []:
//                        mapArray[i][j] = 5;
//                        desAndOpt[i][j] = 2; // opt
                }
            }
        }
    //Test
    //mapArray[4][4] = 4;
    mapArray[5][5] = 5;
    mapArray[3][4] = 5;
    mapArray[3][6] = 5;
    mapArray[3][8] = 5;
    mapArray[6][7] = 5;
    mapArray[6][3] = 5;
    mapArray[8][2] = 5;
    for (int i = 1; i <= Width; i++) {
            for (int j = 1; j <= Length; j++) {
                cout << mapArray[i][j] << " ";
            }
            cout << endl;
        }
    ///----------------------------------

    ShowCursor(FALSE);
    // 註冊視窗
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.lpszClassName = L"SampleWindowClass";
    RegisterClassW(&wc);

    // 創建視窗
    HWND hwnd = CreateWindowExW(0, L"SampleWindowClass", L"Sample Window", WS_OVERLAPPEDWINDOW,
        0, 0, 0, 0, NULL, NULL, GetModuleHandleW(NULL), NULL);

    if (hwnd == NULL)
    {
        cerr << "視窗創建失敗！" << endl;
        ShowCursor(TRUE);
        return 1;
    }

    // 設置為全螢幕
    SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);
    SetWindowPos(hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);

    // 顯示視窗
    ShowWindow(hwnd, SW_SHOWDEFAULT);

    // 進入消息循環
    MSG msg = {};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    ShowCursor(TRUE);

    // 恢復編譯器警告設定
    #pragma GCC diagnostic pop

    // 在程序結束前釋放記憶體
    for (int i = 0; i < Width + 1; i++) {
        delete[] mapArray[i];
        delete[] passing_arr_p1[i];
        delete[] passing_arr_p2[i];
    }
    delete[] mapArray;
    delete[] passing_arr_p1;
    delete[] passing_arr_p2;
    return 0;
}
