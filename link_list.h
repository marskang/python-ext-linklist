#ifndef PY_LINK_LIST_H
#define PY_LINK_LIST_H

#include <Python.h>

typedef struct PyLinkList_Node {
    PyObject_HEAD
    Py_ssize_t count;
    PyObject* content;
    struct PyLinkList_Node* cursor;
    struct PyLinkList_Node* next;
    struct PyLinkList_Node* tail;
} PyLinkList_Node;

int pyLinkList_init(PyLinkList_Node* self);
void pyLinkList_dealloc(PyLinkList_Node* self);
void pyLinkList_append(PyLinkList_Node* self, PyObject* obj);
PyObject* pyLinkList_iternext(PyLinkList_Node* self);
PyObject* pyLinkList_getiter(PyLinkList_Node* self);
PyObject* pyLinkList_item(PyLinkList_Node* self, Py_ssize_t index);
Py_ssize_t PyLinkList_count(PyLinkList_Node* self);

#endif