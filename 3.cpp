#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
using namespace std;

// 定义人员结构体
struct people{
    string name;       // 姓名
    string birthday;   // 出生日期
    int married;       // 婚姻状况
    string address;    // 住址
    int alive;         // 生死状况
    string deathday;   // 死亡日期
    string parent;     // 父母姓名
};

// 定义树节点结构体
typedef struct TreeNode{
    people data;               // 人员信息
    struct TreeNode *firstchild;  // 第一个子节点
    struct TreeNode *nextbrother; // 下一个兄弟节点
}TreeNode,*Tree;

// 初始化家谱树
Tree InitFamilyTree(){
    unordered_map<string,Tree> ftm; // 用于存储姓名和对应的树节点
    ifstream file("3.txt");        // 打开文件
    people p;
    if(!file.is_open()){
        cout<<"不能打开文件"<<endl;
        return NULL;
    }
    // 从文件中读取数据并构建树节点
    while(file>>p.name>>p.birthday>>p.married>>p.address>>p.alive>>p.deathday>>p.parent){
        Tree t=new TreeNode;
        t->data=p;
        t->firstchild=NULL;
        t->nextbrother=NULL;
        ftm[p.name]=t;
    }
    file.close();
    Tree root=NULL;
    // 构建家谱树
    for(auto &item:ftm){
        if(item.second->data.parent!="张父"){
            if(ftm.find(item.second->data.parent)!=ftm.end()){
                if(ftm[item.second->data.parent]->firstchild==NULL){
                    ftm[item.second->data.parent]->firstchild=item.second;
                }
                else{
                    Tree temp=ftm[item.second->data.parent]->firstchild;
                    while(temp->nextbrother!=NULL){
                        temp=temp->nextbrother;
                    }
                    temp->nextbrother=item.second;
                }
            }
        }
        else if(item.second->data.parent=="张父"){
            root=item.second;
        }
    }
    return root;
}

// 通过生日查询成员信息
void SearchByBirthday(Tree root,const string birthday){
    if(root!=NULL){
        if(root->data.birthday==birthday){
            string information="";
            information+="姓名："+root->data.name+" 出生日期："+root->data.birthday+" 婚姻状况："+(root->data.married?"已婚":"未婚")+" 住址："+root->data.address+" 生死："+(root->data.alive?"存活":"死亡")+" 死亡日期："+root->data.deathday;
            cout<<information<<endl;
            return;
        }
        SearchByBirthday(root->firstchild,birthday);
        SearchByBirthday(root->nextbrother,birthday);
    }
}

// 通过姓名查询成员信息
void SearchByName(Tree root,const string name){
    if(root!=NULL){
        if(root->firstchild!=NULL){
            Tree temp=root->firstchild;
            while(temp!=NULL){
                if(temp->data.name==name){
                    cout<<"姓名："+temp->data.name+" 出生日期："+temp->data.birthday+" 婚姻状况："+(temp->data.married?"已婚":"未婚")+" 住址："+temp->data.address+" 生死："+(temp->data.alive?"存活":"死亡")+" 死亡日期："+temp->data.deathday<<endl;
                    cout<<"父母姓名："<<root->data.name<<" 出生日期："<<root->data.birthday<<" 婚姻状况："<<(root->data.married?"已婚":"未婚")<<" 住址："<<root->data.address<<" 生死："<<(root->data.alive?"存活":"死亡")<<" 死亡日期："<<root->data.deathday<<endl;
                    Tree t=temp->firstchild;
                    while(t!=NULL){
                        cout<<"子女姓名："+t->data.name<<" 出生日期："+t->data.birthday<<" 婚姻状况："<<(t->data.married?"已婚":"未婚")<<" 住址："+t->data.address<<" 生死："<<(t->data.alive?"存活":"死亡")<<" 死亡日期："+t->data.deathday<<endl;
                        t=t->nextbrother;
                    }
                }
                temp=temp->nextbrother;
            }
        }
        SearchByName(root->firstchild,name);
        SearchByName(root->nextbrother,name);
    }
}

// 插入新成员
void Insert(Tree root,people p){
    if(root!=NULL){
        if(p.parent==root->data.name){
            Tree t=new TreeNode;
            t->data=p;
            if(root->firstchild==NULL){
                root->firstchild=t;
            }
            else{
                Tree temp=root->firstchild;
                while(temp->nextbrother!=NULL){
                    temp=temp->nextbrother;
                }
                temp->nextbrother=t;
            }
            ofstream file("3.txt",ios::app);
            file<<p.name<<" "<<p.birthday<<" "<<p.married<<" "<<p.address<<" "<<p.alive<<" "<<p.deathday<<" "<<p.parent<<endl;
            file.close();
            return;
        }
        Insert(root->firstchild,p);
        Insert(root->nextbrother,p);
    }
}

// 删除树节点
void Delete(Tree root){
    if(root==NULL){
        return;
    }
    Delete(root->firstchild);
    Delete(root->nextbrother);
    delete root;
}

// 删除成员
void DeleteMember(Tree root,const string name){
    if(root!=NULL){
        if(root->firstchild!=NULL){
            if(root->firstchild->data.name==name){
                Delete(root->firstchild);
                root->firstchild=NULL;
                return;
            }
        }
        if(root->nextbrother!=NULL){
            if(root->nextbrother->data.name==name){
                Delete(root->nextbrother);
                root->nextbrother=NULL;
                return;
            }
        }
        DeleteMember(root->firstchild,name);
        DeleteMember(root->nextbrother,name);
    }
}

// 打印家谱树
void PrintTree(Tree root,const string &prefix=""){
    if(root!=NULL){
        cout<<prefix<<"|--"<<root->data.name<<endl;
        PrintTree(root->firstchild,prefix+"|  ");
        PrintTree(root->nextbrother,prefix);
    }
}

// 打印指定世代成员信息
void PrintGenara(Tree root,int targena,int curentgena=1){
    if(root!=NULL){
        if(curentgena==targena){
            cout<<"姓名："+root->data.name+" 出生日期："+root->data.birthday+" 婚姻状况："+(root->data.married?"已婚":"未婚")+" 住址："+root->data.address+" 生死："+(root->data.alive?"存活":"死亡")+" 死亡日期："+root->data.deathday<<endl;
        }
        PrintGenara(root->firstchild,targena,curentgena+1);
        PrintGenara(root->nextbrother,targena,curentgena);
    }
}

// 清空文件内容
void ClearFile(){
    ofstream file("3.txt",ios::out);
    file.close();
}

// 保存家谱树到文件
void saveTree(Tree root ){
    if(root!=NULL){
        ofstream file("3.txt",ios::app);
        file<<root->data.name<<" "<<root->data.birthday<<" "<<root->data.married<<" "<<root->data.address<<" "<<root->data.alive<<" "<<root->data.deathday<<" "<<root->data.parent<<endl;
        file.close();
        saveTree(root->firstchild);
        saveTree(root->nextbrother);
    }
}

// 判断两人是否有亲属关系
bool relatives(Tree root,string name1,string name2){
    if(root==NULL){
        return false;
    }
    if(root!=NULL){
        if(root->nextbrother!=NULL){
            Tree temp=root;
            int count=0;
            while(temp!=NULL){
                if(temp->data.name==name1||temp->data.name==name2){
                    count++;
                }
                temp=temp->nextbrother;
            }
            if(count==2){
                cout<<name1<<"和"<<name2<<"是兄弟关系"<<endl;
                return true;
            }
        }
        if(root->firstchild!=NULL){
            int count=0;
            if(root->data.name==name1||root->data.name==name2){
                count++;
                Tree temp=root->firstchild;
                while(temp!=NULL){
                    if(temp->data.name==name1||temp->data.name==name2){
                        count++;
                    }
                    temp=temp->nextbrother;
                }
            }
            if(count==2){
                cout<<name1<<"和"<<name2<<"是父子关系"<<endl;
                return true;
            }
        }
        if(root->firstchild!=NULL){
            int count=0;
            if(root->data.name==name1||root->data.name==name2){
                count++;
                Tree temp1=root->firstchild;
                while(temp1!=NULL){
                    if(temp1->firstchild!=NULL){
                        Tree temp2=temp1->firstchild;
                        while(temp2!=NULL){
                            if(temp2->data.name==name1||temp2->data.name==name2){
                                count++;
                            }
                            temp2=temp2->nextbrother;
                        }
                    }
                    temp1=temp1->nextbrother;
                }
                if(count==2){
                    cout<<name1<<"和"<<name2<<"是祖孙关系"<<endl;
                    return true;
                }
            }
        }
    }
    return relatives(root->firstchild,name1,name2) || relatives(root->nextbrother,name1,name2);
}

// 修改成员信息
void modify(Tree root,string name){
    if(root!=NULL){
        if(root->data.name==name){
            people p;
            cout<<"请输入修改的信息"<<endl;
            cout<<"姓名：";
            cin>>p.name;
            cout<<"出生日期：";
            cin>>p.birthday;
            cout<<"婚姻状况(1为已婚，0为未婚)：";
            cin>>p.married;
            cout<<"住址：";
            cin>>p.address;
            cout<<"生死(1为存活，0为死亡)：";
            cin>>p.alive;
            cout<<"死亡日期（若存活则填无）：";
            cin>>p.deathday;
            cout<<"父母姓名：";
            cin>>p.parent;
            root->data=p;
            ClearFile();
            saveTree(root);
            return;
        }
        modify(root->firstchild,name);
        modify(root->nextbrother,name);
    }
}

int main(){
    while(true){
        Tree root=InitFamilyTree(); // 初始化家谱树
        int choice;
        cout<<"1.通过生日查询  2.通过姓名查询  3.添加成员  4.删除成员  5.打印成员 6.打印世代  7.判断关系 8.修改成员  9.退出"<<endl;
        cin>>choice;
        if(choice==1){
            string birthday;
            cout<<"请输入生日：";
            cin>>birthday;
            SearchByBirthday(root,birthday); // 通过生日查询成员信息
        }
        else if(choice==2){
            string name;
            cout<<"请输入姓名：";
            cin>>name;
            SearchByName(root,name); // 通过姓名查询成员信息
        }
        else if(choice==3){
            people p;
            cout<<"请输入姓名：";
            cin>>p.name;
            cout<<"请输入生日：";
            cin>>p.birthday;
            cout<<"请输入婚姻状况(1为已婚，0为未婚)：";
            cin>>p.married;
            cout<<"请输入住址：";
            cin>>p.address;
            cout<<"请输入生死(1为存活，0为死亡)：";
            cin>>p.alive;
            cout<<"请输入死亡日期（若存活则填无）：";
            cin>>p.deathday;
            cout<<"请输入父母姓名：";
            cin>>p.parent;
            Insert(root,p); // 插入新成员
        }
        else if(choice==4){
            string name;
            cout<<"请输入姓名：";
            cin>>name;
            DeleteMember(root,name); // 删除成员
            ClearFile();
            saveTree(root);
        }
        else if(choice==5){
            PrintTree(root); // 打印家谱树
        }
        else if(choice==6){
            int targena;
            cout<<"请输入要查询的世代：";
            cin>>targena;
            PrintGenara(root,targena); // 打印指定世代成员信息
        }
        else if(choice==7){
            string name1,name2;
            cout<<"请输入姓名1：";
            cin>>name1;
            cout<<"请输入姓名2：";
            cin>>name2;
            bool flag=relatives(root,name1,name2); // 判断两人是否有亲属关系
            if(flag==false){
                cout<<name1<<"和"<<name2<<"没有关系"<<endl;
            }
        }
        else if(choice==8){
            string name;
            cout<<"请输入姓名：";
            cin>>name;
            modify(root,name); // 修改成员信息
            ClearFile();
            saveTree(root);
        }
        else if(choice==9){
            break; // 退出程序
        }
    }
    system("pause");
    return 0;
}