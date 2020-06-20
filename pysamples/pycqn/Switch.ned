//
// Sends received packets to a randomly chosen output. Gates are
// chosen with uniform probability.
//
simple PySwitch
{
    parameters:
        int priority = default(0);
        @display("i=block/switch");
    gates:
        input in;
        output out[];
}
