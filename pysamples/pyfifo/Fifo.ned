//
// Single-server queue, service rate independent of message length
//
simple PyFifo
{
    parameters:
        volatile double serviceTime @unit(s);
        @display("i=block/queue;q=queue");
        @signal[qlen](type="long");
        @signal[busy](type="bool");
        @signal[queueingTime](type="simtime_t");
        @statistic[qlen](title="queue length";record=vector,timeavg,max;interpolationmode=sample-hold);
        @statistic[busy](title="server busy state";record=vector?,timeavg;interpolationmode=sample-hold);
        @statistic[queueingTime](title="queueing time at dequeue";unit=s;record=vector,mean,max;interpolationmode=none);
    gates:
        input in;
        output out;
}
