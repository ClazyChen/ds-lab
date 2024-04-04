#import "template.typ": *

= 栈 <sta:chapter>

线性表是一个非常“开放的”数据结构，用户可以读、写、插入和删除线性表上的任何一个数据单元。但在实际的计算模型中，并不是所有的数据结构都允许这样做。通过对允许访问的数据进行限制，我们可以过滤掉多余的信息，简化模型的复杂程度，从而更快更好地解决问题。

*栈*（stack）、*队列*（queue）以及*双端队列*（deque）就是典型的“访问受限制”的数据结构，它们的实现都是以线性表作为基础的；如果说线性表是“容器”，那么栈和队列这些结构更接近于“接口”，在C++ STL中称它们为容器*适配器*（adapter）。另一种经典的限制访问的数据结构是*优先队列*（priority queue），它需要以树作为基础，因此放到较后的章节讨论。栈和队列限制了用户访问元素的范围，这使得它们能够防止用户做出对进程、系统、计算机或网络有害的“非法”行为，在《操作系统》和《网络原理》中都能看到它们的应用。本章将首先讨论栈及其应用。

== 栈的性质 <sta:property>

=== 栈的定义 <sta:definition>

*栈*（stack）是一种特殊的线性表，又称*下推表*。对于栈`S[0:n]`，它的插入、访问（因为只能读一个元素，不存在“查找”）、删除操作均只能对*栈顶*元素（top或peek，即栈的最后一个元素）`S[n-1]`进行。如@fig:sta1 所示，当栈顶为`x`时，三种操作都只能在`x`处进行。

#figure(
    image("images/sta1.svg", width: 80%),
    caption: "栈的推入、访问和弹出",
) <fig:sta1>

+ 栈的插入就是在`S[n-1]`后面插入新的元素（称为*入栈*或*推入*，push）。
+ 栈的访问就是取`S[n-1]`。
+ 栈的删除就是将`S[n-1]`从栈中删除（称为*出栈*或*弹出*，pop）。

```cpp
template <typename T>
class AbstractStack {
public:
    virtual void push(const T& e) = 0;
    virtual T pop() = 0;
    virtual T& top() = 0;
};
```

#linebreak()#v(-1.5em)

=== 栈的实现 <sta:implementation>

由于栈实质上是对线性表的访问权限作出限定，所以很容易在线性表的基础上建立栈。以向量（顺序表）实现的栈称为顺序栈，以列表（链表）实现的栈称为链栈。测试程序位于#bluetxt[`stack.cpp`]。

```cpp
template <typename T, template<typename> typename L = DefaultVector>
class Stack : public AbstractStack<T> {
protected:
    L<T> V;
public:
    void push(const T& e) override {
        V.push_back(e);
    }
    T pop() override {
        return V.pop_back();
    }
    T& top() override {
        return V.back();
    }
    std::size_t size() const override {
        return V.size();
    }
};
```

#h(2em)这里需要注意的是，如果以向量（常规方式）实现栈，则我们总是使用末尾元素代表栈顶，就像在上一节所定义的那样。向量在头部做插入和删除操作的效率非常低，所以向量只能以尾部作为栈顶。但如果以单向列表实现栈，末尾元素变得不可访问，所以应当反过来以起始元素代表栈顶。双向列表因为是对称的，所以在末尾或起始都可以作为栈顶。

显然无论是顺序栈还是链栈，取顶的时间复杂度是$O(1)$，入栈和出栈的时间复杂度在分摊意义下也是$O(1)$。根据@lis:chapter 的分析可知，用列表实现的栈，时间效率和空间效率都不如向量，但稳定性稍好。
在绝大多数应用场景下，使用向量实现栈都比使用列表实现栈更优，很少会出现使用链栈的场景。

=== 后入先出 <sta:lifo>

因为栈只能从尾部进行操作的特性，栈可以被写作$[S_0, S_1, S_2, ..., S_(n-1) angle.r$的形式，左侧的中括号表示不可操作的一端（*盲端*），右侧的尖括号表示可操作的一端（*自由端*）。更常见的一种表示方法是把它竖过来：

#figure(
    image("images/sta2.svg", width: 80%),
    caption: "栈的桶式表示",
) <fig:sta2>

如@fig:sta2 所示，可以把栈想成一个桶，盲端是桶的底部，自由端（栈顶）是桶中最上面的物品。于是，`push`操作就是往桶里放东西，后放的东西总是放在先放的东西的上面，像上图中，B放在A上面，C放在B上面。而要取东西（`pop`）的时候，每次只能取桶里最上面的东西，也就是栈顶元素。在@fig:sta2 里，为了把B取出来，必须先把B上面的C取出来。

就像列表相关问题经常用链式图帮助思考一样，处理和分析栈的相关问题经常要用到上面这种桶式图。从@fig:sta2 中不难发现，栈的最重要的性质是：_先入栈的元素后出栈，后入栈的元素先出栈_，简称为“*后入先出*”（*LIFO*, Last In First Out）。生活中有不少LIFO的例子，比如，桌子上一大摞书需要先搬开上面的才能拿下面的。这些现象被抽象化到计算机中处理就对应栈这种数据结构。比如，在@lis:remove-static-list 中实现对象池的辅助向量就是一个栈。严格来说，栈也应该采用LIFO进行定义，只要能够实现LIFO功能的数据结构都可以认为是栈，而不需要限制在@sta:definition 中这种带盲端和自由端的线性表。不过由于这种单端自由的线性表实现非常简单，通常默认栈是用这种方法实现的。在《队列》和《优先队列》章中，读者将看到其他适配器结构会有多种不同的实现方法。// TODO: 优先队列的实现方法

== 出栈序列 <sta:pop-sequence>

=== 出栈序列的定义 <sta:pop-sequence-definition>

*出栈序列*（又称*栈混洗序列*）是关于栈的一个重点问题。下面首先给出出栈序列的定义。

给定一个序列$A=(a_1,a_2,...,a_n)$，如果对于序列$B=(b_1,b_2,...,b_n)$，存在对空栈`S`的入、出栈操作各`n`次的操作序列$O=(o_1,o_2,...,o_(2n))$，使得当序列`O`中的入栈操作为依次将$a_1,a_2,...,a_n$入栈时，序列`O`中的出栈操作出栈的元素恰好依次为$b_1,b_2,...,b_n$，则称序列`B`为序列`A`的一个*出栈序列*。

操作序列中的每个元素是`push`或者`pop`，为简化叙述，本书将使用$or$和$and$分别表示它们。如果用桶式图想象栈的形象，那么这两个符号是直观的。

一个显而易见的结论是：_出栈序列是全排列的子集_。以入栈序列为`(1,2,3)`为例，则`(3,2,1)`是它的一个出栈序列，对应的操作序列为$ or or or and and and$。容易验证，`(2,3,1)`、`(2,1,3)`、`(1,3,2)`以及`(1,2,3)`自身，都是它的一个出栈序列；而`(3,1,2)`则无法成为一个出栈序列。事实上，如果入栈序列是$(1,2,...,n)$，则$B=(b_1,b_2,...,b_n)$是出栈序列，当且仅当不存在“312”模式。即：不存在$i<j<k$，使得$b_j<b_k<b_i$（证明留作习题）。

根据出栈序列的定义，可以立刻得到，在给定`A`的情况下，从`O`到`B`是一个满射。那就自然地会引发猜想，从`O`到`B`也应该是一个单射。这个问题可以用递归法分析。设$O=(o_1,o_2,...,o_(2n))$中的最后一个$or$是$o_j$，则下一个操作$o_(j+1)$一定是$and$，且$o_j$入栈的和$o_(j+1)$出栈的元素都是$a_n$。那么，将$o_j$、$o_(j+1)$和$a_n$从序列里删去，就递归到了`n-1`的情形。递归边界`n=1`结论显然。这样，每个出栈序列就唯一对应了一个操作序列。

=== 出栈序列的计数 <sta:pop-sequence-count>

从@sta:pop-sequence-definition 的分析可知，给定入栈序列的情况下，出栈序列的数量等于操作序列的数量，而操作序列$O=(o_1,o_2,...,o_(2n))$的数量和入栈序列的内容无关，只和入栈序列的长度`n`相关。

在这一小节中，设长度为`2n`的操作序列`O`的数量是$f(n)$。我们希望求出$f(n)$。为了求解这个函数，需要确定操作序列`O`需要满足的条件。

+ 包括`n`个$or$和`n`个$and$。
+ 对于操作序列的任意一个_前缀_，前缀中$or$的数量一定_不小于_$and$的数量。否则，在这个前缀的操作结束之后，栈的规模会变成负数，这是不可能的。

#h(2em)容易验证满足上述两个条件的序列，也一定是合法的、可以生成对应出栈序列的操作序列。下面利用这两个条件，去推导$f(n)$满足的递归式。

由于条件（2），$o_1$必定是$or$。设$o_1$入栈的$a_1$在$o_k$时出栈，其中$1<k<= 2n$。又由于$a_1$在$o_1$的时候被压入了栈的底部，所以$o_k$之后，栈变成了空栈。因此，$(o_1,o_2,...,o_k)$和$(o_(k+1),o_(k+2),...,o_(2n))$各自都是一个比较短的、符合条件的操作序列。因而$k$必须是偶数，设$k = 2i$，其中$1<= i <= n$。

在这两个操作序列中，$o_1$和$o_k$已经被确定了，而$(o_2,o_3,...,o_(k-1))$有$f(i-1)$种可能性，$(o_(k+1),o_(k+2),...,o_(2n))$有$f(n-i)$种可能性。于是：
$
f(n)=sum_(i=1)^n f(i-1)f(n-i)
$

上述递归方程可以被改写为：
$
f(n+1)=sum_(k=0)^n f(k)f(n-k)
$

#h(2em)下面介绍一种基于生成函数的解法@cormen2022introduction ，这一方法可以用于求解许多类似的递归方程问题。

设$H(x)=sum_(n=0)^infinity h_n x^n$，其中$h_n$为待求解的函数$f(n)$。于是，
$
H^2(x)&=sum_(n=0)^infinity h_n x^n sum_(k=0)^infinity h_k x^k=sum_(n=0)^infinity sum_(k=0)^infinity h_n h_k x^(n+k) \
&= sum_(n=0)^infinity x^n sum_(k=0)^n h_k h_(n-k) = sum_(n=0)^infinity h_(n+1) x^n \
&= (H(x)-h_0)/x
$

#h(2em)由$H(0)=h_0=1$，解得$H(x)=(1-sqrt(1-4x))/(2x)$。将分子上的二次根式泰勒（Taylor）展开，即可得到：
$
H(x)=sum_(n=0)^infinity C_(2n)^n /(n+1) x^n
$
于是，使用生成函数法可以得到，这个递归方程可以解出显式的通项公式：
$
f(n)=C_(2n)^n /(n+1) = (2n)!/(n!(n+1)!)
$
这个数被称为*卡特兰*（Catalan）*数*，记为$"Catalan"(n)$。

除了生成函数法之外，还有一种基于一一映射的计数方法@knuth1997art ，这种方法相对生成函数法更加巧妙。在这种方法中，我们分析出栈序列需要满足的两个条件。

+ 为了满足条件（1），需要在长度为`2n`的序列中安排`n`个$or$和`n`个$and$，可能的情况有$C_(2n)^n$种。
+ 如果满足条件（1）而不满足条件（2），则必定存在一个最小的`k`，使得序列的前`k`项中，$or$比$and$少1个；后`n-k`项中，$or$比$and$多1个。那么，保持后`n-k`项不变，令前`k`项的$or$变为$and$、$and$变为$or$，则得到了一个新的序列，这个序列中有`n+1`个$or$和`n-1`个$and$。可以证明这是一个一一映射。因此，不满足条件的情况有$C_(2n)^(n+1)$种。

#h(2em)
因此，出栈序列的数量为
$
C_(2n)^n - C_(2n)^(n+1) = C_(2n)^n /(n+1)
$

#h(2em)递归方程（即使用生成函数求解递归方程）法和一一映射法，是计数问题的两种基本方法。相对来说，递归方程法比较常规，很容易列出递归方程，但计算比较复杂；一一映射法则需要较高的构造技巧，而计算比较简单。

=== 随机出栈序列 <sta:random-pop-sequence>

这一小节从扩展卡特兰数的角度出发，讨论如何生成一个随机的栈操作序列@knuth1997art 。

考虑包含`p`个$or$和`q`个$and$的序列，其中$0<= p<= q $。如果在这个序列前添加`q-p`个$or$可以得到合法的栈操作序列，则下文称其为`p,q`的_后缀操作序列_（显然，一个合法操作序列的任一后缀都是后缀操作序列）。设`p,q`的后缀操作序列有$C_(p,q)$个，则可以得到$C_(n,n)="Catalan"(n)$。

直接对栈的操作序列做递归比较困难。而定义了后缀栈操作序列之后，就可以研究生成过程中的中间结果。我们的总体目标是生成`n+n`的后缀操作序列。如果从前到后生成，在某一步处已经生成了`n-p`个$or$和`n-q`个$and$，那么剩余部分是一个`p,q`的后缀操作序列，有$C_(p,q)$种等概率的可能性。从而，这一步生成$or$的概率是$(C_(p-1,q))/(C_(p,q))$，生成$and$的概率是$(C_(p,q-1))/(C_(p,q))$。

上述结论表明，只要计算出$C_(p,q)$的通项公式，就可以从前向后随机地逐个生成操作序列上的操作。使用@sta:pop-sequence-count 介绍的方法构造一一映射，容易证明：
$
C_(p,q)=C_(p+q)^q - C_(p+q)^(q+1) = (q-p+1)/(q+1) C_(p+q)^q
$

因此，生成$or$的概率为：
$
C_(p-1,q)/C_(p,q) = (q-p+2)/(q-p+1) dot p/(p+q)
$

#h(2em)这意味着不需要实际计算组合数，就可以得到每次生成$or$和$and$的概率。从而可以得到时间复杂度为$Theta(n)$的算法（假设随机数生成器的时间复杂度为$O(1)$）生成一个长度为`2n`的合法操作序列。

#pagebreak()

== 括号序列 <sta:parentheses>

=== 合法括号序列的计数 <sta:parentheses-count>

卡特兰数不仅仅是出栈序列（合法操作序列）的数量，同时也是其他很多问题的答案。这些问题的解决，也普遍具有两条路径：

+ 从模型角度入手，将其变换为等价的出栈序列或合法操作序列问题，然后套用卡特兰数的通项公式。（一一映射法）
+ 从计算角度入手，列出递归方程，发现和卡特兰数的递归方程的相似性（不需要使用生成函数实际求解）后，利用已知的卡特兰数通项公式求解。（递归方程法）

#h(2em)一个典型的例子是合法括号序列问题。考虑左括号和右括号组成的合法括号序列，可以用以下的递推定义：
+ 空串是合法括号序列。
+ 如果`S`和`T`是合法括号序列，那么`(S)T`也是合法括号序列。

#h(2em)条件（2）的一个等价形式拆分成两个条件：如果`S`是合法括号序列，则`(S)`是合法括号序列；以及，如果`S`和`T`是合法括号序列，那么`S T`也是合法括号序列。这种两个条件的版本更符合直观认知。

可以注意到，从`(S)T`递归到长度更短的`S`和`T`，这个递归方法和前面推导合法操作序列数量时使用的递归方法如出一辙，因此，用$or$代替左括号，用$and$代替右括号，就可以建立合法括号序列到合法操作序列的一个映射。容易证明这是一个双射，从而化归到出栈序列的问题上来。另一个方向也是类似的，容易通过条件（2）得到和之前完全一样的递归方程，从而解出卡特兰数的通项公式。

在这个问题上，从模型角度入手建立一一对应是显然的。但有一些问题，模型上可能一时看不出来，但通过计算角度发现结果是卡特兰数之后，就可以自然地联想到从模型上可以建立一一对应。后面还会遇到的一个非常重要的卡特兰数应用是树和二叉树的计数问题，我们将在对应章节进行分析讨论。// TODO: 树和二叉树的计数问题
// 比如，将凸$n$边形通过若干条互不相交的对角线划的分为$n-2$个三角形，划分方法数量。
// 这个问题的答案也是卡特兰数，由于和《数据结构》关系不大，这里不再展开。\textit{您可以自己分析这个问题。}

=== 判断括号序列是否合法 <sta:parentheses-check>

回到合法括号序列的问题上来。发现合法括号序列的数量和出栈序列相同之后，在讨论合法括号序列的问题时，就可以自然地联想到从栈的角度入手。_数量上相同只是初步的结论，更重要的是建立了一一映射。_比如，借助这个一一映射，就可以用栈来判断一个括号序列是否合法。

#bluetxt[实验`paren.cpp`。]在这个实验中，我们将同时讨论小括号、中括号和大括号，因此我们首先建立括号直接的对应关系。在存在多种括号的场合，只需要修改合法括号序列中的条件（2），将递推定义的合法括号序列包括`[S]T`和`{S}T`。

```cpp
class ParenMatch : public Algorithm<bool(const std::string&)> {
protected:
    char left(char c) {
        switch (c) {
            case '(': case ')': return '(';
            case '[': case ']': return '[';
            case '{': case '}': return '{';
            default: return 0;
        }
    }
};
```

#h(2em)从上面的分析中我们可以看到，左括号和右括号在建立一一映射的时候分别被映射为了$or$和$and$。因此，我们从左到右扫描括号序列的时候，每次扫描到左括号，就进行一次入栈操作；每次扫描到右括号，就进行一次出栈操作。由于我们考虑了三种括号，所以我们需要保证左括号和右括号是同一种。因此，每次扫描到左括号，就将它入栈（因此栈里只有左括号）；每次扫描到右括号，就从栈里弹出一个左括号，判断是否和右括号匹配。示例程序如下所示。

```cpp
bool operator()(const std::string& expr) {
    Stack<char> S;
    for (auto c : expr) {
        if (auto l { left(c) }; l == c) {
            S.push(c);
        } else if (l) {
            if (S.empty() || S.top() != l) return false;
            S.pop();
        }
    }
    return S.empty();
}
```
#h(2em)如果只有一种括号，那么栈中的所有元素都是相等的。在这种情况下，栈也变得不必要，因为我们事实上只需要知道栈的规模，用一个整数来表示即可；从而将空间复杂度降低到了$O(1)$。

== 栈与表达式 <sta:expression>

=== 表达式的定义 <sta:expression-definition>
括号主要的用处，就是给表达式规定计算顺序。所以，自然地就能想到，栈也可以被用来计算表达式。

回顾@sta:parentheses ，括号序列和栈的操作序列相对应，而一个出栈序列，是由入栈序列和操作序列共同决定的。从信息的角度看，如果要在出栈序列和表达式之间建立联系，且已知操作序列和括号序列之间有联系，那么顺理成章地，就会想到，入栈序列应当和表达式中的其他部分——也就是*操作数*（operand）和*运算符*（operator）建立联系。

那么接下来，就需要推导这个联系了。为了更清晰地展示推导过程，以下采用表达式$1+2*(3-4)$作为例子。

+ 建立完整的括号序列。

  这样做的目的是，让_括号序列_能够完全地和_运算次序_形成一一对应。原有的表达式中，有一些括号被省略了，这一步的目的是将它补全。在例子中，$1 + 2 * ( 3 - 4 )$被变换为$((1)+((2)*((3)-(4))))$。

  这里给每一个操作数也加上了一堆括号；这是因为，我们的目标是将“入栈序列”和“操作数与运算符”之间建立联系。在表达式中，操作数和运算符一共有7个（$1,2,3,4,+,*,-$），因此入栈序列的长度为7，相应地，操作序列的长度应该为14，也就是说括号序列的长度应该为14（7对括号）。

  在这7对括号中，有3对是用来描述_运算符的计算次序_的，而另外4对则用来描述_操作数的位置_。这样才能达成一一对应。

+ 构造入栈序列和出栈序列。

  利用第1步中建立的“括号对”和“操作数与运算符”的一一对应关系，决定每一对括号对应的入栈或出栈的元素。如果某一对括号对应的运算符或者操作数为$c$，那么将左括号变换为$or(c)$，右括号变换为$and$（出栈元素恰好为$c$），就得到了一个带参数的操作序列。

  在例子中，第1步加入了7个括号，其中3对用来描述3个运算符的运算次序，4对用来描述4个操作数的位置，也就是说，每一对括号都对应了一个运算符或者操作数。它对应的带参数操作序列是：$or(+) or(1) and or(*) or(2) and or(-) or(3) and or(4) and and and and $。对应的入栈序列是：`+ 1 * 2 - 3 4`；对应的出栈序列是：`1 2 3 4 - * +`。

#h(2em)定义这个入栈序列为*前缀表达式*（又称*波兰式*），出栈序列为*后缀表达式*（又称*逆波兰式*）。在前缀表达式和后缀表达式中，由于两个数字可能连在一起，所以为了区分边界，通常使用空格或其他分隔符隔开相邻的两个元素。熟知的数学的表达式形式，即$1+2*(3-4)$，被称为*中缀表达式*。这三个概念来自于运算符相对于操作数的位置。在前缀表达式中，运算符出现在它所对应的操作数之前；后缀表达式放在之后，而中缀表达式的运算符出现在中间。后缀表达式和前缀表达式的性质大多是对偶的，而后缀表达式更适合使用计算机计算。

=== 表达式中的元素 <sta:expression-element>

为简单考虑，本书中只讨论操作数为整数的表达式，讨论的运算符则包括7种：加、减、乘、除、取余、乘方、阶乘。通过对负号和阶乘的支持，我们要求表达式处理中允许单目运算符的存在，且单目运算符可以放在操作数的左侧（负号）或者右侧（阶乘）。

需要指出的是，表达式的实现在各个教材中会有很大的、基础性的差异。比如，针对表达式中的元素的抽象，有三种解决方案。

+ 分离方案@邓俊辉2013数据结构 。操作数和运算符被分离，使用不同的类型的数据单元进行存储。这种方案的优点在于不需要自己写类对数据单元进行封装（可以使用基本数据类型，如`char`和`int`），缺点是在处理表达式时往往需要两个栈分别处理操作数和运算符。而在表达式中，操作数和运算符本质上都是表达式的元素，分离方案没有显示出这一点。
+ 合取方案。在每个数据单元中，存储一个运算符_和_一个操作数。对于输入的中缀表达式，将每个运算符和紧随其后的一个操作数合并在一个数据单元存储。通过在整个表达式的开头和结尾增加一对括号，我们可以保证在中缀表达式中每个操作数都紧跟在某个运算符之后。这种方案的优点在于处理中缀表达式时非常方便，缺点是将操作数和它前面的运算符合并，这一操作并不自然。
+ 析取方案。在每个数据单元中，存储一个运算符_或_一个操作数。这种方法可以很好地表达操作数和运算符都是表达式元素的本质。缺点是为了区分运算符和操作数，需要额外的标记，从而增加了存储空间的开销。

#h(2em)本书将采用析取方案，我们将从表达式元素的基本功能开始，逐步往其中添加功能，使其可以支持表达式转换和计算。表达式的每个元素可以定义如下。

```cpp
template <typename T>
class ExpressionElement {
    std::variant<char, T> m_element;
};
```

#graybox[【C++学习】#linebreak()#h(2em)
`std::variant`是C++17引入的新特性，它是一种类型安全的联合体，可以存储多种类型的值，但在任意时刻只能存储其中一种。可以通过`std::get`来访问存储在其中的值并转换为目标类型，也可以通过`std::holds_alternative`来判断当前存储的是哪种类型。示例代码如下：
```cpp
char getOperator() const { return std::get<char>(m_element); }
bool isOperator() const { 
    return std::holds_alternative<char>(m_element);
}
```
]

#graybox[
#h(2em)`std::variant`通常被用于替代C语言中的`union`。和使用`constexpr`代替常规宏的做法不同，采用`std::variant`替代`union`不是无损的。二者的比较如下：
+ `std::variant`是类型安全的，能够提供运行时的类型检查和动态分派；而`union`不是，需要程序员自己保证类型安全性，如果程序员错误地访问了`union`中的非活跃成员，那么程序的行为是未定义的。
+ `std::variant`需要存储额外的类型信息，因此会占用更多的内存；而`union`不需要额外的类型信息，只占用存储在其中的最大类型的内存。
+ `std::variant`是一个类模板，容易通过模板元编程的方法和其他模板类一起使用，支持通过模板特化和继承的方法扩展。相应不可避免地，`std::variant`也会带来更长的编译时间（不过在小型程序中通常都是可以忽略的）。
+ `union`允许程序员直接控制内存布局，在一些低级编程任务（比如和硬件交互）中，这种控制是必要的。
]

我们使用`apply`方法表示运算，当元素是运算符时，它接收至多两个操作数，返回运算的结果；当元素是操作数时，它直接返回操作数本身。`operandPosition`方法则表示操作数的位置，当元素是运算符时，两个返回值各表示左侧和右侧是否可以接受操作数（比如阶乘，就只允许左侧有操作数）；当元素是操作数时，当然两边都不能有操作数。

```cpp
T apply(const T& lhs, const T& rhs) const override {
    if (isOperator()) {
        switch (getOperator()) {
        case '+': return lhs + rhs;
        case '-': return lhs - rhs;
        case '*': return lhs * rhs;
        case '/': return lhs / rhs;
        case '%': return lhs % rhs;
        case '^': return m_power(lhs, rhs);
        case '!': return m_factorial(lhs);
        }
    }
    return getOperand();
}

std::pair<bool, bool> operandPosition() const {
    if (isOperator()) {
        switch (getOperator()) {
        case '(': return { false, true };
        case ')': case '!': return { true, false };
        case '+': case '-':
        case '*': case '/':
        case '%': case '^': return { true, true };
        }
    }
    return { false, false };
}
```

#h(2em)这里乘方计算可以通过 @sum:complexity:power 所介绍的快速幂算法得到，对于阶乘，我们可以使用递归的方法进行计算。
```cpp
constexpr T factorial(T n) const {
    return n == 0 ? T { 1 } : n * factorial(n - 1);
}
```
#graybox[【C++学习】#linebreak()
    #h(2em)这里使用了`constexpr`关键字，当我们正常调用一个`factorial(n)`时，这个关键字并不会产生效果，但如果这个函数的参数是一个常量表达式，那么编译器会在编译时计算这个函数的返回值，而不是在运行时计算。比如，`factorial(3)`就会在编译时被计算为6。这样可以提高程序的性能。
]

=== 将字符串解析为中缀表达式 <sta:parse-infix>

本节讨论将字符串解析为中缀表达式。我们将表达式看成是析取元素组成的线性表，每一个元素存储一个操作数或一个运算符。在将字符串解析为中缀表达式的这个过程中，我们需要进行两件事：

+ 将操作数提取出来。每个运算符都是一个字符，而操作数可能不止有一个字符。因此，当扫描到数字时，不能立刻将它加入到表达式中，需要继续向下扫描，直到扫描到运算符或字符串尾，才能确认这个数字结束。
+ 将表达式中的负号和减号区分开来。减号需要两个操作数进行运算，而负号只是将它后面的操作数取反，二者具有不同的语义。通过分析可以发现，运算符`-`表示负号，当且仅当它在字符串起始位置，或前一个元素是左括号。

#h(2em)一种可能的实现如下所示。可以看到在字符串的左边界和右边界都有可能需要特判，如果不想特判也可以在字符串两端加上括号，然后再解析。

```cpp
Expression(std::string_view expr, std::size_t base = 10) {
    auto start { 0uz };
    T temp {};
    auto add { [&](std::size_t end) {
        if (end > start) {
            std::from_chars(expr.data() + start, 
                            expr.data() + end, temp, base);
            push_back(std::move(temp));
        }
    } };
    for (auto i { 0uz }; i < expr.size(); i++) {
        if (auto c { expr[i] }; !std::isdigit(c)) {
            add(i);
            if (c == '-' && (empty() || back() == '(')) {
                start = i;
            } else {
                start = i + 1;
                push_back(c);
            }
        }
    }
    add(expr.size());
}
```

#graybox[【C++学习】#linebreak()
    #h(2em)上面的代码中，我们使用了`std::from_chars`函数将字符串转换为整数。这个函数是C++17引入的新特性，它可以将字符串转换为整数。借助这个函数，我们可以使用一个函数调用来完成字符串到整数的转换，而不需要根据用户传入的整数类型选择不同的转换函数（如`std::stoi`、`std::stol`等）。当用户定义了自己的整数类型时，只需要重载`std::from_chars`函数即可。扩展成其他类型的转换也是类似的。
]

=== 中缀表达式转换为后缀表达式 <sta:infix-to-postfix>

在@sta:expression-definition 中，我们已经介绍了将中缀表达式转换为后缀表达式的方法。对于短表达式，也可以采用以下的快速手工方法，如@fig:sta3 所示。
+ 补全运算符对应的括号，而不补操作数对应的括号。
+ 对于每一对括号，设它对应的运算符为`o`，则删掉左括号，将右括号替换为`o`；如此就得到了后缀表达式。

#figure(
    image("images/sta3.svg", width: 80%),
    caption: "手工将中缀表达式转换为后缀表达式",
) <fig:sta3>

如果要得到前缀表达式，类似地，只需要删掉右括号，将左括号替换为`o`即可。前缀表达式的运算符位于入栈的位置上，而后缀表达式的运算符位于出栈的位置上，这都是非常自然的；反而，我们熟悉的中缀表达式，运算符的位置是不自然的。所以，为了计算中缀表达式，可以先将它转换为前缀表达式或后缀表达式。此处以后缀表达式为例，您可以自行完成前缀表达式的情况。

在手工方法中，操作数仍然在它原本的位置上，而运算符则被移动到了它出栈的位置（必定后于运算符出现的位置）。因此，我们可以对中缀表达式从前到后扫描，当扫描到操作数的时候将其直接加入后缀表达式，而当扫描到运算符的时候将其入栈，在运算符出栈的时候，再将它加入到后缀表达式中。但是，当在计算机中处理中缀表达式的时候，并不是每一个运算符都有对应的一对括号，因此，我们需要配合运算符的优先级进行处理。

我们的目标是寻找每个运算符应当在哪个位置出栈。我们考虑不为左右括号的运算符`o`，因为左右括号不会被加入到后缀表达式中。考虑表达式`( A o B )`，其中，左右的一对括号是`o`对应的括号（在中缀表达式中有可能实际存在，也有可能实际上不存在），`A`和`B`都是表达式。

+ 如果这对括号实际存在，那么我们分析表达式`B`的情况。如果`B`是操作数或者被一对括号包裹，则它确实参与了`o`的运算。如果`B`是其他情况，则可以设`B`为`C p D`，其中`p`是`B`在最后一步进行的运算，`C`和`D`都是表达式。那么，只有`o`的优先级_小于_`p`时，`B`作为整体才会参与`o`的运算。否则，在`A o C p D`中，会先计算`A o C`，`o`对应的括号应当不包括`p D`这一段。
+ 反过来，如果这对括号实际不存在，那么我们需要定位到它的位置。根据上面的分析可以知道，我们从`o`向后可以继续扫描，直到扫描到优先级_不大于_`o`的运算符`p`为止（不包括括号里的运算符）。`o`对应的括号应当出现在`p`之前；所以在扫描到`p`的时候将`o`出栈。当然还有一种特别的情况就是检查到表达式的末尾。如果在@sta:parse-infix 中对中缀表达式的开始和结尾加上了一对括号，那么此时就不需要特殊处理。

#h(2em)综上所述，我们可以得到这样的算法框架：
+ 当扫描到操作数时，直接加入后缀表达式。
+ 当扫描到非括号的运算符`o`时，首先将栈内优先级不小于`o`的运算符依次弹出并加入后缀表达式，然后将`o`入栈。

#h(2em)现在考虑括号的情况。我们会发现，左括号作为运算符，很难确定它的优先级。一方面，当扫描到左括号的时候，它应当具有最高的优先级，左括号内部永远应当先于当前的栈顶运算符进行计算。另一方面，当左括号在栈内时，它应当具有（除了右括号外）最低的优先级，因为必须计算完括号才能脱括号，所以在找到其对应的右括号之前，任何运算符都不应该让左括号出栈。

这就意味着我们不能用单一的_优先级_来描述运算符，而是需要将其拆分为栈内优先级和栈外优先级。当我们扫描到运算符`o`时，将栈顶的栈内优先级，和`o`的栈外优先级进行比较。如果栈顶的栈内优先级不小于`o`的栈外优先级，则将栈顶弹出。左括号出栈后不进入后缀表达式，右括号则不必入栈，因为右括号对应的栈操作是$and$，扫描到右括号的时候恰好会弹出它对应的左括号。

除了左括号外，其他运算符的栈内和栈外优先级相同；右括号的栈内优先级和栈外优先级都是最低。左括号具有最高的栈外优先级，以及除右括号之外最低的栈内优先级。因此，左括号入栈必定不会引起其他运算符出栈（括号内总是先于括号外计算），且仅当扫描到右括号的时候左括号可以出栈。

只考虑左结合运算的情况下，可以将运算符优先级实现如下：
```cpp
inline static const std::unordered_map<char, int> priority_left {
    {'(', 0}, {')', 0}, {'^', 3}, {'!', 4},
    {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'%', 2}
};
inline static const std::unordered_map<char, int> priority_right {
    {'(', 6}, {')', 0}, {'^', 3}, {'!', 4},
    {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'%', 2}
};

bool prior(char next) const override {
    if (isOperator()) {
        auto prior_left { priority_left.find(getOperator()) };
        auto prior_right { priority_right.find(next) };
        if (prior_left != priority_left.end() && 
            prior_right != priority_right.end()) {
            return prior_left->second >= prior_right->second;
        }
    }
    return false;
}
```

#h(2em)上面虽然进行了很多分析，但目的只是为了让读者更好地理解我们为什么要这样设计算法。算法的实际实现非常简单，如下所示。

```cpp
void infix2suffix() {
    Stack<ExpressionElement<T>> S;
    Expression<T> suffix;
    auto process { [&](const ExpressionElement<T>& e) {
        while (!S.empty() && S.top().prior(e.getOperator())) {
            if (auto op { S.pop() }; op != '(') {
                suffix.push_back(std::move(op));
            } else {
                break;
            }
        }
    } };
    for (auto& e : *this) {
        if (e.isOperand()) {
            suffix.push_back(std::move(e));
        } else {
            process(e);
            if (e != ')') {
                S.push(std::move(e));
            }
        }
    }
    process(')');
    *this = std::move(suffix);
}
```

#linebreak()#v(-1.5em)
=== 后缀表达式转换为中缀表达式 <sta:postfix-to-infix>

现在考虑上述问题的反问题：如何从后缀表达式转换为中缀表达式？

下面从信息的角度分析，“中缀转后缀”和“后缀转中缀”有什么不同。
注意到，中缀表达式是由“语义元素”（操作数和非括号的运算符）以及“括号序列”共同组成的。“语义元素”是参与运算的元素，对应了入、出栈过程中操作的数据；而“括号序列”是进行运算的次序，对应了入、出栈过程中的操作序列。所以知道中缀表达式，也就知道了操作序列的每一步入、出栈了什么元素，从而得到入栈序列（前缀表达式）和出栈序列（后缀表达式）。从信息的角度看，中缀表达式中包含的信息，包括了前缀表达式和后缀表达式所需的信息。

但是，后缀表达式仅仅是由“语义元素”组成的，它本身只是一个出栈序列。从前面的小节中您已经知道，给定出栈序列而不给定操作序列，则有`Catalan(n)`种可能的入栈序列。所以，如果仅仅依赖后缀表达式的_文本_，而不借助其他先验信息，那么从后缀表达式反推中缀（前缀）表达式是不可能的、是缺少信息的。要能够实现后缀表达式到中缀表达式的转换，必须要知道_文本_以外的额外信息。

这个先验的额外信息就是：_每个运算符需要的操作数数量_。在常见的运算符中，加、减、乘、除、乘方，操作数数量都是2；阶乘的操作数数量是1。需要特别注意的是，当“减号”和“负号”都被作为运算符处理，且采用同一符号`-`时，它的操作数数量是不确定的，这一不确定性会导致后缀表达式出现歧义。中缀表达式可以通过判断`-`之前是否为操作数来进行区分，而后缀表达式无法进行区分。比如：对于后缀表达式`1 2 - 3 - -`，它的三个`-`中有一个代表负号，另外两个代表减号。因此，这个后缀表达式可能对应3种不同的中缀表达式：`1-((-2)-3)`、`(1-2)-(-3)`和`-((1-2)-3)`。@sta:parse-infix 中我们处理中缀表达式的时候，在解析阶段就已经将减号和负号进行了分离。负号被认为是操作数的一部分，而所有出现在表达式中的运算符`-`都是减号；在后缀表达式的处理中我们也将延续这一约定以避免上述歧义。

下面解释这个先验的额外信息是如何帮助后缀表达式转换成中缀表达式的。

假设某个运算符`o`的操作数数量是2。在补全了所有括号的中缀表达式中，这个操作数出现在`((A) o (B))`这个片段里，其中`A`和`B`都是中缀表达式。按照@sta:infix-to-postfix 的方法，这个片段化为后缀表达式是`A' B' o`，其中`A'`和`B'`是`A`和`B`对应的后缀表达式。于是得到：对于后缀表达式`E`中的每个运算符`o`，都存在`E`中的一个连续的子序列`E(o)`，`E(o)`是以`o`结尾的后缀表达式。

在之前讨论一般的出栈序列的时候，`A'`和`B'`的划分是任意的，所以会得到卡特兰数的递归方程。但在讨论后缀表达式的时候，需要受到_操作数数量_的限制，因此`A'`和`B'`的划分不是任意的。事实上，只有一种划分是合法的。我们知道，后缀表达式的开头必定是操作数，末尾必定是运算符。对于语义元素组成的序列$C=(c_1,c_2,...,c_n)$，定义：
$
f(c_i)=cases(
    1","&c_i "是操作数", \
    1-"Cnt"(c_i)","&c_i "是运算符"
)
$

#h(2em)其中$"Cnt"(c_i)$表示运算符$c_i$的操作数数量。令$f(C)=sum f(c_i)$，则对于合法的后缀表达式`E`，可以证明$f(E)=1$：使用数学归纳法，对$E$中含有的运算符数量归纳。
+ 对于单个操作数，$f(E)=1$。
+ 对于以`k`元运算符结尾的表达式，设`E`为$A_1 A_2 ... A_k c$，其中$A_i$是后缀表达式。从而$f(E) = sum f(A_i) + f(c) = k + (1-k) = 1$。

#h(2em)进一步地，用数学归纳法可以证明，对于合法后缀表达式$E$的任意前缀$E_j=(e_1,e_2, ...,e_j),j <= n$，始终有$f(E_j)>= 1$。
+ 由于$e_1$是操作数，所以$f(E_1)=1$。
+ 如果$e_j$是操作数，则$f(E_j)=f(E_(j-1))+1 >= 1+1>1$。
+ 如果$e_j$是运算符，根据前面的证明的结果，必定存在某个$(e_t,e_(t+1), ...,e_j), 1 <= t<j$是以$e_j$结尾的后缀表达式。于是我们知道$f(e_t,e_(t+1),...,e_j)=1$，从而$f(E_j)=f(E_(t-1)) + f(e_t,e_(t+1),...,e_j)>= f(e_t,e_(t+1),...,e_j)=1$。

#h(2em)在上述结论的基础上，我们就可以使用反证法证明最开始提出的那个命题：对于以`k`元运算符`c`结尾的后缀表达式`E`，只有唯一的合法划分方式使`E`被划分为$A_1 A_2 ...  A_k c$。

如果有不止一种合法的划分方式，设两种划分方式中$(A_1,A_2,...,A_k)$序号最大的一个不同的项是$A_j$和$A_j'$。不妨设$A_j$的长度大于$A_j'$，那么$A_j$则可以被拆分为两段$P A_j'$。因为$A_j$和$A_j'$都是合法的后缀表达式，所以根据此前的结论，$f(P) = f(A_j) - f(A_j') = 1-1=0$。但由于$P$是$A_j$的前缀，于是$f(P)>= 1$，矛盾。

因此，递归地进行唯一合法的划分，就可以得到最终的中缀表达式。类似可以得到前缀表达式转换为中缀表达式的手段。

需要说明的是，上面的转换方式并不实用，本书在此处也没有给出相应的示例代码，仅仅从理论的角度进行介绍。实际转换的时候，通常利用*表达式树*作为中介进行转换，表达式树的内容将会在后面的章节介绍。在本章介绍表达式，是希望读者在思维中巩固“栈”和“表达式”之间的联系。 // TODO: 以后可以考虑增加表达式树的内容。

=== 后缀表达式转换为前缀表达式 <sta:postfix-to-prefix>

在@sta:postfix-to-infix 中使用了一个$f(dot)$函数用来辅助证明，这个函数并不是凭空产生的，而是另一个栈的产物。我们采用下面的方法构造一个操作序列。

+ 将后缀表达式中的操作数`i`，用$or(i)$替代。
+ 将后缀表达式中的运算符`o`，用$and ^k or(o)$替代，其中$and^k$表示`k`个连续的$and$，`k`是这个运算符的运算所需操作数数量。
+ 在整个序列的结尾处增加一个$and$。

#h(2em)根据@sta:postfix-to-infix 的结论，我们可以确定，这样得到的操作序列是合法的。

我们发现，在这个操作序列中，对应的_入栈序列恰好是后缀表达式_。自然地，我们想到要分析它的出栈序列。仍然以前面的`1 2 3 4 - * +`作为例子，它对应$or(1) or(2) or(3) or(4) and and or(-) and and or(*) and and or(+) and$，从而可以得到出栈序列是`4 3 - 2 * 1 +`。这恰好是_倒序的前缀表达式_。//\textit{对于一般情形，您可以用递归（归纳）的方法证明这个结论。对偶地，您也可以得到前缀表达式转换成后缀表达式的手段。}

=== 后缀表达式的计算 <sta:calculate-postfix>

后缀表达式的计算是重要的基本功之一。要计算后缀表达式，只需要对@sta:postfix-to-prefix 中的构造操作序列作出一点点修改。

+ 将后缀表达式中的操作数`i`，用$or(i)$替代。
+ 将后缀表达式中的运算符`o`，用$and ^k or(r)$替代，其中$and^k$表示`k`个连续的$and$，`k`是这个运算符的运算所需操作数数量。`r`是_本次运算的结果_。设第$i$个$and$出栈的数为$A_i$，则$r=c(A_1,A_2,...,A_k)$。
+ 在整个序列的结尾处增加一个$and$。这次出栈的数就是后缀表达式的计算结果。

```cpp
T calSuffix() const {
    Stack<T> S;
    for (auto& e : *this) {
        if (e.isOperand()) {
            S.push(e.getOperand());
        } else {
            auto [l, r] { e.operandPosition() };
            T rhs { r ? S.pop() : 0 };
            T lhs { l ? S.pop() : 0 };
            S.push(e.apply(lhs, rhs));
        }
    }
    return S.pop();
}
```

#h(2em)实际出现的后缀表达式计算问题，可以使用上述算法手工计算。不过和之前一样，我更推荐使用表达式树而不是栈进行计算。诚然，表达式树的做法比栈要复杂一些；但表达式树的方法更加清晰，更加适合答题结束后的检查，更加不容易出错。知道后缀表达式如何计算之后，由于之前已经介绍过各种表达式之间互相转换的方法，所以也就能够得到计算前缀表达式和中缀表达式的算法了。

=== 中缀表达式的计算 <sta:calculate-infix>

#bluetxt[实验`expr.cpp`。]在这个实验中将实现中缀表达式的计算，从而实现一个类似计算器的功能。根据前面的结论，我们可以先将中缀表达式转换为后缀表达式，再使用后缀表达式计算。

== 栈与递归 <sta:recursion>

栈和递归的关系非常紧密，因为调用递归函数本质上相当于使用了系统栈。具体的原理参见《操作系统》。系统栈的空间是有限的，如果递归层次过多，就会发生栈溢出（stack overflow）错误。为了避免这种情况发生，我们可以通过手写栈将递归改写为迭代形式。本节将介绍使用栈消除递归的方法。

=== 消除多个实例的尾递归 <sta:tail-recursion>

=== 计算组合数 <sta:combination>

=== 消除一般的递归 <sta:recursion-elimination>

== 共享栈 <sta:shared-stack>

当我们使用顺序栈的时候，我们为栈申请了一片连续内存作为存储空间。注意到，我们的栈只使用了前半部分的空间，而没有使用后半部分的空间。有一种高效利用空间的方法称为*共享栈*（shared stack），它由两个共享同一片连续内存的栈组成。其中一个栈使用前半部分，以秩为0的元素为栈底，以秩最大的元素为栈顶；另一个栈使用后半部分，以秩为`n-1`的元素为栈底，以秩最小的元素为栈顶。两个栈的栈顶“相向而行”。在大多数的软件开发过程中，由于空间相对来说是充足的，所以共享栈并不常见。但是在一些特殊的场合，共享栈可以发挥出它的优势。比如在一些嵌入式系统中，使用一个栈作为系统栈，另一个栈作为用户栈，可以有效地隔离系统和用户的内存空间。

=== 共享栈的实现 <sta:shared-stack-implementation>

=== 多于两个的栈的共享 <sta:shared-stack-more>




== 本章习题 <sta:exercise>
 
在 @sta:property 中：
+ #easy 仿照@lis:summary 列出一个表格，比较顺序栈和链栈的优缺点。

#h(2em)在 @sta:pop-sequence 中：
+ #easy 设入栈序列为1234，则可能的出栈序列有哪些？
+ #hard 证明：如果入栈序列是$(1, 2,..., n)$，则$B=(b_1,b_2, ..., b_n)$是出栈序列，当且仅当不存在“312”模式。即：不存在$i<j<k$，使得$b_j<b_k<b_i$。
+ #medium 如果令$or = 0, and = 1$，那么就可以将一个操作序列转换为为`2n`位的二进制数。借助二进制数的大小关系，我们可以比较两个操作序列，这种比较方法称为*字典序*。给定一个合法操作序列，设计一个算法，求字典序中的下一个合法操作序列。
+ #medium 给定一个合法操作序列，设计一个算法，求它在字典序中的排名。

#h(2em)在 @sta:parentheses 中：
+ #easy 括号匹配算法是减治算法还是分治算法？
+ #easy 设计一个算法，解决只有一种括号的匹配问题。
+ #medium 正则文法是指产生式满足下列形式的文法：$A->a$或$A->a B$，其中$A,B$是非终结符，$a$是终结符。满足正则文法的字符串可以用正则表达式（`regex`）或有限状态机（FSM）判别。合法的括号序列是否是正则文法？如果是，给出相应的产生式。
+ #hard 如果限定括号序列中的嵌套层数不超过`k`，其中`k`是一个常数，那么括号序列是否是正则文法？如果是，给出相应的产生式。

#h(2em)在 @sta:expression 中：
+ #medium 证明@sta:postfix-to-prefix 中的方法可以得到倒序的前缀表达式。
+ #medium 有一些运算符的字符数可能不止一个。在本书的代码基础上，增加对Python风格的乘方运算符`**`的支持。
+ #hard 对于最多二元的运算符来说，如果使用合取方法来定义表达式中的元素，则@sta:calculate-infix 中的计算算法可以得到大幅度简化：因为表达式里的每个元素都是运算符，而操作数是附带在前方最近的运算符上的。写出这种简化后的计算算法。
+ #hard 在本书的代码基础上，增加对三元运算符`?:`的支持。
+ #hard 常规的运算符几乎都是左结合的，但我们也可以加入一些右结合运算符。一个比较常见的右结合运算符是$arrow.t$，即高德纳（Knuth）箭头，定义$a arrow.t b = a ^b$，同时$a arrow.t b arrow.t c = a arrow.t (b arrow.t c)$；因此，我们让本书中的乘方运算符`^`改为右结合是非常合理的做法。在本书的代码基础上，增加对右结合运算符的支持。

#h(2em)在 @sta:shared-stack 中：
+ #hard 如果在一个向量中维护多于2个的栈，则它们无法像共享栈那样很方便地，有固定的栈底和移动的栈顶。假设我们有一个容量充分大的向量`V`存储`n`个栈$S_1, S_2, ..., S_n$，所有的栈紧挨着存储。初始时所有的栈顶都位于`V[0]`。当向栈$S_i$插入一个元素时，它的栈顶向后移动一个位置，同时让$S_(i+1), S_(i+2), ..., S_n$之后的栈的所有元素及其栈顶都向后移动一个位置。这当然是一个很低效的方法。假设对于每次插入，插入到每个栈的概率相同，求从起始状态开始，连续插入`m`个元素造成的平均移动次数。
+ #hard 在第1题的基础上，如果插入到每个栈的概率不同，分别为$p_1,p_2,...,p_n$，则插入`m`个元素造成的平均移动次数是多少？利用不对称性改进算法，改变栈的顺序，使得平均移动次数最小。
+ #hard 在第1题的基础上，如果初始时每个栈被分配了`t`个元素的空间，即每个栈的栈顶分别位于`V[0], V[t], V[2t], ..., V[(n-1)t]`，则插入`m`个元素造成的平均移动次数是多少？