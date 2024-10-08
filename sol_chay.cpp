#include <bits/stdc++.h>
using namespace std;
const int maxn = 256;
int power2[9] = {1, 2, 4, 8, 16, 32, 64, 128, 256};

//int dx[3] = {0, 1, 0};
//int dy[3] = {-1, 0, 1};

//int dx[3] = {0, 0, 1};
//int dy[3] = {-1, 1, 0};

//int dx[3] = {0, 0, 1};
//int dy[3] = {1, -1, 0};
//
//int dx[3] = {0, 1, 0};
//int dy[3] = {1, 0, -1};
//
//int dx[3] = {1, 0, 0};
//int dy[3] = {0, -1, 1};

int dx[3] = {1, 0, 0};
int dy[3] = {0, 1, -1};

bool visited[maxn][maxn];

//Định nghĩa các Steps là mỗi bước di chuyển
struct Steps {
    int p, x, y, s;
    Steps(int p, int x, int y, int s): p(p), x(x), y(y), s(s) {}

    string json_Steps() {
        return "{\"p\":" + to_string(this->p) + ", \"x\":" + to_string(this->x) + ", \"y\":" + to_string(this->y) + ", \"s\":" + to_string(this->s) + "}";
    }
};

//Định nghĩa đối tượng lưu các Steps
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
        if(!v_steps.empty()) ans.pop_back();
        ans += "]}";
        return ans;
    }
};

//Định nghĩa bảng (lưu các giá trị 0-3)
struct Board {
    int width = 0;
    int height = 0;
    vector<vector<int>> arr;

    Board() {}

    Board(int w, int h) : width(w), height(h), arr(h, vector<int>(w)) {}

    void read(ifstream &infile) {
        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j)
                infile >> arr[i][j];
    }

    void print() const {
        for (const auto &row : arr) {
            for (int val : row) cout << val << " ";
            cout << endl;
        }
        cout << endl;
    }
};

//khởi tạo bảng từ dữ kiện đầu vào
void init(Board &start_board, Board &goal_board){
    ifstream infile("input.txt");
    int w, h;
    infile >> w >> h;

    start_board = Board(w, h);
    goal_board = Board(w, h);

    start_board.read(infile);
    goal_board.read(infile);
}


//lưu và cập nhật các Steps trong quá trình đưa ô (u, v) về vị trí (i, j)
//vì tìm số lần lật chuỗi nên nếu lùi thì cần cộng để tìm ra vị trí của cái cần dời
//sau khi tìm được, chỉ đơn giản cần thao tác tại vị trí muốn đưa tới
void init_Steps(Board &board, Answer &answer, int i, int j, int u, int v) {
    int w = board.width;
    int h = board.height;
    int m = abs(v - j);
    int n = abs(u - i);

    //đưa về cùng cột
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

    //đưa về cùng hàng
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
//    else if(u < i) {
//        vector<int> vt;
//        for(int k = 0; k <= i; ++k) vt.push_back(board.arr[k][j]);
//        for(int k = 0; k <= i; ++k) {
//            int x = (k + i + 1 - n) % (i + 1);
//            board.arr[k][j] = vt[x];
//        }
//
//        for(int k = i; k >= u + 1; --k) {
//            Steps step = Steps(0, j, i, 1);
//            answer.push_Steps(step);
//        }
//    }
}


void die_cutting(Board &start_board, Board &goal_board, Answer &answer) {
    int m = start_board.height;
    int n = start_board.width;

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

            if(goal_board.arr[j][k] == start_board.arr[j1][k1]) break;

            for(int t = 0; t < 3; ++t) {
                int a = j1 + dx[t];
                int b = k1 + dy[t];
                if(a >= 0 && a < m && b >= 0 && b < n && !visited[a][b] && a * n + b > i) {
                    visited[a][b] = true;
                    q.push({a, b});
                }
            }
        }

        //cout << i << " " << j << " " << k << " " << j1 << " " << k1 << endl;
        init_Steps(start_board, answer, j, k, j1, k1);
    }
}

void check(Board &start_board, Board &goal_board, Answer answer) {
    int w = start_board.width;
    int h = start_board.height;
    int n = answer.v_steps.size();
    for(int i = 0; i < n; ++i) {
        int p = answer.v_steps[i].p;
        int x = answer.v_steps[i].y;
        int y = answer.v_steps[i].x;
        int s = answer.v_steps[i].s;
        int d = (p + 2) / 3;
        //cout << p << " " << x << " " << y << " " << s << " " << d << endl;

        if(s == 0) {
            for(int l = max(0, y); l < min(y + power2[d], w); ++l) {
                vector<int> vt;
                int e = max(x, 0);
                int f = min(x + power2[d], h);
                for(int k = e; k < h; ++k) vt.push_back(start_board.arr[k][l]);
                for(int k = e; k < h; ++k) {
                    int x = (k - e + h - e + f - e) % (h - e);
                    start_board.arr[k][l] = vt[x];
                }
            }
        }
        else if(s == 1) {
            for(int l = max(0, y); l < min(y + power2[d], w); ++l) {
                vector<int> vt;
                int e = min(x + power2[d], h);
                for(int k = 0; k < e; ++k) vt.push_back(start_board.arr[k][l]);
                for(int k = 0; k < e; ++k) {
                    int x = (k + e - e + max(0, x)) % (e);
                   start_board.arr[k][l] = vt[x];
                }
            }
        } else if(s == 2) {
            for(int l = max(0, x); l < min(x + power2[d], h); ++l) {
                vector<int> vt;
                int e = max(y, 0);
                int f = min(y + power2[d], w);
                for(int k = e; k < w; ++k) vt.push_back(start_board.arr[l][k]);
                for(int k = e; k < w; ++k) {
                    int x = (k - e + w - e + f - e) % (w - e);
                    start_board.arr[l][k] = vt[x];
                }
            }
        } else if(s == 3) {
            for(int l = max(0, x); l < min(x + power2[d], h); ++l) {
                vector<int> vt;
                int e = min(y + power2[d], w);
                for(int k = 0; k < e; ++k) vt.push_back(start_board.arr[l][k]);
                for(int k = 0; k < e; ++k) {
                    int x = (k + e - e + max(0, y)) % (e);
                    start_board.arr[l][k] = vt[x];
                }
            }
        }
        //start_board.print();
    }

    int check_count = 0;
    for(int i = 0; i < start_board.height; ++i) {
        for(int j = 0; j < start_board.width; ++j) {
            if(start_board.arr[i][j] == goal_board.arr[i][j]) check_count++;
        }
    }
    cout << check_count << " " << answer.v_steps.size() << " " << (double)check_count - (double)answer.v_steps.size() / 20 << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    Board start_board, goal_board;
    Answer answer;

    init(start_board, goal_board);
    Board start_board1 = start_board;

    die_cutting(start_board, goal_board, answer);
    die_cutting(start_board, goal_board, answer);
    ofstream f("output.txt");
    f << answer.json_Answer() << endl;

    check(start_board1, goal_board, answer);
    cout << "OK";
    return 0;
}
