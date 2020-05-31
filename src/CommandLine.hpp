#ifndef COMMANDLINE_HPP
#define COMMANDLINE_HPP

#include <iostream>
#include <string>

///Съхранява работата на командният ред.

class CommandLine {
    private:
        ///Съдържа дали е отворен файл.
        static bool openedFile;

        ///Съдържа пътя към файла.
        static std::string openedFilePath;

        ///Съдържа името на файла.
        static std::string fileName;
    
        ///Взима името от openedFilePath и го слага във fileName.
        static void setFileName();
        
        ///Отваря файл, с който програмата да работи.
        static void open(std::string);

        ///Затваря текущо отвореният файл.
        static void close();

        ///Запазва записаните автомати в даден файл.
        static void saveas(std::string);

        ///Запазва записаните автомати в отворения файл.
        static void save();

        ///Извежда всички команди в командният ред.
        static void help();

        ///Излиза от програмата.
        static void exit();

        /**Записва студент в дадена специалност
         * @param ФН факултетен номер
         * @param специалност името или ID-то на специалността
         * @param група група, който да бъде записан
         * @param име името на студента
         */
        static void enroll(int, std::string, int, std::string);

        ///Повишава студента в горен курс.
        static void advance(int);

        /**Променя информация за студента.
         * @param ФН факултетен номер
         * @param опция курс/група/специалност
         * @param стойност стойност, на която да бъде променена
         * 
         * !!! След справка с ас. Дафина Петкова, е направена програмата да премества студент в друга специалност,
         * когато студентът е взел всички предмети с общи имена и двете специалности от минали години. 
         */
        static void change(int, std::string, std::string);

        ///Дипломира студент.
        static void graduate(int);

        ///Студент прекъсва обучението си.
        static void interrupt(int);

        ///Връщат се студентските права на даден студент.
        static void resume(int);

        ///Извежда информация относно студент.
        static void print(int);

        ///Извежда информация относно всички студенти в дадена специалност и курс.
        static void printall(std::string, int);

        ///Записва студент в дадена дисциплина от текущата специалност.
        static void enrollin(int, std::string);

        ///Добавя оценка към дисциплина на студента.
        static void addgrade(int, std::string, double);

        ///Извежда се протокол за дадена дисциплина.
        static void protocol(std::string);

        ///Извежда се академична справка за даден студент.
        static void report(int);
    public:
        ///Пуска командният ред.
        static void run();
};

#endif