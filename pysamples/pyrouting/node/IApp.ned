package node;

//
// Interface for network traffic generator modules that
// can be used in Node.
//
moduleinterface IApp
{
    parameters:
        int address;  // local node address
    gates:
        input in;
        output out;
}
