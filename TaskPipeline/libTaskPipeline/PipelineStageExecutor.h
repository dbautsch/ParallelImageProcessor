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

#pragma once

#include <functional>
#include <memory>
#include <thread>
#include <mutex>

namespace TaskPipeline
{
    class IPipelineStage;

    class PipelineStageExecutor
    {
    public:
        using StageCompletedCallback = std::function<void(int)>;

        explicit PipelineStageExecutor(
            std::shared_ptr<IPipelineStage> stage,
            std::shared_ptr<PipelineStageExecutor> myParentExecutor,
            int number,
            StageCompletedCallback completedCallback);

        ~PipelineStageExecutor() 
        { 
            Join();
        }

        void Exec();
        void Join();
        std::shared_ptr<IPipelineStage> GetStage();

    private:
        std::shared_ptr<IPipelineStage> pipelineStage;
        std::shared_ptr<PipelineStageExecutor> parentExecutor;
        int stageNumber { 0 };
        StageCompletedCallback onComplete;
        std::unique_ptr<std::thread> workerThread;
        std::mutex initializationMx;

        void WorkerThreadMethod();
    };
} // namespace TaskPipeline
