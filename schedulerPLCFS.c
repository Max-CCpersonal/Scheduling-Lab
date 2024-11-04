#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

#include <stdio.h>
// PLCFS scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    list_t* jobList;
    job_t* current;
    uint64_t when_start;
    list_t* time_track_list;
} scheduler_PLCFS_t;

// Creates and returns scheduler specific info
void* schedulerPLCFSCreate()
{
    scheduler_PLCFS_t* info = malloc(sizeof(scheduler_PLCFS_t));
    info->jobList = list_create(NULL);
    if(info->jobList == NULL){
        //whcih means the malloc in linked_list fail.
        free(info);
        return NULL;
    }
    return info;
}
// Destroys scheduler specific info
void schedulerPLCFSDestroy(void* schedulerInfo)
{
    scheduler_PLCFS_t* info = (scheduler_PLCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if(info != NULL){
        list_destroy(info->jobList);
        free(info);
    }
}

// Called to schedule a new job in the queue
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// job - new job being added to the queue
// currentTime - the current simulated time
void schedulerPLCFSScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_PLCFS_t* info = (scheduler_PLCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if(scheduler->completionEvent != NULL){
        schedulerCancelNextCompletion(scheduler);
        if (jobGetJobTime(info->current)<=jobGetRemainingTime(info->current)){
           
            jobSetRemainingTime(info->current, jobGetJobTime(info->current) - (currentTime - info->when_start));
           
        }
    }
    list_insert(info->jobList,job);
    
    job_t* next_job = (job_t*)list_data(list_head(info->jobList));
    schedulerScheduleNextCompletion(scheduler,currentTime + jobGetJobTime(next_job));
    info->when_start = currentTime;
    
    info->current = next_job;
}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerPLCFSCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_PLCFS_t* info = (scheduler_PLCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if (list_count(info->jobList) == 0){
        return NULL;
    }
    
    list_node_t* completed_node = list_find(info->jobList,info->current);
    job_t* completed_job = info->current;
    
    list_remove(info->jobList,completed_node);

    if (list_count(info->jobList) > 0){
        job_t* next_job = (job_t*)(list_data(list_head(info->jobList)));
        

        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetRemainingTime(next_job));
        info->when_start = currentTime;
        info->current = next_job;
    }
    
    return completed_job;
}
