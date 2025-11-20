#include <iostream>
#include <vector>
#define INF 1e9
using namespace std;

// 定义边结构体
struct edge{
    int u,v,weight;
};

// 定义图结构体
typedef struct MGraph{
    vector<vector<int>> arcs;
}MGraph;

int root[10000];

// 查找并查集的根节点
int find(int x){
    int n=x;
    while(root[x]!=x){
        x=root[x];
    }
    while(root[n]!=n){
        int temp=n;
        n=root[n];
        root[temp]=x;
    }
    return x;
}

// Kruskal算法求最小生成树
void Kruskal(int n,int m,edge e[]){
    for(int i=1;i<=n;i++){
        root[i]=i;
    }
    int sumweight=0;
    cout<<"最小生成树的边是："<<endl;
    for(int i=0;i<m;i++){
        int ru=find(e[i].u);
        int rv=find(e[i].v);
        if(ru!=rv){
            root[ru]=rv;
            sumweight+=e[i].weight;
            cout<<e[i].u<<"--"<<e[i].v<<" "<<e[i].weight<<endl;
        }
    }
    cout<<"最小生成树的权值是："<<sumweight<<endl;
}

// 冒泡排序对边按权值排序
void sort(edge e[],int m){
    for(int i=0;i<m-1;i++){
        for(int j=0;j<m-i-1;j++){
            if(e[j].weight>e[j+1].weight){
                swap(e[j],e[j+1]);
            }
        }
    }
}

// Prim算法求最小生成树
void Prim(int n, MGraph graph) {
    int key[10000];
    bool visited[10000];
    int pre[10000];  
    int sumweight = 0;
    for (int i = 1; i <= n; i++) {
        key[i] = INF;  
        visited[i] = false;  
        pre[i] = -1;
    }
    key[1] = 0;
    for (int i = 0; i < n ; i++) {
        int min = INF, u = -1;
        for (int j = 1; j <= n; j++) {
            if (!visited[j] && key[j] < min) {
                min = key[j];
                u = j;
            }
        }
        visited[u] = true;
        sumweight += key[u];
        for (int v = 1; v <= n; v++) {
            if (graph.arcs[u][v] != INF && !visited[v] && graph.arcs[u][v] < key[v]) {
                key[v] = graph.arcs[u][v];
                pre[v] = u;
            }
        }
    }
    cout << "最小生成树的边是：" << endl;
    for (int i = 2; i <= n; i++) {
        if (pre[i] != -1) {
            cout << pre[i] << "--" << i << " " << graph.arcs[pre[i]][i] << endl;
        }
    }
    cout << "最小生成树的权值是：" << sumweight << endl;
}

int main(){
    int n,m;
    cin>>n>>m;
    edge e[10000];
    MGraph g;
    g.arcs.resize(n+1,vector<int>(n+1,INF));
    for(int i=0;i<m;i++){
        int u,v,weight;
        cin>>u>>v>>weight;
        e[i].u=u;
        e[i].v=v;
        e[i].weight=weight;
        g.arcs[u][v]=weight;
        g.arcs[v][u]=weight;
    }
    sort(e,m);
    cout<<"Kruskal算法："<<endl;
    Kruskal(n,m,e);
    cout<<"Prim算法："<<endl;
    Prim(n,g);
    return 0;
}