w = writer
r = reader
dept = shqWriter.cpp shmQueue.cpp ~/work/shmSvr/myThreadPool.cpp
hp = ~/work/shmSvr/
h = $(hp)/myThreadPool.h
std = --std=c++11

$w : $(dept) $h
	g++ $(dept) -o $w $(std) -I $(hp)

$r : shqReader.cpp shmQueue.cpp
	g++ shqReader.cpp shmQueue.cpp -o $r $(std) 
