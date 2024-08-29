#include <bits/stdc++.h>
using namespace std;
const int maxn = 256;

//int dx[3] = {0, 1, 0};
//int dy[3] = {-1, 0, 1};

//int dx[3] = {0, 1, 0};
//int dy[3] = {1, 0, -1};

int dx[3] = {1, 0, 0};
int dy[3] = {0, 1, -1};
bool visited[maxn][maxn];
bool check[maxn][maxn];

//Định nghĩa các Steps là mỗi bước di chuyển
struct Steps {
    int p, x, y, s;
    Steps(int p, int x, int y, int s): p(p), x(x), y(y), s(s) {}

    string json_Steps() {
        return "{\"p\":" + to_string(this->p) + ", \"x\":" + to_string(this->x) + ", \"y\":" + to_string(this->y) + ", \"s\":" + to_string(this->s) + "}";
    }
};

//Định nghãi đối tượng lưu các Steps
struct Answer{
    vector<Steps> v_steps;
    Answer() {}

    void push_Steps(Steps step) {
        v_steps.push_back(step);
    }

    string json_Answer() {
        string ans = "";
        ans += "{\"n\":" + to_string(v_steps.size()) + ", \"ops\":[";
        for(auto step : v_steps) {
            ans += step.json_Steps() + ",";
        }
        ans.pop_back();
        ans += "]}";
        return ans;
    }
};

//Định nghĩa các die/pattern
struct Die{

};
//Định nghĩa bảng (lưu các giá trị 0-3)
struct Board {
    int width = 0;
    int height = 0;
    //int x = 0, y = 0;
    int **arr;

    Board() {}

    Board(int w, int h) : width(w), height(h) {
        arr = new int*[h];
        for(int i = 0; i < h; ++i) {
            arr[i] = new int[w];
        }
    }
//    board(int w, int h, int x, int y): x(x), y(y) {
//        board(w, h);
//    }
};

void init(Board &start_board, Board &goal_board){
    ifstream infile("input.txt");
    int w, h;
    infile >> w >> h;
    //cout << w << " " << h <<endl;
    start_board = Board(w, h);
    goal_board = Board(w, h);
    cout << "*" << endl;
    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            infile >> start_board.arr[i][j];
        }
    }
    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            infile >> goal_board.arr[i][j];
        }
    }
}

void print_Board(Board board) {
    for(int i = 0; i < board.height; ++i) {
        for(int j = 0; j < board.width; ++j) {
            cout << board.arr[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

//lưu và cập nhật các Steps trong quá trình đưa ô (u, v) về vị trí (i, j)
//vì tìm số lần lật chuỗi nên nếu lùi thì cần cộng để tìm ra vị trí của cái cần dời
void init_Steps(Board &board, Answer &answer, int i, int j, int u, int v) {
    int w = board.width;
    int h = board.height;
    int m = abs(v - j);
    int n = abs(u - i);

    if(v > j) {
        vector<int> vt;
        for(int k = j; k < w; ++k) vt.push_back(board.arr[u][k]);
        for(int k = j; k < w; ++k) {
            int x = (k - j + w - j + m) % (w - j);
            board.arr[u][k] = vt[x];
        }

        for(int k = j; k <= v - 1; ++k) {
            Steps step = Steps(0, j, u, 2);
            answer.push_Steps(step);
        }
    }
    else if(v < j) {
        vector<int> vt;
        for(int k = 0; k <= j; ++k) vt.push_back(board.arr[u][k]);
        for(int k = 0; k <= j; ++k) {
            int x = (k + j + 1 - m) % (j + 1);
            board.arr[u][k] = vt[x];
        }

        for(int k = j; k >= v + 1; --k) {
            Steps step = Steps(0, j, u, 3);
            answer.push_Steps(step);
        }
    }

    if(u > i) {
        vector<int> vt;
        for(int k = i; k < h; ++k) vt.push_back(board.arr[k][j]);
        for(int k = i; k < h; ++k) {
            int x = (k - i + h - i + n) % (h - i);
            board.arr[k][j] = vt[x];
        }

        for(int k = i; k <= u - 1; ++k) {
            Steps step = Steps(0, j, i, 0);
            answer.push_Steps(step);
        }
    }
    else if(u < i) {
        vector<int> vt;
        for(int k = 0; k <= i; ++k) vt.push_back(board.arr[k][j]);
        for(int k = 0; k <= i; ++k) {
            int x = (k + i + 1 - n) % (i + 1);
            board.arr[k][j] = vt[x];
        }

        for(int k = i; k >= u + 1; --k) {
            Steps step = Steps(0, j, i, 1);
            answer.push_Steps(step);
        }
    }
    //print_Board(board);
}


void die_cutting(Board &start_board, Board &goal_board, Answer &answer) {
    int m = start_board.height;
    int n = start_board.width;
    cout << m << " " << n << endl;
    for(int i = 0; i < m * n; ++i) {
        memset(visited, false, sizeof(visited));
        int j = i / n;
        int k = i % n;
        int j1, k1;
        if(goal_board.arr[j][k] == start_board.arr[j][k]) continue;
        queue<pair<int, int>> q;
        q.push({j, k});
        visited[j][k] = true;
        //đi tìm ô cần đổi vị trí
        while(!q.empty()) {
            pair<int, int> p = q.front();
            q.pop();
            j1 = p.first, k1 = p.second;
            if(goal_board.arr[j][k] != start_board.arr[j1][k1]) {
                for(int t = 0; t < 3; ++t) {
                    int a = j1 + dx[t];
                    int b = k1 + dy[t];
                    if(a * n + b <= i) continue;
                    if(a >= 0 && a < m && b >= 0 && b < n && !visited[a][b]) {
                        visited[a][b] = true;
                        q.push({a, b});
                    }
                }
            } else break;
        }

        cout << i << " " << j << " " << k << " " << j1 << " " << k1 << endl;
        //if(answer.v_steps.size() > 100000) return;
        init_Steps(start_board, answer, j, k, j1, k1);
    }
}

//void checkAnswer(Board start_board, Board goal_board) {
//    ifstream file("input1.txt");
//    int n, p, x, y, s;
//    cin >> n;
//    int h = start_board.height;
//    int w = start_board.width;
//    for(int i = 0; i < n; ++i) {
//        cin >> p >> y >> x >> s;
//        int tmp = start_board.arr[x][y];
//        if(s == 0) {
//            for(int j = x; j < h - 1; ++j) start_board.arr[j][y] = start_board.arr[j + 1][y];
//            start_board.arr[h - 1][y] = tmp;
//        } else if(s == 1) {
//            for(int j = x; j >= 1; --j) start_board.arr[j][y] = start_board.arr[j - 1][y];
//            start_board.arr[0][y] = tmp;
//        } else if(s == 2) {
//            for(int j = y; j < w - 1; ++j) start_board.arr[x][j] = start_board.arr[x][j + 1];
//            start_board.arr[x][w - 1] = tmp;
//        } else if(s == 3) {
//            for(int j = y; j >= 0; --j) start_board.arr[x][j] = start_board.arr[x][j - 1];
//            start_board.arr[x][0] = tmp;
//        }
//    }
//
//}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    Board start_board;
    Board goal_board;

    Answer answer;

    init(start_board, goal_board);
    print_Board(start_board);
    print_Board(goal_board);

    die_cutting(start_board, goal_board, answer);

    for(int i = 0; i < start_board.height; ++i) {
        for(int j = 0; j < start_board.width; ++j) {
            if(start_board.arr[i][j] != goal_board.arr[i][j]) cout << i << " " << j << endl;
        }
    }

    ofstream f("output3.txt");
    f << answer.json_Answer() << endl;
    //print_Board(start_board);
    //print_Board(goal_board);
    return 0;
}
