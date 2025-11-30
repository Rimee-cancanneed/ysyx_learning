#include <stdio.h>
#include <assert.h>
#include <string.h>

#define MAX_NAME_LENGTH 100

struct Person {
    char name[MAX_NAME_LENGTH];
    int age;
    int height;
    int weight;
};

// 改为直接初始化结构体，而不是返回指针
struct Person Person_create(char *name, int age, int height, int weight)
{
    struct Person who;
    
    // 使用strncpy防止缓冲区溢出
    strncpy(who.name, name, MAX_NAME_LENGTH - 1);
    who.name[MAX_NAME_LENGTH - 1] = '\0'; // 确保字符串以null结尾
    
    who.age = age;
    who.height = height;
    who.weight = weight;

    return who;
}

// 不再需要destroy函数，因为栈上的变量会自动释放

// 改为接受结构体值（而不是指针）作为参数
void Person_print(struct Person who)
{
    printf("Name: %s\n", who.name);
    printf("\tAge: %d\n", who.age);
    printf("\tHeight: %d\n", who.height);
    printf("\tWeight: %d\n", who.weight);
}

int main(int argc, char *argv[])
{
    // 在栈上直接创建结构体变量
    struct Person joe = Person_create("Joe Alex", 32, 64, 140);
    struct Person frank = Person_create("Frank Blank", 20, 72, 180);

    // 打印它们和它们在内存中的位置
    printf("Joe is at memory location %p:\n", &joe);  // 使用&获取地址
    Person_print(joe);

    printf("Frank is at memory location %p:\n", &frank);
    Person_print(frank);

    // 让每个人都变老20岁并再次打印
    joe.age += 20;
    joe.height -= 2;
    joe.weight += 40;
    Person_print(joe);

    frank.age += 20;
    frank.weight += 20;
    Person_print(frank);

    // 不再需要手动清理，栈上的变量会在函数结束时自动释放

    return 0;
}