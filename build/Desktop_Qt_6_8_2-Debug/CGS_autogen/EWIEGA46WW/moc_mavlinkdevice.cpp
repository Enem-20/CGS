/****************************************************************************
** Meta object code from reading C++ file 'mavlinkdevice.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../mavlinkdevice.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mavlinkdevice.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13MavlinkDeviceE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN13MavlinkDeviceE = QtMocHelpers::stringData(
    "MavlinkDevice",
    "messageReceived",
    "",
    "mavlink_message_t",
    "message",
    "readBytes",
    "waitPacketTimeout",
    "sendCommand",
    "command"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN13MavlinkDeviceE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   41,    2, 0x09,    3 /* Protected */,
       6,    0,   42,    2, 0x09,    4 /* Protected */,
       7,    1,   43,    2, 0x0a,    5 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    8,

       0        // eod
};

Q_CONSTINIT const QMetaObject MavlinkDevice::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN13MavlinkDeviceE.offsetsAndSizes,
    qt_meta_data_ZN13MavlinkDeviceE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN13MavlinkDeviceE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MavlinkDevice, std::true_type>,
        // method 'messageReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<mavlink_message_t, std::false_type>,
        // method 'readBytes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'waitPacketTimeout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendCommand'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const mavlink_message_t &, std::false_type>
    >,
    nullptr
} };

void MavlinkDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MavlinkDevice *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->messageReceived((*reinterpret_cast< std::add_pointer_t<mavlink_message_t>>(_a[1]))); break;
        case 1: _t->readBytes(); break;
        case 2: _t->waitPacketTimeout(); break;
        case 3: _t->sendCommand((*reinterpret_cast< std::add_pointer_t<mavlink_message_t>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (MavlinkDevice::*)(mavlink_message_t );
            if (_q_method_type _q_method = &MavlinkDevice::messageReceived; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *MavlinkDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MavlinkDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN13MavlinkDeviceE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MavlinkDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void MavlinkDevice::messageReceived(mavlink_message_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
