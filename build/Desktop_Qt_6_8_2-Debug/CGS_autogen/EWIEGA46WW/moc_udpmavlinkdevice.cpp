/****************************************************************************
** Meta object code from reading C++ file 'udpmavlinkdevice.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../udpmavlinkdevice.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'udpmavlinkdevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN16UDPMavlinkDeviceE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN16UDPMavlinkDeviceE = QtMocHelpers::stringData(
    "UDPMavlinkDevice",
    "readBytes",
    "",
    "sendCommand",
    "mavlink_message_t",
    "command"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN16UDPMavlinkDeviceE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   26,    2, 0x09,    1 /* Protected */,
       3,    1,   27,    2, 0x0a,    2 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,

       0        // eod
};

Q_CONSTINIT const QMetaObject UDPMavlinkDevice::staticMetaObject = { {
    QMetaObject::SuperData::link<MavlinkDevice::staticMetaObject>(),
    qt_meta_stringdata_ZN16UDPMavlinkDeviceE.offsetsAndSizes,
    qt_meta_data_ZN16UDPMavlinkDeviceE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN16UDPMavlinkDeviceE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<UDPMavlinkDevice, std::true_type>,
        // method 'readBytes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendCommand'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const mavlink_message_t &, std::false_type>
    >,
    nullptr
} };

void UDPMavlinkDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<UDPMavlinkDevice *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->readBytes(); break;
        case 1: _t->sendCommand((*reinterpret_cast< std::add_pointer_t<mavlink_message_t>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *UDPMavlinkDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UDPMavlinkDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN16UDPMavlinkDeviceE.stringdata0))
        return static_cast<void*>(this);
    return MavlinkDevice::qt_metacast(_clname);
}

int UDPMavlinkDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MavlinkDevice::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
