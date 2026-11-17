#include <iostream>
#include <string>
#include <cctype>
using namespace std;

int main() {
    string text;

    // 用 getline 读一整行，空格也会被读进来
    getline(cin, text);

    int letter_count = 0;
    int digit_count = 0;
    int other_count = 0;

    // 一个一个字符检查
    for (size_t i = 0; i < text.size(); i++) {
        // isalpha 和 isdigit 的参数最好是 unsigned char
        // 直接传 char 在某些编译器上会有警告
        unsigned char ch = static_cast<unsigned char>(text[i]);

        if (isalpha(ch)) {
            // 英文字母
            letter_count++;
        } else if (isdigit(ch)) {
            // 数字 0~9
            digit_count++;
        } else {
            // 空格、标点、符号等
            other_count++;
        }
    }

    cout << "英文字母个数：" << letter_count << endl;
    cout << "数字个数：" << digit_count << endl;
    cout << "其他字符个数：" << other_count << endl;

    return 0;
}
