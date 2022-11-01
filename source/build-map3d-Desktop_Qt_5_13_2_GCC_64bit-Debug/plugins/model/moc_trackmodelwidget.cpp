/****************************************************************************
** Meta object code from reading C++ file 'trackmodelwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../map3d/plugins/model/trackmodelwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trackmodelwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TrackModelWidget_t {
    QByteArrayData data[26];
    char stringdata0[219];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TrackModelWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TrackModelWidget_t qt_meta_stringdata_TrackModelWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "TrackModelWidget"
QT_MOC_LITERAL(1, 17, 14), // "onModelClicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 4), // "type"
QT_MOC_LITERAL(4, 38, 4), // "name"
QT_MOC_LITERAL(5, 43, 7), // "isClick"
QT_MOC_LITERAL(6, 51, 10), // "modelAdded"
QT_MOC_LITERAL(7, 62, 11), // "modelRemove"
QT_MOC_LITERAL(8, 74, 5), // "onPin"
QT_MOC_LITERAL(9, 80, 1), // "t"
QT_MOC_LITERAL(10, 82, 10), // "unTrackAll"
QT_MOC_LITERAL(11, 93, 7), // "isCheck"
QT_MOC_LITERAL(12, 101, 6), // "isDock"
QT_MOC_LITERAL(13, 108, 6), // "height"
QT_MOC_LITERAL(14, 115, 9), // "modelInfo"
QT_MOC_LITERAL(15, 125, 1), // "n"
QT_MOC_LITERAL(16, 127, 3), // "str"
QT_MOC_LITERAL(17, 131, 5), // "close"
QT_MOC_LITERAL(18, 137, 8), // "minimize"
QT_MOC_LITERAL(19, 146, 5), // "isMax"
QT_MOC_LITERAL(20, 152, 13), // "modelPosition"
QT_MOC_LITERAL(21, 166, 8), // "latitude"
QT_MOC_LITERAL(22, 175, 9), // "longitude"
QT_MOC_LITERAL(23, 185, 8), // "altitude"
QT_MOC_LITERAL(24, 194, 10), // "changeSize"
QT_MOC_LITERAL(25, 205, 13) // "setUnTrackAll"

    },
    "TrackModelWidget\0onModelClicked\0\0type\0"
    "name\0isClick\0modelAdded\0modelRemove\0"
    "onPin\0t\0unTrackAll\0isCheck\0isDock\0"
    "height\0modelInfo\0n\0str\0close\0minimize\0"
    "isMax\0modelPosition\0latitude\0longitude\0"
    "altitude\0changeSize\0setUnTrackAll"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TrackModelWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   74,    2, 0x06 /* Public */,
       6,    2,   81,    2, 0x06 /* Public */,
       7,    2,   86,    2, 0x06 /* Public */,
       8,    1,   91,    2, 0x06 /* Public */,
      10,    1,   94,    2, 0x06 /* Public */,
      12,    1,   97,    2, 0x06 /* Public */,
      14,    3,  100,    2, 0x06 /* Public */,
      17,    0,  107,    2, 0x06 /* Public */,
      18,    1,  108,    2, 0x06 /* Public */,
      20,    5,  111,    2, 0x06 /* Public */,
      24,    1,  122,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      25,    1,  125,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool,    3,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    9,   15,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Double, QMetaType::Double, QMetaType::Double,    9,   15,   21,   22,   23,
    QMetaType::Void, QMetaType::Bool,    9,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   11,

       0        // eod
};

void TrackModelWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TrackModelWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onModelClicked((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 1: _t->modelAdded((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->modelRemove((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->onPin((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->unTrackAll((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->isDock((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->modelInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: _t->close(); break;
        case 8: _t->minimize((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->modelPosition((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5]))); break;
        case 10: _t->changeSize((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->setUnTrackAll((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TrackModelWidget::*)(QString , QString , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackModelWidget::onModelClicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TrackModelWidget::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackModelWidget::modelAdded)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TrackModelWidget::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackModelWidget::modelRemove)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TrackModelWidget::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackModelWidget::onPin)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TrackModelWidget::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackModelWidget::unTrackAll)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TrackModelWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackModelWidget::isDock)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TrackModelWidget::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackModelWidget::modelInfo)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (TrackModelWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackModelWidget::close)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (TrackModelWidget::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackModelWidget::minimize)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (TrackModelWidget::*)(QString , QString , double , double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackModelWidget::modelPosition)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (TrackModelWidget::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TrackModelWidget::changeSize)) {
                *result = 10;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TrackModelWidget::staticMetaObject = { {
    &QQuickWidget::staticMetaObject,
    qt_meta_stringdata_TrackModelWidget.data,
    qt_meta_data_TrackModelWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TrackModelWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TrackModelWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TrackModelWidget.stringdata0))
        return static_cast<void*>(this);
    return QQuickWidget::qt_metacast(_clname);
}

int TrackModelWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void TrackModelWidget::onModelClicked(QString _t1, QString _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TrackModelWidget::modelAdded(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TrackModelWidget::modelRemove(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TrackModelWidget::onPin(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void TrackModelWidget::unTrackAll(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TrackModelWidget::isDock(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void TrackModelWidget::modelInfo(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void TrackModelWidget::close()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void TrackModelWidget::minimize(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void TrackModelWidget::modelPosition(QString _t1, QString _t2, double _t3, double _t4, double _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void TrackModelWidget::changeSize(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
