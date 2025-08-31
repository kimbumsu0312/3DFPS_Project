#include "pch.h"
#include "Node.h"

CNode::CNode()
{
}

void CNode::Reset()
{
	m_iTreeIndex = 0;
}

void CNode::Free()
{
	__super::Free();
}
