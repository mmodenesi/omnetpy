//
// Generates jobs (messages) with the given interarrival time,
// which can be a random variate.
//
simple PySource
{
    parameters:
        volatile double sendIaTime @unit(s);
        @display("i=block/source");
    gates:
        output out;
}


