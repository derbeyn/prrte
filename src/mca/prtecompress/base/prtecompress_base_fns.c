/*
 * Copyright (c) 2004-2010 The Trustees of Indiana University.
 *                         All rights reserved.
 * Copyright (c) 2015      Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
 *
 * Copyright (c) 2018      Amazon.com, Inc. or its affiliates.  All Rights reserved.
 * Copyright (c) 2019-2020 Intel, Inc.  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "prrte_config.h"

#include <string.h>
#include <sys/wait.h>
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif  /* HAVE_FCNTL_H */
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#include "src/mca/mca.h"
#include "src/mca/base/base.h"
#include "src/include/constants.h"
#include "src/util/os_dirpath.h"
#include "src/util/output.h"
#include "src/util/argv.h"
#include "src/util/printf.h"

#include "src/mca/prtecompress/prtecompress.h"
#include "src/mca/prtecompress/base/base.h"

/******************
 * Local Function Defs
 ******************/

/******************
 * Object stuff
 ******************/

int prrte_prtecompress_base_tar_create(char ** target)
{
    int exit_status = PRRTE_SUCCESS;
    char *tar_target = NULL;
    char **argv = NULL;
    pid_t child_pid = 0;
    int status = 0;

    prrte_asprintf(&tar_target, "%s.tar", *target);

    child_pid = fork();
    if( 0 == child_pid ) { /* Child */
        char *cmd;
        prrte_asprintf(&cmd, "tar -cf %s %s", tar_target, *target);

        argv = prrte_argv_split(cmd, ' ');
        status = execvp(argv[0], argv);

        prrte_output(0, "prtecompress:base: Tar:: Failed to exec child [%s] status = %d\n", cmd, status);
        exit(PRRTE_ERROR);
    }
    else if(0 < child_pid) {
        waitpid(child_pid, &status, 0);

        if( !WIFEXITED(status) ) {
            exit_status = PRRTE_ERROR;
            goto cleanup;
        }

        free(*target);
        *target = strdup(tar_target);
    }
    else {
        exit_status = PRRTE_ERROR;
        goto cleanup;
    }

 cleanup:
    if( NULL != tar_target ) {
        free(tar_target);
    }

    return exit_status;
}

int prrte_prtecompress_base_tar_extract(char ** target)
{
    int exit_status = PRRTE_SUCCESS;
    char **argv = NULL;
    pid_t child_pid = 0;
    int status = 0;

    child_pid = fork();
    if( 0 == child_pid ) { /* Child */
        char *cmd;
        prrte_asprintf(&cmd, "tar -xf %s", *target);

        argv = prrte_argv_split(cmd, ' ');
        status = execvp(argv[0], argv);

        prrte_output(0, "prtecompress:base: Tar:: Failed to exec child [%s] status = %d\n", cmd, status);
        exit(PRRTE_ERROR);
    }
    else if(0 < child_pid) {
        waitpid(child_pid, &status, 0);

        if( !WIFEXITED(status) ) {
            exit_status = PRRTE_ERROR;
            goto cleanup;
        }

        /* Strip off the '.tar' */
        (*target)[strlen(*target)-4] = '\0';
    }
    else {
        exit_status = PRRTE_ERROR;
        goto cleanup;
    }

 cleanup:

    return exit_status;
}

/******************
 * Local Functions
 ******************/
