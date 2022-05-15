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

#include <gtest/gtest.h>

#include "Pipeline.h"
#include "PipelineStages/GetImageFromDevice.h"
#include "PipelineStages/OutputImage.h"
#include "PipelineStages/TransformImage.h"

#include <string>

TEST(ElementsOrderingTest, TwoStages)
{
    TaskPipeline::Pipeline pipeline;
    std::vector<std::string> callOrders;

    EXPECT_NO_THROW(pipeline.AddStage(
        TaskPipeline::Pipeline::ImageCaptureFunction(
            [&callOrders]() -> TaskPipeline::Image { callOrders.push_back("capture"); return TaskPipeline::Image(); }))
    );

    EXPECT_NO_THROW(pipeline.AddStage(
        TaskPipeline::Pipeline::ImageTransformFunction(
            [&callOrders](TaskPipeline::Image img) -> TaskPipeline::Image { callOrders.push_back("transform"); return img; }))
    );

    EXPECT_NO_THROW(pipeline.Execute());

    EXPECT_EQ(callOrders.size(), 2);
    EXPECT_EQ(callOrders[0], "capture");
    EXPECT_EQ(callOrders[1], "transform");
}

TEST(ElementsOrderingTest, MultipleStages)
{
    TaskPipeline::Pipeline pipeline;
    std::vector<std::string> callOrders;

    EXPECT_NO_THROW(pipeline.AddStage(
        TaskPipeline::Pipeline::ImageCaptureFunction(
            [&callOrders]() -> TaskPipeline::Image { callOrders.push_back("capture"); return TaskPipeline::Image(); }))
    );

    EXPECT_NO_THROW(pipeline.AddStage(
        TaskPipeline::Pipeline::ImageTransformFunction(
            [&callOrders](TaskPipeline::Image img) -> TaskPipeline::Image { callOrders.push_back("transform0"); return img; }))
    );

    EXPECT_NO_THROW(pipeline.AddStage(
        TaskPipeline::Pipeline::ImageTransformFunction(
            [&callOrders](TaskPipeline::Image img) -> TaskPipeline::Image { callOrders.push_back("transform1"); return img; }))
    );    

    EXPECT_NO_THROW(pipeline.AddStage(
        TaskPipeline::Pipeline::ImageOutputFunction(
            [&callOrders](TaskPipeline::Image) { callOrders.push_back("output"); }))
    );

    EXPECT_NO_THROW(pipeline.Execute());

    EXPECT_EQ(callOrders.size(), 4);
    EXPECT_EQ(callOrders[0], "capture");
    EXPECT_EQ(callOrders[1], "transform0");
    EXPECT_EQ(callOrders[2], "transform1");
    EXPECT_EQ(callOrders[3], "output");
}

TEST(CornerCases, ZeroStages)
{
    TaskPipeline::Pipeline pipeline;
    EXPECT_NO_THROW(pipeline.Execute());
}

TaskPipeline::Image CustomCaptureMethod()
{
    TaskPipeline::Image image;

    image.metadata = "this is custom method!";

    return image;
}

TEST(Flexibility, CustomFunction)
{
    TaskPipeline::Pipeline pipeline;

    EXPECT_NO_THROW(pipeline.AddStage(
        TaskPipeline::Pipeline::ImageCaptureFunction(CustomCaptureMethod))
    );

    EXPECT_NO_THROW(pipeline.Execute());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
