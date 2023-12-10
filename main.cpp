//BUG:�����޿�A���Ҽ{��ɩM��ê��
#include<iostream>
#include<string>
#include<ctime>
#include<cstdlib>
#include<vector>
using namespace std;

class Weapon{
protected:
  string name;//�Z���W
  int attack;//�����O
  int level;//�Z������
public:
  Weapon(string name,int attack,int level)
    : name(name),attack(attack),level(level){}
};


class Character{
protected:
  string name;//����W
  int health;//�ͩR��
  int positionX;//�����mx
  int positionY;//�����my
  Weapon *weapon;//�Z��
public:
  //�]�w����W�r �M ��m ��L�����w�]��
  Character(string n,int x,int y) : name(n),health(100),positionX(x),positionY(y){};

  void move(int step){//��{�����޿� �n�Ҽ{��ê���M�a�����
    // ²�檺�����޿�A���Ҽ{��ɩM��ê��
    positionX += step;
    positionY += step;
    cout << name << " moved " << step << " steps.\n";
  }
  virtual void attack(Character& enemy){//��{�����޿� 
    // ²�檺�����޿�
    enemy.health -= 10;
    cout << name << " attacked " << enemy.name << ". "
      << enemy.name << "'s health: " << enemy.health << "\n";
  }
  void displayInfo() {//��ܨ�����
    // ��ܨ���H��
    cout << "Name: " << name << ", Position: (" << positionX 
      << ", " << positionY << "), Health: " << health << "\n";
  }
  string getName () const {
    return name ;
  }
  int getHealth () const {
    return health ;
  }
  int getX() const{
    return positionX ;
  }
  int getY() const{
    return positionY ;
  }

};

class GameMap{
protected:
  int size;
  vector<vector<int>> obstacles; // 1��ܻ�ê��
public:
  void generateObstacles(){// �n��{�H���ͦ���ê�����޿�
    // ²�檺�H���ͦ���ê�����޿�
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            obstacles[i][j] = rand() % 2; 
          // �H���ͦ�0��1�A��ܦ��L��ê��
        }
    }
  }

  GameMap(int size) : size(size){//��l��map�j�p �n��{��ê��
    obstacles.resize(size, vector<int>(size, 0));
    generateObstacles();
  }
  void displayMap(const vector<Character>& characters) {// ��ܦa�ϡA�]�A�����m�M��ê��
    // ��ܦa�ϡA�]�A�����m�M��ê��
    cout << "Game Map:\n";
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            // ²���ܨ���M��ê��
            if(i == characters[0].getX() && j == characters[0].getY()){
              cout << "A ";
            } else if(i == characters[1].getX() && j == characters[1].getY()){
              cout << "B ";
            } else if (obstacles[i][j] == 1) {
                cout << "X ";
            }else {
                cout << ". ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
  }

  
  int getSize() const {
    return size;
  }
};

class Game{
private:
  GameMap map;
  vector<Character> players;
public:
  void initializePlayers(){//�����ܨ���
    for (int i = 1; i <= 2; ++i) {
        string name = "Player" + to_string(i);
        int startX = rand() % map.getSize();
        int startY = rand() % map.getSize();
        Character player(name, startX, startY);
        players.push_back(player);
    }
  }
  Game(int mapSize) : map(mapSize){//�]�w�a��
     initializePlayers();
  }
  void startGame() {//�}�l�C��
    while (true) {
      // ��ܦa�ϩM���a�H��
      map.displayMap(players);
      for (auto &player : players) {
        player.displayInfo();
      }

      // ���a�ާ@
      for (auto &player : players) {
        int steps = rand() % 3 + 1; // ²�檺�B�ƥͦ��޿�
        player.move(steps);

        // ²�檺�����޿�A�H����ܧ������
        int targetIndex = rand() % players.size();
        if (targetIndex != &player - &players[0]) {
          player.attack(players[targetIndex]);
        }

        // ²�檺�ӭt�P�_�A���]��q��0�ɬ�����
        if (players[targetIndex].getHealth() <= 0) {
          cout << players[targetIndex].getName() << " has been defeated! " 
            << player.getName() << " wins!\n";
          return;
        }
      }
    }
  }
};

int main(){
    srand(time(0));
    Game game(10);
    game.startGame();
    return 0;
}

