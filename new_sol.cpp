//cải tiến thuật toán từ di chuyển từng ô, thành di chuyển các ma trận 2^n x 2^n
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

vector<int> tach(int n) {
    vector<int> v;
    for(int i = 8; i >= 0; --i) {
        if(n == 0) return v;
        if(n >= power2[i]) {
            n -= power2[i];
            v.push_back(i);
        }
    }
    return v;
}

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
            ans += "\n" + step.json_Steps() + ",";
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
    //cout << i << " " << j << " " << u << " " << v << endl;

    vector<int> v1 = tach(m);
    vector<int> v2 = tach(n);

    //đưa về cùng cột
    if(v > j) {
        for(auto c : v1) {
            for(int l = u; l < min(u + power2[c], h); ++l) {
                vector<int> vt;
                for(int k = j; k < w; ++k) vt.push_back(board.arr[l][k]);
                for(int k = j; k < w; ++k) {
                    int x = (k - j + w - j + power2[c]) % (w - j);
                    board.arr[l][k] = vt[x];
                }
            }

            Steps step = Steps(3 * c - 2, j, u, 2);
            if(c == 0) step.p = 0;
            answer.push_Steps(step);
        }
    }
    else if(v < j) {
        for(auto c : v1) {
            for(int l = u; l < min(u + power2[c], h); ++l) {
                vector<int> vt;
                for(int k = 0; k <= j; ++k) vt.push_back(board.arr[l][k]);
                for(int k = 0; k <= j; ++k) {
                    int x = (k + j + 1 - power2[c]) % (j + 1);
                    board.arr[l][k] = vt[x];
                }
            }

            Steps step = Steps(3 * c - 2, j - power2[c] + 1, u, 3);
            if(c == 0) step.p = 0;
            answer.push_Steps(step);
        }
    }

    //đưa về cùng hàng
    if(u > i) {
        for(auto c : v2) {
            for(int l = j; l < min(j + power2[c], w); ++l) {
                vector<int> vt;
                for(int k = i; k < h; ++k) vt.push_back(board.arr[k][l]);
                for(int k = i; k < h; ++k) {
                    int x = (k - i + h - i + power2[c]) % (h - i);
                    board.arr[k][l] = vt[x];
                }
            }

            Steps step = Steps(3 * c - 2, j, i, 0);
            if(c == 0) step.p = 0;
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
    //board.print();
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
                if(a >= 0 && a < m && b >= 0 && b < n && !visited[a][b] && a * n + b > i && b >= k) {
                    visited[a][b] = true;
                    q.push({a, b});
                }
            }
        }

        //cout << i << " " << j << " " << k << " " << j1 << " " << k1 << endl;
        init_Steps(start_board, answer, j, k, j1, k1);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    Board start_board, goal_board;
    Answer answer;

    init(start_board, goal_board);

    die_cutting(start_board, goal_board, answer);

    ofstream f("output.txt");
    f << answer.json_Answer() << endl;
//    vector<int> v = tach(3);
//    for(auto c :v) cout << c << " ";
    cout << "OK";
    return 0;
}
