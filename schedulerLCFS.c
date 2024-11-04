#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

#include <stdio.h>
// LCFS scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    list_t* jobList;
    job_t* current;
    job_t* latest;
} scheduler_LCFS_t;

// Creates and returns scheduler specific info
void* schedulerLCFSCreate()
{
    scheduler_LCFS_t* info = malloc(sizeof(scheduler_LCFS_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->jobList = list_create(NULL);
    if(info->jobList == NULL){
        //whcih means the malloc in linked_list fail.
        free(info);
        return NULL;
    }
    return info;
}

// Destroys scheduler specific info
void schedulerLCFSDestroy(void* schedulerInfo)
{
    scheduler_LCFS_t* info = (scheduler_LCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if(info != NULL){
        list_destroy(info->jobList);
        free(info);
    }
    //donr free when it's free'd   
}

// Called to schedule a new job in the queue
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// job - new job being added to the queue
// currentTime - the current simulated time
void schedulerLCFSScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_LCFS_t* info = (scheduler_LCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    //printf("insert %ld \n",job->id);
    list_insert(info->jobList,job);
    info->latest = job;

    if (list_count(info->jobList) == 1){
        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetJobTime(job));
        
        info->current = job;
    }
}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerLCFSCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_LCFS_t* info = (scheduler_LCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if(list_count(info->jobList) == 0){
        return NULL;
    }

    job_t* complete_job = NULL;
    if (jobGetId(info->current) != jobGetId(info->latest)&& info->jobList->head != NULL ){
        list_node_t* complete_node = list_find(info->jobList,info->current);
        complete_job = info->current;
        
        list_remove(info->jobList, complete_node);
    }
    else if(jobGetId(info->current) == jobGetId(info->latest)&& info->jobList->head != NULL ){
        list_node_t* complete_node = list_head(info->jobList);
        complete_job = (job_t*)list_data(complete_node);
        
        list_remove(info->jobList, complete_node);
        if(list_count(info->jobList) > 0){
            info->latest = (job_t*)(list_data(list_head(info->jobList)));
        }
    }

    job_t* next_job = NULL;
    if (list_count(info->jobList) > 0){
        //the job list is not empty, make schedule for next job.
        next_job = info->latest;
        
        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetJobTime(next_job));
        info->current = next_job;  
    }
    
    
    return complete_job;
}
