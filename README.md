# RayTracing 光线追踪器
一个C++语言实现的光线追踪器，[南加利福尼亚大学CSCI 480的课程试验](http://run.usc.edu/cs480-s13/assignments/assign3/assign3.html)
This is a project of a ray tracer that be able to handle opaque surfaces with lighting and shadows.
##The input type likes following:
    amb: 0.3 0.3 0.3
    sphere
    pos: 0.0 0.0 -3.0
    rad: 1
    dif: 0.3 0.3 0.3
    spe: 0.5 0.5 0.5
    shi: 1
    light
    pos: 0 0 0
    col: 1 1 1   
    ...
##The output type likes following:
![](https://github.com/wangzhaode/RayTracing/blob/master/RayTracing/5_notrace.png)  


(The project used OpenMP to speed it up.)
