#include "link_list.h"


static PyMethodDef pyLinkList_methods[] = {
    {"append", (PyCFunction)pyLinkList_append, METH_O, "append item"},
    {NULL},
};

static PySequenceMethods linklist_as_sequence = {
    (lenfunc)PyLinkList_count,                       /* sq_length */
    0,                                               /* sq_concat */
    0,                                               /* sq_repeat */
    (ssizeargfunc)pyLinkList_item,                   /* sq_item */
    0,                                               /* sq_slice */
    0,                                               /* sq_ass_item */
    0,                                               /* sq_ass_slice */
    0,                                               /* sq_contains */
    0,                                               /* sq_inplace_concat */
    0,                                               /* sq_inplace_repeat */
};


PyTypeObject pyLinkList_NodeType = {
        PyVarObject_HEAD_INIT(NULL, 0)
        "PyLinkList",                                   /* tp_name */
        sizeof(PyLinkList_Node),                        /* tp_basicsize */
        0,                                              /* tp_itemsize */
        (destructor)pyLinkList_dealloc,                 /* tp_dealloc */
        0,                                              /* tp_print */
        0,                                              /* tp_getattr */
        0,                                              /* tp_setattr */
        0,                                              /* tp_reserved */
        0,                                              /* tp_repr */
        0,                                              /* tp_as_number */
        &linklist_as_sequence,                          /* tp_as_sequence */
        0,                                              /* tp_as_mapping */
        0,                                              /* tp_hash */
        0,                                              /* tp_call */
        0,                                              /* tp_str */
        0,                                              /* tp_getattro */
        0,                                              /* tp_setattro */
        0,                                              /* tp_as_buffer */
        Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,         /* tp_flags */
        0,                                              /* tp_doc */
        0,                                              /* tp_traverse */
        0,                                              /* tp_clear */
        0,                                              /* tp_richcompare */
        0,                                              /* tp_weaklistoffset */
        (getiterfunc)pyLinkList_getiter,                /* tp_iter */
        (iternextfunc)pyLinkList_iternext,              /* tp_iternext */
        pyLinkList_methods,                             /* tp_methods */
        0,                                              /* tp_members */
        0,                                              /* tp_getset */
        0,                                              /* tp_base */
        0,                                              /* tp_dict */
        0,                                              /* tp_descr_get */
        0,                                              /* tp_descr_set */
        0,                                              /* tp_dictoffset */
        (initproc)pyLinkList_init,                      /* tp_init */
        0,                                              /* tp_alloc */
        0,                                              /* tp_new */
        0                                               /* tp_free */
};

Py_ssize_t PyLinkList_count(PyLinkList_Node* self) {
    return self->count;
}

int pyLinkList_init(PyLinkList_Node* self) {
    self->count = 0;
    self->next = NULL;
    self->content = Py_BuildValue("s", "link list head");
    self->tail = self;
    self->cursor = self;
    return 0;
}

void pyLinkList_dealloc(PyLinkList_Node* self) {
    //内存回收
    PyLinkList_Node* p = self->next;
    PyLinkList_Node* q = NULL;
    while(p) {
        q = p->next;
        Py_XDECREF(p);
        p = q;
    }
    Py_CLEAR(self);
}

static PyObject *indexerr = NULL;

PyObject* pyLinkList_item(PyLinkList_Node* self, Py_ssize_t index) {
    if (index < 0 || index >= self->count) {
        if (indexerr == NULL) {
            indexerr = PyString_FromString("link list index out of range");
            if (indexerr == NULL)
                return NULL;
        }
        //返回数组越界异常
        PyErr_SetObject(PyExc_IndexError, indexerr);
        return NULL;
    }
    Py_ssize_t start = 0;
    //过滤头节点
    PyLinkList_Node* p = self->next;
    while(p) {
        if (start == index) {
            Py_INCREF(p->content);
            return p->content;
        }
        p = p->next;
        start++;
    }
    return NULL;
}

void pyLinkList_append(PyLinkList_Node* self, PyObject* obj) {
    PyLinkList_Node* node = (PyLinkList_Node*) (pyLinkList_NodeType.tp_alloc(&pyLinkList_NodeType, 0));
    if (!node) {
        return;
    }
    Py_INCREF(self);
    Py_INCREF(self->content);
    node->content = obj;
    node->next = NULL;
    self->tail->next = node;
    self->tail = node;
    self->count++;
}

PyObject* pyLinkList_getiter(PyLinkList_Node* self) {
    Py_INCREF(self);
    return (PyObject*)self;
}

PyObject* pyLinkList_iternext(PyLinkList_Node* self) {
    PyLinkList_Node* next;
    if(!self->next || !self->cursor) {
        //将游标重置
        self->cursor = self;
        return NULL;
    }
    if (self == self->cursor) {
        // 过滤头节点
        self->cursor = self->cursor->next;
        next = self->next;
    } else {
        next = self->cursor;
    }
    self->cursor = self->cursor->next;
    return next->content;
}

static PyMethodDef pyLinkList_module_methods[] = {
    {NULL},
};

PyMODINIT_FUNC initlinklist(void) {
    pyLinkList_NodeType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&pyLinkList_NodeType) < 0) {
        return;
    }
    PyObject* m = Py_InitModule3("linklist", pyLinkList_module_methods, "test");
    Py_INCREF(&pyLinkList_NodeType);
    PyModule_AddObject(m, "LinkList", (PyObject*)&pyLinkList_NodeType);
}

