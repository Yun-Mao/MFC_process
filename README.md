# Windows下进程通信种类

一般共有13种方式：文件映射、共享内存、匿名管道、命名管道、邮件槽、剪贴板、动态数据交换、对象连接与嵌入、动态连接库、远程过程调用、NetBios函数、Sockets、WM\_COPYDATA消息。

本次报告基于MFC编程，对剪贴板、匿名管道、命名管道、邮件槽进行理解和实现。



# 剪贴板

## 定义

剪贴板是由操作系统维护的一块内存区域，这块内存区域不属于任何单独的进程，但是每一个进程又都可以访问这块内存区域，而实质上当在一个进程中复制数据时，就是将数据放到该内存区域中，而当在另一个进程中粘贴数据时，则是从该块内存区域中取出数据。

如果在物理内存中划分出一块内存，这一块内存不为任何的进程所私有，但是任何的进程又都可以访问这块内存，那么进程A就可以往这块内存中存放数据Data，然后进程B也是可以访问这块内存，从而进程B就可以访问到数据Data了，以上这种思路就是通过剪贴板实现了进程A和进程B之间的通信。

## 剪贴板的使用

```cpp
OpenClipboard() //打开剪贴板
EmptyClipboard(void) // 清空剪贴板
CloseClipboard(void) // 关闭剪贴板
SetClipboardData(UINT uFormat, HANDLE hMem)  //放数据到剪贴板
GetClipboardData(UINT uFormat) // 取数据到剪贴板
```

## 剪贴板中的内存从何而来

*GlobalAlloc 函数*

GlobalAlloc函数是从堆上分配指定数目的字节，

与其他的内存管理函数相比，全局内存函数的运行速度会稍微慢一些，但是全局函数支持动态数据交换，同时，其分配的内存也不为任何一个进程所私有，而是由操作系统来管理这块内存，所以用在给剪贴板分配内存空间是很适合的。

```cpp
HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, Str.size);
//GMEM_MOVEABLE 分配一块可移动内存。
memcpy_s(GlobalLock(hData), Str.size, str.LockBuffer(), Str.size);
GlobalUnlock(hData);
str.UnlockBuffer();
```

# 匿名管道

## 定义

管道(Pipe)是一种具有两个端点的通信通道：有一端句柄的进程可以和有另一端句柄的进程通信。管道可以是单向------一端是只读的，另一端点是只写的；也可以是双向的------管道的两端点既可读也可写。

匿名管道(Anonymous Pipe)是
在父进程和子进程之间，或同一父进程的两个子进程之间传输数据的无名字的单向管道。通常由父进程创建管道，然后由要通信的子进程继承通道的读端点句柄或写
端点句柄，然后实现通信。父进程还可以建立两个或更多个继承匿名管道读和写句柄的子进程。这些子进程可以使用管道直接通信，不需要通过父进程。

匿名管道是单机上实现子进程标准I/O重定向的有效方法，它不能在网上使用，也不能用于两个不相关的进程之间。

## 主要过程

通过创建匿名管道CreatePipe,获得管道的读写句柄;

父进程通过CreateProcess,把读写句柄作为子进程的标准输入输出句柄传递给子进程；

子进程通过GetStdHandle获取父进程指定的输入输出句柄；

父子进程使用WriteFile、ReadFile向句柄中写入或读出数据。

## 实现方法

创建匿名管道

```{.c++ language="C++"}
BOOL CreatePipe(
PHANDLE hReadPipe, 
PHANDLE hWritePipe, 
LPSECURITY_ATTRIBUTES lpPipeAttributes, 
DWORD nSize );    //使用缓冲区大小，0使用缺省值
typedef struct _SECURITY_ATTRIBUTES {
DWORD nLength;
LPVOID lpSecurityDescriptor;//安全描述NULL缺省值
BOOL bInheritHandle;          //子进程能否继承此管道
} SECURITY_ATTRIBUTES
```

创建进程

```{.c++ language="C++"}
BOOL CreateProcess( 
LPCTSTR lpApplicationName, 
LPTSTR lpCommandLine,
LPSECURITY_ATTRIBUTES lpProcessAttributes, 
LPSECURITY_ATTRIBUTES lpThreadAttributes,
BOOL bInheritHandles, 
DWORD dwCreationFlags, 
LPVOID lpEnvironment, 
LPCTSTR lpCurrentDirectory, 
LPSTARTUPINFO lpStartupInfo, 
LPPROCESS_INFORMATION lpProcessInformation ); 
```

获取标准输入/输出/错误句柄

```{.c++ language="C++"}
HANDLE GetStdHandle( DWORD nStdHandle ); 
/*
nStdHandle:
STD_INPUT_HANDLE 
STD_OUTPUT_HANDLE 
STD_ERROR_HANDLE 
*/
//示例：
CreatePipe(&hRead,&hWrite,&sa,0)
hRead=GetStdHandle(STD_INPUT_HANDLE);
hWrite=GetStdHandle(STD_OUTPUT_HANDLE);
```

# 命名管道

## 定义

命名管道(Named
Pipe)是服务器进程和一个或多个客户进程之间通信的单向或双向管道。不同于匿名管道的是命名管道可以在不相关的进程之间和不同计算机之间使用，服务器建立命名管道时给它指定一个名字，任何进程都可以通过该名字打开管道的另一端，根据给定的权限和服务器进程通信。

命名管道提供了相对简单的编程接口，使通过网络传输数据并不比同一计算机上两进程之间通信更困难，不过如果要同时和多个进程通信它就力不从心了。

只有服务器端能创建命名管道并接收连接请求，客户端只能同一个现成的管道通信。\
命名管道提供了两种通信方式：

字节模式：字节流方式

消息模式：一个数据块，有消息定界符，必须整体发送和接受

## 使用命名管道的主要过程

*服务器端：*

使用CreateNamedPipe构造命名管道；

使用ConnectNamedPipe等待客户端连接请求，同时指定一个事件对象；

使用WaitForSingleObject等待；

使用ReadFile WriteFile通信。

*客户端：*

使用WaitNamedPipe 等待指定管道的出现；

使用CreateFile打开命名管道；

使用ReadFile WriteFile通信。

## 实现方法

创建管道

```{.c++ language="C++"}
HANDLE CreateNamedPipe(
LPCTSTR lpName,
DWORD dwOpenMode, 
DWORD dwPipeMode, 
DWORD nMaxInstances, 
DWORD nOutBufferSize, 
DWORD nInBufferSize, 
DWORD nDefaultTimeOut, 
LPSECURITY_ATTRIBUTES lpSecurityAttributes ); 
```

服务器等待连接请求

```{.c++ language="C++"}
BOOL ConnectNamedPipe( 
HANDLE hNamedPipe, 
LPOVERLAPPED lpOverlapped ); 
//OVERLAPPED是一个包含了用于异步输入输出的信息的结构体
```

客户端函数

```{.c++ language="C++"}
BOOL WaitNamedPipe(
LPCTSTR lpNamedPipeName, 
DWORD nTimeOut ); 
```

# 邮件槽

## 定义

邮件槽(Mailslots)提供进程间单向通信能力，任何进程都能建立邮件槽成为邮件槽服务器。其它进程，称为邮件槽客户，可以通过邮件槽的名字给邮件槽服务器进程发送消息。进来的消息一直放在邮件槽中，直到服务器进程读取它为止。一个进程既可以是邮件槽服务器也可以是邮件槽客户，因此可建立多个邮件槽实现进程间的双向通信。

通过邮件槽可以给本地计算机上的邮件槽、其它计算机上的邮件槽或指定网络区域中所有计算机上有同样名字的邮件槽发送消息。广播通信的消息长度不能超过400字节，非广播消息的长度则受邮件槽服务器指定的最大消息长度的限制。

邮件槽与命名管道相似，不过它传输数据是通过不可靠的数据报(如TCP/IP协议中的UDP包)完成的，一旦网络发生错误则无法保证消息正确地接收，而命名管道传输数据则是建立在可靠连接基础上的。不过邮件槽有简化的编程接口和给指定网络区域内的所有计算机广播消息的能力，所以邮件槽不失为应用程序发送和接收消息的另一种选择。

不可靠原因：发送者无法确认接收者是否已经收到了信息。

## 邮件槽通信过程

服务器使用CreateMailslot创建邮槽并等待接受数据；

客户端使用CreateFile打开邮槽并用WriteFIle写入数据；

服务器使用ReadFile读出数据。

## 实现方法

```{.c++ language="C++"}
HANDLE CreateMailslot( 
LPCTSTR lpName, 
//\\\\.\\mailslot\\[path]name 
DWORD nMaxMessageSize, 
//0表示任意大小
DWORD lReadTimeout, 
//0~MAILSLOT_WAIT_FOREVER 
LPSECURITY_ATTRIBUTES lpSecurityAttributes ); 
```

# MFC的实现

[Github](https://github.com/chinatianyudai/MFC_process)