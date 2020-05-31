#ifndef ENUMS_HPP
#define ENUMS_HPP

///Представлява студентският статус.
enum class Student_Status { 
    ///Дипломиран
    GRADUATED = 1,
    ///Записан
    SIGNED = 2,
    ///Прекъснал 
    INTERRUPTED = 3,
    UNKNOWN
};

///Представлява тип на дисциплина
enum class Type {
    ///Избираема дисциплина
    OPTIONAL = 1, 
    ///Задължителна дисциплина
    COMPULSORY = 2,
    UNKNOWN
};
#endif