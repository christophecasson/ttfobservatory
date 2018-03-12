


#include <cmath>
#include <memory>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <stdarg.h>

#define MAXARGS 20

bool RunScript(char* script, ...)
{
    int pid = fork();
    if (pid == -1)
    {
        //DEBUG(INDI::Logger::DBG_ERROR, "Fork failed");
	printf("Fork failed\r\n");
        return false;
    }
    else if (pid == 0)
    {
        char tmp[256];

        strncpy(tmp, script, sizeof(tmp));

        char **args = (char **)malloc(MAXARGS * sizeof(char *));
        int arg     = 1;
        char *p     = tmp;

        while (arg < MAXARGS)
        {
            char *pp = strstr(p, " ");

            if (pp == nullptr)
                break;
            *pp++       = 0;
            args[arg++] = pp;
            p           = pp;
        }

        va_list ap;
        va_start(ap, script);
        while (arg < MAXARGS)
        {
            char *pp    = va_arg(ap, char *);
            args[arg++] = pp;
            if (pp == nullptr)
                break;
        }
        va_end(ap);
        char path[256];
        snprintf(path, 256, "%s/%s", "/home/astro/dev/ttfobservatory/tests", tmp);
	printf("args[0]=%s\r\nargs[1]=%s\r\nargs[2]=%s\r\n", args[0], args[1], args[2]);

	snprintf(args[0], 256, "%s", tmp);
	printf("executing %s %s\r\n", path, args);
        execvp(path, args);
        printf("Failed to execute script\r\n");
        exit(0);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        printf("Script %s returned %d\r\n", script, status);
        return status == 0;
    }
}




int main()
{
    printf("START\r\n");
    char *name  = tmpnam(nullptr);
    printf("temp=%s\r\n", name);
    char *scriptname = "status.py";
    printf("scriptname=%s\r\n", scriptname);

    bool status = RunScript(scriptname, name, nullptr);
    if (status)
    {
	printf("script status True\r\n");
        int parked = 0, shutter = 0;
        float az   = 0;
        printf("opening file %s...\r\n", name);
        FILE *file = fopen(name, "r");
	printf("file opened\r\n");
        int ret    = 0;
        ret = fscanf(file, "%d %d %f", &parked, &shutter, &az);
	printf("file read\r\n");
        fclose(file);
	printf("file closed\r\n");
        unlink(name);
	printf("file deleted\r\n");
	printf("parked = %d shutter = %d az = %f\r\n", parked, shutter, az);
    }
    else
    {
	printf("script status False\r\n");
    }
    printf("END\r\n");
}



