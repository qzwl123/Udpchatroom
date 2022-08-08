# P2P聊天室的实现

## 效果展示

![image-20220808123919351](https://typora-images-qz.oss-cn-beijing.aliyuncs.com/img/image-20220808123919351.png)

测试端

![](https://typora-images-qz.oss-cn-beijing.aliyuncs.com/img/image-20220808123919351.png)

## 总体设计

### 数据结构

1. 用户
2. 报文定义

```c
#define CHAT_SYN 0x001
#define CHAT_ACK 0x002
#define CHAT_FIN 0x004
#define CHAT_HEART 0x008

struct Msg {
    int type;
    char from[20];
    char to[20];
    char buff[1024];
};
```

### 架构说明

1. `server`和`client`在同一个程序中
2. 程序跑起来之后，没有明确的客户和服务端区别

### 用户存储

1. 选用合适的数据结构来存储用户信息
2. 可链表，可数组，也可以是其他的复杂结构

```c
//此结构不参与任何节点之间的数据交互，可自行维护
#define FL_SYNSENT 0x001
#define FL_ONLINE 0x002

struct User {
    char name[20];
    char ip[20];
    int flag; //用户当前的状态
    int fd;
    int cnt；//发出心跳的次数，当这个值到达10的时候，删除好友
};
```

### 心跳线程

1. 该线程用来遍历每一个用户，对其发送心跳包
2. 并判断是否需要将用户移出

### 接收线程

1. 循环接受用户信息，将用户的请求压到任务队列中

### 发送线程

读取标准输入，并将数据发送给工作线程，负责将数据转发出去

## 第一阶段

### 节点发现

1. 每个节点启动后，建立好主反应堆后，启动一个新的线程，给所有iplist中的所有`ip`发送SYN报文

   > 注意，在这里先把自己的ip加到iplist文件中，换一个端口，开一个相应的进程测试

2. 发送完SYN报文后，将该节点信息添加到用户存储结构中

3. 等待对方的`SYNACK`报文，如果收到`SYNACK`报文，则认为节点在线「实际是在主反应堆中做」

4. 使用UDP的`connect`与对方建立一条确定的伪连接

5. 并标记为在线

### 节点被发现

1. 在主反应堆中，处于监听状态的套接字，收到数据连接之后，收到包，需要判断是否是合法的包
2. 如果合法，并且该包是一个`SYN`包,则给对方发送一个`SYNACK`报文
3. 通过`connect`连接到对方，建立一条确认的伪连接
4. 并标记为对方在线
5. 如果收到的是`SYNACK`，处理如上节节点发现

### 循环打印发现的结点

1. 设置一个专门的线程，或者可以直接使用上面节点发现里第1条所使用的线程来接续完成下面的工作
2. 循环打印自己发现的结点

## 第二阶段

### 心跳的发送

> 心跳的目的是用来判断对端是否在线，也就是帮助你确定，好友列表里的这些个人，是否真的在线。

1. 启动一个单独的线程`heart_beat`
2. 这个线程，每隔一段时间给自己的所有好友发一个心跳报文`CHAT_HEART`
3. 当心跳线程发现某个好友，已经发了10个心跳包了，但是一直没有任何数据响应，那么任务他失联了，标记为下线

### 心跳的确认

> 在这里，需要明确一点，对心跳的ACK算是对心跳的确认，但，同时，给对方的任何数据，其实都是在告知对方：“我还活着”。

1. 如果收到一个心跳包，那么给对方回一个心跳确认包`CHAT_ACK`

 