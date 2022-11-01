/****************************************************************************
** Meta object code from reading C++ file 'map3dwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../map3d/map3dlib/map3dwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'map3dwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MousePicker_t {
    QByteArrayData data[9];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MousePicker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MousePicker_t qt_meta_stringdata_MousePicker = {
    {
QT_MOC_LITERAL(0, 0, 11), // "MousePicker"
QT_MOC_LITERAL(1, 12, 15), // "currentWorldPos"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 10), // "osg::Vec3d"
QT_MOC_LITERAL(4, 40, 3), // "pos"
QT_MOC_LITERAL(5, 44, 15), // "mousePressEvent"
QT_MOC_LITERAL(6, 60, 12), // "QMouseEvent*"
QT_MOC_LITERAL(7, 73, 5), // "event"
QT_MOC_LITERAL(8, 79, 5) // "frame"

    },
    "MousePicker\0currentWorldPos\0\0osg::Vec3d\0"
    "pos\0mousePressEvent\0QMouseEvent*\0event\0"
    "frame"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MousePicker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       5,    1,   32,    2, 0x06 /* Public */,
       8,    0,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,

       0        // eod
};

void MousePicker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MousePicker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->currentWorldPos((*reinterpret_cast< osg::Vec3d(*)>(_a[1]))); break;
        case 1: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 2: _t->frame(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MousePicker::*)(osg::Vec3d );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MousePicker::currentWorldPos)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MousePicker::*)(QMouseEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MousePicker::mousePressEvent)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MousePicker::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MousePicker::frame)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MousePicker::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MousePicker.data,
    qt_meta_data_MousePicker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MousePicker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MousePicker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MousePicker.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "osgGA::GUIEventHandler"))
        return static_cast< osgGA::GUIEventHandler*>(this);
    return QObject::qt_metacast(_clname);
}

int MousePicker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void MousePicker::currentWorldPos(osg::Vec3d _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MousePicker::mousePressEvent(QMouseEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MousePicker::frame()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
struct qt_meta_stringdata_Map3dWidget_t {
    QByteArrayData data[25];
    char stringdata0[313];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Map3dWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Map3dWidget_t qt_meta_stringdata_Map3dWidget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Map3dWidget"
QT_MOC_LITERAL(1, 12, 7), // "setZoom"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 4), // "home"
QT_MOC_LITERAL(4, 26, 11), // "typeChanged"
QT_MOC_LITERAL(5, 38, 9), // "getMapSRS"
QT_MOC_LITERAL(6, 48, 33), // "const osgEarth::SpatialRefere..."
QT_MOC_LITERAL(7, 82, 10), // "goPosition"
QT_MOC_LITERAL(8, 93, 8), // "latitude"
QT_MOC_LITERAL(9, 102, 9), // "longitude"
QT_MOC_LITERAL(10, 112, 5), // "range"
QT_MOC_LITERAL(11, 118, 26), // "setObjectInfoWidgetVisible"
QT_MOC_LITERAL(12, 145, 8), // "bVisible"
QT_MOC_LITERAL(13, 154, 19), // "setSelectedAirplane"
QT_MOC_LITERAL(14, 174, 32), // "osgEarth::Annotation::ModelNode*"
QT_MOC_LITERAL(15, 207, 8), // "airplane"
QT_MOC_LITERAL(16, 216, 7), // "onFrame"
QT_MOC_LITERAL(17, 224, 19), // "saveCurrentPosition"
QT_MOC_LITERAL(18, 244, 4), // "name"
QT_MOC_LITERAL(19, 249, 13), // "mouseWorldPos"
QT_MOC_LITERAL(20, 263, 10), // "osg::Vec3d"
QT_MOC_LITERAL(21, 274, 3), // "pos"
QT_MOC_LITERAL(22, 278, 15), // "onMapPressEvent"
QT_MOC_LITERAL(23, 294, 12), // "QMouseEvent*"
QT_MOC_LITERAL(24, 307, 5) // "event"

    },
    "Map3dWidget\0setZoom\0\0home\0typeChanged\0"
    "getMapSRS\0const osgEarth::SpatialReference*\0"
    "goPosition\0latitude\0longitude\0range\0"
    "setObjectInfoWidgetVisible\0bVisible\0"
    "setSelectedAirplane\0"
    "osgEarth::Annotation::ModelNode*\0"
    "airplane\0onFrame\0saveCurrentPosition\0"
    "name\0mouseWorldPos\0osg::Vec3d\0pos\0"
    "onMapPressEvent\0QMouseEvent*\0event"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Map3dWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x0a /* Public */,
       3,    0,   72,    2, 0x0a /* Public */,
       4,    1,   73,    2, 0x0a /* Public */,
       5,    0,   76,    2, 0x0a /* Public */,
       7,    3,   77,    2, 0x0a /* Public */,
      11,    1,   84,    2, 0x0a /* Public */,
      13,    1,   87,    2, 0x0a /* Public */,
      16,    0,   90,    2, 0x0a /* Public */,
      17,    1,   91,    2, 0x08 /* Private */,
      19,    1,   94,    2, 0x08 /* Private */,
      22,    1,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    0x80000000 | 6,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double,    8,    9,   10,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void, 0x80000000 | 23,   24,

       0        // eod
};

void Map3dWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Map3dWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setZoom((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->home(); break;
        case 2: _t->typeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: { const osgEarth::SpatialReference* _r = _t->getMapSRS();
            if (_a[0]) *reinterpret_cast< const osgEarth::SpatialReference**>(_a[0]) = std::move(_r); }  break;
        case 4: _t->goPosition((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 5: _t->setObjectInfoWidgetVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setSelectedAirplane((*reinterpret_cast< osgEarth::Annotation::ModelNode*(*)>(_a[1]))); break;
        case 7: _t->onFrame(); break;
        case 8: _t->saveCurrentPosition((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->mouseWorldPos((*reinterpret_cast< osg::Vec3d(*)>(_a[1]))); break;
        case 10: _t->onMapPressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Map3dWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_Map3dWidget.data,
    qt_meta_data_Map3dWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Map3dWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Map3dWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Map3dWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Map3dWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
