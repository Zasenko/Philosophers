#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

long get_time()
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1)
        return (-1);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
    // (seconds * 1000) + (microsec / 1000)
}
int main(void)
{
    long time;
    time = get_time();
    long now;
    now = get_time();
    printf("%ld\n", now);
    while (now - time < 200)
    {
        usleep(100);
        now = get_time();
    }
    printf("%ld\n", now);
    sleep(5);

    now = get_time();
    printf("%ld\n", now);
    while (usleep(200 * 1000))
    {
        continue;
    }
    now = get_time();
    printf("%ld\n", now);
}