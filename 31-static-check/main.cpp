#include <QDebug>

int div_by_zero(int z) {
    int x = 0;
    if (z == 0) {
        x = 1 / z;
    }
    return x;
}

int null_ptr(int *p) {
    if (!p)
        return p[0];
    return 0;
}

char const *p;
int escape_stack() {
    char const str[] = "hello!";
    p = str;
    return 0;
}

int uninitialized_var() {
    int x;
    int y = x + 1;
    return y;
}

int use_after_delete() {
    int *x = new int[10];
    delete[] x;
    return x[0];
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;
    qDebug() << "div" << div_by_zero(argc);
    qDebug() << "ptr" << null_ptr((int *)argv);
    qDebug() << "esc" << escape_stack();
    qDebug() << "ini" << uninitialized_var();
    qDebug() << "del" << use_after_delete();
    return 0;
}
