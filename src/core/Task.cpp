#include "Task.h"

Moho::CTask::~CTask() {
    if (this->taskThread) {
        this->taskThread->val1 = 0;
        this->TaskResume(0, true);
        Moho::CTask **i;
        for (i = &this->taskThread->task; *i != this; i = &(*i)->subtask)
            {}
        *i = this->subtask;
        this->subtask = nullptr;
        this->taskThread = nullptr;
    }
    if (this->destroyed != nullptr) {
        *this->destroyed = true;
    }
}

// 0x00408C40
Moho::CTask::CTask(Moho::CTaskThread *thread, bool owning) :
    destroyed{nullptr},
    taskThread{nullptr},
    subtask{nullptr},
    isOwning{false}
{
    if (owning) {
        this->isOwning = true;
        this->taskThread = thread;
        this->subtask = thread->task;
        thread->task = this;
    }
}

// 0x00408D70
void Moho::CTask::TaskInterruptSubtasks() {
    if (this->taskThread != nullptr) {
        while (this->taskThread->task != this) {
            Moho::CTask *task = taskThread->task;
            if (task != nullptr) {
                taskThread->task = task->subtask;
                task->subtask = nullptr;
                task->taskThread = nullptr;
                if (! task->isOwning) {
                    delete(task);
                }
            }
        }
    }
}

// 0x00408DB0
void Moho::CTask::TaskResume(int a2, bool interrupt) {
    if (this->taskThread != nullptr) {
        this->taskThread->val1 = a2;
        this->taskThread->Unstage();
        if (interrupt) {
            this->TaskInterruptSubtasks();
        }
    }
}

// 0x004093E0
void Moho::CTaskThread::Stage() {
    if (! this->staged) {
        this->prev->next = this->next;
        this->next->prev = this->prev;
        this->prev = this;
        this->next = this;
        this->prev = this->stage->threads.prev;
        this->next = this->stage;
        this->stage->threads.prev = this;
        this->prev->next = this;
        this->staged = true;
    }
}

// inline e.g. 0x004091C0
void Moho::CTaskThread::Unstage() {
    if (this->staged) {
        this->prev->next = this->next;
        this->next->prev = this->prev;
        this->prev = this;
        this->next = this;
        this->prev = this->stage->threads.prev;
        this->next = this->stage;
        this->stage->threads.prev = this;
        this->prev->next = this;
        this->staged = false;
    }
}
