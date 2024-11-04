#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

#include <stdio.h>

// FCFS scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    list_t* jobList;
} scheduler_FCFS_t;

// Creates and returns scheduler specific info
void* schedulerFCFSCreate()
{
    scheduler_FCFS_t* info = malloc(sizeof(scheduler_FCFS_t));
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
void schedulerFCFSDestroy(void* schedulerInfo)
{
    scheduler_FCFS_t* info = (scheduler_FCFS_t*)schedulerInfo;
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
void schedulerFCFSScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_FCFS_t* info = (scheduler_FCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_insert(info->jobList,job);

    

    //only job here just do it.
    if(list_count(info->jobList) == 1){
        
        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetJobTime(job));
    }

}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerFCFSCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_FCFS_t* info = (scheduler_FCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if(list_count(info->jobList) == 0){
        return NULL;
    }

    list_node_t* complete_node = list_tail(info->jobList);
    job_t* complete_job = (job_t*)list_data(complete_node);
    
    list_remove(info->jobList, complete_node);

    if (list_count(info->jobList) > 0){
        //the job list is not empty, make schedule for next job.
        list_node_t* nextjob_node = list_tail(info->jobList);
        //add to head, so tail is the next.
        job_t* next_job = (job_t*)list_data(nextjob_node);
        
        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetJobTime(next_job));
    }
    return complete_job;
}
