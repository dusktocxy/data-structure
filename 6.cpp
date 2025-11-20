#include <iostream>
using namespace std;

// 定义边结构体
typedef struct edge{
    int u, v, w; // u和v是边的两个顶点，w是边的权重
} edge;

int root[10000]; // 用于存储每个节点的根节点

// 查找节点x的根节点
int find(int x){
    int n = x;
    // 找到x的根节点
    while(root[x] != x){
        x = root[x];
    }
    // 路径压缩
    while(root[n] != n){
        int m = n;
        n = root[n];
        root[m] = x;
    }
    return x;
}

// Kruskal算法实现
void Kruskal(edge e[], int n, int m){
    // 初始化每个节点的根节点为自己
    for(int i = 0; i <= n; i++){
        root[i] = i;
    }
    int max = 0; // 用于存储最大边的权重
    // 遍历所有边
    for(int i = 0; i < m; i++){
        int ru = find(e[i].u); // 找到边的一个顶点的根节点
        int rv = find(e[i].v); // 找到边的另一个顶点的根节点
        // 如果两个顶点的根节点不同，说明这条边不会形成环
        if(ru != rv){
            root[ru] = rv; // 合并两个集合
            if(e[i].w > max){
                max = e[i].w; // 更新最大边的权重
            }
        }
        // 如果1和n已经连通，提前结束
        if(find(1) == find(n)){
            break;
        }
    }
    cout<<"修建整条地铁最少需要的天数为：" << max << endl; // 输出最大边的权重
}

// 冒泡排序对边按权重进行排序
void sort(edge e[], int len){
    for(int i = 0; i < len - 1; i++){
        for(int j = 0; j < len - i - 1; j++){
            if(e[j].w > e[j + 1].w){
                edge temp = e[j];
                e[j] = e[j + 1];
                e[j + 1] = temp;
            }
        }
    }
}

int main(){
    int n, m;
    cout<<"请输入交通枢纽的数量和候选隧道的数量："<<endl;
    cin >> n >> m; // 输入节点数和边数
    edge e[10000]; // 定义边数组
    // 输入每条边的信息
    cout<<"请输入每条隧道的起点、终点和长度："<<endl;
    for(int i = 0; i < m; i++){
        cin >> e[i].u >> e[i].v >> e[i].w;
    }
    sort(e, m); // 对边进行排序
    Kruskal(e, n, m); // 调用Kruskal算法
    return 0;
}