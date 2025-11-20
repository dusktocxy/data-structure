#include <iostream>
#include <unordered_map>
#include <fstream>
using namespace std;

// 定义节点结构体
typedef struct Node {
    char data; // 节点数据
    int weight; // 节点权重
    Node *lchild, *rchild; // 左右子节点指针
} Node, *HTree;

// 初始化哈希表
void InitHashMap(unordered_map<char, HTree> &hashmap) {
    ifstream file("source.txt");
    if (!file.is_open()) {
        cout << "文件打开失败" << endl;
        return;
    }
    char ch;
    while (file.get(ch)) {
        if (hashmap.find(ch) == hashmap.end()) {
            HTree node = (HTree)malloc(sizeof(Node));
            node->data = ch;
            node->weight = 1;
            node->lchild = NULL;
            node->rchild = NULL;
            hashmap[ch] = node;
        } else {
            hashmap[ch]->weight++;
        }
    }
    file.close(); // 关闭文件
}

// 初始化哈夫曼树
HTree InitHuffmanTree(unordered_map<char, HTree> &hashmap) {
    HTree node[hashmap.size()];
    int i = 0;
    for (auto &item : hashmap) {
        node[i] = item.second;
        i++;
    }
    HTree root = NULL;
    for(int i=0;i<hashmap.size()-1;i++){
        int min1=-1,min2=-1;
        for(int j=0;j<hashmap.size();j++){
            if(node[j]!=NULL){
                if(min1==-1||node[j]->weight<node[min1]->weight){
                    min2=min1;
                    min1=j;
                }
                else if(min2==-1||node[j]->weight<node[min2]->weight){
                    min2=j;
                }
            }
        }
        root=(HTree)malloc(sizeof(Node));
        root->data='\0';
        root->weight=node[min1]->weight+node[min2]->weight;
        root->lchild=node[min1];
        root->rchild=node[min2];
        node[min1]=root;
        node[min2]=NULL;
    }
    return root;
}

// 哈夫曼编码
string HuffmanEncode(HTree root, char data, string code = "") {
    if (root != NULL) {
        if (root->data == data) {
            return code;
        }
        string leftCode = HuffmanEncode(root->lchild, data, code + "0");
        if (!leftCode.empty()) {
            return leftCode;
        }
        string rightCode = HuffmanEncode(root->rchild, data, code + "1");
        if (!rightCode.empty()) {
            return rightCode;
        }
    }
    return "";
}

// 哈夫曼解码
char HuffmanDecode(HTree root, string code) {
    HTree p = root;
    for (int i = 0; i < code.length(); i++) {
        if (code[i] == '0') {
            p = p->lchild;
        } else if (code[i] == '1') {
            p = p->rchild;
        }
        if (p->data != '\0') {
            char result = p->data;
            p = root;
            return result;
        }
    }
    return '\0';
}

// 编码文件
void Encode(HTree root) {
    ofstream file("code.dat");
    ifstream source("source.txt");
    if (!source.is_open()) {
        cout << "文件打开失败" << endl;
        return;
    }
    char ch;
    while (source.get(ch)) {
        string code = HuffmanEncode(root, ch);
        file << code;
    }
    file.close();
    source.close();
}

// 解码文件
void Decode(HTree root) {
    ifstream file("code.dat");
    ofstream decode("recode.txt");
    if (!file.is_open()) {
        cout << "文件打开失败" << endl;
        return;
    }
    char ch;
    string code;
    while (file.get(ch)) {
        code += ch;
        char decodedChar = HuffmanDecode(root, code);
        if (decodedChar != '\0') {
            decode << decodedChar;
            code.clear(); // 清空编码字符串以继续解码下一个字符
        }
    }
    file.close();
    decode.close();
}

// 主函数
int main() {
    unordered_map<char, HTree> hashmap;
    InitHashMap(hashmap);
    ofstream file("Huffman.txt");
    HTree root = InitHuffmanTree(hashmap);
    for (auto &item : hashmap) {
        if(item.second->data=='\n'){
            cout<<"\\n"<<" "<<item.second->weight<<" "<<HuffmanEncode(root,item.second->data)<<endl;
            file<<"\\n"<<" "<<item.second->weight<<" "<<HuffmanEncode(root,item.second->data)<<endl;
        }
        else if(item.second->data==' '){
            cout<<"\' \'"<<" "<<item.second->weight<<" "<<HuffmanEncode(root,item.second->data)<<endl;
            file<<"\' \'"<<" "<<item.second->weight<<" "<<HuffmanEncode(root,item.second->data)<<endl;
        }
        else{
            cout<<item.second->data<<" "<<item.second->weight<<" "<<HuffmanEncode(root,item.second->data)<<endl;
            file<<item.second->data<<" "<<item.second->weight<<" "<<HuffmanEncode(root,item.second->data)<<endl;
        }
    }
    file.close();
    Encode(root);
    Decode(root);
    return 0;
}