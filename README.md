# os-challenge-softboys

In this project, we were tasked with implementing a linux server, which should handle various requests.
To do this, we implemented various features, to optimize the server in regards to speed and priority of requests.

## Priority queue
We implemented a priority queue system, to handle the given requests. This was done with a linked list, where the request with the highest priority is put closest to the head, and the least priority to the end. This makes the list sorted. A request is always taken from closest to head. 

To see how this feature affected the score, we tested it with one thread, to see how the priority queue affected the score. Without the priority queue, it just takes the newest request. That way, it works just like a stack. 

Priority queue: 100% reliability with a score of 135667709 points. <br />
Stack: 100% reliability with a score of 163875879 points. 

The difference is 28208170 points, which is a speedup of 17.21%. Keep in mind that this run by the vagrant from a local machine, and the score may differ if run on another machine. 

The difference is 4419840 points, which is very significant. 

## Threading Experiment
Once the linked list containing the request was implemented, we saw that the linked list was populated faster than the server could be able to handle. This gave rise to the idea that if we had multiple threads there would be enough request to have each thread continusly working. 

In this experiment we will implement a hash tread that takes a request from the prioty sorted linked list. It will then crack the hash and get a new one. The hash threads will be created in the main, and if a second argument after the port number is given to the sserve this will represent the number of threads created, this is to allow to test with a variyng amount of threads to find a optimal amount.

The experiment will test different amount of threads(from 1 to 8) to see what the optimal amount of threads is. The test will be done on the vagrant setup and run 4 times for each amount of threads. The run will be done on the milestone setup. The test has been condicted on the main branch on this hash `4bd8523126dee2fc39baf3dbaf587ee7dc376e78`.

![Threading scores](Threadscore.png)

The above figure shows the avarage score at different thread amounts. It is here seen that as soon as the thread amount reaches 4 the score stop dropping. This was to be expected since the vagrant setup of the server has 4 CPU's. What is interesting is that the scores stays low above 4 threads and it dosent start to increase since it would be excpected that more threads would result in longer run time since the server would need more time to do context switching when the amount of threads is greater that the amount of CPU's on the server. This makes os aske the question what if we increase the amount of threads with a big amount.

![Threading scores BIG](threadscoreBIG.png)

On the above figure it can be seen that even incrasing the amount of threads to 1000 dosent have a score penealty due to have only 4 CPU's on the server. But if threads does not get a request they will go to sleep for 1 second, this might be the reason for wich there in not a penalty on the score, if threads would hve continualsy tried to get a request even though there was none, we would maybe have seen a much higher score for a 1000 threads.

Based on this experiment we can say that it is better to have more than 1 thread and based on the data we have chosen to go with 4 threads since more thrads does not yield a lower result.

