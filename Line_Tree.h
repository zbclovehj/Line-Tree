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
	std::vector<T>add;//������
};



#endif // !LINE_TREE_H
//ѯ��A[L,R]�ĺ� ���������Ʊ�ǵĺ�����
//��Ϊ��������޸ģ����Լ�������ֵ���޸�ֵ���Գ���
template<class T>
inline void Line_Tree<T>::PushDown(int rt, int ln, int rn) {
	if (add[rt])
	{
		add[rt << 1] += add[rt];
		add[rt << 1 | 1] += add[rt];
		t[rt << 1] += add[rt] * ln;
		t[rt << 1 | 1] += add[rt] * rn;
		add[rt] = 0;//����Ͱѵ�ǰ�ڵ�Ҫ������������
	}
}
template<class T>
inline Line_Tree<T>::Line_Tree(T maxx)
{
	a.resize(maxx);
	t.resize(maxx << 2);
	add.resize(maxx << 2);//���˱��
}

template<class T>
inline void Line_Tree<T>::Pushup(int k)
{
	//���º�����������ʵ�����ֵ ��ͬ����Ա�ɣ���Сֵ������͵�
	t[k] = t[k << 1] + t[k << 1 | 1];
}

template<class T>
inline void Line_Tree<T>::build(int k, int l, int r)
{//kΪ��ǰ��Ҫ�����Ľ�㣬lΪ��ǰ��Ҫ�����������˵㣬r��Ϊ�Ҷ˵�
	if (l == r) {
		t[k] = a[l]; //��˵�����Ҷ˵㣬��ΪҶ�ӽڵ㣬ֱ�Ӹ�ֵ����
	}
	else {
		int m = l + ((r - l) >> 1);    //m��Ϊ�м�㣬����ӵĽ������Ϊ[l,m],�Ҷ��ӵĽ������Ϊ[m+1,r]
	    build(k << 1, l, m);    //�ݹ鹹������ӽ��
	    build(k << 1 | 1, m + 1, r);    //�ݹ鹹���Ҷ��ӽ��
		Pushup(k);    //���¸��ڵ�
	}
}
//t[p]+=v һ��Ӹ��ڵ����
template<class T>
inline void Line_Tree<T>::updata_single(int p, int v, int l, int r, int k)
{
	//pΪԭ�����±귶Χ��vΪҪ���ϵ�ֵ��l��rΪ������䣬kΪ����±�
	if (l == r)    //��˵�����Ҷ˵㣬��ΪҶ�ӽ�㣬ֱ�Ӽ���v����
	{
		t[k] += v;

	}
   //ԭ������߶������鶼�õ�����
	else {
		int m = l + ((r - l) >> 1);    //m��Ϊ�м�㣬����ӵĽ������Ϊ[l,m],�Ҷ��ӵĽ������Ϊ[m+1,r]
		if (p <= m)    //�����Ҫ���µĽ��������������
			updata_single(p, v, l, m, k << 1);
		else    //�����Ҫ���µĽ��������������
			updata_single(p, v, m + 1, r, k << 1 | 1);
		Pushup(k);    //���¸��ڵ��ֵ
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
//t[L,R]+=c ���ڵ�Ϊ������� l rһ��Ϊ 1 �� n Ҳ����ԭ��������з�Χ
template<class T>
inline void Line_Tree<T>::updata_range(int L, int R, int C, int l, int r, int rt) {
	if (L<=l&&r<=R)
	{
		t[rt] += C * (r - l + 1);////�������ֺͣ����ϱ�����ȷ
		add[rt] += C;////����Add��ǣ���ʾ�������Sum��ȷ���������Sum����Ҫ����Add��ֵ������
		return;
	} 
	int m = (l + r) >> 1;
	PushDown(rt, m - l + 1, r - m);
	if (L <= m)updata_range(L, R, C, l, m, rt << 1);
	if (R > m)updata_range(L, R, C, m + 1, r, rt << 1 | 1);
	Pushup(rt);//���±��ڵ����Ϣ
}


template<class T>//���ڵ�Ϊ������� l rһ��Ϊ 1 �� n Ҳ����ԭ��������з�Χ
inline int Line_Tree<T>::query_range(int L, int R, int l, int r, int rt) {
	if (L <= l && r <= R) {
		//�������ڣ�ֱ�ӷ��� ��ǰ�ڵ�ֵ
		return t[rt];
	}
	//���Ʊ�ǣ�����Sum���ܲ���ȷ
	int m = (l + r) >> 1;
	PushDown(rt, m - l + 1, r - m);//��ǰ�ڵ�����ҳ���
	int ANS = 0;
	if (L <= m)ANS += query_range(L, R, l, m, rt << 1);
	if (R > m)ANS += query_range(L, R, m+1, r, rt << 1 | 1);
	return ANS;
}