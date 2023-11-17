#pragma once
#include <memory>
#include <list>

template <typename T>
class Queue {
private:
    int qIdx;
    std::list<T>* container;
public:
    Queue() {
        qIdx = 0;
        container = new list<T>;
    }
    void push(const T& data) {
        container->push_back(data);
    }
    void push(T&& data) {
        container->emplace_back(data);
    }
    T front() {
        return container->front();
    }
    void pop() {
        if(container->size() > 0){
            container->pop_front();
        }
    }
    bool empty()
    {
        return container->size() == 0;
    }
    std::list<T>* getContainer()
    {
        return container;
    } 
    ~Queue()
    {
        delete container;
    }
};
// using linked list from scratch
template<class T>
class Node {
public:
    Node(T pass) : data(pass), next(nullptr), prev(nullptr) {}
    Node<T>* next;
    Node<T>* prev;
    T data;

    ~Node()
    {
        next = nullptr;
        prev = nullptr;
        data = NULL;
    }
};

template<class T>
class LList {
    Node<T>* head;
    Node<T>* tail;
    size_t node_count;
public:
    LList() : head(nullptr), tail(nullptr), node_count(0) {}
    LList(const LList<T>&) = delete; // no shallow copies
    LList<T>& operator=(const LList<T>& list) = delete; // no shallow copies

    // Doubly implementation
    void addNode(const T& data)
    {
        if(head == nullptr)
        {
            head = new Node<T>(data);
            tail = head;
        }
        else if(tail == head)
        {
            tail = new Node<T>(data);
            tail->prev = head;
            head->next = tail;
        }
        else
        {
            auto* temp = new Node<T>(data);
            tail->next = temp;
            temp->prev = tail;
            tail = temp;
        }
        node_count ++;
    }
    void addFront(const T& data)
    {
        if(head == nullptr)
        {
            head = new Node<T>(data);
            tail = head;
        }
        else
        {
            auto* temp = new Node<T>(data);
            
            temp->next = head;
            head->prev = temp;

            head = temp;
        }
        node_count ++;
    }
    void deletePtr(Node<T>* ptr)
    {
        if(!node_count || ptr == nullptr)
            return;
        
        if(head == ptr)
            head = ptr->next;
        
        if(ptr->next != nullptr)
            ptr->next->prev = ptr->prev;
        
        if(ptr->prev != nullptr)
            ptr->prev->next = ptr->next;

        auto* i = head;
        if(i != nullptr)
        {
            for(; i->next != nullptr; i = i->next) {}
        }
        tail = i;
        node_count --;
    }
    Node<T>* getHead()
    {
        return head;
    }
    Node<T>* getTail()
    {
        return tail;
    }
    size_t size()
    {
        return node_count;
    }
    void PrintList()
    {
        cout << "[";
        for(auto i = head; i != nullptr; i = i->next)
        {
            cout << i->data;
            if(i->next != nullptr) { cout << ", "; }
        }
        cout << "]" << endl;
    }
    ~LList()
    {
        auto i = head;
        while(i != nullptr)
        {
            auto next = i->next;
            delete i;
            i = next;
        }
    }
};

template <typename T>
class ListQueue {
public:
    std::shared_ptr<LList<T>> container;
    ListQueue()
    {
        container = std::make_shared<LList<T>>();
    }
    ListQueue(const LList<T>& other)  {
        container = other.container;
    }
    ListQueue<T>& operator=(const ListQueue<T>& other) {
        container = other.container;
    }
    void push(const T& data)
    {
        container->addNode(data);
    }
    const T front()
    {
        return container->getHead()->data;
    }
    void pop()
    {
        container->deletePtr(container->getHead());
    }
    LList<T> getContainer() {
        return container;
    }
    const size_t size() const
    {
        return container->size();
    }

};
