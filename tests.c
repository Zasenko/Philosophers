#include "philo.h"

void *print_message(void *arg);
void *get_time(void *arg);

void tests(void)
{
    write(1, "Hello, world!\n", 14);

    //----------------------------------------------
    printf("-- kill --\n");
    //----------------------------------------------
    // int kill(pid_t pid, int sig);
    // pid — ID процесса или специальное значение :
    //  • > 0 — сигнал отправляется процессу с этим PID.
    //  • 0 — сигнал отправляется всем процессам в той же группе, что и текущий процесс.
    //  • < -1 — сигнал отправляется группе процессов с ID | pid |.
    // sig — сигнал, который нужно отправить(например, SIGKILL, SIGTERM, SIGUSR1).
    //       1. Вызов fork() создаёт новый процесс.В дочернем процессе pid == 0, а в родительском — PID дочернего процесса.
    //       2. Дочерний процесс выполняет бесконечный цикл.3. Родительский процесс ждёт 3 секунды, а затем отправляет сигнал SIGKILL дочернему процессу с помощью kill.
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    if (pid == 0)
    {
        printf("CHILD\n");
        while (1)
        {
            printf("Дочерний процесс работает...\n");
            sleep(1);
        }
    }
    else
    {
        printf("PERENT\n");

        // Родительский процесс
        sleep(3);
        printf("Отправляем сигнал SIGKILL дочернему процессу\n");
        kill(pid, SIGKILL); // Убиваем дочерний процесс
    }
   
   //----------------------------------------------
    printf("-- pthread_create --\n");
    //----------------------------------------------
    // #include <pthread.h>
    // int pthread_create(pthread_t * thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
    // • thread — указатель на переменную типа pthread_t, в которой будет сохранён ID нового потока.
    // • attr — указатель на структуру атрибутов потока(можно передать NULL для стандартных атрибутов).
    // • start_routine — функция, которая будет выполняться в новом потоке.
    // • arg — аргумент для функции start_routine.

    pthread_t thread;
    const char *message = "Hello, World!";

    // Создаём поток
    if (pthread_create(&thread, NULL, print_message, (void *)message) != 0)
    {
        perror("pthread_create");
        return ;
    }
    // Ожидаем завершения потока
    printf("1\n");

    // #include <pthread.h>
    // int pthread_join(pthread_t thread, void **retval);
    //	• thread — ID потока, завершения которого ждём.
    //	• retval — указатель на значение, возвращённое функцией потока.

    void *result;
    pthread_join(thread, &result);
    printf("Результат потока: %s\n", (char *)result);

    //----------------------------------------------
    printf("-- pthread_detach --\n");
    //----------------------------------------------
    pthread_t thread2;
    const char *message2 = "Hello from detached thread!";
    if (pthread_create(&thread2, NULL, get_time, (void *)message2) != 0)
    {
        perror("Ошибка при создании потока");
        return;
    }
    printf("Основной поток 2.\n");
    pthread_detach(thread);

    printf("Основной поток 2 Начало паузы\n");
    sleep(2); // Пауза на 500000 микросекунд (0.5 секунды)
    printf("Основной поток 2 Конец паузы\n");
    printf("Основной поток 2 завершён.\n");

    printf("END\n");
}

void *print_message(void *arg)
{
    printf("-- print_message --\n");
    printf("print_message сообщение: %s\n", (char *)arg);

    // #include <unistd.h>
    // int usleep(useconds_t usec);
    printf("print_message Начало паузы\n");
    usleep(5000000); // Пауза на 500000 микросекунд (0.5 секунды)
    printf("print_message Конец паузы\n");
    printf("-- print_message END --\n");
    return (void *)"print_message Поток завершён!";
    // return NULL;
}
// #include <sys/time.h>
// int gettimeofday(struct timeval *tv, struct timezone *tz);
//  • tv — структура для сохранения текущего времени(в секундах и микросекундах).
//	• tz — устаревший параметр(можно передать NULL).void *get_time(void *arg)
void *get_time(void *arg)
{
    printf("-- get_time --\n");
    printf("get_time принято: %s\n", (char *)arg);
    printf("get_time Начало паузы\n");
    for (int i = 0; i < 100000000; i++)
        printf("%d get_time work\n", i);
    printf("get_time Конец паузы\n");
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("get_time Секунды: %ld, Микросекунды: %d\n", tv.tv_sec, tv.tv_usec);
    printf("-- get_time END --\n");
    return NULL;
}