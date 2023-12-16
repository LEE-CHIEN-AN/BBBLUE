#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"

#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;


int Length = 15, Width = 10; // 定義地圖的長度和寬度
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
    DrawIcon(hdc, 2 * pixel, 1 * pixel, hIcon_des); // 機會與命運顯示
    for (int i = 1; i <= Width; i++)
    {
        DrawIcon(hdc, (Length + 1) * pixel, i * pixel, hIcon_rock);
    }
    for (int i = 1; i <= Length + 1; i++)
    {
        DrawIcon(hdc, i * pixel, (Width + 1) * pixel, hIcon_rock);
    }
}

void DesAndOpt_p1(int x, int y)
{
    if(mapArray[x][y] == 4)
    {
        int opportunityCase = generateRandomNumber(1, 6); // opportinity機會 : 改變步數
        switch(opportunityCase)
        {
            case 1:
                moveChange_p1 = 2;
            case 2:
                moveChange_p1 = 0.5;
            case 3:
                moveChange_p2 = 0.5;
            case 4:
                moveChange_p1 = 0;
            case 5:
                moveChange_p2 = 0;
            case 6:
                moveChange_p1 = 1.5;
        }
    }
    else if(mapArray[x][y] == 5)
    {
        int destinyCase = generateRandomNumber(1, 6); // destiny命運 : 改變障礙物跟位置
        switch(destinyCase)
        {
            case 1:
                if(mapArray[x-1][y] + mapArray[x+1][y] + mapArray[x][y-1] + mapArray[x][y+1] <= 5)
                {
                    mapArray[x-1][y] = 3;
                }
            case 2:
                if(mapArray[x-1][y] + mapArray[x+1][y] + mapArray[x][y-1] + mapArray[x][y+1] <= 5)
                {
                    mapArray[x+1][y] = 3;
                }
            case 3:
                if(mapArray[x-1][y] + mapArray[x+1][y] + mapArray[x][y-1] + mapArray[x][y+1] <= 5)
                {
                    mapArray[x][y-1] = 3;
                }
            case 4:
                if(mapArray[x-1][y] + mapArray[x+1][y] + mapArray[x][y-1] + mapArray[x][y+1] <= 5)
                {
                    mapArray[x][y+1] = 3;
                }
        }
    }
}

//void DesAndOpt_p1(int x, int y, )
//{
//    if(mapArray[])
//}

#pragma region WindowProc
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);

    switch (uMsg) {
        case WM_CREATE:
        {
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

            // 設置黑色背景
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            FillRect(hdc, &clientRect, blackBrush);

            if (!isGameStarted) //起始畫面
            {
                currentCharacter = 0;
                text = L"Press Enter to start...";
                SetTimer(hwnd, 1, 30, NULL);
            }
            else if(isGameStarted)
            {
                currentCharacter = 0;
                text = L"Please choose small (press 1), medium (press 2), or large (press 3)...";
                SetTimer(hwnd, 1, 30, NULL);
                // Check if initial drawing is done
                if (!isInitialDrawDone) {
                    printOutAll(hwnd);
                    DrawIcon(hdc, 1 * pixel, 1 * pixel, hIcon_player1);
                    DrawIcon(hdc, Length * pixel, Width * pixel, hIcon_player2);

                    isInitialDrawDone = true;
                }

                currentCharacter = 0;
                text = L"p1 steps : ";
                SetTimer(hwnd, 1, 30, NULL);
                DrawIcon(hdc, i * pixel, j * pixel, hIcon_water);
//                currentCharacter = 0;
//                wstring transfer = L"p1 steps : " + to_wstring(p1_moves);
//                text = transfer.c_str();
//                SetTimer(hwnd, 1, 30, NULL);
                cout << p1_moves << " " << p2_moves << endl;
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
                        if(p1_y > 1 && passing_arr_p1[p1_x][p1_y - 1] == 0)
                        {
                            DesAndOpt_p1(p1_x, p1_y);
                            cout << "up" << endl;
                            DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_flag);
                            p1_y -= 1;
                            DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_player1);
                            p1_moves--;
                            passing_arr_p1[p1_y][p1_x] = 1;
                            break;
                        }
                        else break;


                    case VK_DOWN:
                        if(p1_y < Width && passing_arr_p1[p1_y + 1][p1_x] == 0)
                        {
                            cout << "down" << endl;
                            DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_flag);
                            p1_y += 1;
                            DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_player1);
                            p1_moves--;
                            passing_arr_p1[p1_y][p1_x] = 1;
                            break;
                        }
                        else break;


                    case VK_LEFT:
                        if(p1_x > 1 && passing_arr_p1[p1_y][p1_x - 1] == 0)
                        {
                            cout << "left" << endl;
                            DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_flag);
                            p1_x -= 1;
                            DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_player1);
                            p1_moves--;
                            passing_arr_p1[p1_y][p1_x] = 1;
                            break;
                        }
                        else break;


                    case VK_RIGHT:
                        if(p1_x < Length && passing_arr_p1[p1_y][p1_x + 1] == 0)
                        {
                            cout << "right" << endl;
                            DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_flag);
                            p1_x += 1;
                            DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_player1);
                            p1_moves--;
                            passing_arr_p1[p1_y][p1_x] = 1;
                            break;
                        }
                        else break;

                    default:
                        break;
                }
                if(p1_moves == 0)
                {
                    currentPlayer = 2;
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
                            DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_player2);
                            p2_moves--;
                            passing_arr_p2[p2_y][p2_x] = 1;
                            break;
                        }
                        else break;

                    case 'S':
                        if(p2_y < Width && passing_arr_p2[p2_y + 1][p2_x] == 0)
                        {
                            cout << "S pressed" << endl;
                            DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_flag);
                            p2_y += 1;
                            DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_player2);
                            p2_moves--;
                            passing_arr_p2[p2_y][p2_x] = 1;
                            break;
                        }
                        else break;


                    case 'A':
                        if(p2_x > 1 && passing_arr_p2[p2_y][p2_x - 1] == 0)
                        {
                            cout << "A pressed" << endl;
                            DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_flag);
                            p2_x -= 1;
                            DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_player2);
                            p2_moves--;
                            passing_arr_p2[p2_y][p2_x] = 1;
                            break;
                        }
                        else break;


                    case 'D':
                        if(p2_x < Length && passing_arr_p2[p2_y][p2_x + 1] == 0)
                        {
                            cout << "D pressed" << endl;
                            DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_flag);
                            p2_x += 1;
                            DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_player2);
                            p2_moves--;
                            passing_arr_p2[p2_y][p2_x] = 1;
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

                    // clear all
                    RECT clientRect;
                    GetClientRect(hwnd, &clientRect);
                    FillRect(hdc, &clientRect, blackBrush);

                    printOutAll(hwnd);
                    DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_player1);
                    DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_player2);

                    currentPlayer = 1;
                    p1_moves = generateRandomNumber(2, 6);
                    p2_moves = generateRandomNumber(2, 6);

                    for(int i = 1 ; i <= Width ; i++)
                    {
                        for(int j = 1 ; j <= Length ; j++)
                        {
                            if(mapArray[i][j] == 1)
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
    //desAndOpt = new int*[Width + 1];
    for (int i = 0; i < Width + 1; i++) {
        mapArray[i] = new int[Length + 1];
        passing_arr_p1[i] = new int[Length + 1];
        passing_arr_p2[i] = new int [Length + 1];
        //desAndOpt[i] = new int[Length + 1];
        for (int j = 0; j < Length + 1; j++) {
            mapArray[i][j] = 0; // 初始化陣列值為 0
            passing_arr_p1[i][j] = 0;
            passing_arr_p2[i][j] = 0;
            //desAndOpt[i][j] = 0;
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
    for (int i = 1; i <= Width; i++) {
            for (int j = 1; j <= Length; j++) {
                cout << mapArray[i][j] << " ";
            }
            cout << endl;
        }

    //desAndOpt[1][2] = 1;
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
        //delete[] desAndOpt[i];
    }
    delete[] mapArray;
    delete[] passing_arr_p1;
    delete[] passing_arr_p2;
    //delete[] desAndOpt;
    return 0;
}
