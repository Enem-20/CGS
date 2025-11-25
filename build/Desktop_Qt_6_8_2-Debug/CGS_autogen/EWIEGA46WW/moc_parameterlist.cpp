/****************************************************************************
** Meta object code from reading C++ file 'parameterlist.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../parameterlist.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'parameterlist.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13ParameterListE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN13ParameterListE = QtMocHelpers::stringData(
    "ParameterList",
    "parametersRequest",
    "",
    "mavlink_message_t",
    "msg",
    "on_syncWithVehicle_clicked",
    "onAutopilotHeartbeat",
    "handleMavlink",
    "mavlink_param_value_t",
    "mavlink_param_ext_value_t"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN13ParameterListE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   47,    2, 0x08,    3 /* Private */,
       6,    1,   48,    2, 0x0a,    4 /* Public */,
       7,    1,   51,    2, 0x0a,    6 /* Public */,
       7,    1,   54,    2, 0x0a,    8 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 8,    4,
    QMetaType::Void, 0x80000000 | 9,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject ParameterList::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN13ParameterListE.offsetsAndSizes,
    qt_meta_data_ZN13ParameterListE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN13ParameterListE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ParameterList, std::true_type>,
        // method 'parametersRequest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const mavlink_message_t &, std::false_type>,
        // method 'on_syncWithVehicle_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAutopilotHeartbeat'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const mavlink_message_t &, std::false_type>,
        // method 'handleMavlink'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const mavlink_param_value_t &, std::false_type>,
        // method 'handleMavlink'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const mavlink_param_ext_value_t &, std::false_type>
    >,
    nullptr
} };

void ParameterList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ParameterList *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->parametersRequest((*reinterpret_cast< std::add_pointer_t<mavlink_message_t>>(_a[1]))); break;
        case 1: _t->on_syncWithVehicle_clicked(); break;
        case 2: _t->onAutopilotHeartbeat((*reinterpret_cast< std::add_pointer_t<mavlink_message_t>>(_a[1]))); break;
        case 3: _t->handleMavlink((*reinterpret_cast< std::add_pointer_t<mavlink_param_value_t>>(_a[1]))); break;
        case 4: _t->handleMavlink((*reinterpret_cast< std::add_pointer_t<mavlink_param_ext_value_t>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (ParameterList::*)(const mavlink_message_t & );
            if (_q_method_type _q_method = &ParameterList::parametersRequest; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *ParameterList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ParameterList::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN13ParameterListE.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ParameterList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ParameterList::parametersRequest(const mavlink_message_t & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
