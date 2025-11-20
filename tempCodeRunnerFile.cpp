#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <tuple>
using namespace std;

// 定义路线结构体
typedef struct route{
    int inner_id; // 站点在路线中的序号
    int station_id; // 站点ID
    route *next; // 指向下一个站点的指针
}route;

// 定义路线列表结构体
typedef struct route_list{
    route *route_head[730]; // 每条路线的头指针
    route *route_cur[730]; // 每条路线的当前指针
}route_list;

// 初始化路线列表
void init_route_list(route_list &rl){
    for(int i=0;i<730;i++){
        rl.route_head[i]=(route*)malloc(sizeof(route));
        rl.route_head[i]->inner_id=-1;
        rl.route_head[i]->station_id=-1;
        rl.route_head[i]->next=NULL;
        rl.route_cur[i]=rl.route_head[i];
    }
}

// 插入路线
void insert_route(route_list &rl,int route_id,int inner_id,int station_id){
    route *new_route=(route*)malloc(sizeof(route));
    new_route->inner_id=inner_id;
    new_route->station_id=station_id;
    new_route->next=rl.route_cur[route_id]->next;
    rl.route_cur[route_id]->next=new_route;
    rl.route_cur[route_id]=new_route;
}

// 定义关系结构体
typedef struct relation{
    int route_id; // 路线ID
    int inner_id; // 站点在路线中的序号
    int station_id; // 站点ID
    relation *next; // 指向下一个关系的指针
}relation;

// 定义站点结构体
typedef struct station{
    int route_id; // 路线ID
    int inner_id; // 站点在路线中的序号
    relation *firstrel; // 指向第一个关系的指针
}station;

// 定义站点地图结构体
typedef struct station_map{
    station sta[7000]; // 站点数组
}station_map;

// 初始化站点地图
void init_station_map(station_map &sm,route_list rl){
    for(int i=0;i<7000;i++){
        sm.sta[i].route_id=-1;
        sm.sta[i].inner_id=-1;
        sm.sta[i].firstrel=NULL;
    }
    for(int i=0;i<730;i++){
        relation *rel;
        route *r=rl.route_head[i]->next;
        while(r!=NULL){
            if(r->next!=NULL){
                rel=(relation*)malloc(sizeof(relation));
                rel->route_id=i;
                rel->inner_id=r->next->inner_id;
                rel->station_id=r->next->station_id;
                rel->next=sm.sta[r->station_id].firstrel;
                sm.sta[r->station_id].firstrel=rel;
                
                rel=(relation*)malloc(sizeof(relation));
                rel->route_id=i;
                rel->inner_id=r->inner_id;
                rel->station_id=r->station_id;
                rel->next=sm.sta[r->next->station_id].firstrel;
                sm.sta[r->next->station_id].firstrel=rel;
            }
            r=r->next;
        }
    }
}

// 广度优先搜索，找到经过最少站点的路径
void BFS_least_visit(station_map &sm,int start_station,int end_station){  
    queue<int> q;
    q.push(start_station);   //将起点站点加入队列
    bool visited[7000];
    int pre_station[7000];
    for(int i=0;i<7000;i++){
        visited[i]=false;
        pre_station[i]=-1;
    }
    visited[start_station]=true;
    while(!q.empty()){
        int cur_station=q.front();
        q.pop();
        if(cur_station==end_station){  //找到终点站点
            stack<int> path;
            int cur_id=end_station;
            path.push(cur_id);  //将终点站点加入栈
            int min_visit=0;
            while(pre_station[cur_id]!=-1){
                path.push(pre_station[cur_id]);  //将路径上的站点加入栈
                cur_id=pre_station[cur_id];
                min_visit++;
            }
            cout<<"经过最少站点的次数为："<<min_visit<<endl;
            cout<<"经过站点最少的路径为："<<endl;
            while(!path.empty()){
                cout<<path.top();
                path.pop();
                if(!path.empty()){
                    cout<<"->";
                }
            }
            cout<<endl;
            return;
        }
        relation *rel=sm.sta[cur_station].firstrel;
        while(rel!=NULL){
            if(!visited[rel->station_id]){  //相邻站点且未访问过
                q.push(rel->station_id);  //将相邻站点加入队列
                visited[rel->station_id]=true;
                pre_station[rel->station_id]=cur_station;
            }
            rel=rel->next;
        }
    }
}

// 广度优先搜索，找到经过最少换乘的路径
void BFS_least_change(station_map &sm,route_list rl,int start_station,int end_station){
    bool route_visited[730];
    int pre_route[730];
    for(int i=0;i<730;i++){
        route_visited[i]=false;
        pre_route[i]=-1;
    }
    queue<int> q;
    relation *rel=sm.sta[start_station].firstrel;
    while(rel!=NULL){ //将起点站点所在的所有路线加入队列
        q.push(rel->route_id);
        route_visited[rel->route_id]=true;
        rel=rel->next;
    }
    while(!q.empty()){
        int cur_route=q.front();
        q.pop();
        route *r=rl.route_head[cur_route]->next;
        while(r!=NULL){
            if(r->station_id==end_station){  //找到终点站点
                stack<int> path;
                int route_id=cur_route;
                path.push(route_id);  //将终点路线加入栈
                int min_change=0;
                while(pre_route[route_id]!=-1){
                    path.push(pre_route[route_id]);  //将路径上的路线加入栈
                    route_id=pre_route[route_id];
                    min_change++;
                }
                cout<<"经过最少换乘的次数为："<<min_change<<endl;
                cout<<"经过换乘最少的线路为："<<endl;
                while(!path.empty()){
                    cout<<path.top();
                    path.pop();
                    if(!path.empty()){
                        cout<<"->";
                    }
                }
                cout<<endl;
                return;
            }
            r=r->next;
        }
        r=rl.route_head[cur_route]->next;
        while(r!=NULL){
            relation *rel=sm.sta[r->station_id].firstrel;
            while(rel!=NULL){
                if(!route_visited[rel->route_id]){  //相邻路线且未访问过
                    q.push(rel->route_id);   //将相邻路线加入队列
                    route_visited[rel->route_id]=true;
                    pre_route[rel->route_id]=cur_route; 
                }
                rel=rel->next;
            }
            r=r->next;
        }
    }
}

int main(){
    route_list rl;
    init_route_list(rl);
    ifstream fin("7.txt");
    if(!fin.is_open()){
        cout<<"文件打开失败"<<endl;
        return 0;
    }
    int route_id,inner_id,station_id;
    while(fin>>route_id>>inner_id>>station_id){
        insert_route(rl,route_id,inner_id,station_id);
    }
    fin.close();
    station_map sm;
    init_station_map(sm,rl);
    cout<<"请输入起点站点ID：";
    int start_station;
    cin>>start_station;
    cout<<"请输入终点站点ID：";
    int end_station;
    cin>>end_station;
    BFS_least_change(sm,rl,start_station,end_station);
    BFS_least_visit(sm,start_station,end_station);
    return 0;
}