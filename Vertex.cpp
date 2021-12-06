#include "Vertex.h"

Vertex::Vertex(){
    m_key = -1;
    m_size = -1;
    m_pEHead = nullptr;
    m_pNext = nullptr;
}
Vertex::Vertex(int key){
    m_key = key;
    m_size = 0;
    m_pEHead = nullptr;
    m_pNext = nullptr;
}
Vertex::~Vertex(){

}

void Vertex::SetNext(Vertex* pNext){
    m_pNext = pNext;
    return;
}

int Vertex::GetKey() const{
    return m_key;
}

Vertex* Vertex::GetNext() const{
    return m_pNext;
}

int Vertex::Size() const{
    return m_size;
}


void Vertex::AddEdge(int edgeKey, int weight){
    Edge* newEdge = new Edge(edgeKey, weight);

    if(m_pEHead == nullptr){
        m_pEHead = newEdge;
        m_size++;
        return;
    }

    Edge* temp = m_pEHead;
    while(temp->GetNext()){
        temp = temp->GetNext();
    }
    temp->SetNext(newEdge);
    m_size++;
    return;
}

Edge* Vertex::GetHeadOfEdge() const{
    return m_pEHead;
}

void Vertex::Clear(){
    if(m_pEHead == nullptr){
        return;
    }

    Edge* temp = m_pEHead;
    while(m_pEHead){
        m_pEHead = m_pEHead->GetNext();
        delete temp;
        temp = m_pEHead;
    }

    return;
}