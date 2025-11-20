#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <windows.h>
#include <fstream>
#include <unordered_map>
using namespace std;

// 定义全局数组
int arr1[100000];
int arr2[100000];
int arr3[100000];
int arr4[100000];
int arr5[100000];
int arr6[100000];
int arr7[100000];
int arr8[100000];
int arr9[9][11];

// 冒泡排序
void bubbleSort(int arr[], int n) {
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if(arr[j]>arr[j+1]){
                int temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
        }
    }
}

// 插入排序
void InsertionSort(int arr[], int n){
    for(int i=1;i<n;i++){
        int key=arr[i];
        int j;
        for(j=i-1;j>=0;j--){
            if(arr[j]>key){
                arr[j+1]=arr[j];
            }
            else{
                break;
            }
        }
        arr[j+1]=key;
    }
}

// 选择排序
void SelectionSort(int arr[], int n){
    for(int i=0;i<n-1;i++){
        int min=i;
        for(int j=i+1;j<n;j++){
            if(arr[j]<arr[min]){
                min=j;
            }
        }
        swap(arr[i],arr[min]);
    }
}

// 希尔排序
void ShellSort(int arr[], int n){
    int gap=n/2;
    while(gap>=1){
        for(int i=gap;i<n;i++){
            int temp=arr[i];
            int j;
            for(j=i-gap;j>=0;j-=gap){
                if(arr[j]>temp){
                    arr[j+gap]=arr[j];
                }
                else{
                    break;
                }
            }
            arr[j+gap]=temp;
        }
        gap/=2;
    }
}

// 快速排序的分区函数
int Partition(int arr[],int low,int high){
    int pivot=arr[low];
    while(low<high){
        while(low<high&&arr[high]>=pivot){
            high--;
        }
        arr[low]=arr[high];
        while(low<high&&arr[low]<=pivot){
            low++;
        }
        arr[high]=arr[low];
    }
    arr[low]=pivot;
    return low;
}

// 快速排序的分区函数（重载）
int Partition(pair<int,int> arr[],int low,int high){
    int pivot=arr[low].second;
    while(low<high){
        while(low<high&&arr[high].second>=pivot){
            high--;
        }
        arr[low]=arr[high];
        while(low<high&&arr[low].second<=pivot){
            low++;
        }
        arr[high]=arr[low];
    }
    arr[low].second=pivot;
    return low;
}

// 快速排序
void QuickSort(int arr[],int low,int high){
    if(low<high){
        int pi=Partition(arr,low,high);
        QuickSort(arr,low,pi-1);
        QuickSort(arr,pi+1,high);
    }
}

// 快速排序（重载）
void QuickSort(pair<int,int> arr[],int low,int high){
    if(low<high){
        int pi=Partition(arr,low,high);
        QuickSort(arr,low,pi-1);
        QuickSort(arr,pi+1,high);
    }
}

// 最大堆调整
void Max_Heapify(int arr[],int start,int end){
    int dad=start;
    int son=2*dad+1;
    while(son<=end){
        if(son+1<=end&&arr[son]<arr[son+1]){
            son++;
        }
        if(arr[son]<arr[dad]){
            return;
        }
        else{
            swap(arr[son],arr[dad]);
            dad=son;
            son=2*dad+1;
        }
    }
}

// 堆排序
void HeapSort(int arr[],int n){
    for(int i=n/2-1;i>=0;i--){
        Max_Heapify(arr,i,n-1);
    }
    for(int i=n-1;i>=1;i--){
        swap(arr[0],arr[i]);
        Max_Heapify(arr,0,i-1);
    }
}

// 归并排序
void MergeSort(int arr[],int n){
    int *a=arr;
    int *b=new int[n];
    for(int seg=1;seg<n;seg+=seg){
        for(int start=0;start<n;start+=seg+seg){
            int low=start,mid=min(start+seg,n),high=min(start+seg+seg,n);
            int i=low;
            int start1=low,end1=mid;
            int start2=mid,end2=high;
            while(start1<end1&&start2<end2){
                b[i++]=(a[start1]<a[start2])?a[start1++]:a[start2++];
            }
            while(start1<end1){
                b[i++]=a[start1++];
            }
            while(start2<end2){
                b[i++]=a[start2++];
            }
        }
        int *temp=a;
        a=b;
        b=temp;
    }
    if(a!=arr){
        for(int i=0;i<n;i++){
            b[i]=a[i];
        }
        b=a;
    }
    delete[] b;
}

// 获取数组中最大数的位数
int maxbit(int arr[],int n){
    int max=arr[0];
    for(int i=1;i<n;i++){
        if(arr[i]>max){
            max=arr[i];
        }
    }
    int d=1;
    int p=10;
    while(max>=p){
        max/=10;
        ++d;
    }
    return d;
}

// 基数排序
void RadixSort(int arr[],int n){
    int d=maxbit(arr,n);
    int *temp=new int[n];
    int *count=new int[10];
    int i,j,k;
    int radix=1;
    for(i=0;i<=d;i++){
        for(j=0;j<10;j++){
            count[j]=0;
        }
        for(j=0;j<n;j++){
            k=(arr[j]/radix)%10;
            count[k]++;
        }
        for(j=1;j<10;j++){
            count[j]=count[j]+count[j-1];
        }
        for(j=n-1;j>=0;j--){
            k=(arr[j]/radix)%10;
            temp[count[k]-1]=arr[j];
            count[k]--;
        }
        for(j=0;j<n;j++){
            arr[j]=temp[j];
        }
        radix*=10;
    }
    delete[] temp;
    delete[] count;
}

// 测试排序算法的性能
void test(int n){
    srand(time(NULL));
    ofstream outfile("9.txt");
    for(int i=0;i<100000;i++){
        int val=rand()%5000000;
        outfile<<val<<endl;
    }
    outfile.close();
    int i=0;
    ifstream infile("9.txt");
    while(infile>>arr1[i]){
        arr2[i]=arr1[i];
        arr3[i]=arr1[i];
        arr4[i]=arr1[i];
        arr5[i]=arr1[i];
        arr6[i]=arr1[i];
        arr7[i]=arr1[i];
        arr8[i]=arr1[i];
        i++;
    }
    infile.close();
    DWORD start,end;
    start=GetTickCount(); //获取系统时间
    bubbleSort(arr1,i);
    end=GetTickCount();  //获取系统时间
    arr9[1][n]=end-start;

    start=GetTickCount();
    InsertionSort(arr2,i);
    end=GetTickCount();
    arr9[2][n]=end-start;

    start=GetTickCount();
    SelectionSort(arr3,i);
    end=GetTickCount();
    arr9[3][n]=end-start;

    start=GetTickCount();
    ShellSort(arr4,i);
    end=GetTickCount();
    arr9[4][n]=end-start;

    start=GetTickCount();
    QuickSort(arr5,0,i-1);
    end=GetTickCount();
    arr9[5][n]=end-start;

    start=GetTickCount();
    HeapSort(arr6,i);
    end=GetTickCount();
    arr9[6][n]=end-start;

    start=GetTickCount();
    MergeSort(arr7,i);
    end=GetTickCount();
    arr9[7][n]=end-start;

    start=GetTickCount();
    RadixSort(arr8,i);
    end=GetTickCount();
    arr9[8][n]=end-start;
}

// 统计数组中每个元素出现的次数并排序
void getTimes(int arr[],int n){
    unordered_map<int,int> times;
    for(int i=0;i<n;i++){
        if(times.find(arr[i])==times.end()){
            times[arr[i]]=1;
        }
        else{
            times[arr[i]]++;
        }
    }
    int num=0;
    for(pair<int,int> p:times){
        num++;
    }
    pair<int,int> p[num];
    int i=0;
    for(pair<int,int> val:times){
        p[i]=val;
        i++;
    }
    QuickSort(p,0,num-1);
    for(int i=num-1;i>=0;i--){
        cout<<p[i].first<<" "<<p[i].second<<" "<<endl;
    }
}

// 主函数
int main(){
    for(int i=1;i<=10;i++){
        test(i);
    }
    cout<<"次数统计："<<" "<<endl;
    getTimes(arr1,100000);
    cout<<"冒泡排序时间："<<endl;
    for(int i=1;i<=10;i++){
        cout<<i<<" "<<arr9[1][i]<<"ms "<<endl;
    }
    cout<<"插入排序时间："<<endl;
    for(int i=1;i<=10;i++){
        cout<<i<<" "<<arr9[2][i]<<"ms "<<endl;
    }
    cout<<"选择排序时间："<<endl;
    for(int i=1;i<=10;i++){
        cout<<i<<" "<<arr9[3][i]<<"ms "<<endl;
    }
    cout<<"希尔排序时间："<<endl;
    for(int i=1;i<=10;i++){
        cout<<i<<" "<<arr9[4][i]<<"ms "<<endl;
    }
    cout<<"快速排序时间："<<endl;
    for(int i=1;i<=10;i++){
        cout<<i<<" "<<arr9[5][i]<<"ms "<<endl;
    }
    cout<<"堆排序时间："<<endl;
    for(int i=1;i<=10;i++){
        cout<<i<<" "<<arr9[6][i]<<"ms "<<endl;
    }
    cout<<"归并排序时间："<<endl;
    for(int i=1;i<=10;i++){
        cout<<i<<" "<<arr9[7][i]<<"ms "<<endl;
    }
    cout<<"基数排序时间："<<endl;
    for(int i=1;i<=10;i++){
        cout<<i<<" "<<arr9[8][i]<<"ms "<<endl;
    }
    return 0;
}