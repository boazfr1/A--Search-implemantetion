#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::sort;
using std::string;
using std::vector;
using std::abs;

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};
vector<vector<State>> board{};
vector<vector<int>> openNodes {};


vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',') {
      if (n == 0) {
        row.push_back(State::kEmpty);
      } else {
        row.push_back(State::kObstacle);
      }
    }
    return row;
}


vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile (path);
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}

string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return "⛰️   ";
    case State::kPath: return "🚗   ";
    case State::kStart: return "🚦   ";
    case State::kFinish: return "🏁   ";
    default: return "0   "; 
  }
}

void PrintBoard() {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      cout << CellString(board[i][j]);
    }
    cout << "\n";
  }
}

void AddToOpen(int x, int y, int g, int h){
  vector<int> node = {x, y, g, h};
  openNodes.push_back(vector<int>{x, y, g, h});
  board[x][y] = State::kClosed;
}

bool Compare(vector<int> node1, vector<int> node2){
  if (node1.size() != 2 && node2.size() != 2)
  {
    cout << "there is an error with the argument!\n";
    return false;
  }
  return node1[0] + node1[1] > node2[0] + node2[1];
}


void CellSort() {
  sort(openNodes.begin(), openNodes.end(), Compare);
}

int Heuristic(int pointOnTheBoardX1, int pointOnTheBoardY1, int pointOnTheBoardX2, int pointOnTheBoardY2){
  return abs(pointOnTheBoardX1 - pointOnTheBoardY1) + abs(pointOnTheBoardX2 - pointOnTheBoardY2);
}

bool CheckValidCell(int X, int Y){
  
  if ((X > 0 && X < board.size()) && (Y > 0 && Y < board.size()))
  {
    return board[X][Y] == State::kEmpty;
  }
  return false;
}

void ExpandNeighbors(vector<int> &currentNode, int goalNode[2]) {
  int XPointToStartFindNeighbors = currentNode[0] - 1;
  int YPointToStartFindNeighbors = currentNode[1] - 1;
  int GValue = currentNode[2];
  for (int YPoint = 0; YPoint < 8; YPoint++)
  {
    for (int XPoint = 0; XPoint < 8; XPoint++)
    {
      if (CheckValidCell((XPointToStartFindNeighbors + XPoint), (YPointToStartFindNeighbors + YPoint))){
        int GValue2 = GValue + 1;
        int HeuristicValue = Heuristic((XPointToStartFindNeighbors + XPoint), (YPointToStartFindNeighbors + YPoint), goalNode[0], goalNode[1]);
        AddToOpen((XPointToStartFindNeighbors + XPoint), (YPointToStartFindNeighbors + YPoint), GValue2, HeuristicValue);
      }
    }
  }
  
}

vector<vector<State>> Search(int startPoint[2], int endPoint[2]){

  int cost = 0;
  int gridX = startPoint[0];
  int gridY = startPoint[1];
  int HeuristicValue = Heuristic(startPoint[0], startPoint[1], endPoint[0], endPoint[1]);

  AddToOpen(gridX, gridY, cost, HeuristicValue);

  while (openNodes.size() < 0)
  {
    CellSort();
    auto current_node = openNodes.back();
    openNodes.pop_back();
    gridX = current_node[0];
    gridY = current_node[1];
    board[gridX][gridY] = State::kPath;

    if (current_node[0] == endPoint[0] && current_node[1] == endPoint[1])
    {
      cout << "we reached the goal!\n";
      return board;
    }
    ExpandNeighbors(current_node, endPoint);
  }


  cout << "No path found!\n";
  return vector<vector<State>>{};
}


void ExpandNeighbors(vector<int> &currentNode, int (&goalNode)[2]) {
  int XPointToStartFindNeighbors = currentNode[0] - 1;
  int YPointToStartFindNeighbors = currentNode[1] - 1;
  int GValue = currentNode[2];
  for (int YPoint = 0; YPoint < 8; YPoint++)
  {
    for (int XPoint = 0; XPoint < 8; XPoint++)
    {
      if (CheckValidCell((XPointToStartFindNeighbors + XPoint), (YPointToStartFindNeighbors + YPoint))){
        int GValue2 = GValue + 1;
        int HeuristicValue = Heuristic((XPointToStartFindNeighbors + XPoint), (YPointToStartFindNeighbors + YPoint), goalNode[0], goalNode[1]);
        AddToOpen((XPointToStartFindNeighbors + XPoint), (YPointToStartFindNeighbors + YPoint), GValue2, HeuristicValue);
      }
    }
  }
  
}

int main() {

  int init[2]{0, 0};
  int goal[2]{4, 5};
  auto board = ReadBoardFile("1.board");
  auto solution = Search(init, goal);
  PrintBoard();

  // auto board = ReadBoardFile("1.board");
  // vector<vector<State>> solution;
  // int start[2]{0, 0};
  // int end[2]{4, 5};
  // solution = Search(board, start, end);
  // PrintBoard(solution);
}

