#ifndef LINE_TREE_H
#define LINE_TREE_H
#include<vector>
#include<iostream>
template<class T>
class Line_Tree
{
public:
	Line_Tree(T maxx);
	~Line_Tree();
	void Pushup(int k);
	void build(int k, int l, int r);
	void updata_single(int p, int v, int l, int r, int k);
	void updata_range(int L, int R, int C, int l, int r, int rt);
	int query_single(int L, int R, int l, int r, int rt);
	int query_range(int L, int R, int l, int r, int rt);
	void PushDown(int rt, int ln, int rn);
 	std::vector<T>a;
	std::vector<T>t;
	std::vector<T>add;//懒惰标记
};



#endif // !LINE_TREE_H
//询问A[L,R]的和 首先是下推标记的函数：
//因为是区间和修改，所以加起来的值是修改值乘以长度
template<class T>
inline void Line_Tree<T>::PushDown(int rt, int ln, int rn) {
	if (add[rt])
	{
		add[rt << 1] += add[rt];
		add[rt << 1 | 1] += add[rt];
		t[rt << 1] += add[rt] * ln;
		t[rt << 1 | 1] += add[rt] * rn;
		add[rt] = 0;//加完就把当前节点要向下延伸的清空
	}
}
template<class T>
inline Line_Tree<T>::Line_Tree(T maxx)
{
	a.resize(maxx);
	t.resize(maxx << 2);
	add.resize(maxx << 2);//懒人标记
}

template<class T>
inline void Line_Tree<T>::Pushup(int k)
{
	//更新函数，这里是实现最大值 ，同理可以变成，最小值，区间和等
	t[k] = t[k << 1] + t[k << 1 | 1];
}

template<class T>
inline void Line_Tree<T>::build(int k, int l, int r)
{//k为当前需要建立的结点，l为当前需要建立区间的左端点，r则为右端点
	if (l == r) {
		t[k] = a[l]; //左端点等于右端点，即为叶子节点，直接赋值即可
	}
	else {
		int m = l + ((r - l) >> 1);    //m则为中间点，左儿子的结点区间为[l,m],右儿子的结点区间为[m+1,r]
	    build(k << 1, l, m);    //递归构造左儿子结点
	    build(k << 1 | 1, m + 1, r);    //递归构造右儿子结点
		Pushup(k);    //更新父节点
	}
}
//t[p]+=v 一般从根节点进入
template<class T>
inline void Line_Tree<T>::updata_single(int p, int v, int l, int r, int k)
{
	//p为原数组下标范围，v为要加上的值，l，r为结点区间，k为结点下标
	if (l == r)    //左端点等于右端点，即为叶子结点，直接加上v即可
	{
		t[k] += v;

	}
   //原数组和线段树数组都得到更新
	else {
		int m = l + ((r - l) >> 1);    //m则为中间点，左儿子的结点区间为[l,m],右儿子的结点区间为[m+1,r]
		if (p <= m)    //如果需要更新的结点在左子树区间
			updata_single(p, v, l, m, k << 1);
		else    //如果需要更新的结点在右子树区间
			updata_single(p, v, m + 1, r, k << 1 | 1);
		Pushup(k);    //更新父节点的值
	}
}
template<class T>
inline int Line_Tree<T>::query_single(int L, int R, int l, int r, int rt) {
	if (L<=l&&r<=R)
	{
		return t[rt];
	}
	int m = (l + r) >> 1;
	int sum = 0;
	if (L <= m) {
		sum += query_single(L, R, l, m, rt << 1);
	}
	if(R > m) {
		sum += query_single(L, R, m + 1, r, rt << 1 | 1);
	}
	return sum;
}
//t[L,R]+=c 根节点为进入入口 l r一般为 1 ， n 也就是原数组的所有范围
template<class T>
inline void Line_Tree<T>::updata_range(int L, int R, int C, int l, int r, int rt) {
	if (L<=l&&r<=R)
	{
		t[rt] += C * (r - l + 1);////更新数字和，向上保持正确
		add[rt] += C;////增加Add标记，表示本区间的Sum正确，子区间的Sum仍需要根据Add的值来调整
		return;
	} 
	int m = (l + r) >> 1;
	PushDown(rt, m - l + 1, r - m);
	if (L <= m)updata_range(L, R, C, l, m, rt << 1);
	if (R > m)updata_range(L, R, C, m + 1, r, rt << 1 | 1);
	Pushup(rt);//更新本节点的信息
}


template<class T>//根节点为进入入口 l r一般为 1 ， n 也就是原数组的所有范围
inline int Line_Tree<T>::query_range(int L, int R, int l, int r, int rt) {
	if (L <= l && r <= R) {
		//在区间内，直接返回 当前节点值
		return t[rt];
	}
	//下推标记，否则Sum可能不正确
	int m = (l + r) >> 1;
	PushDown(rt, m - l + 1, r - m);//当前节点和左右长度
	int ANS = 0;
	if (L <= m)ANS += query_range(L, R, l, m, rt << 1);
	if (R > m)ANS += query_range(L, R, m+1, r, rt << 1 | 1);
	return ANS;
}