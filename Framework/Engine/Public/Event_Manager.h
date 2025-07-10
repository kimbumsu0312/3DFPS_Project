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

    // 이벤트 구독
    template<typename EventType>
    void Subscribe(HandlerFunc<EventType> handler)
    {
        auto& handlers = m_maplisteners[typeid(EventType)];
        handlers.push_back([handler](const void* event) { handler(*static_cast<const EventType*>(event)); });
    }

    // 이벤트 발행
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
	//const void* 인자를 받음
	//void를 반환
	//function 함수를 담을 수 있는 객체

	//함수를 받을 변수
	using Handler = function<void(const void*)>;
	
	//함수들을 보관한다.
	unordered_map<type_index, vector<Handler>> m_maplisteners;

public:
	static CEvent_Manager*	Create();
	virtual void			Free();
};

NS_END