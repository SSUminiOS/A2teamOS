# miniOS with STK SUI


## how to run
1.  `git clone -b gui https://github.com/Choidongjun0830/miniOS-Project---Team-2.git`
2.  `cd miniOS-Project---Team-2/miniOS`
3.  `./miniOS`

> ID: ssu
> <br/>pwd: 1234

or

> ID: minios
> <br/>pwd: 4321

log에 기록되는 user 이름 외에 차이 없음

## commands
### [] -> 인자
### **예외처리가 되어있지 않아 인자를 덜 넣으면 바로 튕김!**
### **shedule의 경우 cmd에서 따로 실행이 됨**

### basic
> **test [a] [b]**<br/>returns a+b (덧셈과 동일)
> <br/><br/>**clear**<br/>clears the output window
> <br/><br/>**repeat [word]**<br/> repeats the word on output
> <br/><br/>**exit**<br/> terminate miniOS (exit() 와 동일)

### memory
> **print_memory**<br/> shows the current memory block
> <br/><br/>**dyna_alloc [n]**<br/> allocate the memory depending on the n and shows allocated ID
> <br/><br/>**dyna_free [ID]**<br/> frees the memory from ID

### schedule
> **scheduling**<br/> run schedule on cmd

### file system
> **filesystem**<br/>activates file system (다른 명령어를 쓰기 전 필수로 실행해야 함)
> <br/><br/>**format**<br/>formats the file system
> <br/><br/>**mount**<br/> mounts the file system
> <br/><br/>**create [t]**<br/> creates file t
> <br/><br/>**read [t]**<br/> reads content from file t 
> <br/><br/>**write [t] [w]**<br/> writes word w to file t
> <br/><br/>**create [t]**<br/> creates file t
> <br/><br/>**close [t]**<br/> deactivate the file system
