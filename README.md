# arduino-maze-robot
面包板上连线记录！！连好之后勿动，节约每次测试前的时间！！！
1.传感器的供电全部使用Arduino5v
2.Motor， graspper lifter的供电直接使用充电宝；
4.面包板一侧的负极一条公共地，正极一条全部是电动机的5v供电；
6.LED校准状态指示灯直接连Arduino的pin13
7.MOtor的pin11左 pin12右
8.Reflectance sensor的1-6对应Arduino的pin3-8（从左到右是从绿到黑线）


PID算法工作是基于sensor的1-6， 接Arduino的8-3;
Arduino的3-8一定对应屏幕上的sensorValues0-5;

landscape算法基于sensor的1-6， 接Arduino的3-8；
将两者合并，最简单的办法是该landscape的判断条件。

车头向前，从左到右：
sensor pin 6 5 4 3 2 1
Arduino pin 3 4 5 6 7 8
sensorValues 0 1 2 3 4 5 

eg. 遇到左转弯，左边黑，右边白， 这样空间的左右和sensorvalues的从左到右是对应的
sensor pin 6 5 4 3 2 1
           1 1 1 1 0 0
Arduino pin 3 4 5 6 7 8
            
sensorValues 0 1 2 3 4 5 

T不变，左右转要对调即可


