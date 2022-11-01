/****************************************************************************
** Meta object code from reading C++ file 'cameramanipulatorwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../map3d/map3dlib/cameramanipulatorwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cameramanipulatorwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CameraManipulatorWidget_t {
    QByteArrayData data[29];
    char stringdata0[381];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CameraManipulatorWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CameraManipulatorWidget_t qt_meta_stringdata_CameraManipulatorWidget = {
    {
QT_MOC_LITERAL(0, 0, 23), // "CameraManipulatorWidget"
QT_MOC_LITERAL(1, 24, 11), // "zoomChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 7), // "newZoom"
QT_MOC_LITERAL(4, 45, 13), // "upDownChanged"
QT_MOC_LITERAL(5, 59, 9), // "newUpDown"
QT_MOC_LITERAL(6, 69, 16), // "leftRightChanged"
QT_MOC_LITERAL(7, 86, 12), // "nemLeftRight"
QT_MOC_LITERAL(8, 99, 12), // "pitchChanged"
QT_MOC_LITERAL(9, 112, 10), // "newBearing"
QT_MOC_LITERAL(10, 123, 11), // "headChanged"
QT_MOC_LITERAL(11, 135, 7), // "newTilt"
QT_MOC_LITERAL(12, 143, 11), // "homeClicked"
QT_MOC_LITERAL(13, 155, 9), // "mapChange"
QT_MOC_LITERAL(14, 165, 3), // "map"
QT_MOC_LITERAL(15, 169, 14), // "stateMapChange"
QT_MOC_LITERAL(16, 184, 15), // "onZoomInClicked"
QT_MOC_LITERAL(17, 200, 16), // "onZoomOutClicked"
QT_MOC_LITERAL(18, 217, 11), // "onUPClicked"
QT_MOC_LITERAL(19, 229, 13), // "onDownClicked"
QT_MOC_LITERAL(20, 243, 14), // "onRightClicked"
QT_MOC_LITERAL(21, 258, 13), // "onLeftClicked"
QT_MOC_LITERAL(22, 272, 16), // "onPitchUpClicked"
QT_MOC_LITERAL(23, 289, 18), // "onPitchDownClicked"
QT_MOC_LITERAL(24, 308, 15), // "onHeadUpClicked"
QT_MOC_LITERAL(25, 324, 17), // "onHeadDownClicked"
QT_MOC_LITERAL(26, 342, 13), // "onHomeClicked"
QT_MOC_LITERAL(27, 356, 12), // "ontToggelMap"
QT_MOC_LITERAL(28, 369, 11) // "getStateMap"

    },
    "CameraManipulatorWidget\0zoomChanged\0"
    "\0newZoom\0upDownChanged\0newUpDown\0"
    "leftRightChanged\0nemLeftRight\0"
    "pitchChanged\0newBearing\0headChanged\0"
    "newTilt\0homeClicked\0mapChange\0map\0"
    "stateMapChange\0onZoomInClicked\0"
    "onZoomOutClicked\0onUPClicked\0onDownClicked\0"
    "onRightClicked\0onLeftClicked\0"
    "onPitchUpClicked\0onPitchDownClicked\0"
    "onHeadUpClicked\0onHeadDownClicked\0"
    "onHomeClicked\0ontToggelMap\0getStateMap"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CameraManipulatorWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  119,    2, 0x06 /* Public */,
       4,    1,  122,    2, 0x06 /* Public */,
       6,    1,  125,    2, 0x06 /* Public */,
       8,    1,  128,    2, 0x06 /* Public */,
      10,    1,  131,    2, 0x06 /* Public */,
      12,    0,  134,    2, 0x06 /* Public */,
      13,    1,  135,    2, 0x06 /* Public */,
      15,    0,  138,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    0,  139,    2, 0x0a /* Public */,
      17,    0,  140,    2, 0x0a /* Public */,
      18,    0,  141,    2, 0x0a /* Public */,
      19,    0,  142,    2, 0x0a /* Public */,
      20,    0,  143,    2, 0x0a /* Public */,
      21,    0,  144,    2, 0x0a /* Public */,
      22,    0,  145,    2, 0x0a /* Public */,
      23,    0,  146,    2, 0x0a /* Public */,
      24,    0,  147,    2, 0x0a /* Public */,
      25,    0,  148,    2, 0x0a /* Public */,
      26,    0,  149,    2, 0x0a /* Public */,
      27,    1,  150,    2, 0x0a /* Public */,
      28,    0,  153,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Double,    9,
    QMetaType::Void, QMetaType::Double,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   14,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   14,
    QMetaType::Bool,

       0        // eod
};

void CameraManipulatorWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CameraManipulatorWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->zoomChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->upDownChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->leftRightChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->pitchChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->headChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->homeClicked(); break;
        case 6: _t->mapChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->stateMapChange(); break;
        case 8: _t->onZoomInClicked(); break;
        case 9: _t->onZoomOutClicked(); break;
        case 10: _t->onUPClicked(); break;
        case 11: _t->onDownClicked(); break;
        case 12: _t->onRightClicked(); break;
        case 13: _t->onLeftClicked(); break;
        case 14: _t->onPitchUpClicked(); break;
        case 15: _t->onPitchDownClicked(); break;
        case 16: _t->onHeadUpClicked(); break;
        case 17: _t->onHeadDownClicked(); break;
        case 18: _t->onHomeClicked(); break;
        case 19: _t->ontToggelMap((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: { bool _r = _t->getStateMap();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CameraManipulatorWidget::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraManipulatorWidget::zoomChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CameraManipulatorWidget::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraManipulatorWidget::upDownChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CameraManipulatorWidget::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraManipulatorWidget::leftRightChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CameraManipulatorWidget::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraManipulatorWidget::pitchChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CameraManipulatorWidget::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraManipulatorWidget::headChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (CameraManipulatorWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraManipulatorWidget::homeClicked)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (CameraManipulatorWidget::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraManipulatorWidget::mapChange)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (CameraManipulatorWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CameraManipulatorWidget::stateMapChange)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CameraManipulatorWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_CameraManipulatorWidget.data,
    qt_meta_data_CameraManipulatorWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CameraManipulatorWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CameraManipulatorWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CameraManipulatorWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CameraManipulatorWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void CameraManipulatorWidget::zoomChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CameraManipulatorWidget::upDownChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CameraManipulatorWidget::leftRightChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CameraManipulatorWidget::pitchChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CameraManipulatorWidget::headChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CameraManipulatorWidget::homeClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void CameraManipulatorWidget::mapChange(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CameraManipulatorWidget::stateMapChange()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
