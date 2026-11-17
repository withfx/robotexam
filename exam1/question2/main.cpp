#include <iostream>
#include <string>
using namespace std;

class Book {
private:
    // 私有成员：只能在类里面的函数访问，main 里不能直接改
    string title;
    float price;
    int stock;

public:
    // 构造函数：创建对象的时候自动调用，先把成员变成初始值
    Book() {
        title = "";
        price = 0.0f;
        stock = 0;
    }

    // 析构函数：delete 对象的时候自动调用
    // 这个类没有自己 new 出来的资源，所以函数体里不用写释放代码
    ~Book() {
    }

    // 从键盘读入书籍信息
    void input() {
        // 书名里可能有空格，所以用 getline 读一整行
        getline(cin, title);
        cin >> price;
        cin >> stock;
    }

    // 输出书籍信息
    void display() {
        cout << "书名：" << title << endl;
        cout << "价格：" << price << endl;
        cout << "库存：" << stock << endl;
    }
};

int main() {
    // 用 new 在堆上动态创建一个 Book 对象，返回的是指针
    Book *book = new Book();

    // 指针用 -> 调用成员函数，等价于 (*book).input()
    book->input();
    book->display();

    // 动态创建的对象要手动 delete，否则会内存泄漏
    // delete 的时候会自动调用析构函数
    delete book;
    book = nullptr;

    return 0;
}
