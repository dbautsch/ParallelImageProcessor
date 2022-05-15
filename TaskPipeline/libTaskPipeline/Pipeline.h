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

#include <vector>
#include <memory>
#include <functional>
#include <shared_mutex>

#include "PipelineStageExecutor.h"
#include "Image.h"

#include "PipelineStages/GetImageFromDevice.h"
#include "PipelineStages/OutputImage.h"
#include "PipelineStages/TransformImage.h"

namespace TaskPipeline
{
    class Pipeline
    {
    public:
        // Define three prototypes of functions that are allowed:
        // 1. Image(void)
        // 2. Image(Image)
        // 3. void(Image)
        // These are complete set of functions to implement any stages you want,
        // just provide apropriate function, functor or lambda...
        using ImageCaptureFunction = std::function<TaskPipeline::Image()>;
        using ImageTransformFunction = std::function<TaskPipeline::Image(TaskPipeline::Image)>;
        using ImageOutputFunction = std::function<void(TaskPipeline::Image)>;

        template <typename ImageOperator>
        void AddStage(ImageOperator imageOperator)
        {
            auto stage = CreateStage(imageOperator);
            stages.emplace_back(stage);
        }

        void Execute();

    private:
        std::vector<std::shared_ptr<IPipelineStage>> stages;
        std::vector<std::shared_ptr<PipelineStageExecutor>> executors;
    
        std::shared_ptr<PipelineStages::GetImageFromDevice> CreateStage(ImageCaptureFunction myOperator)
        {
            return std::make_shared<PipelineStages::GetImageFromDevice>(myOperator);
        }

        std::shared_ptr<PipelineStages::TransformImage> CreateStage(ImageTransformFunction myOperator)
        {
            return std::make_shared<PipelineStages::TransformImage>(myOperator);
        }

        std::shared_ptr<PipelineStages::OutputImage> CreateStage(ImageOutputFunction myOperator)
        {
            return std::make_shared<PipelineStages::OutputImage>(myOperator);
        }
    };
} // namespace TaskPipeline
