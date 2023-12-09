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
  Weapon(string name,int attack,int level) : name(name),attack(attack),level(level){}
};


class Character{
 public:
  string name;//����W
  int health;//�ͩR��
  int positionX;//�����mx
  int positionY;//�����my
  Weapon *weapon;//�Z��

  //�]�w����W�r �M ��m ��L�����w�]��
  Character(string n,int x,int y) : name(n),health(100),positionX(x),positionY(y){};

  void move(int step){//��{�����޿� �n�Ҽ{��ê���M�a�����
    // ²�檺�����޿�A���Ҽ{��ɩM��ê��
    positionX += step;
    positionY += step;
    cout << name << " moved " << step << " steps.\n";
  }
  void attack(Character& enemy){//��{�����޿� 
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
};

class GameMap{
public:
  int size;
  vector<vector<int>> obstacles; // 1��ܻ�ê��

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
  void displayMap() {// ��ܦa�ϡA�]�A�����m�M��ê��
    // ��ܦa�ϡA�]�A�����m�M��ê��
    cout << "Game Map:\n";
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            // ²���ܨ���M��ê��
            if (obstacles[i][j] == 1) {
                cout << "X ";
            } else {
                cout << ". ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
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
        int startX = rand() % map.size;
        int startY = rand() % map.size;
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
      map.displayMap();
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
        if (players[targetIndex].health <= 0) {
        cout << players[targetIndex].name << " has been defeated! " 
           << player.name << " wins!\n";
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

