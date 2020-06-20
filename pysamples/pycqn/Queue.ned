//
// Single-server queue. Configurable is the service time (which can also
// be random, i.e. different for each job), and the number jobs that
// are initially created and inserted into the queue.
//
simple PyQueue
{
    parameters:
        int numInitialJobs;
        volatile double serviceTime @unit(s);
        int priority = default(0);
        @display("i=block/queue;q=queue");
    gates:
        input in[];
        output out;
}
