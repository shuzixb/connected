#ifndef CONNECTED_H
#define CONNECTED_H
#include<iostream>
#include<ctime>
#include<random>
#include<vector>
#include"stack.h"
#define MAX_PICTURE_KIND 16
#define MAX_ROW_NUMBER 20
#define MAX_COLUMN_NUMBER 20
using namespace std;

class Connected               //连连看算法类
{
public:
    Connected(int row,int column);   //x表示水平方向图片数，y表示垂直方向图片数
    void Print();             //打印出连连看数据矩阵

    /* 判断a、b两图片是否相连通，a_row表示a所处的行数,a_column表示a点所处的列数,b类推 */
    bool Is_connected(int a_row,int a_column,int b_row,int b_column);
    void Ai_remove();         //自动智能消除图片
private:
    int _row;           //图片行数
    int _column;        //图片列数
    int _con[MAX_ROW_NUMBER+2][MAX_COLUMN_NUMBER+2]={0};       //用于存储连连看图片数据的二维数组
    int _num_of_kind[MAX_PICTURE_KIND]={0};//用于记录矩阵内每种图的数量

    bool Straigh_line_connected(int a_row,int a_column,int b_row,int b_column);  //判断a,b两点是否直线相通
    bool One_turning_connected(int a_row,int a_column,int b_row,int b_column);   //判断a,b两点是否经一个拐点相通
    bool Two_turning_connected(int a_row,int a_column,int b_row,int b_column);   //判断a,b两点是否能经两个拐点相通
    void Auto_remove_picture();      //自动清除暴力算法
    bool Is_all_remove();            //判断矩阵内图片是否都已清除
};


Connected::Connected(int row=6,int column=6)    //构造函数
{
    if(row>MAX_ROW_NUMBER)              //输入行数大于最大值则取最大值
        row=MAX_ROW_NUMBER;
    if(column>MAX_COLUMN_NUMBER)        //输入的列数大于最大值则取最大值
        column=MAX_COLUMN_NUMBER;
    if((row*column)%2!=0)            //确保行列相乘为偶数，也就是图片总数为偶数
    {    
        cout<<"行或列应至少有一个为偶数"<<endl;
        row<MAX_ROW_NUMBER?row++:row--;
    }

    
    _row=row;
    _column=column;

    Stack<int> v;          //临时存储数字矩阵的到一个栈内
    Stack<int> s;          //临时存储每个种类的图片的个数

    static default_random_engine e(time(0));                  //C++11中生成随机数的(知识点)
    static uniform_int_distribution<unsigned> u(1,MAX_PICTURE_KIND);

    for(int i=0;i<_row*_column;i++)
    {
        int k=u(e);
        v.push(k);
        _num_of_kind[k-1]++;
    }   

    for(size_t i=0;i<MAX_PICTURE_KIND;i++)  //将矩阵中出现次数是奇数的图片ID压入栈中
        if(_num_of_kind[i]%2!=0)
            s.push(i+1);

    while(!s.empty())        //每种图片的数量必须是偶数
    {
        int k=s.pop();
        for(size_t i=0;i<_row*_column;i++)
        {
            if(v[i]==k)
            {
                v[i]=s.pop();
                break;
            }
        }
    }

    for(int i=1;i<=_row;i++)
        for(int j=1;j<=_column;j++)
            _con[i][j]=v.pop();

}

void Connected::Print()
{
    for(int i=0;i<=_row+1;i++)
    {
        for(int j=0;j<=_column+1;j++)
            cout<<_con[i][j]<<" ";
        cout<<endl;
    }
    cout<<endl;    
}
 
bool Connected::Straigh_line_connected(int a_row,int a_column,int b_row,int b_column)  //直线相连判断
{

    if((a_column==b_column)&&(a_row!=b_row))        //纵向在一条线上
    {
        int min_row=(a_row<b_row?a_row:b_row);
        int max_row=(a_row>b_row?a_row:b_row);
        for(int i=min_row+1;i<max_row;i++)
        {
            if(_con[i][a_column]!=0)
                return false;
        }
        return true;
    }
    if((a_row==b_row)&&(a_column!=b_column))        //横向在一条线上
    {
        int min_column=(a_column<b_column?a_column:b_column);
        int max_column=(a_column>b_column?a_column:b_column);
        for(int j=min_column+1;j<max_column;j++)
            if(_con[a_row][j]!=0)
                return false;
        return true;
    }    
    return false;
}

bool Connected::One_turning_connected(int a_row,int a_column,int b_row,int b_column)                   //一拐点相连判断
{
    if(_con[b_row][a_column]==0&&Straigh_line_connected(a_row,a_column,b_row,a_column)&&Straigh_line_connected(b_row,a_column,b_row,b_column))
        return true;
    else if(_con[a_row][b_column]==0&&Straigh_line_connected(a_row,a_column,a_row,b_column)&&Straigh_line_connected(a_row,b_column,b_row,b_column))
        return true;
    else
        return false;
}

bool Connected::Two_turning_connected(int a_row,int a_column,int b_row,int b_column)  //判断a,b两点是否能经两个拐点相连
{
    for(int i=0;i<_row+2;i++)
    {
        int tmp_a_row=i;
        int tmp_a_column=a_column;

        if(i==a_row)
            continue;
        if(tmp_a_row==b_row&&tmp_a_column==b_column)
            continue;

        int tmp_b_row=tmp_a_row;
        int tmp_b_column=b_column;

        if(_con[tmp_a_row][tmp_a_column]==0&&_con[tmp_b_row][tmp_b_column]==0
                &&Straigh_line_connected(a_row,a_column,tmp_a_row,tmp_a_column)
                &&Straigh_line_connected(tmp_a_row,tmp_a_column,tmp_b_row,tmp_b_column)
                &&Straigh_line_connected(tmp_b_row,tmp_b_column,b_row,b_column))
            return true;
    }


    for(int j=0;j<_column+2;j++)
    {
        int tmp_a_row=a_row;
        int tmp_a_column=j;

        if(j==a_column)
            continue;
        if(tmp_a_row==b_row&&tmp_a_column==b_column)
            continue;

        int tmp_b_row=b_row;
        int tmp_b_column=tmp_a_column;

        if(_con[tmp_a_row][tmp_a_column]==0&&_con[tmp_b_row][tmp_b_column]==0
                &&Straigh_line_connected(a_row,a_column,tmp_a_row,tmp_a_column)
                &&Straigh_line_connected(tmp_a_row,tmp_a_column,tmp_b_row,tmp_b_column)
                &&Straigh_line_connected(tmp_b_row,tmp_b_column,b_row,b_column))
            return true;
    }

    return false;
}

bool Connected::Is_connected(int a_row,int a_column,int b_row,int b_column)  //判断两点时否连通
{
    
    if(Straigh_line_connected(a_row,a_column,b_row,b_column)
            ||One_turning_connected(a_row,a_column,b_row,b_column)
            ||Two_turning_connected(a_row,a_column,b_row,b_column))
        return true;
    else
        return false;
}

void Connected::Auto_remove_picture()
{
    for(int i=1;i<=_row;i++)
    {
        for(int j=1;j<=_column;j++)
        {
            if(_con[i][j]==0)
                continue;
            for(int k=1;k<=_row;k++)
            {
                for(int s=1;s<=_column;s++)
                {
                    if(_con[k][s]==0||(i==k&&j==s))
                        continue;
                    if((_con[i][j]==_con[k][s])&&(Is_connected(i,j,k,s)))
                    {
                        _con[i][j]=0;
                        _con[k][s]=0;
                        Print();
                    }
                }
            }    
        }    
    }    
}

bool Connected::Is_all_remove()
{
    int flag=0;
    for(int i=0;i<=_row;i++)
        for(int j=0;j<=_column;j++)
        {
            if(_con[i][j]!=0)
                return false;
        }
    return true;
}

void Connected::Ai_remove()
{
    int flag=0;
    while(!Is_all_remove())
    {
        Auto_remove_picture();
        flag++;
        if(flag>10)
        {
            cout<<"进入死局"<<endl;
            break;
        }
    }
}

#endif //CONNECTED_H
