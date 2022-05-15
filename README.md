## Task Pipeline

You are building an image processing system and you have a prototype:

```C++
    // Some class that contains an image - the internals of the class are not relevant.
    class Image;

    // Get an image from a device, write to a device.
    Image getImageFromDevice();
    void outputImageToDevice(Image);


    // The details of these functions are not important.
    // An Image comes in, a processed image goes out
    Image applyTransform0(Image);
    Image applyTransform1(Image);
    Image applyTransform2(Image);


    void processImages()
    {
        while(true)
        {
			auto image = getImageFromDevice();

			image = applyTransform0(image);
			image = applyTransform1(image);
			image = applyTransform2(image);

			outputImageToDevice(image);
        }
    }
```

Now you realize you will need to parallelize this pipeline to improve the throughput.  Your goal is to make a pipeline class which parallelizes the execution of the transformations.  We want n threads running n transformations in parallel, with each image undergoing the transformation in the proper sequence.  You can assume that the number of transformations is not significantly more than the number of cores on the system.

Your class should not need to know anything about the transformations it is applying.  It should be easy for a user of your class to add, remove, or reorder transformatoins.  We are looking for a simple interface like the following:

   ```C++
       Pipeline pipe;

       pipe.addStage(getImageFromDevice);
       pipe.addStage(applyTransform0);
       pipe.addStage(applyTransform1);
       pipe.addStage(outputImageToDevice);

       pipe.run();
   ```

So in the code above, we create a pipeline with two transformatoin stages, as well as stages for getting and outputting images.  During execution an image should be passed from ``getImageFromDevice`` to ``transform0`` to ``transform1`` to ``outputImageToDevice``.   Each stage should execute on a separate thread from the others.  So  ``transform1`` should execute on a seperate thread from ``transform0`` while ``transform0`` moves on the next image in the queue.  I.E. you want to process the sequence in a parallelized pipeline where every stage(transform) in the pipeline runs on its own thread.  There are no guarentees on how long each stage might take.

Please implement such a pipeline in  C++.
