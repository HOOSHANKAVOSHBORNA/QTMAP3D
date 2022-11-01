/****************************************************************************
** Meta object code from reading C++ file 'locationwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../map3d/map3dlib/locationwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'locationwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LocationWidget_t {
    QByteArrayData data[23];
    char stringdata0[207];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LocationWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LocationWidget_t qt_meta_stringdata_LocationWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "LocationWidget"
QT_MOC_LITERAL(1, 15, 10), // "goPosition"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 8), // "latitude"
QT_MOC_LITERAL(4, 36, 9), // "longitude"
QT_MOC_LITERAL(5, 46, 5), // "range"
QT_MOC_LITERAL(6, 52, 12), // "saveLocation"
QT_MOC_LITERAL(7, 65, 4), // "name"
QT_MOC_LITERAL(8, 70, 12), // "savePosition"
QT_MOC_LITERAL(9, 83, 3), // "str"
QT_MOC_LITERAL(10, 87, 1), // "x"
QT_MOC_LITERAL(11, 89, 1), // "y"
QT_MOC_LITERAL(12, 91, 10), // "openWidget"
QT_MOC_LITERAL(13, 102, 1), // "a"
QT_MOC_LITERAL(14, 104, 1), // "b"
QT_MOC_LITERAL(15, 106, 1), // "c"
QT_MOC_LITERAL(16, 108, 14), // "changePosition"
QT_MOC_LITERAL(17, 123, 8), // "location"
QT_MOC_LITERAL(18, 132, 16), // "onCurrentClicked"
QT_MOC_LITERAL(19, 149, 5), // "close"
QT_MOC_LITERAL(20, 155, 15), // "currentLocation"
QT_MOC_LITERAL(21, 171, 16), // "setMousePosition"
QT_MOC_LITERAL(22, 188, 18) // "setCurrentLocation"

    },
    "LocationWidget\0goPosition\0\0latitude\0"
    "longitude\0range\0saveLocation\0name\0"
    "savePosition\0str\0x\0y\0openWidget\0a\0b\0"
    "c\0changePosition\0location\0onCurrentClicked\0"
    "close\0currentLocation\0setMousePosition\0"
    "setCurrentLocation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LocationWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   64,    2, 0x06 /* Public */,
       6,    1,   71,    2, 0x06 /* Public */,
       8,    4,   74,    2, 0x06 /* Public */,
      12,    3,   83,    2, 0x06 /* Public */,
      16,    1,   90,    2, 0x06 /* Public */,
      18,    1,   93,    2, 0x06 /* Public */,
      19,    0,   96,    2, 0x06 /* Public */,
      20,    2,   97,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      21,    1,  102,    2, 0x0a /* Public */,
      22,    2,  105,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float,    3,    4,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Double, QMetaType::Double, QMetaType::Double,    9,   10,   11,    5,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool,   13,   14,   15,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,

       0        // eod
};

void LocationWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LocationWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->goPosition((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 1: _t->saveLocation((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->savePosition((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 3: _t->openWidget((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 4: _t->changePosition((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->onCurrentClicked((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->close(); break;
        case 7: _t->currentLocation((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 8: _t->setMousePosition((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->setCurrentLocation((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LocationWidget::*)(float , float , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LocationWidget::goPosition)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LocationWidget::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LocationWidget::saveLocation)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (LocationWidget::*)(QString , double , double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LocationWidget::savePosition)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (LocationWidget::*)(bool , bool , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LocationWidget::openWidget)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (LocationWidget::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LocationWidget::changePosition)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (LocationWidget::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LocationWidget::onCurrentClicked)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (LocationWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LocationWidget::close)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (LocationWidget::*)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LocationWidget::currentLocation)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LocationWidget::staticMetaObject = { {
    &QQuickWidget::staticMetaObject,
    qt_meta_stringdata_LocationWidget.data,
    qt_meta_data_LocationWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LocationWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LocationWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LocationWidget.stringdata0))
        return static_cast<void*>(this);
    return QQuickWidget::qt_metacast(_clname);
}

int LocationWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void LocationWidget::goPosition(float _t1, float _t2, float _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LocationWidget::saveLocation(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LocationWidget::savePosition(QString _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void LocationWidget::openWidget(bool _t1, bool _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void LocationWidget::changePosition(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void LocationWidget::onCurrentClicked(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void LocationWidget::close()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void LocationWidget::currentLocation(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
