/****************************************************************************
** Meta object code from reading C++ file 'toolbarwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../map3d/map3dlib/toolbarwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'toolbarwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ToolBarWidget_t {
    QByteArrayData data[19];
    char stringdata0[182];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ToolBarWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ToolBarWidget_t qt_meta_stringdata_ToolBarWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ToolBarWidget"
QT_MOC_LITERAL(1, 14, 13), // "onItemClicked"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 8), // "Category"
QT_MOC_LITERAL(4, 38, 8), // "category"
QT_MOC_LITERAL(5, 47, 4), // "name"
QT_MOC_LITERAL(6, 52, 7), // "ischeck"
QT_MOC_LITERAL(7, 60, 10), // "changeSize"
QT_MOC_LITERAL(8, 71, 1), // "t"
QT_MOC_LITERAL(9, 73, 7), // "isclose"
QT_MOC_LITERAL(10, 81, 5), // "onPin"
QT_MOC_LITERAL(11, 87, 9), // "itemAdded"
QT_MOC_LITERAL(12, 97, 6), // "isDock"
QT_MOC_LITERAL(13, 104, 6), // "height"
QT_MOC_LITERAL(14, 111, 16), // "onGetItemClicked"
QT_MOC_LITERAL(15, 128, 13), // "setSizeWidget"
QT_MOC_LITERAL(16, 142, 11), // "getItemName"
QT_MOC_LITERAL(17, 154, 11), // "getItemIcon"
QT_MOC_LITERAL(18, 166, 15) // "getItemCategory"

    },
    "ToolBarWidget\0onItemClicked\0\0Category\0"
    "category\0name\0ischeck\0changeSize\0t\0"
    "isclose\0onPin\0itemAdded\0isDock\0height\0"
    "onGetItemClicked\0setSizeWidget\0"
    "getItemName\0getItemIcon\0getItemCategory"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ToolBarWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   69,    2, 0x06 /* Public */,
       7,    1,   76,    2, 0x06 /* Public */,
       9,    0,   79,    2, 0x06 /* Public */,
      10,    1,   80,    2, 0x06 /* Public */,
      11,    1,   83,    2, 0x06 /* Public */,
      12,    1,   86,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    3,   89,    2, 0x0a /* Public */,
      15,    1,   96,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      16,    0,   99,    2, 0x02 /* Public */,
      17,    0,  100,    2, 0x02 /* Public */,
      18,    0,  101,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, QMetaType::Bool,    4,    5,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Int,   13,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool,    4,    5,    6,
    QMetaType::Void, QMetaType::Bool,    8,

 // methods: parameters
    QMetaType::QVariant,
    QMetaType::QVariant,
    QMetaType::QVariant,

       0        // eod
};

void ToolBarWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ToolBarWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onItemClicked((*reinterpret_cast< Category(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 1: _t->changeSize((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->isclose(); break;
        case 3: _t->onPin((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->itemAdded((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->isDock((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onGetItemClicked((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 7: _t->setSizeWidget((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: { QVariant _r = _t->getItemName();
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = std::move(_r); }  break;
        case 9: { QVariant _r = _t->getItemIcon();
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = std::move(_r); }  break;
        case 10: { QVariant _r = _t->getItemCategory();
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ToolBarWidget::*)(Category , QString , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ToolBarWidget::onItemClicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ToolBarWidget::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ToolBarWidget::changeSize)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ToolBarWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ToolBarWidget::isclose)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ToolBarWidget::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ToolBarWidget::onPin)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ToolBarWidget::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ToolBarWidget::itemAdded)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ToolBarWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ToolBarWidget::isDock)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ToolBarWidget::staticMetaObject = { {
    &QQuickWidget::staticMetaObject,
    qt_meta_stringdata_ToolBarWidget.data,
    qt_meta_data_ToolBarWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ToolBarWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ToolBarWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ToolBarWidget.stringdata0))
        return static_cast<void*>(this);
    return QQuickWidget::qt_metacast(_clname);
}

int ToolBarWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void ToolBarWidget::onItemClicked(Category _t1, QString _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ToolBarWidget::changeSize(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ToolBarWidget::isclose()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ToolBarWidget::onPin(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ToolBarWidget::itemAdded(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ToolBarWidget::isDock(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
