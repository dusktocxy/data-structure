#include <iostream>
#include <fstream>
using namespace std;
int main(){
    ifstream file("source.txt");
    if(!file){
        cout<<"文件打开失败"<<endl;
        return 0;
    }
    char ch;
    while(file.get(ch)){
        cout<<ch;
    }   
    file.close();
    return 0;
}