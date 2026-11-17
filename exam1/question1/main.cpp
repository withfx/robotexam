#include <iostream>
using namespace std;

int main() {
    // 题目已经给出了 10 名同学的成绩，直接放进数组里
    int scores[10] = {78, 92, 65, 88, 70, 95, 82, 60, 100, 85};

    // 冒泡排序：每一轮把当前没有排好的数里最小的一个换到最后面
    // 外层循环控制轮数，10 个数比较 9 轮就够了
    for (int i = 0; i < 9; i++) {
        // 内层循环负责比较相邻的两个数
        // 每排好一轮，最后面就多一个有序的数，所以是 9 - i
        for (int j = 0; j < 9 - i; j++) {
            // 前一个数比后一个数小，说明顺序不对，要交换
            if (scores[j] < scores[j + 1]) {
                int temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }

    cout << "从高到低排序后的成绩：";
    for (int i = 0; i < 10; i++) {
        // 第一个数前面不加空格，后面的数前面加一个空格
        if (i > 0) {
            cout << " ";
        }
        cout << scores[i];
    }
    cout << endl;

    return 0;
}
