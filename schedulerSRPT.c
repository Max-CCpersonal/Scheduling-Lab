#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"


#include <stdio.h>
// SRPT scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    list_t* jobList;
    job_t* currentJob;
    //currentJob using when it's preemptive
    uint64_t when_start;
} scheduler_SRPT_t;

int job_SRPT_compare_fn (void* data1, void* data2){
    job_t* job1 = (job_t*)data1;
    job_t* job2 = (job_t*)data2;

    if(jobGetRemainingTime(job1) > jobGetRemainingTime(job2)){
        return 1;
    }
    else if(jobGetRemainingTime(job1) < jobGetRemainingTime(job2)){
        return -1;
    }
    else{
        //tie breaker here.
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
void* schedulerSRPTCreate()
{
    scheduler_SRPT_t* info = malloc(sizeof(scheduler_SRPT_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->jobList = list_create(job_SRPT_compare_fn);
    if(info->jobList == NULL){
        //whcih means the malloc in linked_list fail.
        free(info);
        return NULL;
    }
    return info;
}

// Destroys scheduler specific info
void schedulerSRPTDestroy(void* schedulerInfo)
{
    scheduler_SRPT_t* info = (scheduler_SRPT_t*)schedulerInfo;
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
void schedulerSRPTScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_SRPT_t* info = (scheduler_SRPT_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if(list_count(info->jobList) != 0){
        jobSetRemainingTime(info->currentJob,jobGetRemainingTime(info->currentJob) - (currentTime - info->when_start));
        uint64_t curr_remain_time = jobGetRemainingTime(info->currentJob);


        if(curr_remain_time > jobGetJobTime(job)){
            schedulerCancelNextCompletion(scheduler);
            //cancel bigger
            list_insert(info->jobList, job);
            //add new job to list
            

            //reinsert the job to keep list in correct order.

            job_t* temp2 = info->currentJob;

            list_node_t* temp1 = list_find(info->jobList,info->currentJob);
            
            list_remove(info->jobList,temp1);
            list_insert(info->jobList,temp2);

            
            // schedule it.
            schedulerScheduleNextCompletion(scheduler,currentTime + jobGetJobTime(job));
            info->currentJob = job;
            info->when_start = currentTime;
            jobSetRemainingTime(job,jobGetJobTime(job));

            //since it's new job, so R time is job time.
        }

        else{
            //if remain and the new one are same, 
            list_insert(info->jobList, job);
            info->when_start = currentTime;
            // dont cancel and keep the old current one running
        }
    }
    else{
        //first job in list, just mak eit running
        list_insert(info->jobList,job);
        schedulerScheduleNextCompletion(scheduler,currentTime + jobGetRemainingTime(job));
        info->currentJob = job;
        info->when_start = currentTime;
        jobSetRemainingTime(job,jobGetJobTime(job));
    }
}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerSRPTCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_SRPT_t* info = (scheduler_SRPT_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if (list_count(info->jobList) == 0){
        return NULL;
    }

    job_t* completed_job = info->currentJob;
    list_node_t* completed_node = list_find(info->jobList,info->currentJob);
    list_remove(info->jobList,completed_node);

    //head will be SRPT next job
    if (list_count(info->jobList) > 0){
        job_t* next_job = (job_t*)(list_data(list_head(info->jobList)));
        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetRemainingTime(next_job));
        info->currentJob = next_job;
        info->when_start = currentTime;
        info->currentJob = next_job;
    }

    return completed_job;
}
