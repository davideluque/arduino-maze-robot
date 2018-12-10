# arduino-maze-robot
面包板上连线记录！！连好之后勿动，节约每次测试前的时间！！！
1.传感器的供电全部使用Arduino5v
2.Motor， graspper lifter的供电直接使用充电宝；
4.面包板一侧的负极一条公共地，正极一条全部是电动机的5v供电；
6.LED校准状态指示灯直接连Arduino的pin13
7.MOtor的pin11左 pin12右
8.Reflectance sensor的1-6对应Arduino的pin3-8（从左到右是从绿到黑线）


现在的问题是PID到底怎么连线才能正确寻线？
