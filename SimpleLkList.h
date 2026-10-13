#if !defined(_SIMPLE_LK_LIST_H_)
#define _SIMPLE_LK_LIST_H_
#include "LinkListBase.h"
#include <stddef.h>
template <class ElemType> class SimpleLkList :public LinkListBase< ElemType, Node<ElemType> >
{
public:
	SimpleLkList();//默认构造函数
	~SimpleLkList();//析构函数
	SimpleLkList(const SimpleLkList& source);//拷贝构造函数
protected:
	virtual bool Link(Node<ElemType>* pPreNode,Node<ElemType>* pCurNode) ;//重写虚函数 Link
};
template <class ElemType>
SimpleLkList< ElemType>::SimpleLkList()
{
}
template <class ElemType>
SimpleLkList< ElemType>::SimpleLkList(const SimpleLkList& source)
{
	*this = source;//通过赋值运算符 '=' 进行值复制，避免重复
}
template <class ElemType>
SimpleLkList< ElemType>::~SimpleLkList()
{
}

template <class ElemType>
bool SimpleLkList< ElemType>::Link(Node<ElemType>* pPreNode, Node<ElemType>* pCurNode)
{
	if (!pPreNode)
		return false;
	pPreNode->m_pNext = pCurNode;//前一个结点的 m_pNext 指向 pCurNode
	return true;
}
#endif
