#include "Task.h"

Moho::CTask::~CTask() {
    if (this->mTaskThread) {
        this->mTaskThread->val1 = 0;
        this->TaskResume(0, true);
        Moho::CTask **i;
        for (i = &this->mTaskThread->mTask; *i != this; i = &(*i)->mSubtask)
            {}
        *i = this->mSubtask;
        this->mSubtask = nullptr;
        this->mTaskThread = nullptr;
    }
    if (this->mDestroyed != nullptr) {
        *this->mDestroyed = true;
    }
}

// 0x00408C40
Moho::CTask::CTask(Moho::CTaskThread *thread, bool owning) :
    mDestroyed{nullptr},
    mTaskThread{nullptr},
    mSubtask{nullptr},
    mIsOwning{false}
{
    if (owning) {
        this->mIsOwning = true;
        this->mTaskThread = thread;
        this->mSubtask = thread->mTask;
        thread->mTask = this;
    }
}

// 0x00408D70
void Moho::CTask::TaskInterruptSubtasks() {
    if (this->mTaskThread != nullptr) {
        while (this->mTaskThread->mTask != this) {
            Moho::CTask *task = taskThread->mTask;
            if (task != nullptr) {
                taskThread->task = task->mSubtask;
                task->mSubtask = nullptr;
                task->mTaskThread = nullptr;
                if (! task->mIsOwning) {
                    delete(task);
                }
            }
        }
    }
}

// 0x00408DB0
void Moho::CTask::TaskResume(int a2, bool interrupt) {
    if (this->mTaskThread != nullptr) {
        this->mTaskThread->val1 = a2;
        this->mTaskThread->Unstage();
        if (interrupt) {
            this->TaskInterruptSubtasks();
        }
    }
}

// 0x004093E0
void Moho::CTaskThread::Stage() {
    if (! this->mStaged) {
        this->mPrev->next = this->mNext;
        this->mNext->prev = this->mPrev;
        this->mPrev = this;
        this->mNext = this;
        this->mPrev = this->mStage->mThreads.mPrev;
        this->mNext = this->mStage;
        this->mStage->mThreads.mPrev = this;
        this->mPrev->mNext = this;
        this->mStaged = true;
    }
}

// inline e.g. 0x004091C0
void Moho::CTaskThread::Unstage() {
    if (this->mStaged) {
        this->mPrev->next = this->mNext;
        this->mNext->prev = this->mPrev;
        this->mPrev = this;
        this->mNext = this;
        this->mPrev = this->mStage->mThreads.mPrev;
        this->mNext = this->mStage;
        this->mStage->mThreads.mPrev = this;
        this->mPrev->mNext = this;
        this->mStaged = false;
    }
}
