simple PyTxc18 extends PyTxc16
{
}

network TicToc18
{
    parameters:
        int numCentralNodes = default(2);
    types:
        channel Channel extends ned.DelayChannel {
            delay = 100ms;
        }
    submodules:
        tic[numCentralNodes+4]: PyTxc18;
    connections:
        // connect the 2 nodes in one side to the central nodes
        tic[0].gate++ <--> Channel <--> tic[2].gate++;
        tic[1].gate++ <--> Channel <--> tic[2].gate++;
        // connect the central nodes together
        for i=2..numCentralNodes+1 {
            tic[i].gate++ <--> Channel <--> tic[i+1].gate++;
        }
        // connect the 2 nodes on the other side to the central nodes
        tic[numCentralNodes+2].gate++ <--> Channel <--> tic[numCentralNodes+1].gate++;
        tic[numCentralNodes+3].gate++ <--> Channel <--> tic[numCentralNodes+1].gate++;
}
