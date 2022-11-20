# os-challenge-softboys

In this project, we were tasked with implementing a linux server, which should handle various requests.
To do this, we implemented various features, to optimize the server in regards to speed and priority of requests.

## Priority queue
We implemented a priority queue system, to handle the given requests. This was done with a linked list, where the request with the highest priority is put closest to the head, and the least priority to the end. This makes the list sorted. A request is always taken from closest to head. 

To see how this feature affected the score, we tested it with 4 threads and with cache, to see how the priority queue affected the score. Without the priority queue, it just takes the newest request. That way, it works just like a stack. 

Priority queue: 100% reliability with a score of 17634707 points. <br />
Stack: 100% reliability with a score of 22054547 points. 

The difference is 4419840 points, which is very significant. 

## In-request threading
To come up with this experiment, we started thinking about what was the trade-off between optimizing the handling of different request with threading opposed to optimizing each individual request with threading. After further consideration of the experiment we also thought it could improve the score by giving higher priority requests lower response time, since there is a chance a high priority request comes in just as all threads started cracking other request leading to a relatively long response time with the other threading method.

This experiments includes the also implementation of the priority queue and caching. The experiment's implementation consists of a main thread that manages each individual request by splitting it up into a number of parts, which we will test to see which is more optimal, and have a child thread work each part of the fragmented request and try to find the solution. When all the child threads are done with their part the main thread returns the solution to the client and starts processing the next request. 

#### Setup
First in order to test the experiment and compare it with the other former threading implementation we set it up so that it was split into 4 parts and then tested it later with other values among themselves. 

First I ran the former implementation with 8 threads with the following parameters for the client:

- Total: 100
- Start: 0
- Difficulty: 30000000
- Repetition probability: 20%
- Delay: 600000
- Priority $\lambda$: 1.5

Afterwards I tested different parameters for amount of child threads but the same client configuration. Since we figured this method has an influence on the priority handling we also decided to run the experiment with Priority $\lambda$ = 1.3 and then = 1.0.


#### Results 
The resulting score of running the former implementation with 8 threads was 12582285 so I'll be using this as the parameter in order to conclude the outcome of the experiment.

The results of running with different amount of threads are shown in the following table:

| # Threads | Score |
| :---: | :---:|
| 2 | 151701544 |
| 4 | 63791141 |
| 6 | 62774341 |
| 8 | 63091071 |

The results of changing the priority were:
| $\lambda$ | Score |
| :---: | :---:|
| 1.5 | 63091071 |
| 1.3 | 64941635 |
| 1.0 | 57437576 |

#### Conclusion
In conclusion this experiment showed a great downgrade in comparison to the other threading implementation, this is likely due to the fact that for each request the time it takes is in theory constant, since all child threads have to finish before returning the result to the client even if they don't have the answer in the scope they were given. 

The implementation did show a very small improvement with a higher variety of priorities, but far from enough to justify implementing it in the final, solution in any way.