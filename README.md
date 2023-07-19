# Luna_project
github链接：https://github.com/shiirin-schariac/Luna_project

## 使用方法
```shell
$ cd /path_to_luna_project
$ make
$ ./theia.out # then you can write luna immediately!
$ ./theia.out examples/filename.luna # to test my example files
$ # you can use `make clean` to delete the `.out` file and re-compile
```

### 项目简介
**一句话版：theia是一个使用c++实现的自创编程语言luna的解析器。**
~~（和lua这个语言并无任何关系（真的））~~

> theia是一个使用c++实现的自创编程语言luna的解析器。luna是一个**非常非常简单**的编程语言，它的特点是（表面弱类型）强类型、偏用户以及非常简单，可以实现变量的自定义、变量的重赋值、表达式的解析、函数的自定义等功能。

### 项目背景
总的来说就是那天在github上刷到一个叫c4的神项目，只用了几百行就实现了一个简单的c++编译器，简直惊为天人，经过一段时间的尝试理解我发现照抄c4的思路完全不行，一是因为我的电脑不是通常的X86架构，二是我对编译原理的认识几乎为0……于是我决定写一个巨型模拟，不触及到任何CPU的指令，抽象地运行一个我自己设置语法的语言，毕竟计算机科学有一句名言……
（后来我知道了这个东西原来叫解析器）

### 项目思路
首先将语句分割成词法单元，传入parser（解析器）中，根据规则构建抽象语法树，再使用visitor访问，实现功能。

### 项目参考
[Youtube_make a programming language in c](https://www.youtube.com/watch?v=WABO4o_y8qc&t=422s)
[Stanford compilers](https://web.stanford.edu/class/cs143/)
《编译原理》(Alfred V. Aho, Monica S.Lam, Ravi Seti etc.)

### 关于Luna
基础语法演示如下：
1. 定义变量与变量的重赋值（消除左递归且实现了正确的数学运算，类型自动转换）
	```
	var a = 42;
	var b = a + 41.0;
	a = 10 + 1 - 25 / 5 * 2;
	print(a,b);
	```
2. 定义有参函数与无参函数
	```
	function say(x, y){
	    print(x);
	    print(y);
	};
	say(42, "Luna");
	function doit()
	{
	    print("tsuki ga kirei da.");
	};
	doit();
	```
3. 局域变量与全局变量的演示
	```
	var a = 35;
	function test(){
    var a = 25;
    a = 42;
    print(a);
	};
	test();
	print(a);
	```
4. 优质的debug建议

