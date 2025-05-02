#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> readInputProgram() {
    vector<string> lines;
    string line;
    
    cout << "Enter your C++ program (end with a line containing only 'END'):\n";
    while (getline(cin, line)) {
        if (line == "END") break;
        lines.push_back(line);
    }
    
    return lines;
}

void generateDualLanguageProgram(const vector<string>& cppProgram) {
    ofstream outFile("dual_language_program.cpp");
    
    // 输出 Python 兼容部分
    outFile << "/*\n";
    outFile << "'''\n";
    outFile << "import sys\n";
    outFile << "def cpp_main():\n";
    
    // 转换 C++ 代码为 Python 兼容格式
    for (const auto& line : cppProgram) {
        string pythonLine = line;
        
        // 基本转换规则
        size_t pos;
        
        // 转换 cout 为 print
        while ((pos = pythonLine.find("cout")) != string::npos) {
            pythonLine.replace(pos, 4, "print");
        }
        
        // 转换 << 为 ,
        while ((pos = pythonLine.find("<<")) != string::npos) {
            pythonLine.replace(pos, 2, ",");
        }
        
        // 转换 endl 为换行
        while ((pos = pythonLine.find("endl")) != string::npos) {
            pythonLine.replace(pos, 4, "'\\n'");
        }
        
        // 转换 main 函数为 Python 函数
        if (pythonLine.find("main(") != string::npos) {
            pythonLine = "def main():";
        }
        
        // 添加缩进
        if (!pythonLine.empty()) {
            outFile << "    " << pythonLine << "\n";
        } else {
            outFile << "\n";
        }
    }
    
    outFile << "\nif __name__ == \"__main__\":\n";
    outFile << "    cpp_main()\n";
    outFile << "'''\n";
    outFile << "*/\n\n";
    
    // 输出原始 C++ 程序
    outFile << "#include <iostream>\n";
    outFile << "using namespace std;\n\n";
    
    for (const auto& line : cppProgram) {
        outFile << line << "\n";
    }
    
    // 添加 Python 执行部分
    outFile << "\n";
    outFile << "/*\n";
    outFile << "'''\n";
    outFile << "*/\n";
    
    outFile.close();
    
    cout << "successful!\n";
}

int main() {
    vector<string> cppProgram = readInputProgram();
    generateDualLanguageProgram(cppProgram);
    return 0;
}
