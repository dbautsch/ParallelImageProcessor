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

#include <iostream>

#include "Image.h"
#include "Pipeline.h"
#include "Transformations.h"

int main(int /* argc */, char ** /*argv*/)
{
    TaskPipeline::Pipeline pipeline;

    // I decided to create function objects here to prevent from ambiguity of function calls
    // (In C++ overloading is impossible by return type)
    //
    // The pipeline mechanism allows three types of functions
    // 1. create image (Return an instance of new image, capturing image)
    // 2. transform image, take one argument and return an image
    // 3. output an image - take one argument and return nothing
    //
    pipeline.AddStage(
        TaskPipeline::Pipeline::ImageCaptureFunction(TaskPipeline::getImageFromDevice));

    pipeline.AddStage(
        TaskPipeline::Pipeline::ImageTransformFunction(TaskPipeline::applyTransform0));

    pipeline.AddStage(
        TaskPipeline::Pipeline::ImageTransformFunction(TaskPipeline::applyTransform1));

    pipeline.AddStage(
        TaskPipeline::Pipeline::ImageOutputFunction(TaskPipeline::outputImageToDevice));

    pipeline.Execute();

    return 0;
}
