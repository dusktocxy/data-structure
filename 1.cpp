#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <unistd.h> // for sysconf, _SC_CLK_TCK
#include <ctime>

using namespace std;
namespace fs = std::filesystem;

// 链表结构体
typedef struct SLNode {
    string DurationT; // 持续时间
    float PMemory;    // 内存占用
    string Pname;     // 进程名
    SLNode *next;     // 指向下一个节点的指针
} SLNode, *SLinkList;

typedef struct SListInfo {
    SLinkList Head; // 链表头指针
    SLinkList Tail; // 链表尾指针
    int length;     // 链表长度
} SListInfo;

typedef struct DLNode {
    string EndT;      // 结束时间
    int DurationSec;  // 持续时间（秒）
    string Pname;     // 进程名
    DLNode *pre;      // 指向前一个节点的指针
    DLNode *next;     // 指向下一个节点的指针
} DLNode, *DLinkList;

typedef struct DListInfo {
    DLinkList Head; // 链表头指针
    DLinkList Tail; // 链表尾指针
    int length;     // 链表长度
} DListInfo;

// 时间转换函数：将时间字符串转换为秒数
int TimeToSeconds(const string &time) {
    int h, m, s;
    sscanf(time.c_str(), "%d:%d:%d", &h, &m, &s);
    return h * 3600 + m * 60 + s;
}

// 时间转换函数：将秒数转换为时间字符串
string SecondsToTime(int seconds) {
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;
    return to_string(h) + ":" + to_string(m) + ":" + to_string(s);
}

// 初始化单链表
void InitSList(SListInfo &sList) {
    sList.Head = new SLNode;
    sList.Head->next = nullptr;
    sList.Tail = sList.Head;
    sList.length = 0;
}

// 初始化双链表
void InitDList(DListInfo &dList) {
    dList.Head = new DLNode;
    dList.Head->pre = nullptr;
    dList.Head->next = nullptr;
    dList.Tail = dList.Head;
    dList.length = 0;
}

// 插入节点到单链表
void SListInsert(SListInfo &sList, const string &DurationT, float PMemory, const string &Pname) {
    SLNode *p = new SLNode;
    p->DurationT = DurationT;
    p->PMemory = PMemory;
    p->Pname = Pname;
    p->next = nullptr;

    SLNode *prev = sList.Head;
    SLNode *curr = sList.Head->next;

    // 按内存占用排序插入
    while (curr && PMemory < curr->PMemory) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = p;
    p->next = curr;

    if (!curr) sList.Tail = p;

    sList.length++;
}

// 插入节点到双链表
void DListInsert(DListInfo &dList, const string &EndT, int DurationSec, const string &Pname) {
    DLNode *p = new DLNode;
    p->EndT = EndT;
    p->DurationSec = DurationSec;
    p->Pname = Pname;
    p->pre = nullptr;
    p->next = nullptr;

    DLNode *curr = dList.Head->next;
    DLNode *prev = dList.Head;

    // 按持续时间排序插入
    while (curr && DurationSec > curr->DurationSec) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = p;
    p->pre = prev;
    p->next = curr;
    if (curr) curr->pre = p;

    if (!curr) dList.Tail = p;

    dList.length++;
}

// 显示单链表内容
void SListShow(const SListInfo &sList) {
    SLNode *p = sList.Head->next;
    cout << setw(23) << left << "进程名" << setw(15) << left << "持续时间" << setw(20) << right << "内存占用(KB)" << endl;
    while (p != nullptr) {
        cout << setw(20) << left << p->Pname << setw(10) << left << p->DurationT << setw(15) << right << p->PMemory << endl;
        p = p->next;
    }
}

// 显示双链表内容
void DListShow(const DListInfo &dList) {
    DLNode *p = dList.Head->next;
    cout << setw(37) << left << "进程名" << setw(15) << left << "持续时间" << setw(21) << right << "结束时间" << endl;
    while (p != nullptr) {
        cout << setw(35) << left << p->Pname << setw(15) << left << SecondsToTime(p->DurationSec) << setw(15) << right << p->EndT << endl;
        p = p->next;
    }
}

// 删除单链表
void DeleteSList(SListInfo &sList) {
    SLNode *p = sList.Head->next;
    while (p != nullptr) {
        SLNode *temp = p->next;
        delete p;
        p = temp;
    }
    delete sList.Head;
    sList.Head = nullptr;
    sList.Tail = nullptr;
    sList.length = 0;
}

// 删除双链表
void DeleteDList(DListInfo &dList) {
    DLNode *p = dList.Head->next;
    while (p != nullptr) {
        DLNode *temp = p->next;
        delete p;
        p = temp;
    }
    delete dList.Head;
    dList.Head = nullptr;
    dList.Tail = nullptr;
    dList.length = 0;
}

// 获取系统启动时间
time_t GetSystemBootTime() {
    ifstream statFile("/proc/stat");
    if (!statFile) {
        cerr << "无法打开 /proc/stat 文件！\n";
        return 0;
    }

    string line;
    while (getline(statFile, line)) {
        if (line.find("btime") == 0) {
            istringstream iss(line.substr(6));
            time_t bootTime;
            iss >> bootTime;
            return bootTime;
        }
    }

    cerr << "未找到 btime 信息！\n";
    return 0;
}

// 列出进程并加入链表
void ListProcesses(DListInfo &pEnd, SListInfo &pRun) {
    time_t systemBootTime = GetSystemBootTime(); // 获取系统启动时间
    if (systemBootTime == 0) {
        cerr << "系统启动时间不可用，无法继续。\n";
        return;
    }

    for (const auto &entry : fs::directory_iterator("/proc")) { // 遍历 /proc 目录
        string pid = entry.path().filename().string(); // 获取进程 ID
        if (all_of(pid.begin(), pid.end(), ::isdigit)) {  // 判断是否为数字
            string statusPath = "/proc/" + pid + "/status";  // status 文件路径
            string statPath = "/proc/" + pid + "/stat";  // stat 文件路径
            ifstream statusFile(statusPath);   // 打开 status 文件
            ifstream statFile(statPath);   // 打开 stat 文件

            string name;
            unsigned long long startTimeTicks = 0;
            long memoryUsageKB = -1;

            if (statusFile) {
                string line;
                while (getline(statusFile, line)) {
                    if (line.find("Name:") == 0) {  // 查找进程名
                        name = line.substr(6);    // 获取进程名
                        name.erase(name.find_last_not_of(" \t\n\r\f\v") + 1); // 去除末尾空格
                    } else if (line.find("VmRSS:") == 0) {  // 查找内存占用
                        istringstream iss(line.substr(6));   // 从第6个字符开始读取
                        iss >> memoryUsageKB;  // 读取内存占用
                    }
                }
            }

            if (statFile) {       // 读取进程启动时间
                string value;
                int field = 1;
                while (getline(statFile, value, ' ')) {    // 以空格为分隔符读取
                    if (field == 22) {
                        startTimeTicks = stoull(value);   // 读取启动时间
                        break;
                    }
                    field++;
                }
            }

            if (!name.empty()) {  // 如果进程名不为空
                long clockTicks = sysconf(_SC_CLK_TCK); // 获取每秒时钟滴答数
                time_t currentTime = time(nullptr);   // 获取当前时间
                if (startTimeTicks) {
                    time_t startTime = systemBootTime + startTimeTicks / clockTicks;  // 计算进程启动时间
                    auto elapsedTime = currentTime - startTime;  // 计算进程运行时间
                    if (memoryUsageKB < 0) {  // 如果内存占用不可用
                        DListInsert(pEnd, ctime(&currentTime), elapsedTime, name); // 插入结束进程链表
                    } else {
                        SListInsert(pRun, SecondsToTime(elapsedTime), memoryUsageKB, name); // 插入正在运行进程链表
                    }
                }
            }
        }
    }
}

int main() {
    while(true){
        DListInfo pEnd;  // 结束进程链表
        SListInfo pRun;  // 正在运行进程链表
        InitDList(pEnd);
        InitSList(pRun);
        ListProcesses(pEnd, pRun);
        cout << "正在运行的进程:\n";
        SListShow(pRun);
        sleep(5); // 暂停5秒
        cout << "\n已结束的进程:\n";
        DListShow(pEnd);
        sleep(5); // 暂停5秒
        DeleteSList(pRun);
        DeleteDList(pEnd);
    }
    return 0;
}