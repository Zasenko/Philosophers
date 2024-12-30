#include "philo.h"

void *print_message(void *arg);
void *get_time(void *arg);
void *increment(void *arg);
void *increment2(void *arg);
void mutex_test(void);

void test_kill(void);
void test_pthread_create(void);
void test_pthread_detach(void);

pthread_mutex_t lock;
int counter = 0;

int main(void)
{
    //test_kill();
    test_pthread_create();
    //test_pthread_detach();
    //mutex_test();
    printf("END\n");
}

void test_kill(void)
{
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
}

void test_pthread_create(void)
{
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
        return;
    }
    // Ожидаем завершения потока
    printf("1\n");

    // #include <pthread.h>
    // int pthread_join(pthread_t thread, void **retval);
    //	• thread — ID потока, завершения которого ждём.
    //	• retval — указатель на значение, возвращённое функцией потока.

    void *result;
    pthread_join(thread, &result); // Ожидает завершения указанного потока
    printf("Результат потока: %s\n", (char *)result);
}

void test_pthread_detach(void)
{
    //----------------------------------------------
    printf("-- pthread_detach --\n");
    //----------------------------------------------
    // Переводит поток в отделённое состояние.
    // Это освобождает его ресурсы автоматически после завершения.
    // 1. Поток создаётся и немедленно отделяется.
    // 2. Основной поток завершает работу, не ожидая завершения отделённого потока.
    pthread_t thread;
    const char *message2 = "Hello from detached thread!";
    if (pthread_create(&thread, NULL, get_time, (void *)message2) != 0)
    {
        perror("Ошибка при создании потока");
        return;
    }
    printf("Основной поток - начало\n");
    pthread_detach(thread);

    printf("Основной поток  Начало паузы\n");
    for (int i = 0; i < 10; i++)
    {
        printf("основной %d\n", i);
        sleep(1); // Пауза на 500000 микросекунд (0.5 секунды)
    }
        
    printf("Основной поток - Конец паузы\n");
    printf("Основной поток - завершён.\n");
}

void mutex_test(void)
{
    //----------------------------------------------
    printf("-- mutex --\n");
    //----------------------------------------------
    // int pthread_mutex_init(pthread_mutex_t * mutex, const pthread_mutexattr_t *attr);
    // • mutex — указатель на мьютекс.
    // • attr — атрибуты мьютекса(передать NULL для стандартных атрибутов).

    // int pthread_mutex_init(pthread_mutex_t * mutex, const pthread_mutexattr_t *attr);
    // int pthread_mutex_destroy(pthread_mutex_t * mutex);
    // int pthread_mutex_lock(pthread_mutex_t *mutex);
    // int pthread_mutex_unlock(pthread_mutex_t * mutex);;
    pthread_t thread1, thread2;

    pthread_mutex_init(&lock, NULL);

    pthread_create(&thread1, NULL, increment, "1");
    pthread_create(&thread2, NULL, increment2, "2");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Итоговое значение счётчика: %d\n", counter);

    pthread_mutex_destroy(&lock);
}

void *increment(void *arg)
{
    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&lock);
        counter++;
        printf("Поток %s (%p) увеличил счётчик до %d\n", (char *)arg, (void *) pthread_self(), counter);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    return NULL;
}

void *increment2(void *arg)
{
    for (int i = 0; i < 20; i++)
    {
        pthread_mutex_lock(&lock);
        counter++;
        printf("Поток %s (%p) увеличил счётчик до %d\n", (char *)arg, (void *)pthread_self(), counter);
        pthread_mutex_unlock(&lock);
        usleep(100000);
    }
    return NULL;
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

    for (int i = 0; i < 10; i++)
    {
        printf("не основной%d\n", i);
        sleep(2); // Пауза на 500000 микросекунд (0.5 секунды)
    }
    // printf("get_time Начало паузы\n");
    // for (int i = 0; i < 100000000; i++)
    //     printf("%d get_time work\n", i);
    // printf("get_time Конец паузы\n");
    struct timeval tv;
    gettimeofday(&tv, NULL);
 //   printf("get_time Секунды: %ld, Микросекунды: %ld\n", tv.tv_sec, tv.tv_usec);
    printf("get_time Секунды: %ld, Микросекунды: %d\n", tv.tv_sec, tv.tv_usec);
    printf("-- get_time END --\n");
    return NULL;
}