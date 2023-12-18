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
HICON hIcon_opportunity = (HICON)LoadImage(NULL, L"opportunity.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_destiny = (HICON)LoadImage(NULL, L"destiny.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);

HICON hIcon_CLJ = (HICON)LoadImage(NULL, L"CLJ.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_CUZ = (HICON)LoadImage(NULL, L"CUZ.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_CJJ = (HICON)LoadImage(NULL, L"CJJ.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_CGR = (HICON)LoadImage(NULL, L"CGR.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_YRH = (HICON)LoadImage(NULL, L"YRH.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_ATA = (HICON)LoadImage(NULL, L"ATA.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_PTA = (HICON)LoadImage(NULL, L"PTA.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_bigStupidBird = (HICON)LoadImage(NULL, L"BSB.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
HICON hIcon_bigGoose = (HICON)LoadImage(NULL, L"BG.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);

// Brushes
HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
HBRUSH grassBrush = CreateSolidBrush(RGB(0, 201, 87));
HBRUSH waterBrush = CreateSolidBrush(RGB(0, 255, 255));
HBRUSH rockBrush = CreateSolidBrush(RGB(192, 192, 192));

///--------------------------
class Character {
protected:
    int dice[6];  // 六面骰子的值
    string name;  // 角色名稱
public:
    // 构造函数
    Character(string theName, int d1, int d2, int d3, int d4, int d5, int d6) {
        name = theName;
        dice[0] = d1; dice[1] = d2; dice[2] = d3;
        dice[3] = d4; dice[4] = d5; dice[5] = d6;
    }

    // 獲取骰子數字
    int getDiceNum() {
        srand(static_cast<unsigned int>(time(0))); // 注意：理想情況下，srand應該在程序開始時只調用一次。
        int index = rand() % 6;
        return dice[index];
    }
    virtual void display() = 0; // 純虛擬函數，用於顯示角色資訊
    virtual void upgrade() = 0; // 純虛擬函數，用於升級角色
};

class Teacher : public Character {
private:
    string subjectTaught;  // 教授科目
    int yearsOfExperience; // 教學經驗年數
public:
    // 教師构造函数
    Teacher(string theName, int d1, int d2, int d3, int d4, int d5, int d6, string subject, int experience)
    : Character(theName, d1, d2, d3, d4, d5, d6), subjectTaught(subject), yearsOfExperience(experience)
    {}

    // 重寫display函數
    void display() override {
        cout << "Name: " << name;
        cout << "\nSubject Taught: " << subjectTaught << "\nYears of Experience: " << yearsOfExperience;
        cout << "\nDice: ";
        for (int i = 0; i < 6; ++i) {
            cout << dice[i] << " ";
        }
        cout << "\n\n";
    }

    // 重寫upgradeFunction函數
    void upgrade() override {
        for (int& die : dice) {
            die += yearsOfExperience / 5; // 根據經驗年數增加骰子的值
        }
    }
};

class Student : public Character {
private:
    int grade;           // 年級
    string majorSubject; // 主修科目
public:
    // 學生构造函数
    Student(string theName, int d1, int d2, int d3, int d4, int d5, int d6, string major,int grade)
    : Character(theName, d1, d2, d3, d4, d5, d6), grade(grade), majorSubject(major)
    {}

    // 重寫displa函數
    void display() override {
        cout << "Name: " << name;
        cout << "\nGrade: " << grade << "\nMajor Subject: " << majorSubject;
        cout << "\nDice: ";
        for (int i = 0; i < 6; ++i) {
            cout << dice[i] << " ";
        }
        cout << "\n\n";
    }


    // 重寫upgradeFunction函數
    void upgrade() override {
        srand(static_cast<unsigned int>(time(0)));
        int index = rand() % 6;
        dice[index] += grade / 10; // 根據分數增加隨機一個骰子的值
    }
};

const int characterNum = 9;

Character* characters[characterNum]; // 全域的一維 Character 陣列

void initializeCharacters() {
    // 初始化 characters 陣列，例如：
    characters[0] = new Teacher("小傑老師", 1, 2, 3, 4, 5, 6, "PDOGS!", 15);
    characters[1] = new Teacher("莊老師", 3, 3, 3, 4, 4, 4, "莊大刀!", 32);
    characters[2] = new Teacher("建錦主任", 1, 1, 1, 3, 5, 10, "笑容攻擊!", 21);
    characters[3] = new Teacher("國榮老師", 1, 1, 3, 4, 6, 6, "Laplace Transform", 17);
    characters[4] = new Teacher("尤老師", 2, 2, 2, 3, 6, 6, "LIFO", 24);

    characters[5] = new Student("會計助教", 1, 2, 3, 4, 5, 6, "要做Assignment3喔~", 99);
    characters[6] = new Student("程設助教", 3, 3, 3, 4, 4, 4, "隨便寫都比你強!", 88);
    characters[7] = new Student("大笨鳥", 2, 2, 2, 3, 6, 6, "(你笨選到這個)", 32);
    characters[8] = new Student("醉月湖大鵝", 1, 1, 1, 3, 5, 10, "午餐我來了!", 67);

}

void cleanupCharacters() {
    // 釋放 characters 陣列中的記憶體
    for (int i = 0; i < characterNum; ++i) {
        delete characters[i];
    }
}

///--------------------------

//方格的種類
enum TileType {
    GRASS,
    WATER,
    ROCK,
    OPPORTUNITY,
    FATE
};
struct Vector{
    int x;
    int y;
};

int generateRandomNumber(int minValue, int maxValue) {
    return rand() % (maxValue - minValue + 1) + minValue;
}

//這是個別的方格
class Tile {
private:
    TileType type;
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
        return;
    }
};

//生成地圖
template <typename T>
class TileMap {
private:
    int width;
    int length;
    int obstacleCnt;
    Tile*** TileMapObject;
public:
    TileMap(int width, int length){
        this->width = width;
        this->length = length;
        this->obstacleCnt = 0.1*(length*width);

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
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < width; j++) {
                delete TileMapObject[i][j];
            }
            delete[] TileMapObject[i];
        }
        delete[] TileMapObject;
    }

    TileType getTileType(int x, int y) const {
            return TileMapObject[y][x]->getTile();
        }

    void setTileType(int x, int y, TileType type) {
            TileMapObject[y][x]->setTile(type);
        }

    void printTileMap() const {
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < width; j++) {
                switch (getTileType(j, i)) {
                    case GRASS:
                        cout << " G "; break;
                    case WATER:
                        cout << " W "; break;
                    case ROCK:
                        cout << " R "; break;
                    case OPPORTUNITY:
                        cout << " O "; break;
                    case FATE:
                        cout << " F "; break;
                    }
                }
                cout << endl;
            }
        }
    void generateWaterMass() {
        int waterMassWidth = rand() % 3 + 2; // 在[2,4]之間取一個隨機數字當水池寬度
        int waterMassLength = rand() % 3 + 2;

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
    void randomTiles(int tileCnt, T targetType) {
        for (int i = 0; i < tileCnt; i++) {
            int x = rand() % width;
            int y = rand() % length;

            if (getTileType(x, y) == GRASS) {
                setTileType(x, y, targetType);
            } else {
                i--;
            }
        }
    }
};

int** mapArray; // 儲存地圖數據的陣列

///---------------------------

int p1_character = -1, p2_character = -1;

int cnt_UpDown = 0;
int cnt_LeftRight = 0;

int cnt_UpDown_player2 = 100;
int cnt_LeftRight_player2 = 100;

int p1_x = 1, p1_y = 1, p1_moves = 0;
int p2_x = Length, p2_y = Width, p2_moves = 0;

bool isInitialDrawDone = false;

int currentPlayer = 1;

int pixel = 32;

int** passing_arr_p1;
int** passing_arr_p2;

int currentCharacter = 0;

bool isGameStarted = false;
bool isLogo = false;

double moveChange_p1 = 1, moveChange_p2 = 1;

wchar_t* stepText_p1;
wchar_t* stepText_p2;
wchar_t* condition_text;
wchar_t* upgrade_p1;
wchar_t* upgrade_p2;

const int NumDice = 9;
wchar_t* diceDescriptions[NumDice] = {
    L"(1, 2, 3, 4, 5, 6)",
    L"(3, 3, 3, 4, 4, 4)",
    L"(1, 1, 1, 3, 5, 10)",
    L"(1, 1, 3, 4, 6, 6)",
    L"(2, 2, 2, 3, 6, 6)",
    L"(1, 2, 3, 4, 5, 6)",
    L"(3, 3, 3, 4, 4, 4)",
    L"(2, 2, 2, 3, 6, 6)",
    L"(1, 1, 1, 3, 5, 10)"
};
wchar_t* upgradeDescriptions[NumDice] = {
    L"PDOGS!  骰子點數都+3",
    L"莊大刀!  骰子點數都+6",
    L"笑容攻擊!  骰子點數都+3",
    L"Laplace Transform  骰子點數都+3",
    L"LIFO  骰子點數都+3",
    L"要做Assignment3喔  一面點數+3",
    L"隨便寫都比你強!  一面點數+3",
    L"(你笨選到這個)  一面點數+3",
    L"午餐我來了!  一面點數+3"
};

const int NumCharacters = 9;
const wchar_t* characterName[NumCharacters] = {
    L"小傑老師",
    L"莊老師",
    L"建錦主任",
    L"國榮老師",
    L"尤老師",
    L"會計助教",
    L"程設助教",
    L"大笨鳥",
    L"醉月湖大鵝"
};
const wchar_t* characterDescriptions[NumCharacters] = {
    L"動漫愛好者，擁有兩大神獸 pcats 和 pdogs 在手，帶你進入 c++ 世界。",
    L"嚴肅評論家，點評毫不留情，是你成長路上的嚴格導師。",
    L"常關心同學的主任，笑臉下的犀利點評，讓你感受溫暖與挑戰。",
    L"突如其來的日語和 diss，將讓你在笑聲中學到更多。",
    L"有什麼比松鼠還要聒噪，喜歡粉紅色的那個女人。",
    L"英雄般的救星，從全英會計課中拯救你，讓你脫離困境。",
    L"為你指點迷津，幫你跳出 TLE、RE、MLE、WA 惡夢的那群英雄。",
    L"亂飛亂拉屎，為你帶來校園的不可預測性。",
    L"湖邊的驚喜，當心這位校園生物，或許會給你一場驚喜的午餐體驗。"
};
HICON characterIcon[NumCharacters] = {
    hIcon_CLJ,
    hIcon_CUZ,
    hIcon_CJJ,
    hIcon_CGR,
    hIcon_YRH,
    hIcon_ATA,
    hIcon_PTA,
    hIcon_bigStupidBird,
    hIcon_bigGoose
};

const int NumBlocks = 6;
const wchar_t* blockDescriptions[NumBlocks] = {
    L"grass : Can walk ~ ~ ~",
    L"water : Can't walk ! ! !",
    L"rock : Be careful! Don't step on it !",
    L"opportunity : Change steps ( maybe better off , maybe worse off )",
    L"destiny : add or remove rocks",
    L"NOTICE : If there's no way to go, you may be transferred to a random place !"
};
HICON blockIcon[NumBlocks] = {
    hIcon_grass,
    hIcon_water,
    hIcon_rock,
    hIcon_opportunity,
    hIcon_destiny,
    hIcon_location
};

int conditionTextCnt = 0;

wchar_t* text;
void HandleTimer(HWND hwnd, wchar_t* displayText)
{
    HDC hdc = GetDC(hwnd);
    const wchar_t currentChar = displayText[currentCharacter];
    SetBkColor(hdc, RGB(0, 0, 0));  // 設置黑色背景顏色
    SetTextColor(hdc, RGB(255, 255, 255));  // 設置白色文本顏色
    TextOutW(hdc, 1130 + currentCharacter * 10, 830, &currentChar, 1); // 逐個字符顯示

    currentCharacter++; // 增加字符index

    // 如果所有字符都已經顯示，停止定時器
    if (currentCharacter >= wcslen(displayText))
    {
        KillTimer(hwnd, 1);   // 停止定時器
        text = L"";
    }
    ReleaseDC(hwnd, hdc);
}

int p1_upgradeTime = 0, p2_upgradeTime = 0;
void printUpgrade(HWND hwnd)
{
    HDC hdc = GetDC(hwnd);
    HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft JhengHei");
    SelectObject(hdc, hFont);
    SetBkColor(hdc, RGB(0, 0, 0));  // 設置黑色背景顏色
    SetTextColor(hdc, RGB(255, 255, 255));  // 設置白色文本顏色
    upgrade_p1 = upgradeDescriptions[p1_character];
    upgrade_p2 = upgradeDescriptions[p2_character];
    for(int i = 0 ; i < p1_upgradeTime ; i++) TextOutW(hdc, (Length + 2) * pixel + 200 * (i+1) - 50, 75, upgrade_p1, static_cast<int>(wcslen(upgrade_p1)));
    for(int i = 0 ; i < p2_upgradeTime ; i++) TextOutW(hdc, (Length + 2) * pixel + 200 * (i+1) - 50, 145, upgrade_p2, static_cast<int>(wcslen(upgrade_p2)));
}

void printOutAll(HWND hwnd)
{
    HDC hdc = GetDC(hwnd);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    // clear all
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    RECT customRect = {(Length + 2) * pixel, 230, screenWidth-pixel-10, screenHeight-pixel-10};
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
            else if(mapArray[j][i] == 4) DrawIcon(hdc, i * pixel, j * pixel, hIcon_opportunity);
            else if(mapArray[j][i] == 5) DrawIcon(hdc, i * pixel, j * pixel, hIcon_destiny);
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
        DrawIcon(hdc, i * pixel, 190, hIcon_rock);
    }

    DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_player1);
    DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_player2);

    stepText_p1 = L"p1 steps : ";
    stepText_p2 = L"p2 steps : ";
    SetBkColor(hdc, RGB(0, 0, 0));  // 設置黑色背景顏色
    SetTextColor(hdc, RGB(255, 255, 255));  // 設置白色文本顏色
    TextOutW(hdc, (Length + 2) * pixel + 10, 40, stepText_p1, static_cast<int>(wcslen(stepText_p1)));
    TextOutW(hdc, (Length + 2) * pixel + 10, 110, stepText_p2, static_cast<int>(wcslen(stepText_p2)));

    wchar_t* dice_p1;
    wchar_t* dice_p2;
    dice_p1 = diceDescriptions[p1_character];
    dice_p2 = diceDescriptions[p2_character];

    TextOutW(hdc, (Length + 2) * pixel + 10, 75, dice_p1, static_cast<int>(wcslen(dice_p1)));
    TextOutW(hdc, (Length + 2) * pixel + 10, 145, dice_p2, static_cast<int>(wcslen(dice_p2)));
    printUpgrade(hwnd);

    wchar_t* blockDescription = L"Block Description : ";
    TextOutW(hdc, pixel + 10, (Width + 2) * pixel + 15, blockDescription, static_cast<int>(wcslen(blockDescription)));
    for(int i = 0 ; i < p1_moves ; i++)
    {
        DrawIcon(hdc, (Length + 5) * pixel + i * pixel, 40, hIcon_player1);
    }
    for(int i = 0; i < NumBlocks; ++i)
    {
        DrawIcon(hdc, pixel + 10, (Width + 2) * pixel + 10 + (i+1) * pixel + i * 10, blockIcon[i]);
        TextOutW(hdc, pixel * 2 + 20, (Width + 2) * pixel + 10 + (i+1) * pixel + 10 + i * 10, blockDescriptions[i], static_cast<int>(wcslen(blockDescriptions[i])));
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
    TextOutW(hdc, (Length + 2) * pixel + 10, 240 + conditionTextCnt * pixel, player1or2, static_cast<int>(wcslen(player1or2)));

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
            case 7:
                condition_text = L"upgrade your dice !";
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
            case 6:
                condition_text = L"upgrade your dice !";
                break;
            case 10:    // 黑洞傳送
                condition_text = L"there's no way to go, transfer to a new place.";
                break;
        }
    }
    TextOutW(hdc, (Length + 2) * pixel + 100, 240 + conditionTextCnt * pixel, condition_text, static_cast<int>(wcslen(condition_text)));
    conditionTextCnt++;
    if(conditionTextCnt > 20)
    {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        RECT customRect = {(Length + 2) * pixel, 230, screenWidth-pixel-10, screenHeight-pixel-10};
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
        int opportunityCase = generateRandomNumber(1, 7); // opportinity機會 : 改變步數
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
            case 7:
                characters[p1_character]->upgrade();
                p1_upgradeTime++;
                HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft JhengHei");
                SelectObject(hdc, hFont);
                SetBkColor(hdc, RGB(0, 0, 0));  // 設置黑色背景顏色
                SetTextColor(hdc, RGB(255, 255, 255));  // 設置白色文本顏色
                upgrade_p1 = upgradeDescriptions[p1_character];
                for(int i = 0 ; i < p1_upgradeTime ; i++) TextOutW(hdc, (Length + 2) * pixel + 200 * (i+1) - 50, 75, upgrade_p1, static_cast<int>(wcslen(upgrade_p1)));
                break;
        }
        mapArray[y][x] = 1;
        DrawIcon(hdc, x * pixel, y * pixel, hIcon_grass);
        conditionText(1, 4, opportunityCase, hwnd);
    }
    else if(mapArray[y][x] == 5)
    {
        int destinyCase = generateRandomNumber(1, 6); // destiny命運 : 改變障礙物跟位置
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
                    case 6:
                        characters[p1_character]->upgrade();
                        p1_upgradeTime++;
                        conditionText(1, 5, destinyCase, hwnd);
                        HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft JhengHei");
                        SelectObject(hdc, hFont);
                        SetBkColor(hdc, RGB(0, 0, 0));  // 設置黑色背景顏色
                        SetTextColor(hdc, RGB(255, 255, 255));  // 設置白色文本顏色
                        upgrade_p1 = upgradeDescriptions[p1_character];
                        for(int i = 0 ; i < p1_upgradeTime ; i++) TextOutW(hdc, (Length + 2) * pixel + 200 * (i+1) - 50, 75, upgrade_p1, static_cast<int>(wcslen(upgrade_p1)));
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
    rect = {(Length + 5) * pixel, 40, (Length + 5 + 15) * pixel + 32, 40 + 32};
    FillRect(hdc, &rect, blackBrush);
    p1_moves = 1;
}

void DesAndOpt_p2(int x, int y, HWND hwnd)
{
    HDC hdc = GetDC(hwnd);
    if(mapArray[y][x] == 4)
    {
        int opportunityCase = generateRandomNumber(1, 7); // opportinity機會 : 改變步數
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
            case 7:
                characters[p2_character]->upgrade();
                p2_upgradeTime++;
                HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft JhengHei");
                SelectObject(hdc, hFont);
                SetBkColor(hdc, RGB(0, 0, 0));  // 設置黑色背景顏色
                SetTextColor(hdc, RGB(255, 255, 255));  // 設置白色文本顏色
                upgrade_p2 = upgradeDescriptions[p2_character];
                for(int i = 0 ; i < p2_upgradeTime ; i++) TextOutW(hdc, (Length + 2) * pixel + 200 * (i+1) - 50, 145, upgrade_p2, static_cast<int>(wcslen(upgrade_p2)));
                break;
        }
        mapArray[y][x] = 1;
        DrawIcon(hdc, x * pixel, y * pixel, hIcon_grass);
        conditionText(2, 4, opportunityCase, hwnd);
    }
    else if(mapArray[y][x] == 5)
    {
        int destinyCase = generateRandomNumber(1, 6); // destiny命運 : 改變障礙物跟位置
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
                    case 6:
                        characters[p2_character]->upgrade();
                        p2_upgradeTime++;
                        conditionText(2, 5, destinyCase, hwnd);
                        HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft JhengHei");
                        SelectObject(hdc, hFont);
                        SetBkColor(hdc, RGB(0, 0, 0));  // 設置黑色背景顏色
                        SetTextColor(hdc, RGB(255, 255, 255));  // 設置白色文本顏色
                        upgrade_p2 = upgradeDescriptions[p2_character];
                        for(int i = 0 ; i < p2_upgradeTime ; i++) TextOutW(hdc, (Length + 2) * pixel + 200 * (i+1) - 50, 145, upgrade_p2, static_cast<int>(wcslen(upgrade_p2)));
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
    rect = {(Length + 5) * pixel, 110, (Length + 5 + 15) * pixel + 32, 110 + 32};
    FillRect(hdc, &rect, blackBrush);
    p2_moves = 1;
}

void UpDownLeftRight(int player, HWND hwnd)
{
    RECT rect;
    HDC hdc = GetDC(hwnd);
    if(player == 1)
    {
        if(p1_moves > 0)
        {
            rect = {(Length + 5 + p1_moves - 1) * pixel, 40, (Length + 5 + p1_moves - 1) * pixel + 32, 40 + 32};
            FillRect(hdc, &rect, blackBrush);
        }
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
        if(p2_moves > 0)
        {
            rect = {(Length + 5 + p2_moves - 1) * pixel, 110, (Length + 5 + p2_moves - 1) * pixel + 32, 110 + 32};
            FillRect(hdc, &rect, blackBrush);
        }
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
            if(!isGameStarted)HandleTimer(hwnd, text);
            break;
        }
        case WM_PAINT:
        {
            passing_arr_p1[1][1] = 1;
            passing_arr_p2[Width][Length] = 1;

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if (!isLogo)
            {
                // 設置黑色背景
                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                FillRect(hdc, &clientRect, blackBrush);

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


                int LeftPixel = 300, UpPixel = 350;
                for(int i = 0; i < 5; i++) DrawIcon(hdc, LeftPixel, UpPixel+32*i, hIcon_rock);
                DrawIcon(hdc, LeftPixel+32, UpPixel, hIcon_rock);
                DrawIcon(hdc, LeftPixel+64, UpPixel, hIcon_rock);
                DrawIcon(hdc, LeftPixel+96, UpPixel+32, hIcon_rock);
                DrawIcon(hdc, LeftPixel+64, UpPixel+64, hIcon_rock);
                DrawIcon(hdc, LeftPixel+32, UpPixel+64, hIcon_rock);
                DrawIcon(hdc, LeftPixel+96, UpPixel+96, hIcon_rock);
                DrawIcon(hdc, LeftPixel+64, UpPixel+128, hIcon_rock);
                DrawIcon(hdc, LeftPixel+32, UpPixel+128, hIcon_rock);

                LeftPixel = 450; UpPixel = 350;
                for(int i = 0; i < 5; i++) DrawIcon(hdc, LeftPixel, UpPixel+32*i, hIcon_rock);
                DrawIcon(hdc, LeftPixel+32, UpPixel, hIcon_rock);
                DrawIcon(hdc, LeftPixel+64, UpPixel, hIcon_rock);
                DrawIcon(hdc, LeftPixel+96, UpPixel+32, hIcon_rock);
                DrawIcon(hdc, LeftPixel+64, UpPixel+64, hIcon_rock);
                DrawIcon(hdc, LeftPixel+32, UpPixel+64, hIcon_rock);
                DrawIcon(hdc, LeftPixel+96, UpPixel+96, hIcon_rock);
                DrawIcon(hdc, LeftPixel+64, UpPixel+128, hIcon_rock);
                DrawIcon(hdc, LeftPixel+32, UpPixel+128, hIcon_rock);

                LeftPixel = 600; UpPixel = 350;
                for(int i = 0; i < 5; i++) DrawIcon(hdc, LeftPixel, UpPixel+32*i, hIcon_rock);
                DrawIcon(hdc, LeftPixel+32, UpPixel, hIcon_rock);
                DrawIcon(hdc, LeftPixel+64, UpPixel, hIcon_rock);
                DrawIcon(hdc, LeftPixel+96, UpPixel+32, hIcon_rock);
                DrawIcon(hdc, LeftPixel+64, UpPixel+64, hIcon_rock);
                DrawIcon(hdc, LeftPixel+32, UpPixel+64, hIcon_rock);
                DrawIcon(hdc, LeftPixel+96, UpPixel+96, hIcon_rock);
                DrawIcon(hdc, LeftPixel+64, UpPixel+128, hIcon_rock);
                DrawIcon(hdc, LeftPixel+32, UpPixel+128, hIcon_rock);

                LeftPixel = 750; UpPixel = 350;
                for(int i = 0; i < 5; i++) DrawIcon(hdc, LeftPixel, UpPixel+32*i, hIcon_rock);
                for(int i = 0; i < 3; i++)DrawIcon(hdc, LeftPixel+32*i, UpPixel+128, hIcon_rock);

                LeftPixel = 900-32; UpPixel = 350;
                for(int i = 0; i < 4; i++)
                {
                    DrawIcon(hdc, LeftPixel, UpPixel+32*i, hIcon_rock);
                    DrawIcon(hdc, LeftPixel+96, UpPixel+32*i, hIcon_rock);
                }
                DrawIcon(hdc, LeftPixel+32, UpPixel+128, hIcon_rock);
                DrawIcon(hdc, LeftPixel+64, UpPixel+128, hIcon_rock);

                LeftPixel = 1050-32; UpPixel = 350;
                for(int i = 0; i < 5; i++) DrawIcon(hdc, LeftPixel, UpPixel+32*i, hIcon_rock);
                DrawIcon(hdc, LeftPixel+32, UpPixel, hIcon_rock);
                DrawIcon(hdc, LeftPixel+64, UpPixel, hIcon_rock);
                DrawIcon(hdc, LeftPixel+96, UpPixel, hIcon_rock);
                DrawIcon(hdc, LeftPixel+32, UpPixel+64, hIcon_rock);
                DrawIcon(hdc, LeftPixel+64, UpPixel+64, hIcon_rock);
                DrawIcon(hdc, LeftPixel+96, UpPixel+64, hIcon_rock);
                DrawIcon(hdc, LeftPixel+32, UpPixel+128, hIcon_rock);
                DrawIcon(hdc, LeftPixel+64, UpPixel+128, hIcon_rock);
                DrawIcon(hdc, LeftPixel+96, UpPixel+128, hIcon_rock);

                SetBkColor(hdc, RGB(0, 0, 0));  // 設置黑色背景顏色
                SetTextColor(hdc, RGB(192, 192, 192));  // 設置白色文本顏色
                HFONT hFont = CreateFont(25, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft JhengHei");
                SelectObject(hdc, hFont);
                wchar_t* nameTest = L"Made by 資管一  蕭翊璇 藍柏婷 柯絲昀 李捷安";
                TextOutW(hdc, 520, 550, nameTest, static_cast<int>(wcslen(nameTest)));

                currentCharacter = 0;
                text = L"Press Enter to continue...";
                SetTimer(hwnd, 1, 30, NULL);
            }
            else if ((!isGameStarted) && isLogo) //起始畫面
            {
                // 設置黑色背景
                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                FillRect(hdc, &clientRect, blackBrush);

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


                for (int i = 0; i <= screenWidth/pixel; i++)
                {
                    DrawIcon(hdc, i * pixel, 96, hIcon_rock);
                    DrawIcon(hdc, i * pixel, 460, hIcon_rock);
                }
                for (int j = 3; j <= screenHeight/pixel; j++)
                {
                    for(int k = 1 ; k < 5 ; k++)
                    {
                        DrawIcon(hdc, 280 * k, j * pixel, hIcon_rock);
                    }
                }

                // 绘制背景
                SetBkColor(hdc, RGB(0, 0, 0));
                SetTextColor(hdc, RGB(255, 255, 255));
                HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft JhengHei");
                SelectObject(hdc, hFont);

                wchar_t* firstTest = L"選擇你的角色，開啟冒險對戰！";
                TextOutW(hdc, pixel + 15, pixel + 20, firstTest, static_cast<int>(wcslen(firstTest)));
                wchar_t* choose = L"請依序選擇 1 ~ 9 角色";
                TextOutW(hdc, 1165, 510, choose, static_cast<int>(wcslen(choose)));

                // 繪製9個介紹框
                for (int i = 0; i < NumCharacters; i++)
                {
                    DrawIcon(hdc, pixel + 10 + 280 * (i%5), pixel * 3 + 45 + 362 * (i/5), characterIcon[i]);   // 繪製icon
                    TextOutW(hdc, pixel + 10 + 280 * (i%5) + 50, pixel * 3 + 45 + 362 * (i/5) + 5, characterName[i], static_cast<int>(wcslen(characterName[i]))); // 輸出名稱
                    wchar_t* characteristic = L"屬性 : ";
                    TextOutW(hdc, pixel + 10 + 280 * (i%5), pixel * 3 + 45 + 362 * (i/5) + 55, characteristic, static_cast<int>(wcslen(characteristic))); // 屬性
                    TextOutW(hdc, pixel + 10 + 280 * (i%5) + 60, pixel * 3 + 45 + 362 * (i/5) + 55, diceDescriptions[i], static_cast<int>(wcslen(diceDescriptions[i]))); // 輸出屬性
                    RECT textRect = { pixel + 10 + 280 * (i%5), pixel * 3 + 45 + 362 * (i/5) + 120, pixel + 10 + 280 * (i%5) + 230, pixel * 3 + 45 + 362 * (i/5) + 120 + 320 };
                    DrawTextW(hdc, characterDescriptions[i], -1, &textRect, DT_WORDBREAK | DT_LEFT | DT_VCENTER); // 輸出文字
                }

                currentCharacter = 0;
                text = L"    Press Enter to start...";
                SetTimer(hwnd, 1, 30, NULL);
                //KillTimer(hwnd, 1);
            }
            else if(isGameStarted && isLogo)
            {
                // 設置黑色背景
                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                FillRect(hdc, &clientRect, blackBrush);
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
            if(wParam == VK_RETURN && isLogo == false) { // 假設 Enter 鍵關閉遊戲logo
                isLogo = true;
                InvalidateRect(hwnd, NULL, TRUE); // 通知窗口需要重新繪製
            }

            // 按鍵被按下時的處理
            else if (wParam >= '1' && wParam <= '9' && isLogo == true && isGameStarted == false)
            {
                SetBkColor(hdc, RGB(0, 0, 0));  // 設置黑色背景顏色
                SetTextColor(hdc, RGB(255, 255, 255));  // 設置白色文本顏色
                HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft JhengHei");
                SelectObject(hdc, hFont);
                int key = wParam - '0';
                if (p1_character == -1) // 如果 p1 還未選擇角色
                {
                    p1_character = key - 1;
                    wchar_t buffer[20];
                    swprintf_s(buffer, L"player1選擇的角色 : %d", p1_character+1);
                    TextOutW(hdc, 1165, 550, buffer, static_cast<int>(wcslen(buffer)));
                }
                else if (p2_character == -1) // 如果 p1 已選，p2 還未選擇角色
                {
                    p2_character = key - 1;
                    wchar_t buffer[20];
                    swprintf_s(buffer, L"player2選擇的角色 : %d", p2_character+1);
                    TextOutW(hdc, 1165, 590, buffer, static_cast<int>(wcslen(buffer)));
                }
                ReleaseDC(hwnd, hdc);  // 释放窗口设备上下文
                DeleteObject(hFont);   // 释放字体对象
            }

            else if(wParam == VK_RETURN && isLogo == true && isGameStarted == false) { // 假設 Enter 鍵啟動遊戲
                isGameStarted = true;
                InvalidateRect(hwnd, NULL, TRUE); // 通知窗口需要重新繪製
                p1_moves = characters[p1_character]->getDiceNum();
                p2_moves = characters[p2_character]->getDiceNum();
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
                        if(p1_y > 1 && passing_arr_p1[p1_y - 1][p1_x] == 0 && p1_moves > 0)
                        {
                            cout << "up" << endl;
                            DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_flag);
                            p1_y -= 1;
                            UpDownLeftRight(1, hwnd);
                            break;
                        }
                        else break;


                    case VK_DOWN:
                        if(p1_y < Width && passing_arr_p1[p1_y + 1][p1_x] == 0 && p1_moves > 0)
                        {
                            cout << "down" << endl;
                            DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_flag);
                            p1_y += 1;
                            UpDownLeftRight(1, hwnd);
                            break;
                        }
                        else break;


                    case VK_LEFT:
                        if(p1_x > 1 && passing_arr_p1[p1_y][p1_x - 1] == 0 && p1_moves > 0)
                        {
                            cout << "left" << endl;
                            DrawIcon(hdc, p1_x * pixel, p1_y * pixel, hIcon_flag);
                            p1_x -= 1;
                            UpDownLeftRight(1, hwnd);
                            break;
                        }
                        else break;


                    case VK_RIGHT:
                        if(p1_x < Length && passing_arr_p1[p1_y][p1_x + 1] == 0 && p1_moves > 0)
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
                if(p1_moves <= 0) // 當p1是0步時，要再多按一下
                {
                    currentPlayer = 2;
                    for(int i = 0 ; i < p2_moves ; i++)
                    {
                        DrawIcon(hdc, (Length + 5) * pixel + i * pixel, 110, hIcon_player2);
                    }
                }
            }
            else if (currentPlayer == 2 && isGameStarted == true)
            {
                switch (wParam)
                {
                    case 'W':
                        if(p2_y > 1 && passing_arr_p2[p2_y - 1][p2_x] == 0 && p2_moves > 0)
                        {
                            cout << "W pressed" << endl;
                            DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_flag);
                            p2_y -= 1;
                            UpDownLeftRight(2, hwnd);
                            break;
                        }
                        else break;

                    case 'S':
                        if(p2_y < Width && passing_arr_p2[p2_y + 1][p2_x] == 0 && p2_moves > 0)
                        {
                            cout << "S pressed" << endl;
                            DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_flag);
                            p2_y += 1;
                            UpDownLeftRight(2, hwnd);
                            break;
                        }
                        else break;


                    case 'A':
                        if(p2_x > 1 && passing_arr_p2[p2_y][p2_x - 1] == 0 && p2_moves > 0)
                        {
                            cout << "A pressed" << endl;
                            DrawIcon(hdc, p2_x * pixel, p2_y * pixel, hIcon_flag);
                            p2_x -= 1;
                            UpDownLeftRight(2, hwnd);
                            break;
                        }
                        else break;


                    case 'D':
                        if(p2_x < Length && passing_arr_p2[p2_y][p2_x + 1] == 0 && p2_moves > 0)
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
                if(p2_moves <= 0)
                {
                    // Switch turns after a move
                    isInitialDrawDone = 0;
                    // Reset moves for the next player

                    p1_moves = characters[p1_character]->getDiceNum() * moveChange_p1;
                    p2_moves = characters[p2_character]->getDiceNum() * moveChange_p2;

                    printOutAll(hwnd);

                    currentPlayer = 1;
                    if(moveChange_p1 == 0)
                    {
                        currentPlayer = 2;
                        for(int i = 0 ; i < p2_moves ; i++)
                        {
                            DrawIcon(hdc, (Length + 5) * pixel + i * pixel, 110, hIcon_player2);
                        }
                    }

                    moveChange_p1 = 1;
                    moveChange_p2 = 1;

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
    initializeCharacters();

    ///--------------------------------

//    //建立地圖
//    TileMap myTileMap(Length + 1, Width + 1);

    //建立地圖
    TileMap<TileType> myTileMap(Length + 1, Width + 1);
    //TileMap<TileType> chanceMap(width, length);

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
    int oppCnt = generateRandomNumber(static_cast<int>((Length * Width) * 0.02),
                                      static_cast<int>((Length * Width) * 0.05));
    int fateCnt = generateRandomNumber(static_cast<int>((Length * Width) * 0.02),
                                      static_cast<int>((Length * Width) * 0.05));
    //cout << rockCnt << endl;
    myTileMap.randomRocks(rockCnt);
    myTileMap.randomTiles(oppCnt, OPPORTUNITY);
    myTileMap.randomTiles(fateCnt, FATE);

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
                    case OPPORTUNITY:
                        mapArray[i][j] = 4;
                        break;
                    case FATE:
                        mapArray[i][j] = 5;
                        break;
                }
            }
        }
//    //Test
//    //mapArray[4][4] = 4;
//    mapArray[5][5] = 5;
//    mapArray[3][4] = 5;
//    mapArray[3][6] = 5;
//    mapArray[3][8] = 5;
//    mapArray[6][7] = 5;
//    mapArray[6][3] = 5;
//    mapArray[8][2] = 5;
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
    cleanupCharacters();
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
