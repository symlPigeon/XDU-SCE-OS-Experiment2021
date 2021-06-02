### Experiment 4

实验4, 实现一个producer-consumer结构的多线程程序。

程序共享一个int buffer[BUFSIZ]，一个producer往里面放随机数，一个consumer从里面拿随机数。然后他们俩也不一直跑，会sleep一个随机数时间。如果满了就放不进去，如果buffer空了就读不出来。

参考实现还给了两个semaphore的定义，但是我感觉实在是用不上，就单纯拿mutex去实现了。
