#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "paramdef.h"

static struct inputparam  input;
static struct outputparam output;
static struct calcparam   calculate;

FILE *inistream;
char *line = NULL;

int InitConfigure(char *file) {
    inistream = fopen(file, "rw");
    if (inistream == NULL) {
        ERROR("file %s open failed.", file);
        return -1;
    }
    line = malloc(1024);
    if (NULL == line) {
        ERROR("fail to malloc for file line.");
        fclose(inistream);
        return -1;
    }
    memset(&input, 0, sizeof(input));
    memset(&output, 0, sizeof(output));
    memset(&calculate, 0, sizeof(calculate));
    return 0;
}

void FinConfigure() {
    free(line);
    fclose(inistream);
}

int GetInputParam() {
    size_t len = 1024;
    ssize_t nread;
    fseek(inistream, 0, SEEK_SET);
    printf("getinput pararm.\n");
    while ((nread = getline(&line, &len, inistream)) != -1) {
        if (0 == strncmp(line, "mpsize", strlen("mpsize"))) {
            input.mpsize = atoi(line + strlen("mpsize") + 1);
        } else if (0 == strncmp(line, "multnum", strlen("multnum"))) {
            input.multnum = atoi(line + strlen("mpsize") + 1);
        } else if (0 == strncmp(line, "sfiles", strlen("sfiles"))) {
            input.sfiles = atoi(line + strlen("sfiles") + 1);
        } else if (0 == strncmp(line, "afiles", strlen("afiles"))) {
            input.sfiles = atoi(line + strlen("afiles") + 1);
        } else if (0 == strncmp(line, "atimes", strlen("atimes"))) {
            input.atimes = atoi(line + strlen("atimes") + 1);
        } else if (0 == strncmp(line, "pausecmd", strlen("pausecmd"))) {
            input.pausecmd = atoi(line + strlen("pausecmd") + 1);
        } else if (0 == strncmp(line, "starttime", strlen("starttime"))) {
            if (nread - strlen("starttime") - 2 != TIME_FORMAT_LEN) {
                ERROR("start time %d: %s", nread, line);
                return -1;
            }
            memcpy(input.starttime, line + strlen("starttime") + 1, TIME_FORMAT_LEN);
        } else if (0 == strncmp(line, "endtime", strlen("endtime"))) {
            if (nread - strlen("endtime") - 2 != TIME_FORMAT_LEN) {
                ERROR("end time %d: %s", nread, line);
                return -1;
            }
            memcpy(input.endtime, line + strlen("endtime") + 1, TIME_FORMAT_LEN);
        } else if (0 == strncmp(line, "multip", strlen("multip"))) {
            if (nread - strlen("multip") - 2 > MAX_IP_LEN) {
                ERROR("multi ip %d: %s", nread, line);
                return -1;
            }
            memcpy(input.multip, line + strlen("multip") + 1, nread - strlen("multip") - 2);
        } else if (0 == strncmp(line, "multport", strlen("multport"))) {
            input.multport = atoi(line + strlen("multport") + 1);
        } else if (0 == strncmp(line, "mbandwidth", strlen("mbandwidth"))) {
            input.mbandwidth = atol(line + strlen("mbandwidth") + 1);
        } else if (0 == strncmp(line, "smpath", strlen("smpath"))) {
            if (nread - strlen("smpath") - 2 >= 1024) {
                ERROR("file path %d.", nread - strlen("smpath") - 2);
                return -1;
            }
            memcpy(input.smpath, line + strlen("smpath") + 1, nread - strlen("smpath") - 2);
        }
    }
    printInput();
    return 0;
}

void printInput() {
    printf("smpath     %s\n", input.smpath);
    printf("mbandwidth %lu\n", input.mbandwidth);
    printf("multport   %u\n", input.multport);
    printf("afiles     %u\n", input.afiles);
    printf("atimes     %u\n", input.atimes);
    printf("mpsize     %u\n", input.mpsize);
    printf("multnum    %u\n", input.multnum);
    printf("pausecmd   %u\n", input.pausecmd);
    printf("sfiles     %u\n", input.sfiles);
    printf("starttime  %s\n", input.starttime);
    printf("endtime    %s\n", input.endtime);
    printf("multip     %s\n", input.multip);
}
