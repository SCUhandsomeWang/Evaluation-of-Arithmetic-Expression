#if !defined(_LINK_LIST_BASE_H_)
#define  _LINK_LIST_BASE_H_
#include <stddef.h>
#include "ListBase.h"
#include "node.h"
template <class ElemType, class NodeType>  
class LinkListBase:public ListBase<ElemType>
{
public:
	LinkListBase();//默认构造函数
	virtual ~LinkListBase();//析构函数
	virtual int Length() const;//获取链表中元素个数
	virtual bool IsEmpty() const;//判断链表是否为空
	virtual void Clear();//清空链表
	virtual bool GetElem(int position, ElemType& e) const;//获取指定位置元素
	virtual bool SetElem(int position, const ElemType e);//设置指定位置元素
	virtual bool Delete(int position, ElemType& e);//删除指定位置元素
	virtual bool Insert(int position, const ElemType e) ;//在指定位置插入元素
	virtual void Traverse(bool (*visit)(const ElemType&))const ;//遍历链表
	virtual int AddTail(const ElemType e);	//在链表尾部追加元素
	virtual LinkListBase& operator = (const LinkListBase<ElemType, NodeType>& source); // 复制赋值运算符
	virtual void Head();//将当前结点指向头结点
	virtual bool Next(ElemType& e);//当前结点后移，同时获取下一个结点数据
	virtual bool SetCurElem(const ElemType e);//设置当前结点元素值
	virtual bool GetCurElem(ElemType& e) const;//获取当前结点元素值
	virtual bool DeleteCur(ElemType& e);//删除当前结点元素
	
protected:
	virtual bool Link(NodeType* pPreNode,NodeType* pNextNode) =0;//连接前后结点
	virtual NodeType* FindNode(int position, NodeType*&  pPreNode) const;//查找位置并返回结点
	virtual NodeType* GetFirstNode() const;//获取头结点后的第一个元素结点
	virtual bool IsEndWhile(NodeType* pNode) const;//是否到达循环结束条件
	NodeType* m_pNodeHead;//头结点指针
	NodeType* m_pNodeTail;//尾结点指针
	NodeType* m_pNodeCur;//当前结点指针
	int m_nNodeCounts;//当前结点数量
	bool m_bSpecialDetrcutiom;//特殊处理标志，避免重复释放
	
};
//LinkListBase实现
template <class ElemType,class NodeType>
LinkListBase<ElemType, NodeType>::LinkListBase()
{
	m_bSpecialDetrcutiom = false;
	m_pNodeHead = new NodeType;
	m_nNodeCounts = 0;
	m_pNodeTail = m_pNodeHead;//初始化时头结点等于尾结点
	m_pNodeCur = m_pNodeHead;//当前结点为头结点
}

template <class ElemType, class NodeType>
LinkListBase<ElemType, NodeType>& LinkListBase<ElemType, NodeType>::operator = (const LinkListBase<ElemType, NodeType>& source)
{

	Clear();//清空链表
	m_bSpecialDetrcutiom = false;
	NodeType* pSrcNodeHead = source.m_pNodeHead;//获取源链表头结点
	NodeType* pSrcNode = source.GetFirstNode();//pSrcNode为source链表第一个元素结点
	NodeType* pPreNode = m_pNodeHead;
	while (!source.IsEndWhile(pSrcNode))
	{
		NodeType* pNewNode = new NodeType;
		pNewNode->m_TData = pSrcNode->m_TData;//新结点赋值
		Link(pPreNode, pNewNode);//连接前结点和新结点
		pSrcNode = pSrcNode->m_pNext;//源链表当前结点后移
		pPreNode = pNewNode;//前结点更新为新结点
		m_nNodeCounts++;
	}
	m_pNodeTail = pPreNode;
	m_pNodeCur = m_pNodeHead;
	return *this;
}

template <class ElemType, class NodeType>
LinkListBase<ElemType, NodeType>::~LinkListBase()
{
	if (!m_bSpecialDetrcutiom)
	{

		Clear();//清空链表
		if (m_pNodeHead)
		{
			delete m_pNodeHead;
			m_pNodeHead = NULL;
		}
	}
}


template <class ElemType, class NodeType>
int LinkListBase<ElemType, NodeType>::Length() const
{
	int nNodeCounts = 0;//结点数量计数

/*
	如果不使用 m_nNodeCounts 作为结点数量，则需要遍历链表
	NodeType* pNode = GetFirstNode();
	while (!IsEndWhile(pNode))
	{
		nNodeCounts++;
		pNode = pNode->m_pNext;
	}
*/
	nNodeCounts = m_nNodeCounts;
	return nNodeCounts;
}

template <class ElemType, class NodeType>
NodeType* LinkListBase<ElemType, NodeType>::GetFirstNode() const
{
	NodeType* pNode = m_pNodeHead->m_pNext;
	return pNode;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::IsEndWhile(NodeType* pNode) const
{
	if (!pNode || pNode == m_pNodeHead)
		return true;
	return false;
}
template <class ElemType, class NodeType>
void LinkListBase<ElemType, NodeType>::Head()
{
	m_pNodeCur = m_pNodeHead;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::Next(ElemType &e)
{
	if (!m_pNodeCur || !m_pNodeCur->m_pNext)
		return false;//当前结点为尾结点或者其后为空，不能继续移动到当前结点
	m_pNodeCur = m_pNodeCur->m_pNext;
	e = m_pNodeCur->m_TData;
	return true;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::SetCurElem(const ElemType e)
{
	if (m_pNodeCur == m_pNodeHead || !m_pNodeCur)
	{//当前结点为头结点或当前结点为空
		return false;
	}
	m_pNodeCur->m_TData = e;
	return true;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::DeleteCur(ElemType& e)
{
	if (!m_pNodeCur || m_pNodeCur == m_pNodeHead)
		return false;//当前结点为空，当前结点为头结点，不能删除
	NodeType* pNextNode = m_pNodeCur->m_pNext;//直接从当前结点后继获得指针
	NodeType* pPreNode = m_pNodeHead;//前驱结点从头结点开始
	NodeType* pNode = GetFirstNode();
	bool bFindPre = false;
	while (!IsEndWhile(pNode))
	{
		if (pNode == m_pNodeCur)
		{
			bFindPre = true;
			break;//找到当前结点后退出循环
		}
		pNode = pNode->m_pNext;
		pPreNode = pPreNode->m_pNext;
	}
	if (!bFindPre)
		return false;
	Link(pPreNode, pNode->m_pNext);//连接前驱结点和后继结点
	e = pNode->m_TData;

	if (pNode == m_pNodeTail)
	{//如果删除的是尾结点
		m_pNodeTail = pPreNode; //将尾结点设为删除结点的前驱
	}
	m_pNodeCur = pPreNode;//当前结点设为前驱
	delete pNode;//删除结点
	m_nNodeCounts--;//结点数量减一
	return true;
}
template <class ElemType, class NodeType>
void LinkListBase<ElemType, NodeType>::Clear()
{
	if (!m_pNodeHead)
		return;
	NodeType* pNode = GetFirstNode();
	//循环删除所有结点
	while (!IsEndWhile(pNode))
	{
		NodeType* pNodeNext = pNode->m_pNext;
		delete pNode;
		pNode = pNodeNext;
	}
	m_pNodeHead->ClearLink();//重置头结点的连接状态
	m_pNodeTail = m_pNodeHead;//尾结点设为头结点
	m_pNodeCur = m_pNodeHead;//当前结点为头结点
	m_nNodeCounts = 0;//结点数量为0
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::IsEmpty() const
{
	if (m_pNodeHead->m_pNext)
		return false;
	return true;
}
template <class ElemType, class NodeType>
NodeType* LinkListBase<ElemType, NodeType>::FindNode(int position, NodeType*& pPreNode) const
{
	if (position < 0)
		return NULL;
	NodeType* pNode = GetFirstNode();//获取第一个元素结点
	pPreNode = m_pNodeHead;//元素结点的前驱为头结点
	NodeType* pFindNode = NULL;//要查找的结点，初始值为空
	int nIndex = 0;//结点位置索引，初始值为0
	while (!IsEndWhile(pNode))
	{
		if (nIndex == position)
		{//当前位置等于目标位置
			pFindNode = pNode;//找到结点
			break;//退出循环
		}
		pPreNode = pNode;//前驱结点设为当前结点
		pNode = pNode->m_pNext;//当前结点后移
		nIndex++;//位置索引递增1
	}
	return pFindNode;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::SetElem(int position, const ElemType e) 
{
	NodeType* pPreNode;
	NodeType* pNode = FindNode(position, pPreNode);//查找位置并返回结点
	if (!pNode)
		return false;//未找到结点，返回false;
	pNode->m_TData = e;
	return true;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::GetElem(int position, ElemType& e) const
{
	NodeType* pPreNode;
	NodeType* pNode =  FindNode(position, pPreNode);//查找位置并返回结点
	if (!pNode)
		return false;//未找到结点，返回false;
	e = pNode->m_TData;
	return true;
}

template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::Delete(int position, ElemType& e)
{
	NodeType* pPreNode;
	NodeType* pNode = FindNode(position, pPreNode);//查找位置并返回结点
	if (!pNode)
		return false;//未找到结点，返回false;
	
	Link(pPreNode, pNode->m_pNext);//连接前驱结点和后继结点
	e = pNode->m_TData;
	
	if (pNode == m_pNodeTail)
	{//如果删除的是尾结点
		m_pNodeTail = pPreNode; //将尾结点设为删除结点的前驱
	}
	m_pNodeCur = pPreNode;//当前结点设为前驱
	delete pNode;//删除结点
	m_nNodeCounts--;//结点数量减一
	return true;
}
template <class ElemType, class NodeType>
bool LinkListBase<ElemType, NodeType>::Insert(int position, const ElemType e)
{
	NodeType* pPreNode;//待插入位置的前驱结点指针
	NodeType* pNode = FindNode(position, pPreNode);//查找位置并返回结点
	if (!pNode)
	{ //未找到结点
		if (position == m_nNodeCounts)//尾部插入判断
			pPreNode = m_pNodeTail;//尾部插入时前驱为尾结点
		else
			return false;//位置越界，返回false
	}
	NodeType* pNewNode = new NodeType;//创建新结点
	pNewNode->m_TData = e;//给新结点赋值
	Link(pPreNode, pNewNode);//连接前结点和新结点
	Link(pNewNode, pNode);//连接新结点和目标位置节点
	if (position == m_nNodeCounts)//尾部插入判断
		m_pNodeTail = pNewNode;//尾部插入时尾结点为新结点
	m_pNodeCur = pNewNode;//将当前结点设为新结点
	m_nNodeCounts++;
	return true;
}


template <class ElemType, class NodeType>
void LinkListBase<ElemType, NodeType>::Traverse(bool (*visit)(const ElemType&))const
{
	if (!visit)
		return;
	NodeType* pNode =GetFirstNode();
	while (!IsEndWhile(pNode))
	{
		if (!(*visit)(pNode->m_TData))
			break;//如果访问函数返回false，跳出循环
		pNode = pNode->m_pNext;
	}
}
template <class ElemType, class NodeType>
int LinkListBase<ElemType, NodeType>::AddTail(const ElemType e)
{
	NodeType* pNewNode = new NodeType;
	pNewNode->m_TData = e;//给新结点赋值
	Link(m_pNodeTail, pNewNode);//连接尾结点和新结点
	m_pNodeTail = pNewNode;//尾结点更新为新结点
	m_pNodeCur = pNewNode;//将当前结点设为新结点
	m_nNodeCounts++;//结点数量加一
	return m_nNodeCounts - 1;// 返回新插入结点的位置
}
template <class ElemType, class NodeType>
//获取当前结点元素值
bool LinkListBase<ElemType, NodeType>::GetCurElem(ElemType& e) const
{
	if (!m_pNodeCur)
		return false;//当前结点为空，操作失败
	if (m_pNodeCur == m_pNodeHead)
		return false;//头结点不保存有效数据，操作失败
	
	e = m_pNodeCur->m_TData;
	return true;
}
#endif
