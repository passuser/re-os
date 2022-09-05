#include "list.h"
#include "interrupt.h"
#include "stdint.h"
void list_init(struct list* list){
    list->head->previous = NULL;
    list->head->next = &list->tail;
    list->tail->previous = &list->head;
    list->tail->next = NULL;
};

//将链表元素elem插入在before之前
void list_insert_before(struct list_elem* before,struct list_elem* elem){
    enum intr_status old_status = intr_disable();
    elem->previous = before->previous;
    elem->next = before;
    before->previous = elem;
    intr_set_status(old_status);
};

//添加元素到列表队首
void list_push(struct list* plist,struct list_elem* elem){
    list_insert_before(plist->head->next,elem);
};

//追加元素到链表队尾
void list_append(struct list* plist,struct list_elem* elem){
    list_insert_before(&plist->tail,elem);
};

//删除链表元素
void list_remove(struct list_elem* pelem){
    enum intr_status old_status = intr_disable();
    pelem->previous->next = pelem->next;
    pelem->next->previous = pelem->previous;
    intr_set_status(old_status);
};

//将链表第一个元素出栈
struct list_elem* list_pop(struct list* plist){
    struct list_elem* elem = plist->head->next;
    list_remove(elem);
};

//链表中查找元素
int elem_find(struct list* plist,struct list_elem* obj_elem){
    struct list_elem* elem = plist->head->next;
    while(elem != &plist->tail){
        if(elem == obj_elem){
          return true;
        }elem = elem->next;
    };
    return false;
};

//将链表中每个元素传给回调函数，符合返回指针，否则返回NULL
struct list_elem* list_traversal(struct list* plist,function func,int arg){
    struct list_elem* elem = plist->head->next;
    if(list_empty(plist)){
      return NULL;
    }
    while(elem != &plist->tail){
      if(func(elem,arg)){
        return elem;
      }elem = elem->next;
    }
    return NULL;
};

//返回链表长度
uint32_t list_len(struct list* plist){
    struct list_elem* elem = plist->head->next;
    uint32_t length = 0;
    while(elem != &plist->tail){
      length++;
      elem = elem->next;
    };
    return length;
};

//判断链表是否非空
int list_empty(struct list* plist){  
    return (plist->head->next == &plist->tail ? true : false);
};
