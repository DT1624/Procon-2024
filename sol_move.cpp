//cải tiến thuật toán từ di chuyển từng ô, thành di chuyển các ma trận 2^n x 2^n
#include <bits/stdc++.h>
using namespace std;
const int maxn = 256;
int power2[9] = {1, 2, 4, 8, 16, 32, 64, 128, 256};
//int check_count = 0;
int chuoi[256];

int dx_left[3] = {0, 1, -1};
int dy_left[3] = {1, 0, 0};

int dx_right[3] = {0, -1, 1};
int dy_right[3] = {-1, 0, 0};

int dx_top[3] = {1, 0, 0};
int dy_top[3] = {0, 1, -1};

int dx_bottom[3] = {-1, 0, 0};
int dy_bottom[3] = {0, -1, 1};

int dx[4] = {1, 0, 0, -1};
int dy[4] = {0, 1, -1, 0};

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
    for(int i = 0; i < 256; ++i) chuoi[i] = tach(i).size();
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

void init_steps4(Board &board, Answer &answer, int i, int j, int i1, int j1, int mnx, int mxx, int mny, int mxy) {
    int w = board.width;
    int h = board.height;
    int m = abs(i - i1);
    int n = abs(j - j1);
    //cout << i << " " << j << " " << i1 << " " << j1 << endl;

    vector<int> v1 = tach(m);
    vector<int> v2 = tach(n);

    if(j1 > j) {
        vector<int> vt;
        for(int k = j; k < w; ++k) vt.push_back(board.arr[i1][k]);
        for(int k = j; k < w; ++k) {
            int x = (k - j + w - j + n) % (w - j);
            board.arr[i1][k] = vt[x];
        }

        for(int k = 0; k < n; ++k) {
            Steps step = Steps(0, j, i1, 2);
            answer.push_Steps(step);
        }
    } /*else if(j1 < j) {
        vector<int> vt;
        for(int k = mny; k < w; ++k) vt.push_back(board.arr[i1][k]);
        for(int k = mny; k < w; ++k) {
            int x = (k - mny + w - mny - n) % (w - mny);
            board.arr[i1][k] = vt[x];
        }

        for(int k = 0; k < w - mny - n; ++k) {
            Steps step = Steps(0, mny, i1, 2);
            answer.push_Steps(step);
        }
    }*/

    if(i1 > i) {
        vector<int> vt;
        for(int k = i; k < h; ++k) vt.push_back(board.arr[k][j]);
        for(int k = i; k < h; ++k) {
            int x = (k - i + h - i + m) % (h - i);
            board.arr[k][j] = vt[x];
        }

        for(int k = 0; k < m; ++k) {
            Steps step = Steps(0, j, i, 0);
            answer.push_Steps(step);
        }
    }
    //board.print();
}

void init_steps3(Board &board, Answer &answer, int i, int j, int i1, int j1, int mnx, int mxx, int mny, int mxy) {
    int w = board.width;
    int h = board.height;
    int m = abs(i - i1);
    int n = abs(j - j1);
    //cout << i << " " << j << " " << i1 << " " << j1 << endl;

    vector<int> v1 = tach(m);
    vector<int> v2 = tach(n);

    /*if(j1 > j) {
        vector<int> vt;
        for(int k = j; k < w; ++k) vt.push_back(board.arr[i1][k]);
        for(int k = j; k < w; ++k) {
            int x = (k - j + w - j + n) % (w - j);
            board.arr[i1][k] = vt[x];
        }

        for(int k = 0; k < n; ++k) {
            Steps step = Steps(0, j, i1, 3);
            answer.push_Steps(step);
        }
    } else*/ if(j1 < j) {
        vector<int> vt;
        for(int k = mny; k <= j; ++k) vt.push_back(board.arr[i1][k]);
        for(int k = mny; k <= j; ++k) {
            int x = (k - mny + j - mny + 1 - n) % (j - mny + 1);
            board.arr[i1][k] = vt[x];
        }

        for(int k = 0; k < n; ++k) {
            Steps step = Steps(0, j, i1, 3);
            answer.push_Steps(step);
        }
    }

    if(i1 > i) {
        vector<int> vt;
        for(int k = i; k < h; ++k) vt.push_back(board.arr[k][j]);
        for(int k = i; k < h; ++k) {
            int x = (k - i + h - i + m) % (h - i);
            board.arr[k][j] = vt[x];
        }

        for(int k = 0; k < m; ++k) {
            Steps step = Steps(0, j, i, 0);
            answer.push_Steps(step);
        }
    }
    //board.print();
}

void init_steps2(Board &board, Answer &answer, int i, int j, int i1, int j1, int mnx, int mxx, int mny, int mxy) {
    int w = board.width;
    int h = board.height;
    int m = abs(i - i1);
    int n = abs(j - j1);
    //cout << i << " " << j << " " << i1 << " " << j1 << endl;

    vector<int> v1 = tach(m);
    vector<int> v2 = tach(n);

    if(j1 > j) {
        vector<int> vt;
        for(int k = j; k < w; ++k) vt.push_back(board.arr[i1][k]);
        for(int k = j; k < w; ++k) {
            int x = (k - j + w - j + n) % (w - j);
            board.arr[i1][k] = vt[x];
        }

        for(int k = 0; k < n; ++k) {
            Steps step = Steps(0, j, i1, 2);
            answer.push_Steps(step);
        }
    } /*else if(j1 < j) {
        vector<int> vt;
        for(int k = mny; k < w; ++k) vt.push_back(board.arr[i1][k]);
        for(int k = mny; k < w; ++k) {
            int x = (k - mny + w - mny - n) % (w - mny);
            board.arr[i1][k] = vt[x];
        }

        for(int k = 0; k < w - mny - n; ++k) {
            Steps step = Steps(0, mny, i1, 2);
            answer.push_Steps(step);
        }
    }*/

    if(i1 < i) {
        vector<int> vt;
        for(int k = 0; k <= i; ++k) vt.push_back(board.arr[k][j]);
        for(int k = 0; k <= i; ++k) {
            int x = (k + i + 1 + h - i - m) % (i + 1);
            board.arr[k][j] = vt[x];
        }

        for(int k = 0; k < m; ++k) {
            Steps step = Steps(0, j, i, 1);
            answer.push_Steps(step);
        }
    }
    //board.print();
}

void init_steps1(Board &board, Answer &answer, int i, int j, int i1, int j1, int mnx, int mxx, int mny, int mxy) {
    int w = board.width;
    int h = board.height;
    int m = abs(i - i1);
    int n = abs(j - j1);
    //cout << i << " " << j << " " << i1 << " " << j1 << endl;

    vector<int> v1 = tach(m);
    vector<int> v2 = tach(n);

    /*if(j1 > j) {
        vector<int> vt;
        for(int k = j; k < w; ++k) vt.push_back(board.arr[i1][k]);
        for(int k = j; k < w; ++k) {
            int x = (k - j + w - j + n) % (w - j);
            board.arr[i1][k] = vt[x];
        }

        for(int k = 0; k < n; ++k) {
            Steps step = Steps(0, j, i1, 2);
            answer.push_Steps(step);
        }
    } else*/ if(j1 < j) {
        vector<int> vt;
        for(int k = mny; k <= j; ++k) vt.push_back(board.arr[i1][k]);
        for(int k = mny; k <= j; ++k) {
            int x = (k - mny + j - mny + 1 - n) % (j - mny + 1);
            board.arr[i1][k] = vt[x];
        }

        for(int k = 0; k < n; ++k) {
            Steps step = Steps(0, j, i1, 3);
            answer.push_Steps(step);
        }
    }

    if(i1 < i) {
        vector<int> vt;
        for(int k = 0; k <= i; ++k) vt.push_back(board.arr[k][j]);
        for(int k = 0; k <= i; ++k) {
            int x = (k + i + 1 + h - i - m) % (i + 1);
            board.arr[k][j] = vt[x];
        }

        for(int k = 0; k < m; ++k) {
            Steps step = Steps(0, j, i, 1);
            answer.push_Steps(step);
        }
    }
    //board.print();
}

void die_cutting(Board &start_board, Board &goal_board, Answer &answer) {
    int m = start_board.height;
    int n = start_board.width;

    int mnx = m, mxx = 0, mny = n, mxy = 0;

    for(int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j) {
            if(start_board.arr[i][j] != goal_board.arr[i][j]) {
                mnx = min(mnx, i);
                mxx = max(mxx, i);
                mny = min(mny, j);
                mxy = max(mxy, j);
            }
        }
    }
    cout << mnx << " " << mxx << " " << mny << " " << mxy << endl;

    if(mxx == m - 1 && mxy == n - 1) {
        for(int i = mnx; i <= mxx; ++i) {
            for(int j = mny; j <= mxy; ++j) {
                    //cout << i << " " << j << endl;
                memset(visited, false, sizeof(visited));
                if(goal_board.arr[i][j] == start_board.arr[i][j]) continue;

                int i1, j1;
                queue<pair<int, int>> q;
                q.push({i, j});
                visited[i][j] = true;

                while(!q.empty()) {
                    pair<int, int> p = q.front();
                    q.pop();
                    i1 = p.first, j1 = p.second;

                    if(goal_board.arr[i][j] == start_board.arr[i1][j1]) break;
                    for(int k = 0; k < 4; ++k) {
                        int a = i1 + dx[k];
                        int b = j1 + dy[k];
                        if(a >= mnx && a <= mxx && b >= mny && b <= mxy && !visited[a][b] && a * n + b > i * n + j && b >= j) {
                            q.push({a, b});
                            visited[a][b] = true;
                        }
                    }
                }
                init_steps4(start_board, answer, i, j, i1, j1, mnx, mxx, mny, mxy);
            }
        }
    } else if(mxx == m - 1 && mny == 0) {
        for(int i = mnx; i <= mxx; ++i) {
            for(int j = mxy; j >= mny; --j) {
                    //cout << i << " " << j << endl;
                memset(visited, false, sizeof(visited));
                if(goal_board.arr[i][j] == start_board.arr[i][j]) continue;

                int i1, j1;
                queue<pair<int, int>> q;
                q.push({i, j});
                visited[i][j] = true;

                while(!q.empty()) {
                    pair<int, int> p = q.front();
                    q.pop();
                    i1 = p.first, j1 = p.second;

                    if(goal_board.arr[i][j] == start_board.arr[i1][j1]) break;
                    for(int k = 0; k < 4; ++k) {
                        int a = i1 + dx[k];
                        int b = j1 + dy[k];
                        if(a >= mnx && a <= mxx && b >= mny && b <= mxy && !visited[a][b] && a * n - b > i * n - j && b <= j) {
                            q.push({a, b});
                            visited[a][b] = true;
                        }
                    }
                }
                init_steps3(start_board, answer, i, j, i1, j1, mnx, mxx, mny, mxy);
            }
        }
    } else if(mnx == 0 && mxy == n - 1) {
        for(int i = mxx; i >= mnx; --i) {
            for(int j = mny; j <= mxy; ++j) {
                    //cout << i << " " << j << endl;
                memset(visited, false, sizeof(visited));
                if(goal_board.arr[i][j] == start_board.arr[i][j]) continue;

                int i1, j1;
                queue<pair<int, int>> q;
                q.push({i, j});
                visited[i][j] = true;

                while(!q.empty()) {
                    pair<int, int> p = q.front();
                    q.pop();
                    i1 = p.first, j1 = p.second;

                    if(goal_board.arr[i][j] == start_board.arr[i1][j1]) break;
                    for(int k = 0; k < 4; ++k) {
                        int a = i1 + dx[k];
                        int b = j1 + dy[k];
                        if(a >= mnx && a <= mxx && b >= mny && b <= mxy && !visited[a][b] &&  b - a * n > j - i * n && b >= j) {
                            q.push({a, b});
                            visited[a][b] = true;
                        }
                    }
                }
                init_steps2(start_board, answer, i, j, i1, j1, mnx, mxx, mny, mxy);
            }
        }
    } else if(mnx == 0 && mny == 0) {
        for(int i = mxx; i >= mnx; --i) {
            for(int j = mxy; j >= mny; --j) {
                    //cout << i << " " << j << endl;
                memset(visited, false, sizeof(visited));
                if(goal_board.arr[i][j] == start_board.arr[i][j]) continue;

                int i1, j1;
                queue<pair<int, int>> q;
                q.push({i, j});
                visited[i][j] = true;

                while(!q.empty()) {
                    pair<int, int> p = q.front();
                    q.pop();
                    i1 = p.first, j1 = p.second;

                    if(goal_board.arr[i][j] == start_board.arr[i1][j1]) break;
                    for(int k = 0; k < 4; ++k) {
                        int a = i1 + dx[k];
                        int b = j1 + dy[k];
                        if(a >= mnx && a <= mxx && b >= mny && b <= mxy && !visited[a][b] && a * n + b < i * n + j && b <= j) {
                            q.push({a, b});
                            visited[a][b] = true;
                        }
                    }
                }
                init_steps1(start_board, answer, i, j, i1, j1, mnx, mxx, mny, mxy);
            }
        }
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
                    int z = (k - e + h - e + f - e) % (h - e);
                    start_board.arr[k][l] = vt[z];
                }
            }
        }
        else if(s == 1) {
            for(int l = max(0, y); l < min(y + power2[d], w); ++l) {
                vector<int> vt;
                int e = min(x + power2[d], h);
                for(int k = 0; k < e; ++k) vt.push_back(start_board.arr[k][l]);
                for(int k = 0; k < e; ++k) {
                    int z = (k + e - e + max(0, x)) % (e);
                   start_board.arr[k][l] = vt[z];
                }
            }
        } else if(s == 2) {
            for(int l = max(0, x); l < min(x + power2[d], h); ++l) {
                vector<int> vt;
                int e = max(y, 0);
                int f = min(y + power2[d], w);
                for(int k = e; k < w; ++k) vt.push_back(start_board.arr[l][k]);
                for(int k = e; k < w; ++k) {
                    int z = (k - e + w - e + f - e) % (w - e);
                    start_board.arr[l][k] = vt[z];
                }
            }
        } else if(s == 3) {
            for(int l = max(0, x); l < min(x + power2[d], h); ++l) {
                vector<int> vt;
                int e = min(y + power2[d], w);
                for(int k = 0; k < e; ++k) vt.push_back(start_board.arr[l][k]);
                for(int k = 0; k < e; ++k) {
                    int z = (k + e - e + max(0, y)) % (e);
                    start_board.arr[l][k] = vt[z];
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
    cout << fixed << setprecision(2) << check_count << " " << answer.v_steps.size() << " " << (double)check_count - (double)answer.v_steps.size() / 20 << endl;
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
//    while(check_count != start_board.width * start_board.height) {
//        die_cutting(start_board, goal_board, answer);
//        check_count = 0;
//        for(int i = 0; i < start_board.height; ++i) {
//            for(int j = 0; j < start_board.width; ++j) {
//                if(start_board.arr[i][j] == goal_board.arr[i][j]) check_count++;
//            }
//        }
//    }
    ofstream f("output.txt");
    f << answer.json_Answer() << endl;

    check(start_board1, goal_board, answer);
    cout << "OK\n";
    return 0;
}
