/*!************************************************************************
 \file   Events.h
 \author Ang Jiawei Jarrett
 \par    DP email: a.jiaweijarrett@digipen.edu
 \par    Course: CSD2401A
 \date   11-09-2023
 \brief
 This header file defines a generic event class, overloaded to support the 
 subscribing of functions (observers) that have up to 4 unique parameters.

 E.g. Creating a onMouseClick event that broadcasts its position:
 Event<int, int> onMouseClick; <- Defining the event
 onMouseClick.Subscribe(func); <- Adding your observer
 onMouseClick.Invoke(mouseX, mouseY); <- Broadcasting the mouse pos

 The functions in the Event class include:
 - Invoke
 This observable sends a message to all observers subscribed.
 - Subscribe 
 Adds an observer (function) to the list.
 - Unsubscribe
 Removes an observer (function) from the list.
 - UnsubscribeAll
 Removes all observers from the list, useful when destructing an object 
 containing an Event.
**************************************************************************/

#pragma once

#include <list>

/*!***********************************************************************
 \brief 
 Generic event with no parameters, for functions like func()
*************************************************************************/
template <typename... T>
class Event 
{
private:
    typedef void (*func_ptr)();

    struct EventData 
    {
        func_ptr func;
        int priority;
    };
    
    std::list<EventData> subscribers;

public:

    void Invoke() 
    {
        for (typename std::list<EventData>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) 
        {
			(i->func)();
		}
    }

    void Subscribe(func_ptr func, int priority = 0) 
    {
        EventData new_sub{func, priority};

        if (subscribers.size() >= 1) 
        {
            for (typename std::list<EventData>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) 
            {
                if (i->priority <= priority) 
                {
                    subscribers.insert(i, new_sub);
                    return;
                }
            }
        }

		subscribers.push_back(new_sub);
    }

    void Unsubscribe(func_ptr func) 
    {
        for (typename std::list<EventData>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) 
        {
            if (i->func == func) 
            {
                subscribers.erase(i);
                break;
            }
        }
    }

    void UnsubscribeAll() 
    {
        subscribers.clear();
    }
};

/*!***********************************************************************
 \brief
 Generic event with 1 parameter, for functions like func(T1 var)
*************************************************************************/
template <typename T1, typename... T>
class Event<T1, T...> 
{
private:
    typedef void (*func_ptr)(T1);

    struct EventData 
    {
        func_ptr func;
        int priority;
    };
    
    std::list<EventData> subscribers;

public:
    void Invoke(T1 var1) 
    {
        for (typename std::list<EventData>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) 
        {
			(i->func)(var1);
		}
    }

    void Subscribe(func_ptr func, int priority = 0) 
    {
        EventData new_sub{func, priority};

        if (subscribers.size() >= 1) {
            for (typename std::list<EventData>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) 
            {
                if (i->priority <= priority) 
                {
                    subscribers.insert(i, new_sub);
                    return;
                }
            }
        }

		subscribers.push_back(new_sub);
    }

    void Unsubscribe(func_ptr func) 
    {
        for (typename std::list<EventData>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) 
        {
            if (i->func == func) 
            {
                subscribers.erase(i);
                break;
            }
        }
    }

    void UnsubscribeAll() 
    {
        subscribers.clear();
    }
};

/*!***********************************************************************
 \brief
 Generic event with 2 parameters, for functions like func(T1 var, T2 var)
*************************************************************************/
template <typename T1, typename T2, typename... T>
class Event<T1, T2, T...> 
{
private:
    typedef void (*func_ptr)(T1, T2);

    struct EventData 
    {
        func_ptr func;
        int priority;
    };
    
    std::list<EventData> subscribers;

public:
    void Invoke(T1 var1, T2 var2) {
        for (typename std::list<EventData>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) 
        {
			(i->func)(var1, var2);
		}
    }

    void Subscribe(func_ptr func, int priority = 0) {
        EventData new_sub{func, priority};

        if (subscribers.size() >= 1) {
            for (typename std::list<EventData>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) 
            {
                if (i->priority <= priority) {
                    subscribers.insert(i, new_sub);
                    return;
                }
            }
        }

		subscribers.push_back(new_sub);
    }

    void Unsubscribe(func_ptr func) 
    {
        for (typename std::list<EventData>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) 
        {
            if (i->func == func) 
            {
                subscribers.erase(i);
                break;
            }
        }
    }

    void UnsubscribeAll() 
    {
        subscribers.clear();
    }
};

/*!***********************************************************************
 \brief
 Generic event with 3 parameters, for functions like func(T1 var, T2 var, T3 var)
*************************************************************************/
template <typename T1, typename T2, typename T3, typename... T>
class Event<T1, T2, T3, T...> {
private:
    typedef void (*func_ptr)(T1, T2, T3);

    struct EventData 
    {
        func_ptr func;
        int priority;
    };
    
    std::list<EventData> subscribers;

public:
    void Invoke(T1 var1, T2 var2, T3 var3) 
    {
        for (typename std::list<EventData>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) 
        {
			(i->func)(var1, var2, var3);
		}
    }

    void Subscribe(func_ptr func, int priority = 0) 
    {
        EventData new_sub{func, priority};

        if (subscribers.size() >= 1) 
        {
            for (typename std::list<EventData>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) 
            {
                if (i->priority <= priority) 
                {
                    subscribers.insert(i, new_sub);
                    return;
                }
            }
        }

		subscribers.push_back(new_sub);
    }

    void Unsubscribe(func_ptr func) 
    {
        for (typename std::list<EventData>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) 
        {
            if (i->func == func) 
            {
                subscribers.erase(i);
                break;
            }
        }
    }

    void UnsubscribeAll() 
    {
        subscribers.clear();
    }
};

/*!***********************************************************************
 \brief
 Generic event with 4 parameters, for functions like func(T1 var, T2 var, T3 var, T4 var)
*************************************************************************/
template <typename T1, typename T2, typename T3, typename T4, typename... T>
class Event<T1, T2, T3, T4, T...> 
{
private:
    typedef void (*func_ptr)(T1, T2, T3, T4);

    struct EventData 
    {
        func_ptr func;
        int priority;
    };
    
    std::list<EventData> subscribers;

public:
    void Invoke(T1 var1, T2 var2, T3 var3, T4 var4) 
    {
        for (typename std::list<EventData>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) 
        {
			(i->func)(var1, var2, var3, var4);
		}
    }

    void Subscribe(func_ptr func, int priority = 0) 
    {
        EventData new_sub{func, priority};

        if (subscribers.size() >= 1) 
        {
            for (typename std::list<EventData>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) 
            {
                if (i->priority <= priority) {
                    subscribers.insert(i, new_sub);
                    return;
                }
            }
        }

		subscribers.push_back(new_sub);
    }

    void Unsubscribe(func_ptr func) 
    {
        for (typename std::list<EventData>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) 
        {
            if (i->func == func) 
            {
                subscribers.erase(i);
                break;
            }
        }
    }

    void UnsubscribeAll() 
    {
        subscribers.clear();
    }
};
