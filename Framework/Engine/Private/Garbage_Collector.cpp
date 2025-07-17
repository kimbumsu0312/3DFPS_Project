#include "pch.h"
#include "Garbage_Collector.h"
#include "GameObject.h"
CGarbage_Collector::CGarbage_Collector()
{
}

void CGarbage_Collector::GarbageSweep(CGameObject* pObject)
{
    if (pObject == nullptr)
        return;

    m_Garbage.push(pObject);
}

void CGarbage_Collector::Clear_Garbage()
{
    while (!m_Garbage.empty())
    {
        Safe_Release(m_Garbage.front());
        m_Garbage.pop();
    }
}

CGarbage_Collector* CGarbage_Collector::Create()
{
    return new CGarbage_Collector();
}

void CGarbage_Collector::Free()
{
    __super::Free();
    
    while (!m_Garbage.empty())
    {
        Safe_Release(m_Garbage.front());
        m_Garbage.pop();
    }
}
