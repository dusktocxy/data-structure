#include <iostream> // 包含输入输出流库
#include <fstream>  // 包含文件流库
#include <string>   // 包含字符串库
using namespace std; // 使用标准命名空间

#define MAXSIZE 1000 // 定义队列和栈的最大大小
#define ROW 20       // 定义迷宫的行数
#define COL 20       // 定义迷宫的列数

bool visisted[ROW][COL] = {false}; // 定义一个二维数组来标记访问状态，并初始化为false

// 定义一个点结构体，包含x和y坐标
struct point {
    int x;
    int y;
};

// 定义一个队列结构体，包含指向点的指针、队列的前端、后端和大小
struct queue {
    point *pBase; // 指向队列基地址的指针
    int front;    // 队列的前端索引
    int rear;     // 队列的后端索引
    int size;     // 队列的当前大小
};

// 初始化队列函数
void InitQueue(queue &Q) {
    Q.pBase = (point*)malloc(MAXSIZE * sizeof(point)); // 动态分配内存
    Q.front = Q.rear = 0; // 初始化前端和后端索引为0
    Q.size = 0;           // 初始化队列大小为0
}

// 入队函数
void EnQueue(queue &Q, point p) {
    if ((Q.rear + 1) % MAXSIZE == Q.front) { // 判断队列是否已满
        cout << "队列已满" << endl; // 输出队列已满信息
        return;
    }
    Q.pBase[Q.rear] = p; // 将点加入队列
    Q.rear = (Q.rear + 1) % MAXSIZE; // 更新后端索引
    Q.size++; // 增加队列大小
}

// 出队函数
void DeQueue(queue &Q, point &p) {
    if (Q.front == Q.rear) { // 判断队列是否为空
        cout << "队列为空" << endl; // 输出队列为空信息
        return;
    }
    p = Q.pBase[Q.front]; // 获取队列前端的点
    Q.front = (Q.front + 1) % MAXSIZE; // 更新前端索引
    Q.size--; // 减少队列大小
}

// 定义一个栈结构体，包含指向点的指针、栈顶指针和栈的大小
struct stack {
    point *pBase; // 指向栈基地址的指针
    point *pTop;  // 指向栈顶的指针
    int stacksize; // 栈的当前大小
};

// 初始化栈函数
void InitStack(stack &S) {
    S.pBase = (point*)malloc(MAXSIZE * sizeof(point)); // 动态分配内存
    S.pTop = S.pBase; // 初始化栈顶指针为栈基地址
    S.stacksize = MAXSIZE; // 初始化栈大小为最大值
}

// 入栈函数
void Push(stack &S, point p) {
    if (S.pTop - S.pBase >= S.stacksize) { // 判断栈是否已满
        S.pBase = (point*)realloc(S.pBase, (S.stacksize + MAXSIZE) * sizeof(point)); // 重新分配内存
        S.pTop = S.pBase + S.stacksize; // 更新栈顶指针
        S.stacksize += MAXSIZE; // 增加栈大小
    }
    *S.pTop = p; // 将点加入栈
    S.pTop++; // 更新栈顶指针
}

// 出栈函数
void Pop(stack &S, point &p) {
    if (S.pTop == S.pBase) { // 判断栈是否为空
        cout << "栈为空" << endl; // 输出栈为空信息
        return;
    }
    p = *(S.pTop - 1); // 获取栈顶的点
    S.pTop--; // 更新栈顶指针
}

// 广度优先搜索函数，寻找迷宫路径
bool BFS(int maze[ROW][COL], point start, point end) {
    const int dx[4] = {0, 0, -1, 1}; // 定义四个方向的x轴偏移量
    const int dy[4] = {-1, 1, 0, 0}; // 定义四个方向的y轴偏移量
    queue q; // 定义队列
    InitQueue(q); // 初始化队列
    EnQueue(q, start); // 将起点加入队列
    visisted[start.x][start.y] = true; // 标记起点为已访问
    point prev[ROW][COL]; // 定义前驱点数组
    for (int i = 0; i < ROW; i++) { // 初始化前驱点数组
        for (int j = 0; j < COL; j++) {
            prev[i][j].x = -1;
            prev[i][j].y = -1;
        }
    }
    while (q.front != q.rear) { // 当队列不为空时
        point current; // 定义当前点
        DeQueue(q, current); // 出队
        if (current.x == end.x && current.y == end.y) { // 如果当前点是终点
            stack path; // 定义栈
            InitStack(path); // 初始化栈
            point p; // 定义点
            p = current; // 将当前点赋值给p
            while (p.x != -1 && p.y != -1) { // 当p不是起点时
                Push(path, p); // 将p入栈
                p = prev[p.x][p.y]; // 更新p为前驱点
            }
            while (path.pTop != path.pBase) { // 当栈不为空时
                point p; // 定义点
                Pop(path, p); // 出栈
                cout << "(" << p.x +1<< "," << p.y+1 << ")"; // 输出点坐标
                if (path.pTop != path.pBase) { // 如果栈不为空
                    cout << "->"; // 输出箭头
                }
            }
            return true; // 返回true，表示找到路径
        }
        for (int i = 0; i < 4; i++) { // 遍历四个方向
            int nx = current.x + dx[i]; // 计算新点的x坐标
            int ny = current.y + dy[i]; // 计算新点的y坐标
            if (nx >= 0 && nx < ROW && ny >= 0 && ny < COL && !visisted[nx][ny] && maze[nx][ny] == 0) { // 如果新点在迷宫范围内且未访问且不是障碍物
                point p; // 定义点
                p.x = nx; // 设置点的x坐标
                p.y = ny; // 设置点的y坐标
                EnQueue(q, p); // 将点加入队列
                visisted[nx][ny] = true; // 标记点为已访问
                prev[nx][ny] = current; // 设置前驱点
            }
        }
    }
    return false; // 返回false，表示没有找到路径
}

// 主函数
int main() {
    ifstream file("2.txt"); // 打开文件
    if (!file.is_open()) { // 如果文件打开失败
        cout << "文件打开失败" << endl; // 输出文件打开失败信息
    }
    string str; // 定义字符串
    int maze[ROW][COL]; // 定义迷宫数组
    int i = 0; // 定义行索引
    while (file >> str) { // 读取文件内容
        for (int j = 0; j < COL; j++) { // 遍历列
            maze[i][j] = str[j] - '0'; // 将字符转换为整数并存入迷宫数组
        }
        i++; // 增加行索引
    }
    file.close(); // 关闭文件
    cout << "迷宫地图:" << endl; // 输出迷宫地图
    for (int i = 0; i < ROW; i++) { // 遍历行
        for (int j = 0; j < COL; j++) { // 遍历列
            cout << maze[i][j]; // 输出迷宫数组元素
        }
        cout << endl; // 换行
    }
    point start, end; // 定义起点和终点
    cout << "请输入起点坐标(x,y):"; // 提示输入起点坐标
    cin >> start.x >> start.y; // 输入起点坐标
    cout << "请输入终点坐标(x,y):"; // 提示输入终点坐标
    cin >> end.x >> end.y; // 输入终点坐标
    maze[start.x-1][start.y-1] = 0; // 将起点设置为可通行
    maze[end.x-1][end.y-1] = 0; // 将终点设置为可通行
    start.x--; // 起点x坐标减1
    start.y--; // 起点y坐标减1
    end.x--; // 终点x坐标减1
    end.y--; // 终点y坐标减1
    if (!BFS(maze, start, end)) { // 如果没有找到路径
        cout << "没有找到路径" << endl; // 输出没有找到路径信息
    }
    return 0; // 返回0，表示程序正常结束
}