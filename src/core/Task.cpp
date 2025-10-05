#include "Task.h"

// 0x00408C90
Moho::CTask::~CTask() {
    if (this->mTaskThread != nullptr) {
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
    if (this->mTaskThread == nullptr) {
        return;
    }
    while (this->mTaskThread->mTask != this) {
        Moho::CTask *task = this->mTaskThread->mTask;
        if (task != nullptr) {
            this->mTaskThread->mTask = task->mSubtask;
            task->mSubtask = nullptr;
            task->mTaskThread = nullptr;
            if (! task->mIsOwning) {
                delete(task);
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
        this->ListLinkBefore(&this->mState->mThreads);
        this->mStaged = true;
    }
}

// inline e.g. 0x004091C0
void Moho::CTaskThread::Unstage() {
    if (this->mStaged) {
        this->ListLinkBefore(&this->mState->mThreads);
        this->mStaged = false;
    }
}
