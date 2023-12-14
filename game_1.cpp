#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"

#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int Length = 15, Width = 5; // 定義地圖的長度和寬度

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

int p1_x = 1, p1_y = 1, p1_moves = 3;
int p2_x = Length, p2_y = Width, p2_moves = 3;

bool isInitialDrawDone = false;

int currentPlayer = 1;

int** passing_arr_p1;
int** passing_arr_p2;

#pragma region WindowProc
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);
    const wchar_t* text;
    //HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    //SelectObject(hdc, hFont);
    text = L"Please choose small (press 1), medium (press 2), or large (press 3).";
    TextOutW(hdc, 10, 10, text, wcslen(text));
    ReleaseDC(hwnd, hdc);  // 釋放裝置內文

    /// 方框
    hdc = GetDC(hwnd);

    switch (uMsg) {
        case WM_PAINT:
        {
            passing_arr_p1[1][1] = 1;
            passing_arr_p2[Width][Length] = 1;

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
            HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
            HBRUSH grassBrush = CreateSolidBrush(RGB(0, 201, 87));
            HBRUSH waterBrush = CreateSolidBrush(RGB(0, 255, 255));
            HBRUSH rockBrush = CreateSolidBrush(RGB(192, 192, 192));

            // Check if initial drawing is done
            if (!isInitialDrawDone) {
                // Draw the balack rectangles here
                for (int i = 1; i <= Length; i++) {
                    for (int j = 1; j <= Width; j++) {
                        RECT rect = {i * 50, j * 50, (i + 1) * 50, (j + 1) * 50};
                        if(mapArray[j][i] == 1) FillRect(hdc, &rect, grassBrush);
                        else if(mapArray[j][i] == 2) FillRect(hdc, &rect, waterBrush);
                        else if(mapArray[j][i] == 3) FillRect(hdc, &rect, rockBrush);
                    }
                }
                RECT rect;
                rect = {1 * 50, 1 * 50, (1 + 1) * 50, (1 + 1) * 50};
                FillRect(hdc, &rect, redBrush);

                rect = {Length * 50, Width * 50, (Length + 1) * 50, (Width + 1) * 50};
                FillRect(hdc, &rect, blueBrush);

                isInitialDrawDone = true;
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
            swprintf_s(msg, L"WM_KEYDOWN: 0x%x\n", wParam);
            OutputDebugStringW(msg);
            cout << "WM_KEYDOWN" << endl;

            HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
            HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
            HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));

            RECT rect;

            rect = {p1_x * 50 + 20, p1_y * 50 + 20, (p1_x + 1) * 50 - 20, (p1_y + 1) * 50 - 20};
            FillRect(hdc, &rect, redBrush);

            rect = {p2_x * 50 + 20, p2_y * 50 + 20, (p2_x + 1) * 50 - 20, (p2_y + 1) * 50 - 20};
            FillRect(hdc, &rect, blueBrush);


            if (currentPlayer == 1)
            {
                switch (wParam)
                {
                    case VK_UP:
                        if(p1_y > 1 && passing_arr_p1[p1_x][p1_y - 1] == 0)
                        {
                            cout << "up" << endl;
                            p1_y -= 1;
                            rect = { p1_x * 50, p1_y * 50, p1_x * 50 + 50, p1_y * 50 + 50 }; //紅色方框
                            FillRect(hdc, &rect, redBrush);
                            p1_moves--;
                            passing_arr_p1[p1_y][p1_x] = 1;
                            break;
                        }
                        else break;


                    case VK_DOWN:
                        if(p1_y < Width && passing_arr_p1[p1_y + 1][p1_x] == 0)
                        {
                            cout << "down" << endl;
                            p1_y += 1;
                            rect = { p1_x * 50, p1_y * 50, p1_x * 50 + 50, p1_y * 50 + 50 }; //紅色方框
                            FillRect(hdc, &rect, redBrush);
                            p1_moves--;
                            passing_arr_p1[p1_y][p1_x] = 1;
                            break;
                        }
                        else break;


                    case VK_LEFT:
                        if(p1_x > 1 && passing_arr_p1[p1_y][p1_x - 1] == 0)
                        {
                            cout << "left" << endl;
                            p1_x -= 1;
                            rect = { p1_x * 50, p1_y * 50, p1_x * 50 + 50, p1_y * 50 + 50 }; //紅色方框
                            FillRect(hdc, &rect, redBrush);
                            p1_moves--;
                            passing_arr_p1[p1_y][p1_x] = 1;
                            break;
                        }
                        else break;


                    case VK_RIGHT:
                        if(p1_x < Length && passing_arr_p1[p1_y][p1_x + 1] == 0)
                        {
                            cout << "right" << endl;
                            p1_x += 1;
                            rect = { p1_x * 50, p1_y * 50, p1_x * 50 + 50, p1_y * 50 + 50 }; //紅色方框
                            FillRect(hdc, &rect, redBrush);
                            p1_moves--;
                            passing_arr_p1[p1_y][p1_x] = 1;
                            break;
                        }
                        else break;

                    default:
                        break;
                }
                if(p1_moves != 3)
                {
                    rect = {p1_x * 50 + 20, p1_y * 50 + 20, (p1_x + 1) * 50 - 20, (p1_y + 1) * 50 - 20};
                    FillRect(hdc, &rect, whiteBrush);
                }
                if(p1_moves == 0)
                {
                    currentPlayer = 2;
                    rect = {p1_x * 50 + 20, p1_y * 50 + 20, (p1_x + 1) * 50 - 20, (p1_y + 1) * 50 - 20};
                    FillRect(hdc, &rect, redBrush);
                    rect = {p2_x * 50 + 20, p2_y * 50 + 20, (p2_x + 1) * 50 - 20, (p2_y + 1) * 50 - 20};
                    FillRect(hdc, &rect, whiteBrush);
                }
            }
            else if (currentPlayer == 2)
            {
                switch (wParam)
                {
                    case 'W':
                        if(p2_y > 1 && passing_arr_p2[p2_y - 1][p2_x] == 0)
                        {
                            cout << "W pressed" << endl;
                            p2_y -= 1;
                            rect = { p2_x * 50, p2_y * 50, p2_x * 50 + 50, p2_y * 50 + 50 }; //藍色方框
                            FillRect(hdc, &rect, blueBrush);
                            p2_moves--;
                            passing_arr_p2[p2_y][p2_x] = 1;
                            break;
                        }
                        else break;

                    case 'S':
                        if(p2_y < Width && passing_arr_p2[p2_y + 1][p2_x] == 0)
                        {
                            cout << "S pressed" << endl;
                            p2_y += 1;
                            rect = { p2_x * 50, p2_y * 50, p2_x * 50 + 50, p2_y * 50 + 50 }; //藍色方框
                            FillRect(hdc, &rect, blueBrush);
                            p2_moves--;
                            passing_arr_p2[p2_y][p2_x] = 1;
                            break;
                        }
                        else break;


                    case 'A':
                        if(p2_x > 1 && passing_arr_p2[p2_y][p2_x - 1] == 0)
                        {
                            cout << "A pressed" << endl;
                            p2_x -= 1;
                            rect = { p2_x * 50, p2_y * 50, p2_x * 50 + 50, p2_y * 50 + 50 }; //藍色方框
                            FillRect(hdc, &rect, blueBrush);
                            p2_moves--;
                            passing_arr_p2[p2_y][p2_x] = 1;
                            break;
                        }
                        else break;


                    case 'D':
                        if(p2_x < Length && passing_arr_p2[p2_y][p2_x + 1] == 0)
                        {
                            cout << "D pressed" << endl;
                            p2_x += 1;
                            rect = { p2_x * 50, p2_y * 50, p2_x * 50 + 50, p2_y * 50 + 50 }; //藍色方框
                            FillRect(hdc, &rect, blueBrush);
                            p2_moves--;
                            passing_arr_p2[p2_y][p2_x] = 1;
                            break;
                        }
                        else break;

                    default:
                        break;
                }
                rect = {p2_x * 50 + 20, p2_y * 50 + 20, (p2_x + 1) * 50 - 20, (p2_y + 1) * 50 - 20};
                FillRect(hdc, &rect, whiteBrush);
                if(p2_moves == 0)
                {
                    /*PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);*/

                    HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
                    HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
                    HBRUSH grassBrush = CreateSolidBrush(RGB(0, 201, 87));
                    HBRUSH waterBrush = CreateSolidBrush(RGB(0, 255, 255));
                    HBRUSH rockBrush = CreateSolidBrush(RGB(192, 192, 192));
                    // Switch turns after a move
                    isInitialDrawDone = 0;
                    // Reset moves for the next player
                    p1_moves = 3;
                    p2_moves = 3;
                    // Draw the black rectangles here
                    
                    for (int i = 1; i <= Length; i++) {
                        for (int j = 1; j <= Width; j++) {
                            RECT rect = {i * 50, j * 50, (i + 1) * 50, (j + 1) * 50};
                            if(mapArray[j][i] == 1) FillRect(hdc, &rect, grassBrush);
                            else if(mapArray[j][i] == 2) FillRect(hdc, &rect, waterBrush);
                            else if(mapArray[j][i] == 3) FillRect(hdc, &rect, rockBrush);
                        }
                    }
                    rect = {p1_x * 50 + 20, p1_y * 50 + 20, (p1_x + 1) * 50 - 20, (p1_y + 1) * 50 - 20};
                    FillRect(hdc, &rect, redBrush);

                    rect = {p2_x * 50 + 20, p2_y * 50 + 20, (p2_x + 1) * 50 - 20, (p2_y + 1) * 50 - 20};
                    FillRect(hdc, &rect, blueBrush);
                    currentPlayer = 1;

                    for(int i = 1 ; i <= Width ; i++)
                    {
                        for(int j = 1 ; j <= Length ; j++)
                        {
                            passing_arr_p1[i][j] = 0;
                            passing_arr_p2[i][j] = 0;
                        }
                    }

                    passing_arr_p1[p1_y][p1_x] = 1;
                    passing_arr_p2[p2_y][p2_x] = 1;
                }
            }



            if (p1_x == p2_x && p1_y == p2_y)
            {
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
    int lakeCnt = generateRandomNumber(static_cast<int>(225 * 0.02),
                                       static_cast<int>(225 * 0.04));
    //cout << lakeCnt << endl;
    for (int i = 1; i <= lakeCnt; i++)
        myTileMap.generateWaterMass();

    //加入岩石
    int rockCnt = generateRandomNumber(static_cast<int>(225 * 0.05),
                                       static_cast<int>(225 * 0.1));
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
                        break;
                    case ROCK:
                        mapArray[i][j] = 3;
                        break;
                }
            }
        }
    for (int i = 1; i <= Width; i++) {
            for (int j = 1; j <= Length; j++) {
                cout << mapArray[i][j] << " ";
            }
            cout << endl;
        }
    ///----------------------------------

    // 註冊視窗
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.lpszClassName = L"SampleWindowClass";
    RegisterClassW(&wc);

    // 創建視窗
    HWND hwnd = CreateWindowExW(0, L"SampleWindowClass", L"Sample Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1200, 900, NULL, NULL, GetModuleHandleW(NULL), NULL);

    if (hwnd == NULL)
    {
        cerr << "視窗創建失敗！" << endl;
        return 1;
    }

    // 顯示視窗
    ShowWindow(hwnd, SW_SHOWDEFAULT);

    // 進入消息循環
    MSG msg = {};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

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
