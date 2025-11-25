/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../mainwindow.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14MavlinkContextE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN14MavlinkContextE = QtMocHelpers::stringData(
    "MavlinkContext",
    "heartbeatMessageReceived",
    "",
    "mavlink_message_t",
    "heartbeat",
    "heartbeatUpdated",
    "mavlink_heartbeat_t",
    "modeUpdated",
    "mode",
    "armedUpdated",
    "armed",
    "attitudeUpdated",
    "rollPitchYaw",
    "speedsUpdated",
    "speeds",
    "logUpdated",
    "msg",
    "severity",
    "color",
    "paramUpdated",
    "mavlink_param_value_t",
    "param",
    "paramExtUpdated",
    "mavlink_param_ext_value_t",
    "handleMavlinkMessage",
    "loadModes",
    "sendCommand"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN14MavlinkContextE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   86,    2, 0x06,    1 /* Public */,
       5,    1,   89,    2, 0x06,    3 /* Public */,
       7,    1,   92,    2, 0x06,    5 /* Public */,
       9,    1,   95,    2, 0x06,    7 /* Public */,
      11,    1,   98,    2, 0x06,    9 /* Public */,
      13,    1,  101,    2, 0x06,   11 /* Public */,
      15,    3,  104,    2, 0x06,   13 /* Public */,
      19,    1,  111,    2, 0x06,   17 /* Public */,
      22,    1,  114,    2, 0x06,   19 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      24,    1,  117,    2, 0x08,   21 /* Private */,
      25,    0,  120,    2, 0x0a,   23 /* Public */,
      26,    1,  121,    2, 0x0a,   24 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QColor,   16,   17,   18,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void, 0x80000000 | 23,   21,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,   16,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,   16,

       0        // eod
};

Q_CONSTINIT const QMetaObject MavlinkContext::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN14MavlinkContextE.offsetsAndSizes,
    qt_meta_data_ZN14MavlinkContextE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN14MavlinkContextE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MavlinkContext, std::true_type>,
        // method 'heartbeatMessageReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const mavlink_message_t &, std::false_type>,
        // method 'heartbeatUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const mavlink_heartbeat_t &, std::false_type>,
        // method 'modeUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'armedUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'attitudeUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'speedsUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'logUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<QColor, std::false_type>,
        // method 'paramUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const mavlink_param_value_t &, std::false_type>,
        // method 'paramExtUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const mavlink_param_ext_value_t &, std::false_type>,
        // method 'handleMavlinkMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<mavlink_message_t, std::false_type>,
        // method 'loadModes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendCommand'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const mavlink_message_t &, std::false_type>
    >,
    nullptr
} };

void MavlinkContext::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MavlinkContext *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->heartbeatMessageReceived((*reinterpret_cast< std::add_pointer_t<mavlink_message_t>>(_a[1]))); break;
        case 1: _t->heartbeatUpdated((*reinterpret_cast< std::add_pointer_t<mavlink_heartbeat_t>>(_a[1]))); break;
        case 2: _t->modeUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->armedUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->attitudeUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->speedsUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->logUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QColor>>(_a[3]))); break;
        case 7: _t->paramUpdated((*reinterpret_cast< std::add_pointer_t<mavlink_param_value_t>>(_a[1]))); break;
        case 8: _t->paramExtUpdated((*reinterpret_cast< std::add_pointer_t<mavlink_param_ext_value_t>>(_a[1]))); break;
        case 9: _t->handleMavlinkMessage((*reinterpret_cast< std::add_pointer_t<mavlink_message_t>>(_a[1]))); break;
        case 10: _t->loadModes(); break;
        case 11: _t->sendCommand((*reinterpret_cast< std::add_pointer_t<mavlink_message_t>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (MavlinkContext::*)(const mavlink_message_t & );
            if (_q_method_type _q_method = &MavlinkContext::heartbeatMessageReceived; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (MavlinkContext::*)(const mavlink_heartbeat_t & );
            if (_q_method_type _q_method = &MavlinkContext::heartbeatUpdated; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (MavlinkContext::*)(const QString & );
            if (_q_method_type _q_method = &MavlinkContext::modeUpdated; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (MavlinkContext::*)(const QString & );
            if (_q_method_type _q_method = &MavlinkContext::armedUpdated; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (MavlinkContext::*)(const QString & );
            if (_q_method_type _q_method = &MavlinkContext::attitudeUpdated; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _q_method_type = void (MavlinkContext::*)(const QString & );
            if (_q_method_type _q_method = &MavlinkContext::speedsUpdated; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _q_method_type = void (MavlinkContext::*)(const QString & , const QString & , QColor );
            if (_q_method_type _q_method = &MavlinkContext::logUpdated; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _q_method_type = void (MavlinkContext::*)(const mavlink_param_value_t & );
            if (_q_method_type _q_method = &MavlinkContext::paramUpdated; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _q_method_type = void (MavlinkContext::*)(const mavlink_param_ext_value_t & );
            if (_q_method_type _q_method = &MavlinkContext::paramExtUpdated; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject *MavlinkContext::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MavlinkContext::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN14MavlinkContextE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MavlinkContext::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void MavlinkContext::heartbeatMessageReceived(const mavlink_message_t & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MavlinkContext::heartbeatUpdated(const mavlink_heartbeat_t & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MavlinkContext::modeUpdated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MavlinkContext::armedUpdated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MavlinkContext::attitudeUpdated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MavlinkContext::speedsUpdated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MavlinkContext::logUpdated(const QString & _t1, const QString & _t2, QColor _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MavlinkContext::paramUpdated(const mavlink_param_value_t & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void MavlinkContext::paramExtUpdated(const mavlink_param_ext_value_t & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN10MainWindowE = QtMocHelpers::stringData(
    "MainWindow",
    "paramsRequest",
    "",
    "mavlink_message_t",
    "msg",
    "on_actionParameters_set_triggered",
    "on_actionRefresh_configs_triggered",
    "onParamUpdated",
    "mavlink_param_value_t",
    "param",
    "onParamExtUpdated",
    "paramsRequested"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN10MainWindowE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   53,    2, 0x08,    3 /* Private */,
       6,    0,   54,    2, 0x08,    4 /* Private */,
       7,    1,   55,    2, 0x08,    5 /* Private */,
      10,    1,   58,    2, 0x08,    7 /* Private */,
      11,    1,   61,    2, 0x08,    9 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ZN10MainWindowE.offsetsAndSizes,
    qt_meta_data_ZN10MainWindowE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN10MainWindowE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'paramsRequest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const mavlink_message_t &, std::false_type>,
        // method 'on_actionParameters_set_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionRefresh_configs_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onParamUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const mavlink_param_value_t &, std::false_type>,
        // method 'onParamExtUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const mavlink_param_value_t &, std::false_type>,
        // method 'paramsRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const mavlink_message_t &, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->paramsRequest((*reinterpret_cast< std::add_pointer_t<mavlink_message_t>>(_a[1]))); break;
        case 1: _t->on_actionParameters_set_triggered(); break;
        case 2: _t->on_actionRefresh_configs_triggered(); break;
        case 3: _t->onParamUpdated((*reinterpret_cast< std::add_pointer_t<mavlink_param_value_t>>(_a[1]))); break;
        case 4: _t->onParamExtUpdated((*reinterpret_cast< std::add_pointer_t<mavlink_param_value_t>>(_a[1]))); break;
        case 5: _t->paramsRequested((*reinterpret_cast< std::add_pointer_t<mavlink_message_t>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (MainWindow::*)(const mavlink_message_t & );
            if (_q_method_type _q_method = &MainWindow::paramsRequest; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN10MainWindowE.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::paramsRequest(const mavlink_message_t & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
