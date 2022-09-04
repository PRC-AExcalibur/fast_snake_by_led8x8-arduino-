# 快速贪吃蛇 fast_snake_game

贪吃蛇：基于Arduino,使用8*8LED做显示，使用摇杆控制方向。

本算法对于n*m的地图阵列每次操作的时间复杂度为o(1).
原理为通过数组生成规律排放的struct,通过struct中xy可以快速索引数组的其他元素。
使用指针数组记录蛇，并使用struct中指针记录蛇本身的链，每次只交换两元素两次（更新蛇头和蛇尾），因此保证时间复杂度为常数。

The time complexity of this algorithm for each operation of n*m map array is O(1).
The principle is to generate a regularly arranged struct through the array, and the other elements of the array can be quickly indexed through xy in the struct.
Use pointer array to record the snake, and use the pointer in the struct to record the chain of the snake.
Only exchange two elements twice each time (update the snake head and the snake tail), so the time complexity is guaranteed to be constant.


![Front view](https://github.com/PRC-AExcalibur/fast_snake_by_led8x8-arduino-/blob/main/Wiring%20diagram.jpg)
![Wiring diagram](https://github.com/PRC-AExcalibur/fast_snake_by_led8x8-arduino-/blob/main/Wiring%20diagram.jpg)
