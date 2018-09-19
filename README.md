# Rastarization-3D-Renderer
Rastarization based 3D Renderer with Z-buffer and Phong illumination.

### How to run?
Requirements: C++11, Python
<ul>
  <li> run Makefile 
    <code> make </code> </li>
  <li> clean obj file with objParser.py 
    <code> python objParser gun.obj </code> </li>
  <li> objParser will produce .clean file, run that as a command line argument 
    <code> main gun.obj.clean </code>
    This will auto adjust camera and light in the scene.
  <li> output will be produced as the .ppm image. You can open it using Photoshop or other tools. </li>
  <p>
    For manual adjustments, provide more command line args. </p>
    <p>
    It takes the form: <br/><code> main objFile camPosX camPosY camPosZ lightPosX lightPosY lightPosZ rotateDegreeX rotateDegreeY rotateDegreeZ </code> </p>
    <p>e.g. <code> main gun.obj.clean 100 100 100 200 200 200 -30 30 60 </code></p>
  </p>
</ul>
<code>
  
</code>

## current progress

![dragon4](https://user-images.githubusercontent.com/11765482/36935161-297e608c-1f1c-11e8-836b-895cd0fa08a5.PNG)
![gun2](https://user-images.githubusercontent.com/11765482/36935548-4886b510-1f21-11e8-9524-4053df829632.PNG)
![dragon2](https://user-images.githubusercontent.com/11765482/36934931-f716978e-1f18-11e8-9798-3e8b5dcecb11.PNG)
![dragon1](https://user-images.githubusercontent.com/11765482/36934934-f99962a2-1f18-11e8-91f8-25f1963c24c6.PNG)
![harrypotter2](https://user-images.githubusercontent.com/11765482/36936033-c2e5eef6-1f27-11e8-904e-a087a0be88ff.PNG)

### Shield of a warrior: after bug fix on translate function :)
![rastercapturekabach](https://user-images.githubusercontent.com/11765482/36933366-a984b6c2-1eff-11e8-908f-d5f5a3c8623b.PNG)

### car model displayed after normalization
![raycapturecar](https://user-images.githubusercontent.com/11765482/36802536-46eea118-1cdd-11e8-8dca-a18904c5d548.PNG)

### after polygon face is converted to triangular faces
#### all triangular faces are found ( no black triangles )
![rastercapturetriangular](https://user-images.githubusercontent.com/11765482/36799122-7ca5e8a6-1cd4-11e8-9445-0e820868f879.PNG)

### before polygon face is converted to triangular faces
#### some faces are missing ( black triangles )
![raycapturewithshadow](https://user-images.githubusercontent.com/11765482/36798995-31d0c1ac-1cd4-11e8-839d-13d6149792e7.PNG)

### parsed from obj file
![rastercapture3](https://user-images.githubusercontent.com/11765482/36789151-9e3c1600-1cb8-11e8-98b1-9a8031e82be7.PNG)

### depth buffer with phong illumination
![rastercapture2](https://user-images.githubusercontent.com/11765482/36771135-a501f668-1c76-11e8-8dcf-f7d8626d2af5.PNG)


### Authors
- Bipin Oli bipinoli90@gmail.com
- Aayush Gautam agautam1997@gmail.com
- Kuldip Bhutoria 072bct518@pcampus.edu.np
