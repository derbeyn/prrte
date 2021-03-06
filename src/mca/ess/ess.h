/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2004-2008 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2011 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2010 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2011-2015 Los Alamos National Security, LLC.  All rights
 *                         reserved.
 * Copyright (c) 2012 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2018-2020 Intel, Inc.  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */
/** @file:
 *
 * The PRRTE Environment-Specific Services
 *
 */

#ifndef PRRTE_ESS_H
#define PRRTE_ESS_H

#include "prrte_config.h"
#include "types.h"

#include "src/mca/mca.h"
#include "src/hwloc/hwloc-internal.h"

#include "src/util/proc_info.h"
#include "src/runtime/runtime.h"

BEGIN_C_DECLS

/*
 * API functions
 */

/*
 * Initialize the RTE for this environment
 */
typedef int (*prrte_ess_base_module_init_fn_t)(int argc, char **argv);

/*
 * Finalize the RTE for this environment
 */
typedef int (*prrte_ess_base_module_finalize_fn_t)(void);

/**
 * Abort the current application
 *
 * Aborts currently running application, NOTE: We do NOT call the
 * regular C-library "abort" function, even
 * though that would have alerted us to the fact that this is
 * an abnormal termination, because it would automatically cause
 * a core file to be generated. The "report" flag indicates if the
 * function should create an appropriate file to alert the local
 * orted that termination was abnormal.
 */
typedef void (*prrte_ess_base_module_abort_fn_t)(int status, bool report);

/**
 * Handle fault tolerance updates
 *
 * @param[in] state Fault tolerance state update
 *
 * @retval PRRTE_SUCCESS The operation completed successfully
 * @retval PRRTE_ERROR   An unspecifed error occurred
 */
typedef int  (*prrte_ess_base_module_ft_event_fn_t)(int state);

/*
 * the standard module data structure
 */
struct prrte_ess_base_module_3_0_0_t {
    prrte_ess_base_module_init_fn_t                  init;
    prrte_ess_base_module_finalize_fn_t              finalize;
    prrte_ess_base_module_abort_fn_t                 abort;
    prrte_ess_base_module_ft_event_fn_t              ft_event;
};
typedef struct prrte_ess_base_module_3_0_0_t prrte_ess_base_module_3_0_0_t;
typedef struct prrte_ess_base_module_3_0_0_t prrte_ess_base_module_t;

/*
 * the standard component data structure
 */
struct prrte_ess_base_component_2_0_0_t {
    prrte_mca_base_component_t base_version;
    prrte_mca_base_component_data_t base_data;
};
typedef struct prrte_ess_base_component_2_0_0_t prrte_ess_base_component_2_0_0_t;
typedef struct prrte_ess_base_component_2_0_0_t prrte_ess_base_component_t;

/*
 * Macro for use in components that are of type ess
 */
#define PRRTE_ESS_BASE_VERSION_3_0_0 \
    PRRTE_MCA_BASE_VERSION_2_1_0("ess", 3, 0, 0)

/* Global structure for accessing ESS functions */
PRRTE_EXPORT extern prrte_ess_base_module_t prrte_ess;  /* holds selected module's function pointers */

END_C_DECLS

#endif
