#if !defined(__NODE_H__)
#define __NODE_H__
#include <stddef.h>
// 单链表结点模板类
template <class ElemType>
class Node 
{

public:
// 构造函数模板:
	Node();						// 默认构造函数模板
	Node(const ElemType &e, Node<ElemType> *link = NULL);	// 带元素值和指针构造函数
	~Node();//析构函数
	// 数据成员:
	ElemType m_TData;				// 数据成分
	Node<ElemType>* m_pNext;		// 指向下一个结点

	virtual void ClearLink(); //清空链接状态
};

// 节点模板类实现
template<class ElemType>
//构造函数默认实现
Node<ElemType>::~Node()
{

}

template<class ElemType>
Node<ElemType>::Node()
// 构造函数初始化指针为NULL
{
	m_pNext = NULL;
}

template<class ElemType>
Node<ElemType>::Node(const ElemType &e, Node<ElemType> *link)
// 构造函数设置结点数据为 e 并将指针设为 link
{
	m_TData = e;
	m_pNext = link;
}
template<class ElemType>
//清空结点链接状态
void Node<ElemType>::ClearLink()
{
	m_pNext = NULL;
}
// 双向链表结点模板类
template <class ElemType>
class DbNode 
{

public:
	// 构造函数模板:
	DbNode();						// 默认构造函数模板
	DbNode(const ElemType& e, DbNode<ElemType>* prelink = NULL, DbNode<ElemType>* nextlink = NULL);	// 带元素值和两个指针构造函数
	~DbNode();//析构函数
	//m_TData,m_pNext数据继承自Node
	// 数据成员:
	DbNode<ElemType>* m_pPre;		// 指向前一个结点的指针
	DbNode<ElemType>* m_pNext;		//指向后一个结点的指针
	ElemType m_TData;				// 数据成分
	virtual void ClearLink(); //清空链接状态

};
template<class ElemType>
DbNode<ElemType>::DbNode()
// 构造函数初始化前后指针为空
{
	m_pPre = NULL;
	m_pNext = NULL;
}
template<class ElemType>
DbNode<ElemType>::DbNode(const ElemType& e, DbNode<ElemType>* prelink , DbNode<ElemType>* nextlink )
// 构造函数设置结点数据为 e ，前后指针为 prelink 和 nextlink
{
	this->m_TData = e;
	m_pPre = prelink;
	m_pNext = nextlink;

}

template<class ElemType>
DbNode<ElemType>::~DbNode()
// 析构函数默认实现
{
}

template<class ElemType>
//清空双向链表结点链接状态
void DbNode<ElemType>::ClearLink()
{
	m_pNext = NULL;
	m_pPre = NULL;
}
#endif

