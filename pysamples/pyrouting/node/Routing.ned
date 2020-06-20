package node;

//
// Models a router.
//
simple Routing
{
    parameters:
        @display("i=block/switch");
        @signal[drop](type="long");
        @signal[outputIf](type="long");
        @statistic[drop](title="dropped packet byte length";unit=bytes;record=vector?,count,sum;interpolationmode=none);
        @statistic[outputIf](title="output interface for each routed packet";record=histogram;interpolationmode=none);
    gates:
        input in[];
        output out[];
        input localIn;
        output localOut;
}
