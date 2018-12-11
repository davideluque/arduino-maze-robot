# arduino-maze-robot
**面包板上连线记录！！连好之后勿动，节约每次测试前的时间！！！**
* 传感器的供电全部使用Arduino 5v；
* Motor， graspper lifter的供电直接使用充电宝；
* 面包板一侧的负极一条公共地，正极一条全部是电动机的5v供电；
* LED校准状态指示灯直接连Arduino的pin13；
* MOtor的pin11左 pin12右；
* Reflectance sensor的1-6对应Arduino的pin8-3，保证传感器（小车）的左右和屏幕上的左右是一致的。


PID算法工作是基于sensor的1-6， 接Arduino的8-3;
Arduino的3-8一定对应屏幕上的sensorValues0-5;

landscape算法基于sensor的1-6， 接Arduino的3-8；
将两者合并，最简单的办法是更改landscape的判断条件。

车头向前，从左到右：


1|2|3
--|--|--
4|5|6

sensorpin|6|5|4|3|2|1
---|---|---|---|---|---
Arduinopin|3|4|5|6|7|8
sensorValues|0|1|2|3|4|5 

eg. 遇到左转弯，左边黑，右边白， 这样空间的左右和sensorvalues的从左到右是对应的
sensor pin 6 5 4 3 2 1
           1 1 1 1 0 0
Arduino pin 3 4 5 6 7 8
            
sensorValues 0 1 2 3 4 5 

T不变，左右转要对调即可


//更改PID参数
目前转弯的情况基本是：当车头的传感器到达弯道黑胶带处，一个轮子定住，另一个运动打弯
转弯半径大的原因是传感器的位置不够靠前，解决办法只有把传感器向前固定

其实目前已经用了最大速度，所以更改Kp， Kd值没有效果，转弯的时候一个轮停住，另一个动，是自然的结果

速度调小到135效果更差

