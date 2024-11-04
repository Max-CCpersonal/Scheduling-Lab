#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"


#include <stdio.h>

// PSJF scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    list_t* jobList;
    job_t* current;
    uint64_t when_start;
} scheduler_PSJF_t;

int job_PSJF_compare_fn (void* data1, void* data2){
    job_t* job1 = (job_t*)data1;
    job_t* job2 = (job_t*)data2;

    if(jobGetJobTime(job1) > jobGetJobTime(job2)){
        return 1;
    }
    else if(jobGetJobTime(job1) < jobGetJobTime(job2)){
        return -1;
    }
    else{
        if(jobGetId(job1) > jobGetId(job2)){
            return 1;
        }
        else if(jobGetId(job1) < jobGetId(job2)){
            return -1;
        }
    }
    return 1;
}


// Creates and returns scheduler specific info
void* schedulerPSJFCreate()
{
    scheduler_PSJF_t* info = malloc(sizeof(scheduler_PSJF_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->jobList = list_create(job_PSJF_compare_fn);
    if(info->jobList == NULL){
        //whcih means the malloc in linked_list fail.
        free(info);
        return NULL;
    }
    return info;
}

// Destroys scheduler specific info
void schedulerPSJFDestroy(void* schedulerInfo)
{
    scheduler_PSJF_t* info = (scheduler_PSJF_t*)schedulerInfo;
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
void schedulerPSJFScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_PSJF_t* info = (scheduler_PSJF_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_insert(info->jobList,job);
    jobSetRemainingTime(job,jobGetJobTime(job));


    if(scheduler->completionEvent != NULL){
        if (jobGetJobTime(job) < jobGetJobTime(info->current)){
            schedulerCancelNextCompletion(scheduler);
            
            jobSetRemainingTime(info->current,jobGetRemainingTime(info->current) - (currentTime - info->when_start));
            

            //cancel bigger job
            schedulerScheduleNextCompletion(scheduler,currentTime + jobGetJobTime(job));


            info->current = job;
            info->when_start = currentTime;
        }

    }
    else{
        schedulerScheduleNextCompletion(scheduler,currentTime + jobGetJobTime(job));


        info->current = job;
        info->when_start = currentTime;
    }
}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerPSJFCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_PSJF_t* info = (scheduler_PSJF_t*)schedulerInfo;
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
