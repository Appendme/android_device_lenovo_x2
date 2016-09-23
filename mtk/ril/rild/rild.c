/*
*  Copyright (C) 2014 MediaTek Inc.
*
*  Modification based on code covered by the below mentioned copyright
*  and/or permission notice(s).
*/

/* //device/system/rild/rild.c
**
** Copyright 2014 The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include <telephony/mtk_ril.h>

#include <rild.h>

#ifdef MTK_RIL_MD2
#define LOG_TAG "RILDMD2"
#else
#define LOG_TAG "RILD"
#endif

#include <utils/Log.h>
#include <cutils/properties.h>
#include <cutils/sockets.h>
#include <sys/capability.h>
#include <linux/prctl.h>
#include <librilmtk/ril_ex.h>

#include <private/android_filesystem_config.h>
#include "hardware/qemu_pipe.h"


#define SOC_PATH_PROPERTY  "rild.modem.socket"

#ifdef MTK_RIL_MD2
#define LIB_PATH_PROPERTY   "rild.libpath.md2"
#define LIB_ARGS_PROPERTY   "rild.libargs.md2"
#else
#define LIB_PATH_PROPERTY   "rild.libpath"
#define LIB_ARGS_PROPERTY   "rild.libargs"
#endif

#define MAX_LIB_ARGS        16

static void usage(const char *argv0)
{
    fprintf(stderr, "Usage: %s -l <ril impl library> [-- <args for impl library>]\n", argv0);
    exit(EXIT_FAILURE);
}

extern char rild[MAX_SOCKET_NAME_LENGTH];

extern void RIL_register (const RIL_RadioFunctions *callbacks);

extern void RIL_registerSocket (const RIL_RadioFunctionsSocket *callbacks);

extern void RIL_register_socket (RIL_RadioFunctionsSocket *(*rilUimInit)
        (const struct RIL_EnvSocket *, int, char **), RIL_SOCKET_TYPE socketType, int argc, char **argv);

extern void RIL_onRequestComplete(RIL_Token t, RIL_Errno e,
                           void *response, size_t responselen);

extern void RIL_setRilSocketName(char *);

extern void RIL_onUnsolicitedResponseSocket(int unsolResponse, const void *data,
                                size_t datalen, RIL_SOCKET_ID socket_id);

extern void RIL_onUnsolicitedResponse(int unsolResponse, const void *data,
                                size_t datalen);

extern void RIL_requestTimedCallback (RIL_TimedCallback callback,
                               void *param, const struct timeval *relativeTime);

#ifdef MTK_RIL
extern void RIL_requestProxyTimedCallback (RIL_TimedCallback callback,
                               void *param, const struct timeval *relativeTime, int proxyId);
extern RILChannelId RIL_queryMyChannelId(RIL_Token t);
extern int RIL_queryMyProxyIdByThread();
#endif

static struct RIL_Env s_rilEnv = {
    RIL_onRequestComplete,
    RIL_onUnsolicitedResponse,
    RIL_requestTimedCallback
#ifdef MTK_RIL
    ,RIL_requestProxyTimedCallback
    ,RIL_queryMyChannelId
    ,RIL_queryMyProxyIdByThread
#endif
};

static struct RIL_EnvSocket s_rilEnvSocket = {
    RIL_onRequestComplete,
    RIL_onUnsolicitedResponseSocket,
    RIL_requestTimedCallback
#ifdef MTK_RIL
    ,RIL_requestProxyTimedCallback
    ,RIL_queryMyChannelId
    ,RIL_queryMyProxyIdByThread
#endif
};


extern void RIL_startEventLoop();

static int make_argv(char * args, char ** argv)
{
    // Note: reserve argv[0]
    int count = 1;
    char * tok;
    char * s = args;

    while ((tok = strtok(s, " \0"))) {
        argv[count] = tok;
        s = NULL;
        count++;
    }
    return count;
}

/*
 * switchUser - Switches UID to radio, preserving CAP_NET_ADMIN capabilities.
 * Our group, cache, was set by init.
 */
void switchUser() {
    char debuggable[PROP_VALUE_MAX];

    prctl(PR_SET_KEEPCAPS, 1, 0, 0, 0);
    setuid(AID_RADIO);

    struct __user_cap_header_struct header;
    memset(&header, 0, sizeof(header));
    header.version = _LINUX_CAPABILITY_VERSION_3;
    header.pid = 0;

    struct __user_cap_data_struct data[2];
    memset(&data, 0, sizeof(data));

    data[CAP_TO_INDEX(CAP_NET_ADMIN)].effective |= CAP_TO_MASK(CAP_NET_ADMIN);
    data[CAP_TO_INDEX(CAP_NET_ADMIN)].permitted |= CAP_TO_MASK(CAP_NET_ADMIN);

    data[CAP_TO_INDEX(CAP_NET_RAW)].effective |= CAP_TO_MASK(CAP_NET_RAW);
    data[CAP_TO_INDEX(CAP_NET_RAW)].permitted |= CAP_TO_MASK(CAP_NET_RAW);

    if (capset(&header, &data[0]) == -1) {
        RLOGE("capset failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /*
     * Debuggable build only:
     * Set DUMPABLE that was cleared by setuid() to have tombstone on RIL crash
     */
    property_get("ro.debuggable", debuggable, "0");
    if (strcmp(debuggable, "1") == 0) {
        prctl(PR_SET_DUMPABLE, 1, 0, 0, 0);
    }
}

int main(int argc, char **argv)
{
    const char * rilLibPath = NULL;
    char **rilArgv;
    void *dlHandle;
    const RIL_RadioFunctions *(*rilInit)(const struct RIL_Env *, int, char **);
    const RIL_RadioFunctions *(*rilInitSocket)(const struct RIL_EnvSocket *, int, char **);
    const RIL_RadioFunctionsSocket *(*rilUimInit)(const struct RIL_EnvSocket *, int, char **);
    char *err_str = NULL;
    const RIL_RadioFunctions *funcs;
    const RIL_RadioFunctionsSocket *funcsSocket;
    char libPath[PROPERTY_VALUE_MAX];
    char socPath[PROPERTY_VALUE_MAX];
    unsigned char hasLibArgs = 0;

    int i;
    const char *clientId = NULL;
    RLOGD("**RIL Daemon Started**");
    RLOGD("**RILd param count=%d**", argc);

#ifdef MTK_RIL_MD2
    RLOGD("RILD started (MD2)");
#else
    RLOGD("RILD started");
#endif

    if (mtkInit() == -1) goto done;

    umask(S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
    for (i = 1; i < argc ;) {
        if (0 == strcmp(argv[i], "-l") && (argc - i > 1)) {
            rilLibPath = argv[i + 1];
            i += 2;
        } else if (0 == strcmp(argv[i], "--")) {
            i++;
            hasLibArgs = 1;
            break;
        } else if (0 == strcmp(argv[i], "-c") &&  (argc - i > 1)) {
            clientId = argv[i+1];
            i += 2;
        } else {
            usage(argv[0]);
        }
    }

    if (clientId == NULL) {
        clientId = "0";
    } else if (atoi(clientId) >= MAX_RILDS) {
        RLOGE("Max Number of rild's supported is: %d", MAX_RILDS);
        exit(0);
    }
    if (strncmp(clientId, "0", MAX_CLIENT_ID_LENGTH)) {
        RIL_setRilSocketName(strncat(rild, clientId, MAX_SOCKET_NAME_LENGTH));
    }

    if (rilLibPath == NULL) {
#ifdef MTK_RIL_MD2
        if ( 0 == property_get(LIB_PATH_PROPERTY, libPath, "/system/lib/mtk-rilmd2.so")) {
#else
        if ( 0 == property_get(LIB_PATH_PROPERTY, libPath, "/system/lib/mtk-ril.so")) {
#endif
            // No lib sepcified on the command line, and nothing set in props.
            // Assume "no-ril" case.
            goto done;
        } else {
            rilLibPath = libPath;
        }
    }

    /* special override when in the emulator */
#if 1
    {
        static char*  arg_overrides[5];
        static char   arg_device[32];
        int           done = 0;

#ifdef MTK_RIL_MD2
#define  REFERENCE_RIL_PATH  "/system/lib/mtk-rilmd2.so"
#else
#define  REFERENCE_RIL_PATH  "/system/lib/mtk-ril.so"
#endif
        /* first, read /proc/cmdline into memory */
        char          buffer[1024], *p, *q;
        int           len;
        int           fd = open("/proc/cmdline",O_RDONLY);

        if (fd < 0) {
            RLOGD("could not open /proc/cmdline:%s", strerror(errno));
            goto OpenLib;
        }

        do {
            len = read(fd,buffer,sizeof(buffer)); }
        while (len == -1 && errno == EINTR);

        if (len < 0) {
            RLOGD("could not read /proc/cmdline:%s", strerror(errno));
            close(fd);
            goto OpenLib;
        }
        close(fd);

        if (strstr(buffer, "android.qemud=") != NULL)
        {
            /* the qemud daemon is launched after rild, so
            * give it some time to create its GSM socket
            */
            int  tries = 5;
#define  QEMUD_SOCKET_NAME    "qemud"

            while (1) {
                int  fd;

                sleep(1);

                fd = qemu_pipe_open("qemud:gsm");
                if (fd < 0) {
                    fd = socket_local_client(
                                QEMUD_SOCKET_NAME,
                                ANDROID_SOCKET_NAMESPACE_RESERVED,
                                SOCK_STREAM );
                }
                if (fd >= 0) {
                    close(fd);
                    snprintf( arg_device, sizeof(arg_device), "%s/%s",
                                ANDROID_SOCKET_DIR, QEMUD_SOCKET_NAME );

                    arg_overrides[1] = "-s";
                    arg_overrides[2] = arg_device;
                    done = 1;
                    break;
                }
                RLOGD("could not connect to %s socket: %s",
                    QEMUD_SOCKET_NAME, strerror(errno));
                if (--tries == 0)
                    break;
            }
            if (!done) {
                RLOGE("could not connect to %s socket (giving up): %s",
                    QEMUD_SOCKET_NAME, strerror(errno));
                while(1)
                    sleep(0x00ffffff);
            }
        }

        /* otherwise, try to see if we passed a device name from the kernel */
        if (!done) do {
#define  KERNEL_OPTION  "android.ril="
#define  DEV_PREFIX     "/dev/"

            p = strstr( buffer, KERNEL_OPTION );
            if (p == NULL)
                break;

            p += sizeof(KERNEL_OPTION)-1;
            q  = strpbrk( p, " \t\n\r" );
            if (q != NULL)
                *q = 0;

            snprintf( arg_device, sizeof(arg_device), DEV_PREFIX "%s", p );
            arg_device[sizeof(arg_device)-1] = 0;
            arg_overrides[1] = "-d";
            arg_overrides[2] = arg_device;
            done = 1;

        } while (0);

        if (done) {
            argv = arg_overrides;
            argc = 3;
            i    = 1;
            hasLibArgs = 1;
            rilLibPath = REFERENCE_RIL_PATH;

            RLOGD("overriding with %s %s", arg_overrides[1], arg_overrides[2]);
        }
    }
OpenLib:
#endif
    switchUser();

#ifdef MTK_RIL_MD2
    rilLibPath = "/system/lib/mtk-rilmd2.so";
#endif
    RLOGD("Open ril lib path: %s", rilLibPath);

    dlHandle = dlopen(rilLibPath, RTLD_NOW);

    if (dlHandle == NULL) {
        RLOGE("dlopen failed: %s", dlerror());
        exit(EXIT_FAILURE);
    }

    RIL_startEventLoop();

    rilInitSocket = (const int(*)(void))dlsym(dlHandle, "RIL_InitSocket");

    if (rilInitSocket == NULL) {
        RLOGD("Vendor RIL do not need socket id!");
        rilInit = (const RIL_RadioFunctions *(*)(const struct RIL_Env *, int, char **))dlsym(dlHandle, "RIL_Init");

        if (rilInit == NULL) {
            RLOGE("RIL_Init not defined or exported in %s\n", rilLibPath);
            exit(EXIT_FAILURE);
        }
    } else {
        RLOGD("vendor RIL need socket id");
    }

    dlerror(); // Clear any previous dlerror
    rilUimInit =
        (const RIL_RadioFunctionsSocket *(*)(const struct RIL_EnvSocket *, int, char **))
        dlsym(dlHandle, "RIL_SAP_Init");
    err_str = dlerror();
    if (err_str) {
        RLOGW("RIL_SAP_Init not defined or exported in %s: %s\n", rilLibPath, err_str);
    } else if (!rilUimInit) {
        RLOGW("RIL_SAP_Init defined as null in %s. SAP Not usable\n", rilLibPath);
    }

    if (hasLibArgs) {
        rilArgv = argv + i - 1;
        argc = argc -i + 1;
    } else {
        static char * newArgv[MAX_LIB_ARGS];
        static char args[PROPERTY_VALUE_MAX];
        rilArgv = newArgv;
#ifdef MTK_RIL_MD2
        property_get(LIB_ARGS_PROPERTY, args, "-d /dev/ccci2_tty0");
#else
        property_get(LIB_ARGS_PROPERTY, args, "-d /dev/ttyC0");
#endif
        argc = make_argv(args, rilArgv);
    }

    rilArgv[argc++] = "-c";
    rilArgv[argc++] = clientId;
    RLOGD("RIL_Init argc = %d clientId = %s", argc, rilArgv[argc-1]);

    // Make sure there's a reasonable argv[0]
    rilArgv[0] = argv[0];

    if (rilInitSocket == NULL) {
        RLOGD("Old vendor ril! so RIL_register is called");
        funcs = rilInit(&s_rilEnv, argc, rilArgv);
        RIL_register(funcs);
    } else {
        RLOGD("New vendor ril! so RIL_registerSocket is called");
        funcsSocket = rilInitSocket(&s_rilEnvSocket, argc, rilArgv);
        RIL_registerSocket(funcsSocket);
    }

    if (rilUimInit) {
        RLOGD("RIL_register_socket started");
        RIL_register_socket(rilUimInit, RIL_SAP_SOCKET, argc, rilArgv);
    }

    RLOGD("RIL_register_socket completed");
done:

    RLOGD("RIL_Init starting sleep loop");
    while (true) {
        sleep(UINT32_MAX);
    }
}