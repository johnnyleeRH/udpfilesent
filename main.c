#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <execinfo.h>
#include <stdlib.h>

#include "log.h"
#include "iniopr.h"

int g_isAppToBeStoppedFlag = 0;
void NotifyStopApplication() {
    __sync_add_and_fetch( &g_isAppToBeStoppedFlag, 1 );
}

int IsAppToBeStopped() {
    int flag = __sync_fetch_and_add( &g_isAppToBeStoppedFlag , 0 );
    if ( flag > 0 )
        return 1;
    else
        return 0;
}

char g_proc_status[4096];
inline void dump_procinfo(const char* proc_path, void* buffer, size_t size)
{
    ssize_t term_offset = 0;
    if (!proc_path) return;
    int fd = open(proc_path, O_RDONLY);
    if (fd >=0)
    {
        term_offset = read(fd, buffer, size);
        close(fd);
    }
    ((char*)buffer)[term_offset < 0 ? 0 : term_offset] = '\0';
    return ;
}

void signalhandle(int sig, siginfo_t *siginfo, void* context)
{
    INFO("Received signal %d.  Terminating now!", sig);
    ssize_t nread;

    printf("*[si_signo] %d\n", siginfo->si_signo);
    printf("*[si_errno] %d\n", siginfo->si_errno);
    printf("*[si_code]  %d\n", siginfo->si_code);
    printf("*[si_pid]   %d\n", siginfo->si_pid);
    printf("*[si_uid]   %d\n", siginfo->si_uid);
    printf("*[si_ptr]   %p\n", siginfo->si_ptr);
    printf("*[si_addr]  %p\n", siginfo->si_addr);
    printf("*[si_band]  %ld\n", siginfo->si_band);

    char proc_path[120];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/status",
             siginfo->si_pid);
    dump_procinfo(proc_path, g_proc_status, sizeof(g_proc_status));
    printf("***[/proc/killer/status]***\n%s\n", g_proc_status);

    char cwd[120];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/cwd", siginfo->si_pid);
    nread = readlink(proc_path, cwd, sizeof(cwd) - 1);
    cwd[nread] = '\0';

    char exe[120];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", siginfo->si_pid);
    nread = readlink(proc_path, exe, sizeof(exe) - 1);
    exe[nread] = '\0';

    char root[120];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/root", siginfo->si_pid);
    nread = readlink(proc_path, root, sizeof(root) - 1);
    root[nread] = '\0';


    printf("*cwd=%s\n", cwd);
    printf("*exe=%s\n", exe);
    printf("*root=%s\n", root);

    snprintf(proc_path, sizeof(proc_path), "/proc/self/status");
    dump_procinfo(proc_path, g_proc_status, sizeof(g_proc_status));
    printf("***[/proc/self/status]***\n%s\n", g_proc_status);


    NotifyStopApplication();

    if (sig == SIGSEGV)
    {
        void *ptr[1024];
        int bk_len = backtrace(ptr, 1024);
        backtrace_symbols_fd(ptr, bk_len, STDOUT_FILENO);
        exit(-1);
    }
}

void loginit() {
    time_t now;
    struct tm *ts;
    time(&now);
    ts = localtime(&now);
    char today[16] = "0";
    strftime(today, 9, "%Y%m%d", ts);
    memcpy(today + 8, ".log", 4);
    logfile = open(today, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
}

void siginit() {
    struct sigaction act;
    memset(&act, '\0', sizeof(act));
    act.sa_sigaction = signalhandle;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO | SA_RESTART;

    sigaction(SIGTERM, &act, NULL);
    sigaction(SIGBUS,  &act, NULL);
    sigaction(SIGINT,  &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    sigaction(SIGSEGV, &act, NULL);
    sigaction(SIGPIPE, &act, NULL);
}

void init() {
    loginit();
    siginit();
}

void fin() {
    INFO("app fin.");
    close(logfile);
}

void usage()
{
    printf("Usage: -c <ini_file> : ini configuration file required.\n");
}

int main(int argc, char* argv[]) {
    int opt = 0;
    char* inifile = NULL;

    while((opt = getopt(argc, argv, "c:h")) != -1) {
        switch(opt) {
            case 'c':
                inifile = optarg;
                break;
            default:
                usage(argv[0]);
                return 1;
        }
    }

    if(inifile == NULL) {
        usage();
        return 1;
    }
    init();
    atexit(fin);

    if (InitConfigure(inifile) < 0)
        return -1;
    GetInputParam();

    while(!IsAppToBeStopped())
        sleep(1);

    return 0;
}