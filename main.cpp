#include <bits/stdc++.h>
using namespace std;
const int maxn = 256;

int dx[3] = {0, 1, 0};
int dy[3] = {-1, 0, 1};

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

        for(int k = 1; k <= v - j; ++k) {
            Steps step = Steps(0, v - k, u, 2);
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

        for(int k = 1; k <= j - v; ++k) {
            Steps step = Steps(0, v + k, u, 3);
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

        for(int k = 1; k <= u - i; ++k) {
            Steps step = Steps(0, j, u - k, 0);
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

        for(int k = 1; k <= i - u; ++k) {
            Steps step = Steps(0, j, u + k, 1);
            answer.push_Steps(step);
        }
    }
    print_Board(board);
}

void die_cutting(Board &start_board, Board &goal_board, Answer &answer) {
    int m = start_board.height;
    int n = start_board.width;
    cout << m << " " << n << endl;
    for(int i = 0; i < m * n; ++i) {
        int j = i / n;
        int k = i % n;
        int j1 = j, k1 = k;
        int temp = i;
        //đi tìm ô cần đổi vị trí
        if(goal_board.arr[j][k] == start_board.arr[j1][k1]) continue;
        while(goal_board.arr[j][k] != start_board.arr[j1][k1]) {
            ++temp;
            j1 = temp / n;
            k1 = temp % n;
        }
        cout << i << " " << j << " " << k << " " << j1 << " " << k1 << endl;
        //if(answer.v_steps.size() > 80000) return;
        init_Steps(start_board, answer, j, k, j1, k1);
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    Board start_board;
    Board goal_board;

    Answer answer;

//    Steps s = Steps(1, 2, 3, 0);
//    Steps z = Steps(2, 1, 0, 3);
//    Steps t = Steps(1, 0, 1, 1);
//    Answer answer;
//    answer.push_Steps(s);
//    answer.push_Steps(z);
//    answer.push_Steps(t);
//    cout << answer.json_Answer();
//    Steps m(1, 0, 1, 1);
//    answer.push_Steps(m);
//    cout << answer.json_Answer();
    init(start_board, goal_board);
    print_Board(start_board);
    print_Board(goal_board);

    die_cutting(start_board, goal_board, answer);
    ofstream f("output3.txt");
    f << answer.json_Answer() << endl;
    //f << 2555 << endl;
    return 0;
}
