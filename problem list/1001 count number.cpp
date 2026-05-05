#include<iostream>
#include<string>
using namespace std;

const int N=110;

int main()
{
    int n;
    string s;
    cin>>n;

    cin.ignore();//清除输入缓冲区的换行符\n，防止getline读取到空行导致多读一个空字符串

    while(n--)
    {
        //cin遇到空格会停止输入，
        //getline读取整行输入，包括空格，直到遇到换行符\n为止
        getline(cin,s);

        int count=0;
        int len=s.length();

        for(int j=0;j<len;j++)
            if(s[j]-'0'>=0&&s[j]-'0'<=9)
                count++;

        cout<<count<<endl;
    }

    
    return 0;
}