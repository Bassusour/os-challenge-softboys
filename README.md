# os-challenge-softboys

In this project, we were tasked with implementing a linux server, which should handle various requests.
To do this, we implemented various features, to optimize the server in regards to speed and priority of requests.

## Priority stack
We implemented a priority stack system, to handle the given requests. This was done with a linked list, where the request with the highest priority is put closest to the head, and the least priority to the end. This makes the list sorted. A request is always taken from closest to head. 

To see how this feature affected the score, we tested it with one thread, to see how the priority queue affected the score. Without the priority queue, it just takes the newest request. That way, it works just like a stack. 

Priority queue: 100% reliability with a score of 135667709 points. <br />
Stack: 100% reliability with a score of 163875879 points. 

The difference is 28208170 points, which is a speedup of 17.21%. Keep in mind that this run by the vagrant from a local machine, and the score may differ if run on another machine. 

## Priority thread
To prioritize the requests with a priority level of 2 and higher, we tried having a number of seperate threads only take those requests. That way, as soon as a request with a higher priority was avaliable, there wouldn't be any delay to start working on it right away. There would still be threads working on the requests with priority level 1. 

To see how this feature affected the score, we tested it with a different number of priority threads, that only take prioritized requests, and 4 threads handling requests with priority level 1. In case of 0 priority threads, the 4 threads works like the priority stack. 

0 priority threads: 100% reliability with a score of 16715531 points. <br />
1 priority thread: 100% reliability with a score of 16723936 points. <br />
2 prority threads: 100% reliability with a score of 16924651 points. <br />
3 priority thread: 100% reliability with a score of 17502895 points. <br />
4 priority thread: 100% reliability with a score of 17721286 points. <br />

It can be seen, that by increasing the number of priority threads, the score worsens, and having one priority thread is nearly identidcal to having none. Therefore, we didn't implement this feature in the final product. 