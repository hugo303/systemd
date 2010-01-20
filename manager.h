/*-*- Mode: C; c-basic-offset: 8 -*-*/

#ifndef foomanagerhfoo
#define foomanagerhfoo

#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>

typedef struct Manager Manager;

#include "name.h"
#include "job.h"
#include "hashmap.h"
#include "list.h"
#include "set.h"

struct Manager {
        uint32_t current_job_id;

        /* Note that the set of names we know of is allowed to be
         * incosistent. However the subset of it that is loaded may
         * not, and the list of jobs may neither. */

        /* Active jobs and names */
        Hashmap *names;  /* name string => Name object n:1 */
        Hashmap *jobs;   /* job id => Job object 1:1 */

        /* Names that need to be loaded */
        LIST_HEAD(Meta, load_queue); /* this is actually more a stack than a queue, but uh. */

        /* Jobs to be added */
        Hashmap *transaction_jobs;      /* Name object => Job object list 1:1 */
        JobDependency *transaction_anchor;

        bool dispatching_load_queue:1;
};

Manager* manager_new(void);
void manager_free(Manager *m);

Job *manager_get_job(Manager *m, uint32_t id);
Name *manager_get_name(Manager *m, const char *name);

int manager_load_name(Manager *m, const char *name, Name **_ret);
int manager_add_job(Manager *m, JobType type, Name *name, JobMode mode, bool force, Job **_ret);

void manager_dump_names(Manager *s, FILE *f, const char *prefix);
void manager_dump_jobs(Manager *s, FILE *f, const char *prefix);

void manager_transaction_delete_job(Manager *m, Job *j);

#endif
