# references目录下的参考信息来源
- <https://github.com/DavidNHill/Minesweeper>
- <https://github.com/DavidNHill/Minesweeper2>
- <https://dash.harvard.edu/bitstream/handle/1/14398552/BECERRA-SENIORTHESIS-2015.pdf>
___
# Minesweeper-2023
> ACM 班 2023 级程序设计第一次大作业

~~大作业~~ 扫雷，启动！

## 目录

1. [Minesweeper-2023](#minesweeper-2023)
   1. [目录](#目录)
   2. [简介](#简介)
      1. [背景](#背景)
      2. [作业目标](#作业目标)
   3. [作业要求](#作业要求)
      1. [术语解释](#术语解释)
      2. [作业任务](#作业任务)
         1. [基础任务](#基础任务)
         2. [进阶任务](#进阶任务)
      3. [数据范围](#数据范围)
   4. [须知](#须知)
      1. [截止时间](#截止时间)
      2. [如何完成程序](#如何完成程序)
         1. [基础任务](#基础任务-1)
         2. [进阶任务](#进阶任务-1)
      3. [提交方式](#提交方式)
      4. [评分规则](#评分规则)
         1. [A班](#a班)
         2. [B班](#b班)

## 简介
### 背景

Minesweeper（扫雷）是我们熟悉的经典轻量级小游戏，作为老版本 Windows 操作系统的内置应用，它在许多人的回忆中可能是学生时代在老电脑里的娱乐，信息课上的摸鱼或是竞赛中垃圾时间的消遣。

![微软扫雷](figures/microsoft-minesweeper.png)

自从 Windows 8 从内置游戏中移除了扫雷，关于扫雷的回忆可能已经随我们的那段童年一起被慢慢地尘封。那么在这里，在你初入 ACM 班的第一个大作业中，让我们来回顾一下扫雷吧～

### 作业目标

我们想通过本次作业达到以下目标：
1. 基础任务
   - 熟悉基本的输入输出与控制流语句
   - 熟悉函数的使用
   - 熟悉数组的使用
   - 熟悉函数的递归
   - 了解搜索算法（可选）
   - 边界情况的处理
   - 规范代码风格
2. 进阶任务
   - 提高模拟水平
   - 运用算法解决实际问题的能力
   - 使用 git 与 cmake 管理项目（可选）
   - 运用机器学习解决问题（可选）

## 作业要求

在本次作业中，你需要实现一个扫雷游戏（如果你不知道扫雷是什么，你可以参考 [扫雷_百度百科](https://baike.baidu.com/item/扫雷/12543) ）。

### 术语解释
由于本次作业涉及到一些为了方便表述而使用的术语，我们会先进行解释
- 地图，格子：在上面的游戏截图中，每一个小方块被我们称为一个格子，所有的格子构成地图
- 地雷，非地雷格子，非地雷格子的地雷计数：游戏中的格子可能是地雷或非地雷格子；若一个格子是非地雷格子，则我们将其周围八个格子中地雷的总数称为其地雷计数。
- 访问一个格子：在游戏最开始，所有格子都是**未被访问**的（即其内容对于用户是未知的），用户访问一个格子相当于传统扫雷游戏中左键单击该格子，其状态将变为已访问。
  - 需要注意的是，与实际的扫雷游戏相同，**当你访问某个地雷计数为 0 的格子时，周围的所有格子将自动被访问**（相应地，若自动被访问的格子中也有地雷计数为 0 的，该过程需继续进行下去；对于 B 班同学，你可能需要运用递归的思想解决这一问题，如果你对这一任务的实现没有头绪，请咨询助教）
- 游戏结束条件：若一个格子是地雷，则访问该格子将**立即**导致游戏失败，否则游戏继续，直到所有非地雷的格子均被访问（即游戏胜利）。

### 作业任务

#### 基础任务

在本次作业中，你的基础任务是模拟实现一个命令行交互的扫雷小游戏，即实现该游戏的服务端。

首先，你会通过标准输入得到给定地图的信息（行列数量与每个格子是否为地雷），形式化地，第一行输入两个整数 `n` `m` ，代表地图有 `n` 行 `m` 列，随后 `n` 行，每行输入 `m` 个字符（不计换行符），以换行符结尾，其中第 `i` 行第 `j` 列（0-based，下同）代表地图中第 `i` 行第 `j` 个格子的内容，`.` 代表未放置地雷，`X` 代表放置地雷。

例如下面的输入
```
3 3
.X.
...
..X
```
代表当前游戏的地图大小为 3 * 3，在地图的第 0 行第 1 列与第 2 行第 2 列放置了地雷。

在初始状态下，地图中的所有格子都是未被访问的。

初始化完成后，你需要输出地图（格式见下）。随后，你会通过标准输入得到若干行字符串来表示玩家的操作，每一行有两个正整数 `x y`，分别是两个在 `[0, n - 1]` 和 `[0, m - 1]` 范围的数字，代表玩家这一步希望访问第 `x` 行第 `y` 列的格子。

在读取玩家的输入后，你需要访问指定的格子（若格子本身已被访问，不做任何操作）；并将访问后的地图状态输出，格式与输入相仿，具体地

- 输出 `n` 行 `m` 列，最后输出换行符
  - 未被访问的格子输出为 `?`，例如上面的地图在初始状态下的输出应当为
      ```
      ???
      ???
      ???
      ```
  - 对于已被访问过的格子，若其为地雷，输出 `X`，若其不是地雷，输出一个正整数 `k`，表示其地雷计数。例如对于上面的地图，若从左到右依次访问最下面一行的格子，在右下角的地雷被访问后的输出应当为
      ```
      ???
      12?
      01X
      ```
      （若你不明白为什么 (1, 0) 和 (1, 1) 两个格子也被访问，请参考 [术语解释](#术语解释)）
   - 当所有非地雷格子已被访问后（即游戏胜利前最后一次输出地图时），你需要将在所有为地雷的格子处（均未访问）输出 `@`，非地雷格子同上。例如，对于上面的地图，游戏胜利前最后一次输出应当为
      ```
      1@1
      122
      01@
      ```

- 输出地图后，你需要判定游戏是否结束（失败或胜利），若游戏结束，你应当先输出一行文字，**胜利则为 "YOU WIN!"，失败则为 "GAME OVER!"**，再输出一行两个整数 `visit_count step_count`，以空格分隔，分别代表玩家访问过的**非地雷格子**的个数与总操作数，并立即以 0 为返回值退出程序。
   - 例如，在上面的访问了右下角地雷的例子中，当你输出完地图后，你应输出下面的信息并退出程序：
      ```
      GAME OVER!
      4 3
      ```

#### 进阶任务

在本次作业中，你的进阶任务是游玩你刚刚设计的扫雷小游戏，即实现该游戏的用户端。这一任务是为 A 班，即参加过算法竞赛的同学准备的，你可以将其类比为算法竞赛中的交互题。有所不同的是，我们这里不使用标准输入输出进行交互，而是通过你刚刚自己写的游戏程序交互。

我们下面将游戏程序称为服务端代码（即 `server.h`），游玩程序称为用户端代码（即 `client.h`）。

首先，我们将读入游戏地图作为服务端代码的输入，用户端代码仅可以获知游戏地图的行数与列数；接下来，用户端代码不断发起 `Execute(row, column)` 指令（第一次将由输入数据代你发出，防止第一次就踩雷），即访问某一个方块并获得该访问后的地图情况，具体地，`Execute(row, column)` 函数的实现形如
```
Execute(row, column):
  VisitBlock(row, column)
  ReadMap() from result of PrintMap()
```
因此，如你所见，你需要在 ReadMap 函数中读取有限的地图信息，并储存到一些全局变量中，随后根据这些地图信息做出下一步的决策，通过 `Execute` 再次发起决策。具体的代码结构逻辑可以参考 `advanced.cpp` 和 `client.cpp`，如果你不明白代码的实现方式，请立即询问助教！！！

例如，若输入数据为
```
3 3
.X.
...
..X
2 0
```
最下面一行 `2 0` 是输入数据代你做出的第一次决策，下面给出了一组交互的例子。

```
(Decision)              (result of PrintMap)
                        ???
                        12?
                        01?
1 2
                        ???
                        122
                        01?
0 1               
                        ?X?
                        122
                        01?
                        GAME OVER!
                        5 3
```

注意这里的最后两行是游戏结束（失败）的输出，`step_count` 为 3 是因为将第一次操作 `2 0` 计算入中操作数。

### 数据范围

对于基础任务，地图的行数`n`、列数`m`、地雷数`x`满足：1 < `n`, `m` < 31，0 < `x` < mn。保证输入合法。

## 须知

### 截止时间

第五周周一（10/9）18:00


### 如何完成程序
```
.
├── CMakeLists.txt
├── LICENSE
├── README.md
├── src
│   ├── CMakeLists.txt
│   ├── advanced.cpp
│   ├── include
│   │   ├── client.h
│   │   └── server.h
│   └── main.cpp
└── testcases
    ├── advanced
    └── basic
```
以上是仓库的文件结构。

你有两种方式下载该仓库完成作业：

1. 在 GitHub 页面点击 Code - Download ZIP，仓库代码将会被下载到本地，随后你可以直接在本地仓库上进行编辑。
2. 若你掌握 Git 的使用，你可以 clone 该仓库进行操作，或者以该仓库为 Template 创建自己的仓库，在自己的仓库中进行操作（我们推荐使用最后一种操作方式）。

#### 基础任务

对于基础任务，你的程序主文件是 `main.cpp`，其内容无需更改，你只需要阅读并理解该程序是如何调用 `server.h` 中的函数的。在此基础上，你需要实现 `server.h` 中标记 `TODO` 的函数，并通过运行 `main.cpp` 来测试你的程序。在 `testcases/basic` 文件夹中，有 5 个公开数据点供你完成调试；更详细地，以 `basic1` 测试点为例，请将 `basic1.in` 输入你的程序，你的输出应当形如 `basic1.out`。

若你使用的是 Visual Studio Code，可点击最下方工具栏的 Build 与运行按钮（目标选择 server）来编译运行你的程序。

若你使用的是 CLion，可直接用上方工具栏编译运行 server 目标。

注意，你必须在 IDE 中直接以项目文件夹（即包含该 README 的文件夹）为工作目录打开该项目，否则无法使用以上方式编译运行。

#### 进阶任务

对于进阶任务，你的程序主文件是 `advanced.cpp`，其内容无需更改，你只需要阅读并理解该程序是如何调用 `server.h` 与 `client.h` 中的函数的。在完成基础任务之后，你需要实现 `client.h` 中标记 `TODO` 的函数，并通过运行 `advanced.cpp` 来测试你的程序。在 `testcases/advanced` 文件夹中，有 5 个公开数据点供你完成测试；更详细地，以 `advanced1` 测试点为例，请将 `advanced1.in` 输入你的程序，并等待程序的输出。

在测试该任务时，请你将 `src/CMakeLists.txt` 中最后一行的第一个 `#` 删去并重新构建项目。

若你使用的是 Visual Studio Code，可点击最下方工具栏的 Build 与运行按钮（目标选择 client）来编译运行你的程序。

若你使用的是 CLion，可直接用上方工具栏编译运行 client 目标。

### 提交方式

请在 OnlineJudge 上直接提交你实现后的头文件。对于基础任务，提交 `server.h`，对于进阶任务，提交 `client.h`。

### 数据范围

保证 `n` `m` 均不大于 30，操作序列长度不大于 1000。

### 评分规则

A班、B班均有最多 10% 的 bonus 分数。

#### A班

| 任务 | 占比 |
| :---: | :---: |
| 基础任务 | 60% |
| 进阶任务 | 30% |
| Code Review | 20% |

#### B班

| 任务 | 占比 |
| :---: | :---: |
| 基础任务 | 80% |
| 进阶任务 | 10% |
| Code Review | 20% |

关于进阶任务的评分细则，我们将稍后发布。
