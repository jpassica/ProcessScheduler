#pragma once
template<class T>
class KeyNode 
{

private:
    T Item;
    KeyNode<T>* next;
    int key;

public: 
    KeyNode() 
    {
        next = nullptr;
    }
    void setItem(T data)
    {
        Item = data;
    }
    void setNext(KeyNode<T>* N)
    {
        next = N;
    }
    void setKey(int k)
    {
        key = k;
    }
    T getItem()
    {
        return Item;
    }
    KeyNode<T>* getNext()
    {
        return next;
    }
    int getKey()
    {
        return key;
    }
};

