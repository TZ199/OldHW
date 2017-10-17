//1:22  suit guy

#ifndef priority_queue_h_
#define priority_queue_h_

#include <cassert>
#include <iostream>
#include <vector>

template <class T> class priority_queue {
private:
  std::vector<T> m_heap;

public:
  priority_queue() {}

  priority_queue(std::vector<T> const &values) {
    for (int i = 0; i < values.size(); i++) {
      push(values[i]);
    }
  }

  const T &top() const {
    assert(!m_heap.empty());
    return m_heap[0];
  }

  void push(const T &entry) {
    m_heap.push_back(entry);
    int curidx = m_heap.size() - 1;
    int paridx = (curidx - 1) / 2;
    while (m_heap[curidx] < m_heap[paridx]) {
      std::swap(m_heap[curidx], m_heap[paridx]);
      curidx = paridx;
      paridx = (curidx - 1) / 2;
    }
  }

  void pop() {
    assert(!m_heap.empty());
    m_heap[0] = m_heap[m_heap.size() - 1];
    m_heap.pop_back();
    int curidx = 0;
    int childidx;
    if (m_heap[1] < m_heap[2]) {
      childidx = 1;
    } else {
      childidx = 2;
    }
    while (m_heap[curidx] > m_heap[childidx] &&
           (2 * curidx + 1) < m_heap.size()) {
      std::swap(m_heap[curidx], m_heap[childidx]);
      curidx = childidx;
      if (m_heap[2 * curidx + 1] < m_heap[2 * curidx + 2]) {
        childidx = 2 * curidx + 1;
      } else {
        childidx = 2 * curidx + 2;
      }
    }
  }

  int size() { return m_heap.size(); }
  bool empty() { return m_heap.empty(); }

  //  The following three functions are used for debugging.

  //  Check to see that internally the heap property is realized.
  bool check_heap() { return this->check_heap(this->m_heap); }

  //  Check an external vector to see that the heap property is realized.
  bool check_heap(const std::vector<T> &heap) {
    if (heap.size() == 0)
      return true;

    for (int i = heap.size() - 1; ((i - 1) / 2) >= 0; i--) {
      if (heap[i] < heap[(i - 1) / 2]) {
        return false;
      }
    }

    return true;
  }

  //  A utility to print the contents of the heap.  Use it for debugging.
  void print_heap(std::ostream &ostr) {
    for (unsigned int i = 0; i < m_heap.size(); ++i)
      ostr << i << ": " << m_heap[i] << std::endl;
  }
};

template <class T> void perculate_down(std::vector<T> &v, int begin, int end) {
  T curparent = v[begin];
  int curidx = begin;
  int childidx = 2 * curidx + 1;
  while (childidx < end) {
    if (childidx + 1 < end && v[childidx] < v[childidx + 1]) {
      childidx = childidx + 1;
    }
    if (curparent < v[childidx]) {
      std::swap(v[curidx], v[childidx]);
      curidx = childidx;
      childidx = 2 * curidx + 1;
    } else {
      break;
    }
  }
}



template <class T> void perculate_down2(std::vector<T> &m_heap, int begin, int end) {
  int curidx = begin;
  int childidx;
  if (m_heap[begin+1] < m_heap[begin+2]) {
    childidx = begin+1;
  } else {
    childidx = begin+2;
  }
  while (m_heap[curidx] > m_heap[childidx] &&
         (2 * curidx + 1) < m_heap.size()) {
    std::swap(m_heap[curidx], m_heap[childidx]);
    curidx = childidx;
    if (m_heap[2 * curidx + 1] < m_heap[2 * curidx + 2]) {
      childidx = 2 * curidx + 1;
    } else {
      childidx = 2 * curidx + 2;
    }
  }
  }



template <class T> void heap_sort(std::vector<T> &v) {
  priority_queue<T> temp(v);
  v.clear();
  while(!temp.empty())
  {

    v.push_back(temp.top());
    temp.pop();

  }




  }



#endif
