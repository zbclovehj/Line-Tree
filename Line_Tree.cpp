#include"Line_Tree.h"

#include<iostream>
int main() {
	Line_Tree<int> line(100);
	for (int i = 0; i < line.a.size(); i++)
	{
		line.a[i] = i;
	}
	line.build(1,1,100);
}