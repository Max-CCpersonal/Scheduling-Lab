#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

#include <stdio.h>


// SJF scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    list_t* jobList;
    job_t* smallest;
    job_t* current;
} scheduler_SJF_t;

int job_compare_fn (void* data1, void* data2){
    job_t* job1 = (job_t*)data1;
    job_t* job2 = (job_t*)data2;

    if(jobGetJobTime(job1) == jobGetJobTime(job2)){
        return 0;
    }
    else if(jobGetJobTime(job1) > jobGetJobTime(job2)){
        return -1;
    }
    else{
        return 1;
    }
}

// Creates and returns scheduler specific info
void* schedulerSJFCreate()
{
    scheduler_SJF_t* info = malloc(sizeof(scheduler_SJF_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->jobList = list_create(job_compare_fn);
    if(info->jobList == NULL){
        //whcih means the malloc in linked_list fail.
        free(info);
        return NULL;
    }
    return info;
}

// Destroys scheduler specific info
void schedulerSJFDestroy(void* schedulerInfo)
{
    scheduler_SJF_t* info = (scheduler_SJF_t*)schedulerInfo;
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
void schedulerSJFScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_SJF_t* info = (scheduler_SJF_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if (list_count(info->jobList)== 0){
       
        list_insert(info->jobList,job);
        
        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetJobTime(job));
        info->current = job;
        info->smallest = job;
    }
    else{
        
        list_insert(info->jobList,job);
    }
}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerSJFCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_SJF_t* info = (scheduler_SJF_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if (list_count(info->jobList) == 0){
        return NULL;
    }

    list_node_t* completed_node = list_find(info->jobList,info->current);
    job_t* completed_job = info->current;
    
    list_remove(info->jobList,completed_node);
    if (list_count(info->jobList) != 0){
        
        info->smallest = (job_t*)(list_data(list_tail(info->jobList)));
        
    }
    
    if (list_count(info->jobList) > 0){
        job_t* next_job = info->smallest;
        
        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetJobTime(next_job));
        info->current = next_job;
    }

    return completed_job;
}
