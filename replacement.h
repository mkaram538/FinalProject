// File: replacement.h
// Author(s): Matthew Karam, Saul Diosdado
// Date: 04/23/2020
// Section: 508
// E-mail: sauldiosdado@tamu.edu
// Description:
// Implements both LFU and LRU replacement policies.

#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include "Cache.h"
#include "Memory.h"
#include "Operations.h"
#include <map>
#include <iostream>
#include <string>

using namespace std;

struct Node
{
  int index;
  int uses;
  Node* next = nullptr;
  //Node* prev = nullptr;

  Node (int line_index){
    index = line_index;
    uses = 0;
  }
};

class replacement
{
private:
  Node* head = nullptr;
  Node* tail = nullptr;

public:
  replacement (int num_lines){
    for (int i = 0; i < num_lines; i++){
      insert(i);
    }
  }

  void insert(int index){
    Node* current = new Node(index);
    if (!head){
      head = current;
      tail = current;
    }
    else if (!head->next){
      tail = current;
      head->next = tail;
      //tail->prev = head;
    } else {
      //current->prev = tail;
      tail->next = current;
      tail = current;
    }
  }

  void access(int search){
    Node* current = head;
    if (head == tail){//happens when size == 1
      head->uses++;
      return;
    } else if (head->index == search){
      head = current->next;
      current->next = nullptr;
      //current->prev = tail;
      tail->next = current;
      tail = current;
      current->uses++;
      return;
    } else if (tail->index == search){
      tail->uses++;
      return;
    }
    while (current->next->index != search){
      current = current->next;
    }
    current->next->uses++;
    //current->next->prev = current->prev;
    //current->prev->next = current->next;
    current->next = current->next->next;
    //current->prev = tail;
    current->next = nullptr;
    tail->next = current;
    tail = current;
  }

  // Used when a line is evicted
  void clear(int search){
    Node* current = head;
    if (head->index == search){
      current->uses = 0;
      return;
    } else if (tail->index == search){
      current->uses = 0;
      return;
    }
    while (current->index != search){
      current = current->next;
    }
    current->uses = 0;
  }

  // TESTED, the way the linked_list is set up, the only way that there
  // would be equal values is if it was in the beginning, and those lines
  // are already in order based on index
  int LRU() {
    return head->index;
  }

  // Finds the line with the smallest number of uses, favoring the line with
  // the lower index.
  int LFU() {
    Node* current = head;
    int min_occur = current->uses;
    int min_index = 0;
    int index = 0;
    while (current->next){
      if (current->uses < min_occur){
        min_index = index;
      } else if (current->uses == min_occur && index < min_index){
        min_index = index;
      }
      current = current->next;
      index++;
    }
    return min_index;
  }
};

#endif
