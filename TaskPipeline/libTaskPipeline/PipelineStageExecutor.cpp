/*                                                                                  *
* Copyright 2022 Dawid Bautsch                                                      *
* dawid@bautsch.pl                                                                  *
*                                                                                   *
* Permission is hereby granted, free of charge, to any person obtaining             *
* a copy of this software and associated documentation files (the "Software"),      *
* to deal in the Software without restriction, including without limitation         *
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or   *
* sell copies of the Software, and to permit persons to whom the Software is        *
* furnished to do so, subject to the following conditions:                          *
*                                                                                   *
* The above copyright notice and this permission notice shall be included in all    *
* copies or substantial portions of the Software.                                   *
*                                                                                   *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR        *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS  *
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR    *
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN *
* AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION   *
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   *
*/

#include "PipelineStageExecutor.h"

#include "IPipelineStage.h"
#include <thread>

namespace TaskPipeline
{
PipelineStageExecutor::PipelineStageExecutor(
    std::shared_ptr<IPipelineStage> stage,
    std::shared_ptr<PipelineStageExecutor> myParentExecutor,
    int number,
    PipelineStageExecutor::StageCompletedCallback completedCallback)
    :
    pipelineStage(stage),
    parentExecutor(myParentExecutor),
    stageNumber(number),
    onComplete(completedCallback)
{
    initializationMx.lock();
}

void PipelineStageExecutor::Join()
{
    // extra "spin lock" to prevent joining before
    // even an initialization occured
    while (initializationMx.try_lock() != true)
    {
        std::this_thread::yield();
    }

    try
    {
        if (workerThread && workerThread->joinable())
        {
            workerThread->join();
            workerThread.reset();
        }
    }
    catch(const std::exception&)
    {
        initializationMx.unlock();
        return;
    }
    
    initializationMx.unlock();
}

std::shared_ptr<IPipelineStage> PipelineStageExecutor::GetStage()
{
    return pipelineStage;
}

void PipelineStageExecutor::Exec()
{
    try
    {
        workerThread.reset(new std::thread(
            &PipelineStageExecutor::WorkerThreadMethod, this
        ));
    }
    catch (const std::exception & )
    {
        initializationMx.unlock();
        return;
    }

    initializationMx.unlock();
}

void PipelineStageExecutor::WorkerThreadMethod()
{
    if (pipelineStage)
    {
        if (parentExecutor != nullptr)
        {
            parentExecutor->Join();
            auto parentStage = parentExecutor->GetStage();
            pipelineStage->SetInputImage(
                parentStage->GetResultImage());
        }

        pipelineStage->Exec();
    }

    if (onComplete)
    {
        onComplete(stageNumber);
    }
}
} // namespace TaskPipeline
