<div dir="rtl">

# قراردادهای برنامه نویسی C++


- به جای چند ‍`else if` اگر میتوانید حتما از `switch` استفاده کنید.

- از قالب ‍`<library.h>` برای `include` کردن کتابخانه های خود زبان و کتابخانه های آماده استفاده کنید
و از قالب `"library.h"` برای ‍`include` کردن کتابخانه هایی که در پروژه نوشته شده استفاده کنید.

- برای تعریف `Constructor` یک کلاس اگر میتوانید حتما از ‍*Initializer List* استفاده کنید.

مثال: 

```CPP
...
private:
    int x;
    int y;
public:
    Point(int i, int j):x(i), y(j) {}
    /*  The above use of Initializer list is optional as the
        constructor can also be written as:
        Point(int i = 0, int j = 0) {
            x = i;
            y = j;
        }
    */ 
...
```

> برای *initialization of non-static const data members* و *initialization of reference members*  حتی کامپایلر
> ما را مجبور به این کار میکند.

- یک مورد خیلی کاربردی ‍`Initializer List` هم تعیین پارامترهای کلاس پدر یا 
`Base Class` است. مثال:

```CPP
#include <iostream>
using namespace std;
 
class A {
    int i;
public:
    A(int );
};
 
A::A(int arg) {
    i = arg;
    cout << "A's Constructor called: Value of i: " << i << endl;
}
 
// Class B is derived from A
class B: A {
public:
    B(int );
};
 
B::B(int x):A(x) { //Initializer list must be used
    cout << "B's Constructor called";
}
 
int main() {
    B obj(10);
    return 0;
}
```

> استفاده از این روش **Performance** را هم بهبود می دهد!

- برای initial کردن متغیرها از `{}` استفاده کنید. مثال:

```CPP
int x{2};
```

> این روش باعث می شود کامپایلر برای کد زیر خطا دهد و این معمولا به ما کمک میکند:

```CPP
unsigned x{-1}; // compiler error
```



## قراردادهای نام گذاری

<div align="center">
    <img src="./medias/naming-conventions.png">
</div>

<div align="center">

| Identifier                |         Naming Convention     |       Description     |
| ------------------------- | ----------------------------- | --------------------- |
| file name                 |           camelCase           |       meaningful      |
| class name                |           PascalCase          |       meaningful      |
| namespace name            |           PascalCase          |       meaningful      |
| variable name             |           camelCase           |       meaningful      |
| parameter name            |           camelCase           |       meaningful      |
| function name             |           camelCase           |       meaningful      |
| constant name             |           CONSTANT_CASE       |       meaningful      |
| private member name       |           camelCase           |       m + [name]      |
| typedef name              |           PascalCase          |       meaningful      |
| define guard name         |           CONSTANT_CASE       |       meaningful      |
| enum member name          |           PascalCase          |       meaningful      |

</div>


# منابع
</div>

https://www.geeksforgeeks.org/when-do-we-use-initializer-list-in-c/

https://google.github.io/styleguide/cppguide.html