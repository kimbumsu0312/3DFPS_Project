#pragma once
#include "Base.h"
NS_BEGIN(Engine)
class CEvent_Manager final : public CBase
{
private:
	CEvent_Manager();
	virtual ~CEvent_Manager() = default;

public:
    template<typename EventType>
    using HandlerFunc = function<void(const EventType&)>;

    // �̺�Ʈ ����
    template<typename EventType>
    void Subscribe(HandlerFunc<EventType> handler)
    {
        auto& handlers = m_maplisteners[typeid(EventType)];
        handlers.push_back([handler](const void* event) { handler(*static_cast<const EventType*>(event)); });
    }

    // �̺�Ʈ ����
    template<typename EventType>
    void Publish(const EventType& event)
    {
        auto it = m_maplisteners.find(typeid(EventType));
        if (it != m_maplisteners.end())
        {
            for (auto& handler : it->second) 
            {
                handler(&event);
            }
        }
    }
private:
	//const void* ���ڸ� ����
	//void�� ��ȯ
	//function �Լ��� ���� �� �ִ� ��ü

	//�Լ��� ���� ����
	using Handler = function<void(const void*)>;
	
	//�Լ����� �����Ѵ�.
	unordered_map<type_index, vector<Handler>> m_maplisteners;

public:
	static CEvent_Manager*	Create();
	virtual void			Free();
};

NS_END