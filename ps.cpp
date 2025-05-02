#include <iostream>
#include <string>
#include <regex>
#include <cstdlib>
#include <windows.h>
#include <conio.h>

using namespace std;

// 设置控制台颜色
void SetColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// 清屏函数
void ClearScreen() {
    system("cls");
}

// 验证IPv4地址格式
bool ValidateIPv4(const string& ip) {
    regex pattern("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    return regex_match(ip, pattern);
}

// 绘制界面边框
void DrawBorder() {
    SetColor(11); // 浅蓝色
    cout << "╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                                                            ║" << endl;
    cout << "║                  CMD PING 工具 v1.0                        ║" << endl;
    cout << "║                                                            ║" << endl;
    cout << "╠════════════════════════════════════════════════════════════╣" << endl;
    cout << "║                                                            ║" << endl;
    cout << "║  请输入IPv4地址:                           	             ║" << endl;
    cout << "║                                                            ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝" << endl;
}

// 执行ping命令
void PingIP(const string& ip) {
	cout<<"\n"; 
    string command = "ping " + ip;
    SetColor(7); // 白色
    cout << "\n执行ping命令: " << command << "\n" << endl;
    system(command.c_str());
}

int main() {
    // 设置控制台标题
    SetConsoleTitle("CMD Ping工具");
    
    // 隐藏光标
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    string ipAddress;
    bool running = true;
    
    while (running) {
        ClearScreen();
        DrawBorder();
        
        // 定位到输入位置
        COORD coord;
        coord.X = 25;
        coord.Y = 6;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        
        SetColor(15); // 亮白色
        getline(cin, ipAddress);
        
        if (ipAddress.empty()) {
            SetColor(12); // 红色
            cout<<"\n"; 
            cout << "\n错误: 请输入IP地址!" << endl;
            Sleep(1500);
            continue;
        }
        
        if (ipAddress == "exit" || ipAddress == "quit") {
        	cout<<"\n\n"; 
            running = false;
            continue;
        }
        
        if (!ValidateIPv4(ipAddress)) {
            SetColor(12); // 红色
            cout<<"\n"; 
            cout << "\n错误: 无效的IPv4地址格式!" << endl;
            Sleep(1500);
            continue;
        }
        
        PingIP(ipAddress);
        
        SetColor(14); // 黄色
        cout<<"\n"; 
        cout << "\n按任意键继续，或输入'exit'退出..." << endl;
        _getch();
    }
    
    // 恢复光标
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    return 0;
}
